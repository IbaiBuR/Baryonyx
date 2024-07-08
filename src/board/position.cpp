#include "position.hpp"

#include <format>
#include <iostream>

#include "bitboard/attacks.hpp"
#include "piece.hpp"
#include "../utils/split.hpp"
#include "../utils/zobrist.hpp"

namespace board {

position::position(const std::string& fen) :
    m_pieces(),
    m_key(0ULL) {
    m_pieces.fill(piece::none);

    const auto tokens = utils::split::split_string(fen, ' ');

    if (tokens.size() < 6)
        throw std::invalid_argument("Invalid FEN string: missing fields.\n");

    const auto ranks = utils::split::split_string(tokens[0], '/');

    if (ranks.size() > 8)
        throw std::invalid_argument("Invalid FEN string: too many ranks.\n");
    if (ranks.size() < 8)
        throw std::invalid_argument("Invalid FEN string: missing ranks.\n");

    int rank_index = constants::num_ranks - 1;

    for (const auto& rank : ranks) {

        u8 file_index = 0;

        for (const auto c : rank) {
            if (file_index >= constants::num_files)
                throw std::invalid_argument("Invalid FEN string: too many files.\n");

            if (std::isdigit(c))
                file_index += c - '0';
            else {
                const square sq    = square_of(file_index, rank_index);
                const piece  piece = pieces::char_to_piece(c);

                set_piece(piece, sq);
                ++file_index;
            }
        }
        --rank_index;
    }

    m_stm = tokens[1] == "w" ? color::white : color::black;
    m_key ^= utils::zobrist::get_side_key(m_stm);

    m_castling = castling_rights(tokens[2]);
    m_key ^= utils::zobrist::get_castling_key(m_castling);

    const std::string& en_passant = tokens[3];

    m_ep_sq =
        en_passant == "-" ? square::none : square_of(en_passant[0] - 'a', en_passant[1] - 1 - '0');
    m_key ^= utils::zobrist::get_en_passant_key(m_ep_sq);

    m_half_move_clock  = std::stoi(tokens[4]);
    m_full_move_number = std::stoi(tokens[5]);

    m_hash_history.reserve(constants::max_game_ply - m_full_move_number);

    m_checkers_bb = attacks_to_king(king_square(m_stm), m_stm);

    if (!is_valid())
        throw std::invalid_argument("Invalid FEN string: illegal position.\n");
}

template <color C>
bool position::can_castle_king_side() const {
    const bitboards::bitboard& occupied = occupancies(color::white) | occupancies(color::black);

    if constexpr (C == color::white)
        return castling().king_side_available<color::white>()
            && !is_square_attacked_by(square::f1, color::black)
            && !is_square_attacked_by(square::g1, color::black)
            && !(occupied & bitboards::bitboard(0x60ULL));
    else
        return castling().king_side_available<color::black>()
            && !is_square_attacked_by(square::f8, color::white)
            && !is_square_attacked_by(square::g8, color::white)
            && !(occupied & bitboards::bitboard(0x6000000000000000ULL));
}

template bool position::can_castle_king_side<color::white>() const;
template bool position::can_castle_king_side<color::black>() const;

template <color C>
bool position::can_castle_queen_side() const {
    const bitboards::bitboard& occupied = occupancies(color::white) | occupancies(color::black);

    if constexpr (C == color::white)
        return castling().queen_side_available<color::white>()
            && !is_square_attacked_by(square::d1, color::black)
            && !is_square_attacked_by(square::c1, color::black)
            && !(occupied & bitboards::bitboard(0xEULL));
    else
        return castling().queen_side_available<color::black>()
            && !is_square_attacked_by(square::d8, color::white)
            && !is_square_attacked_by(square::c8, color::white)
            && !(occupied & bitboards::bitboard(0xE00000000000000ULL));
}

template bool position::can_castle_queen_side<color::white>() const;
template bool position::can_castle_queen_side<color::black>() const;

template <color C>
int position::piece_count(const piece_type pt) const {
    if constexpr (C == color::white)
        return (piece_type_bb(pt) & occupancies(color::white)).bit_count();
    else
        return (piece_type_bb(pt) & occupancies(color::black)).bit_count();
}

template int position::piece_count<color::white>(piece_type pt) const;
template int position::piece_count<color::black>(piece_type pt) const;

bitboards::bitboard position::attacks_to_king(const square kingSquare, const color c) const {
    const auto& opp_occupancies = occupancies(~c);
    const auto& opp_pawns       = piece_type_bb(piece_type::pawn) & opp_occupancies;
    const auto& opp_knights     = piece_type_bb(piece_type::knight) & opp_occupancies;
    const auto& opp_queens      = piece_type_bb(piece_type::queen) & opp_occupancies;
    const auto& opp_bishops_queens =
        (piece_type_bb(piece_type::bishop) & opp_occupancies) | opp_queens;
    const auto& opp_rooks_queens = (piece_type_bb(piece_type::rook) & opp_occupancies) | opp_queens;
    const auto& opp_king         = piece_type_bb(piece_type::king) & opp_occupancies;

    const auto& blockers = occupancies(color::white) | occupancies(color::black);

    return (bitboards::attacks::get_pawn_attacks(kingSquare, c) & opp_pawns)
         | (bitboards::attacks::get_knight_attacks(kingSquare) & opp_knights)
         | (bitboards::attacks::get_bishop_attacks(kingSquare, blockers) & opp_bishops_queens)
         | (bitboards::attacks::get_rook_attacks(kingSquare, blockers) & opp_rooks_queens)
         | (bitboards::attacks::get_king_attacks(kingSquare) & opp_king);
}

square position::king_square(const color c) const {
    return static_cast<square>((piece_type_bb(piece_type::king) & occupancies(c)).pop_lsb());
}

void position::set_piece(const piece p, const square sq) {
    m_pieces[std::to_underlying(sq)] = p;

    bitboards::bitboard::set_bit(m_piece_bb[std::to_underlying(pieces::piece_to_piece_type(p))],
                                 sq);
    bitboards::bitboard::set_bit(m_occupied_bb[std::to_underlying(pieces::piece_color(p))], sq);

    m_key ^= utils::zobrist::get_piece_key(p, sq);
}

void position::remove_piece(const piece p, const square sq) {
    m_pieces[std::to_underlying(sq)] = piece::none;

    bitboards::bitboard::clear_bit(m_piece_bb[std::to_underlying(pieces::piece_to_piece_type(p))],
                                   sq);
    bitboards::bitboard::clear_bit(m_occupied_bb[std::to_underlying(pieces::piece_color(p))], sq);

    m_key ^= utils::zobrist::get_piece_key(p, sq);
}

void position::move_piece(const piece p, const square from, const square to) {
    remove_piece(p, from);
    set_piece(p, to);
}

template <bool SaveHashHistory>
void position::make_move(const moves::move move) {
    ++m_half_move_clock;

    if constexpr (SaveHashHistory)
        m_hash_history.push_back(m_key);

    const square    from         = move.from();
    const square    to           = move.to();
    const piece     moving_piece = piece_on(from);
    const direction offset       = m_stm == color::white ? direction::north : direction::south;

    if (move.is_capture()) {
        const square target_square = move.is_en_passant() ? m_ep_sq - offset : to;
        remove_piece(piece_on(target_square), target_square);
        m_half_move_clock = 0;
    }

    remove_piece(moving_piece, from);
    set_piece(move.is_promotion() ? move.get_promoted_piece(m_stm) : moving_piece, to);

    m_key ^= utils::zobrist::get_side_key(m_stm);
    m_key ^= utils::zobrist::get_en_passant_key(m_ep_sq);

    if (move.is_double_push()) {
        m_ep_sq = to - offset;
        m_key ^= utils::zobrist::get_en_passant_key(m_ep_sq);
    }
    else
        m_ep_sq = square::none;

    if (move.is_castling()) {
        switch (to) {
        case square::g1:
            move_piece(piece_on(square::h1), square::h1, square::f1);
            break;
        case square::c1:
            move_piece(piece_on(square::a1), square::a1, square::d1);
            break;
        case square::g8:
            move_piece(piece_on(square::h8), square::h8, square::f8);
            break;
        case square::c8:
            move_piece(piece_on(square::a8), square::a8, square::d8);
            break;
        default:
            throw std::runtime_error("Castling to invalid square!\n");
        }
    }

    const castling_rights previous_castling_rights = m_castling;

    m_castling &= castling_rights_update[std::to_underlying(from)];
    m_castling &= castling_rights_update[std::to_underlying(to)];

    if (m_castling != previous_castling_rights) {
        m_key ^= utils::zobrist::get_castling_key(previous_castling_rights);
        m_key ^= utils::zobrist::get_castling_key(m_castling);
    }

    m_full_move_number += m_stm == color::black;

    if (pieces::piece_to_piece_type(moving_piece) == piece_type::pawn)
        m_half_move_clock = 0;

    m_stm = ~m_stm;
    m_key ^= utils::zobrist::get_side_key(m_stm);

    m_checkers_bb        = attacks_to_king(king_square(m_stm), m_stm);
    m_last_move_was_null = false;
}

template void position::make_move<true>(moves::move move);
template void position::make_move<false>(moves::move move);

void position::make_null_move() {
    ++m_half_move_clock;

    m_hash_history.push_back(m_key);

    m_key ^= utils::zobrist::get_side_key(m_stm);
    m_key ^= utils::zobrist::get_en_passant_key(m_ep_sq);

    m_stm = ~m_stm;
    m_key ^= utils::zobrist::get_side_key(m_stm);

    m_checkers_bb        = attacks_to_king(king_square(m_stm), m_stm);
    m_last_move_was_null = true;
}

void position::reset_to_start_pos() {
    m_hash_history.clear();
    m_pieces.fill(piece::none);

    m_checkers_bb      = bitboards::util::empty_bb;
    m_stm              = color::white;
    m_ep_sq            = square::none;
    m_castling         = castling_rights(castling_rights::castling_flag::all);
    m_half_move_clock  = 0;
    m_full_move_number = 1;
    m_key              = 0x63FB272195DEE353ULL;

    m_piece_bb[std::to_underlying(piece_type::pawn)]   = bitboards::bitboard(0xFF00000000FF00ULL);
    m_piece_bb[std::to_underlying(piece_type::knight)] = bitboards::bitboard(0x4200000000000042ULL);
    m_piece_bb[std::to_underlying(piece_type::bishop)] = bitboards::bitboard(0x2400000000000024ULL);
    m_piece_bb[std::to_underlying(piece_type::rook)]   = bitboards::bitboard(0x8100000000000081ULL);
    m_piece_bb[std::to_underlying(piece_type::queen)]  = bitboards::bitboard(0x800000000000008ULL);
    m_piece_bb[std::to_underlying(piece_type::king)]   = bitboards::bitboard(0x1000000000000010ULL);

    m_occupied_bb[std::to_underlying(color::white)] = bitboards::bitboard(0xFFFFULL);
    m_occupied_bb[std::to_underlying(color::black)] = bitboards::bitboard(0xFFFF000000000000ULL);

    for (u8 sq = std::to_underlying(square::a2); sq <= std::to_underlying(square::h2); ++sq)
        m_pieces[sq] = piece::w_pawn;

    for (u8 sq = std::to_underlying(square::a7); sq <= std::to_underlying(square::h7); ++sq)
        m_pieces[sq] = piece::b_pawn;

    m_pieces[std::to_underlying(square::b1)] = m_pieces[std::to_underlying(square::g1)] =
        piece::w_knight;

    m_pieces[std::to_underlying(square::b8)] = m_pieces[std::to_underlying(square::g8)] =
        piece::b_knight;

    m_pieces[std::to_underlying(square::c1)] = m_pieces[std::to_underlying(square::f1)] =
        piece::w_bishop;

    m_pieces[std::to_underlying(square::c8)] = m_pieces[std::to_underlying(square::f8)] =
        piece::b_bishop;

    m_pieces[std::to_underlying(square::a1)] = m_pieces[std::to_underlying(square::h1)] =
        piece::w_rook;
    m_pieces[std::to_underlying(square::a8)] = m_pieces[std::to_underlying(square::h8)] =
        piece::b_rook;

    m_pieces[std::to_underlying(square::d1)] = piece::w_queen;
    m_pieces[std::to_underlying(square::d8)] = piece::b_queen;

    m_pieces[std::to_underlying(square::e1)] = piece::w_king;
    m_pieces[std::to_underlying(square::e8)] = piece::b_king;
}

template <color C>
bool position::has_no_pawns() const {
    return piece_count<C>(piece_type::pawn) == 0;
}

bool position::has_no_pawns(const color c) const {
    return c == color::white ? has_no_pawns<color::white>() : has_no_pawns<color::black>();
}

bool position::is_square_attacked_by(const square sq, const color c) const {
    const auto& our_pieces  = occupancies(c);
    const auto& our_pawns   = piece_type_bb(piece_type::pawn) & our_pieces;
    const auto& our_knights = piece_type_bb(piece_type::knight) & our_pieces;
    const auto& our_bishops = piece_type_bb(piece_type::bishop) & our_pieces;
    const auto& our_rooks   = piece_type_bb(piece_type::rook) & our_pieces;
    const auto& our_queens  = piece_type_bb(piece_type::queen) & our_pieces;
    const auto& our_king    = piece_type_bb(piece_type::king) & our_pieces;

    const auto& blockers = occupancies(color::white) | occupancies(color::black);

    if (bitboards::attacks::get_pawn_attacks(sq, ~c) & our_pawns)
        return true;

    if (bitboards::attacks::get_knight_attacks(sq) & our_knights)
        return true;

    if (bitboards::attacks::get_bishop_attacks(sq, blockers) & our_bishops)
        return true;

    if (bitboards::attacks::get_rook_attacks(sq, blockers) & our_rooks)
        return true;

    if (bitboards::attacks::get_queen_attacks(sq, blockers) & our_queens)
        return true;

    if (bitboards::attacks::get_king_attacks(sq) & our_king)
        return true;

    return false;
}

bool position::is_valid() const {
    if (piece_type_bb(piece_type::king).bit_count() != 2) {
        std::cerr << std::format("There must be 2 kings on the board.") << std::endl;
        return false;
    }

    const auto& white_occupancies = occupancies(color::white);

    if (white_occupancies.bit_count() > 16) {
        std::cerr << std::format("White must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto& black_occupancies = occupancies(color::black);

    if (black_occupancies.bit_count() > 16) {
        std::cerr << std::format("Black must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto& pawns = piece_type_bb(piece_type::pawn);

    if ((pawns & white_occupancies).bit_count() > 8) {
        std::cerr << std::format("White must have 8 or less pawns.") << std::endl;
        return false;
    }

    if ((pawns & black_occupancies).bit_count() > 8) {
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

bool position::was_legal() const { return !is_square_attacked_by(king_square(~m_stm), m_stm); }

bool position::has_repeated() const {
    const usize history_size      = m_hash_history.size();
    const auto  repetition_offset = std::min<usize>(m_half_move_clock, history_size - 1);

    for (usize i = 4; i <= repetition_offset; i += 2) {
        if (m_key == m_hash_history[history_size - i])
            return true;
    }

    return false;
}


std::string position::to_fen() const {
    std::string fen;

    for (int rank = constants::num_ranks - 1; rank >= 0; --rank) {
        u16 empty_squares = 0;

        for (u8 file = 0; file < constants::num_files; ++file) {
            const square sq            = square_of(file, rank);
            const piece  current_piece = piece_on(sq);

            if (current_piece != piece::none) {
                if (empty_squares > 0)
                    fen += std::to_string(empty_squares);

                fen += pieces::piece_to_char(current_piece);
                empty_squares = 0;
            }
            else
                ++empty_squares;
        }

        if (empty_squares > 0)
            fen += std::to_string(empty_squares);

        fen += rank == 0 ? ' ' : '/';
    }

    fen +=
        std::format("{} {} {} {} {}", m_stm == color::white ? "w" : "b", castling().to_string(),
                    m_ep_sq != square::none ? util::sq_to_coords[std::to_underlying(m_ep_sq)] : "-",
                    m_half_move_clock, m_full_move_number);

    return fen;
}

void print_board(const position& pos) {
    std::cout << std::format("\n+---+---+---+---+---+---+---+---+") << std::endl;

    for (int rank = constants::num_ranks - 1; rank >= 0; --rank) {
        for (u8 file = 0; file < constants::num_files; ++file) {
            const square sq            = square_of(file, rank);
            const piece  current_piece = pos.piece_on(sq);

            std::cout << std::format(
                "| {}", current_piece == piece::none ? ' ' : pieces::piece_to_char(current_piece));

            if (file != constants::num_files - 1)
                std::cout << std::format(" ");
        }
        std::cout << std::format(" | {}\n+---+---+---+---+---+---+---+---+", rank + 1) << std::endl;
    }

    std::cout << std::format("  A   B   C   D   E   F   G   H\n") << std::endl;
    std::cout << std::format("Side to move    : {}",
                             pos.side_to_move() == color::white ? "white" : "black")
              << std::endl;

    const square en_passant = pos.ep_square();

    std::cout << std::format(
        "En passant      : {}",
        en_passant != square::none ? util::sq_to_coords[std::to_underlying(en_passant)] : "-")
              << std::endl;
    std::cout << std::format("Castling rights : {}", pos.castling().to_string()) << std::endl;
    std::cout << std::format("Halfmove clock  : {}", pos.fifty_move_rule()) << std::endl;
    std::cout << std::format("Fullmove number : {}", pos.full_moves()) << std::endl;
    std::cout << std::format("FEN             : {}", pos.to_fen()) << std::endl;
    std::cout << std::format("Hash            : 0x{:016X}\n", pos.key()) << std::endl;
}

} // namespace board