#include "position.hpp"

#include <format>
#include <iostream>

#include "bitboard/attacks.hpp"
#include "piece.hpp"
#include "../utils/split.hpp"
#include "../tt/zobrist.hpp"

namespace board {

Position::Position(const std::string& fen) :
    m_pieces(),
    m_key(0ULL) {
    m_pieces.fill(Piece::NONE);
    const auto tokens = utils::split::split_string(fen, ' ');

    if (tokens.size() < 6)
        throw std::invalid_argument("Invalid FEN string: missing fields.\n");

    const auto ranks = utils::split::split_string(tokens[0], '/');

    if (ranks.size() > 8)
        throw std::invalid_argument("Invalid FEN string: too many ranks.\n");
    if (ranks.size() < 8)
        throw std::invalid_argument("Invalid FEN string: missing ranks.\n");

    int rankIndex = 7;

    for (const auto& rank : ranks) {

        u8 fileIndex = 0;

        for (const auto c : rank) {
            if (fileIndex >= 8)
                throw std::invalid_argument("Invalid FEN string: too many files.\n");

            if (std::isdigit(c))
                fileIndex += c - '0';
            else {
                const auto  sq    = square_of(fileIndex, rankIndex);
                const Piece piece = pieces::char_to_piece(c);
                set_piece(piece, sq);
                ++fileIndex;
            }
        }
        --rankIndex;
    }

    m_stm = tokens[1] == "w" ? Color::WHITE : Color::BLACK;
    m_key ^= tt::zobrist::get_side_key(m_stm);

    m_castling = CastlingRights(tokens[2]);
    m_key ^= tt::zobrist::get_castling_key(m_castling);

    const std::string& enpassant = tokens[3];

    m_epSq =
        enpassant == "-" ? Square::NONE : square_of(enpassant[0] - 'a', enpassant[1] - 1 - '0');
    m_key ^= tt::zobrist::get_en_passant_key(m_epSq);

    m_halfMoveClock  = std::stoi(tokens[4]);
    m_fullMoveNumber = std::stoi(tokens[5]);

    m_checkersBB = attacks_to_king(king_square(m_stm), m_stm);

    if (!is_valid())
        throw std::invalid_argument("Invalid FEN string: illegal position.\n");
}

template <Color c>
bool Position::can_castle_king_side() const {
    const bitboards::Bitboard& occupied = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if constexpr (c == Color::WHITE)
        return castling_rights().king_side_available<Color::WHITE>()
            && !is_square_attacked_by(Square::F1, Color::BLACK)
            && !is_square_attacked_by(Square::G1, Color::BLACK)
            && !(occupied & bitboards::Bitboard(0x60ULL));
    else
        return castling_rights().king_side_available<Color::BLACK>()
            && !is_square_attacked_by(Square::F8, Color::WHITE)
            && !is_square_attacked_by(Square::G8, Color::WHITE)
            && !(occupied & bitboards::Bitboard(0x6000000000000000ULL));
}

template bool Position::can_castle_king_side<Color::WHITE>() const;
template bool Position::can_castle_king_side<Color::BLACK>() const;

template <Color c>
bool Position::can_castle_queen_side() const {
    const bitboards::Bitboard& occupied = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if constexpr (c == Color::WHITE)
        return castling_rights().queen_side_available<Color::WHITE>()
            && !is_square_attacked_by(Square::D1, Color::BLACK)
            && !is_square_attacked_by(Square::C1, Color::BLACK)
            && !(occupied & bitboards::Bitboard(0xEULL));
    else
        return castling_rights().queen_side_available<Color::BLACK>()
            && !is_square_attacked_by(Square::D8, Color::WHITE)
            && !is_square_attacked_by(Square::C8, Color::WHITE)
            && !(occupied & bitboards::Bitboard(0xE00000000000000ULL));
}

template bool Position::can_castle_queen_side<Color::WHITE>() const;
template bool Position::can_castle_queen_side<Color::BLACK>() const;

template <Color c>
int Position::piece_count(const PieceType pt) const {
    if constexpr (c == Color::WHITE)
        return (piece_type_bb(pt) & occupancies(Color::WHITE)).bit_count();
    else
        return (piece_type_bb(pt) & occupancies(Color::BLACK)).bit_count();
}

template int Position::piece_count<Color::WHITE>(PieceType pt) const;
template int Position::piece_count<Color::BLACK>(PieceType pt) const;

bitboards::Bitboard Position::attacks_to_king(const Square kingSquare, const Color c) const {
    const auto& oppOccupancies   = occupancies(~c);
    const auto& oppPawns         = piece_type_bb(PieceType::PAWN) & oppOccupancies;
    const auto& oppKnights       = piece_type_bb(PieceType::KNIGHT) & oppOccupancies;
    const auto& oppQueens        = piece_type_bb(PieceType::QUEEN) & oppOccupancies;
    const auto& oppBishopsQueens = (piece_type_bb(PieceType::BISHOP) & oppOccupancies) | oppQueens;
    const auto& oppRooksQueens   = (piece_type_bb(PieceType::ROOK) & oppOccupancies) | oppQueens;
    const auto& oppKing          = piece_type_bb(PieceType::KING) & oppOccupancies;

    const auto& blockers = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    return (bitboards::attacks::get_pawn_attacks(kingSquare, c) & oppPawns)
         | (bitboards::attacks::get_knight_attacks(kingSquare) & oppKnights)
         | (bitboards::attacks::get_bishop_attacks(kingSquare, blockers) & oppBishopsQueens)
         | (bitboards::attacks::get_rook_attacks(kingSquare, blockers) & oppRooksQueens)
         | (bitboards::attacks::get_king_attacks(kingSquare) & oppKing);
}

Square Position::king_square(const Color c) const {
    return static_cast<Square>((piece_type_bb(PieceType::KING) & occupancies(c)).pop_lsb());
}

void Position::set_piece(const Piece p, const Square sq) {
    m_pieces[std::to_underlying(sq)] = p;

    bitboards::Bitboard::set_bit(m_pieceBB[std::to_underlying(pieces::piece_to_piece_type(p))], sq);
    bitboards::Bitboard::set_bit(m_occupiedBB[std::to_underlying(pieces::piece_color(p))], sq);

    m_key ^= tt::zobrist::get_piece_key(p, sq);
}

void Position::remove_piece(const Piece p, const Square sq) {
    m_pieces[std::to_underlying(sq)] = Piece::NONE;

    bitboards::Bitboard::clear_bit(m_pieceBB[std::to_underlying(pieces::piece_to_piece_type(p))],
                                   sq);
    bitboards::Bitboard::clear_bit(m_occupiedBB[std::to_underlying(pieces::piece_color(p))], sq);

    m_key ^= tt::zobrist::get_piece_key(p, sq);
}

void Position::move_piece(const Piece p, const Square from, const Square to) {
    remove_piece(p, from);
    set_piece(p, to);
}

void Position::make_move(const moves::Move move) {
    ++m_halfMoveClock;

    const Square    from        = move.from();
    const Square    to          = move.to();
    const Piece     movingPiece = piece_on(from);
    const Direction offset      = m_stm == Color::WHITE ? Direction::NORTH : Direction::SOUTH;

    if (move.is_capture()) {
        const auto targetSquare = move.is_en_passant() ? m_epSq - offset : to;
        remove_piece(piece_on(targetSquare), targetSquare);
        m_halfMoveClock = 0;
    }

    remove_piece(movingPiece, from);
    set_piece(move.is_promotion() ? move.get_promoted_piece(m_stm) : movingPiece, to);

    if (move.is_double_push())
        m_epSq = to - offset;
    else
        m_epSq = Square::NONE;

    if (move.is_castling()) {
        switch (to) {
        case Square::G1:
            move_piece(piece_on(Square::H1), Square::H1, Square::F1);
            break;
        case Square::C1:
            move_piece(piece_on(Square::A1), Square::A1, Square::D1);
            break;
        case Square::G8:
            move_piece(piece_on(Square::H8), Square::H8, Square::F8);
            break;
        case Square::C8:
            move_piece(piece_on(Square::A8), Square::A8, Square::D8);
            break;
        default:
            throw std::runtime_error("Castling to invalid square!\n");
        }
    }

    m_castling &= castling_rights_update[std::to_underlying(from)];
    m_castling &= castling_rights_update[std::to_underlying(to)];

    m_fullMoveNumber += m_stm == Color::BLACK;

    if (pieces::piece_to_piece_type(movingPiece) == PieceType::PAWN)
        m_halfMoveClock = 0;

    m_stm        = ~m_stm;
    m_checkersBB = attacks_to_king(king_square(m_stm), m_stm);
}

void Position::reset_to_start_pos() {
    m_checkersBB     = bitboards::util::empty_bb;
    m_stm            = Color::WHITE;
    m_epSq           = Square::NONE;
    m_castling       = CastlingRights(CastlingRights::Flags::ALL);
    m_halfMoveClock  = 0;
    m_fullMoveNumber = 1;

    m_pieceBB[std::to_underlying(PieceType::PAWN)]   = bitboards::Bitboard(0xFF00000000FF00ULL);
    m_pieceBB[std::to_underlying(PieceType::KNIGHT)] = bitboards::Bitboard(0x4200000000000042ULL);
    m_pieceBB[std::to_underlying(PieceType::BISHOP)] = bitboards::Bitboard(0x2400000000000024ULL);
    m_pieceBB[std::to_underlying(PieceType::ROOK)]   = bitboards::Bitboard(0x8100000000000081ULL);
    m_pieceBB[std::to_underlying(PieceType::QUEEN)]  = bitboards::Bitboard(0x800000000000008ULL);
    m_pieceBB[std::to_underlying(PieceType::KING)]   = bitboards::Bitboard(0x1000000000000010ULL);

    m_occupiedBB[std::to_underlying(Color::WHITE)] = bitboards::Bitboard(0xFFFFULL);
    m_occupiedBB[std::to_underlying(Color::BLACK)] = bitboards::Bitboard(0xFFFF000000000000ULL);

    m_pieces.fill(Piece::NONE);

    for (u8 sq = std::to_underlying(Square::A2); sq <= std::to_underlying(Square::H2); ++sq)
        m_pieces[sq] = Piece::W_PAWN;

    for (u8 sq = std::to_underlying(Square::A7); sq <= std::to_underlying(Square::H7); ++sq)
        m_pieces[sq] = Piece::B_PAWN;

    m_pieces[std::to_underlying(Square::B1)] = m_pieces[std::to_underlying(Square::G1)] =
        Piece::W_KNIGHT;

    m_pieces[std::to_underlying(Square::B8)] = m_pieces[std::to_underlying(Square::G8)] =
        Piece::B_KNIGHT;

    m_pieces[std::to_underlying(Square::C1)] = m_pieces[std::to_underlying(Square::F1)] =
        Piece::W_BISHOP;

    m_pieces[std::to_underlying(Square::C8)] = m_pieces[std::to_underlying(Square::F8)] =
        Piece::B_BISHOP;

    m_pieces[std::to_underlying(Square::A1)] = m_pieces[std::to_underlying(Square::H1)] =
        Piece::W_ROOK;
    m_pieces[std::to_underlying(Square::A8)] = m_pieces[std::to_underlying(Square::H8)] =
        Piece::B_ROOK;

    m_pieces[std::to_underlying(Square::D1)] = Piece::W_QUEEN;
    m_pieces[std::to_underlying(Square::D8)] = Piece::B_QUEEN;

    m_pieces[std::to_underlying(Square::E1)] = Piece::W_KING;
    m_pieces[std::to_underlying(Square::E8)] = Piece::B_KING;
}

bool Position::is_square_attacked_by(const Square sq, const Color c) const {
    const auto& ourPieces  = occupancies(c);
    const auto& ourPawns   = piece_type_bb(PieceType::PAWN) & ourPieces;
    const auto& ourKnights = piece_type_bb(PieceType::KNIGHT) & ourPieces;
    const auto& ourBishops = piece_type_bb(PieceType::BISHOP) & ourPieces;
    const auto& ourRooks   = piece_type_bb(PieceType::ROOK) & ourPieces;
    const auto& ourKing    = piece_type_bb(PieceType::KING) & ourPieces;
    const auto& ourQueens  = piece_type_bb(PieceType::QUEEN) & ourPieces;

    const auto& blockers = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if (bitboards::attacks::get_pawn_attacks(sq, ~c) & ourPawns)
        return true;

    if (bitboards::attacks::get_knight_attacks(sq) & ourKnights)
        return true;

    if (bitboards::attacks::get_bishop_attacks(sq, blockers) & ourBishops)
        return true;

    if (bitboards::attacks::get_rook_attacks(sq, blockers) & ourRooks)
        return true;

    if (bitboards::attacks::get_queen_attacks(sq, blockers) & ourQueens)
        return true;

    if (bitboards::attacks::get_king_attacks(sq) & ourKing)
        return true;

    return false;
}

bool Position::is_valid() const {
    if (piece_type_bb(PieceType::KING).bit_count() != 2) {
        std::cerr << std::format("There must be 2 kings on the board.") << std::endl;
        return false;
    }

    const auto& whiteOccupancies = occupancies(Color::WHITE);

    if (whiteOccupancies.bit_count() > 16) {
        std::cerr << std::format("White must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto& blackOccupancies = occupancies(Color::BLACK);

    if (blackOccupancies.bit_count() > 16) {
        std::cerr << std::format("Black must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto& pawns = piece_type_bb(PieceType::PAWN);

    if ((pawns & whiteOccupancies).bit_count() > 8) {
        std::cerr << std::format("White must have 8 or less pawns.") << std::endl;
        return false;
    }

    if ((pawns & blackOccupancies).bit_count() > 8) {
        std::cerr << std::format("Black must have 8 or less pawns.") << std::endl;
        return false;
    }

    if (is_square_attacked_by(king_square(~m_stm), m_stm)) {
        std::cerr << std::format(
            "The king of the player whose turn it is not to move must not be in check.")
                  << std::endl;
        return false;
    }

    return true;
}

bool Position::was_legal() const { return !is_square_attacked_by(king_square(~m_stm), m_stm); }

std::string Position::to_fen() const {
    std::string fen;

    for (int rank = constants::num_ranks - 1; rank >= 0; --rank) {
        u16 emptySquares = 0;

        for (u8 file = 0; file < constants::num_files; ++file) {
            const Square sq           = square_of(file, rank);
            const Piece  currentPiece = piece_on(sq);

            if (currentPiece != Piece::NONE) {
                if (emptySquares > 0)
                    fen += std::to_string(emptySquares);
                fen += pieces::piece_to_char(currentPiece);
                emptySquares = 0;
            }
            else
                ++emptySquares;
        }

        if (emptySquares > 0)
            fen += std::to_string(emptySquares);

        fen += rank == 0 ? ' ' : '/';
    }

    fen += std::format(
        "{} {} {} {} {}", m_stm == Color::WHITE ? "w" : "b", castling_rights().to_string(),
        m_epSq != Square::NONE ? util::sq_to_coords[std::to_underlying(m_epSq)] : "-",
        m_halfMoveClock, m_fullMoveNumber);

    return fen;
}

void print_board(const Position& pos) {
    std::cout << std::format("\n+---+---+---+---+---+---+---+---+") << std::endl;

    for (int rank = constants::num_ranks - 1; rank >= 0; --rank) {
        for (u8 file = 0; file < constants::num_files; ++file) {
            const auto  sq           = square_of(file, rank);
            const Piece currentPiece = pos.piece_on(sq);
            std::cout << std::format(
                "| {}", currentPiece == Piece::NONE ? ' ' : pieces::piece_to_char(currentPiece));

            if (file != constants::num_files - 1)
                std::cout << std::format(" ");
        }
        std::cout << std::format(" | {}\n+---+---+---+---+---+---+---+---+", rank + 1) << std::endl;
    }

    std::cout << std::format("  A   B   C   D   E   F   G   H\n") << std::endl;
    std::cout << std::format("Side to move    : {}",
                             pos.side_to_move() == Color::WHITE ? "white" : "black")
              << std::endl;

    const auto enpassant = pos.ep_square();

    std::cout << std::format(
        "En passant      : {}",
        enpassant != Square::NONE ? util::sq_to_coords[std::to_underlying(enpassant)] : "-")
              << std::endl;
    std::cout << std::format("Castling rights : {}", pos.castling_rights().to_string())
              << std::endl;
    std::cout << std::format("Halfmove clock  : {}", pos.fifty_move_rule()) << std::endl;
    std::cout << std::format("Fullmove number : {}", pos.full_moves()) << std::endl;
    std::cout << std::format("FEN             : {}", pos.to_fen()) << std::endl;
    std::cout << std::format("Hash            : 0x{:016X}\n", pos.key()) << std::endl;
}

} // namespace board