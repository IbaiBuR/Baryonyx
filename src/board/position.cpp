#include "position.hpp"

#include <format>
#include <iostream>

#include "bitboard/attacks.hpp"
#include "piece.hpp"
#include "../utils.hpp"

namespace Board {

Position::Position(const std::string &fen) :
    m_pieces() {
    this->m_pieces.fill(Piece::NO_PIECE);
    m_pieces.fill(Piece::NONE);
    const auto tokens = Utils::splitString(fen, ' ');

    if (tokens.size() < 6)
        throw std::invalid_argument("Invalid FEN string: missing fields.\n");

    const auto ranks = Utils::splitString(tokens[0], '/');

    if (ranks.size() > 8)
        throw std::invalid_argument("Invalid FEN string: too many ranks.\n");
    if (ranks.size() < 8)
        throw std::invalid_argument("Invalid FEN string: missing ranks.\n");

    int rankIndex = 7;

    for (const auto &rank : ranks) {

        u8 fileIndex = 0;

        for (const auto c : rank) {
            if (fileIndex >= 8)
                throw std::invalid_argument("Invalid FEN string: too many files.\n");

            if (std::isdigit(c))
                fileIndex += c - '0';
            else {
                const auto  sq    = Bitboards::Util::squareOf(fileIndex, rankIndex);
                const Piece piece = Pieces::charToPiece.at(c);
                const auto  sq    = squareOf(fileIndex, rankIndex);
                const Piece piece = Pieces::charToPiece(c);
                setPiece(piece, sq);
                ++fileIndex;
            }
        }
        --rankIndex;
    }

    this->m_stm      = tokens[1] == "w" ? Color::WHITE : Color::BLACK;
    this->m_castling = CastlingRights(tokens[2]);

    const std::string &enpassant = tokens[3];

    m_epSq =
        enpassant == "-" ? Square::NONE : squareOf(enpassant[0] - 'a', enpassant[1] - 1 - '0');
    m_key ^= TT::Zobrist::getEnPassantKey(m_epSq);

    this->m_halfMoveClock  = std::stoi(tokens[4]);
    this->m_fullMoveNumber = std::stoi(tokens[5]);
    m_halfMoveClock  = std::stoi(tokens[4]);
    m_fullMoveNumber = std::stoi(tokens[5]);

    m_checkersBB = attacksToKing(kingSquare(m_stm), m_stm);

    if (!isValid())
        throw std::invalid_argument("Invalid FEN string: illegal position.\n");
}

template <Color c>
bool Position::canCastleKingSide() const {
    const Bitboards::Bitboard &occupied = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if constexpr (c == Color::WHITE)
        return castlingRights().kingSideAvailable<Color::WHITE>()
            && !isSquareAttackedBy(Square::F1, Color::BLACK)
            && !isSquareAttackedBy(Square::G1, Color::BLACK)
            && !(occupied & Bitboards::Bitboard(0x60ULL));
    else
        return castlingRights().kingSideAvailable<Color::BLACK>()
            && !isSquareAttackedBy(Square::F8, Color::WHITE)
            && !isSquareAttackedBy(Square::G8, Color::WHITE)
            && !(occupied & Bitboards::Bitboard(0x6000000000000000ULL));
}

template bool Position::canCastleKingSide<Color::WHITE>() const;
template bool Position::canCastleKingSide<Color::BLACK>() const;

template <Color c>
bool Position::canCastleQueenSide() const {
    const Bitboards::Bitboard &occupied = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if constexpr (c == Color::WHITE)
        return castlingRights().queenSideAvailable<Color::WHITE>()
            && !isSquareAttackedBy(Square::D1, Color::BLACK)
            && !isSquareAttackedBy(Square::C1, Color::BLACK)
            && !(occupied & Bitboards::Bitboard(0xEULL));
    else
        return castlingRights().queenSideAvailable<Color::BLACK>()
            && !isSquareAttackedBy(Square::D8, Color::WHITE)
            && !isSquareAttackedBy(Square::C8, Color::WHITE)
            && !(occupied & Bitboards::Bitboard(0xE00000000000000ULL));
}

template bool Position::canCastleQueenSide<Color::WHITE>() const;
template bool Position::canCastleQueenSide<Color::BLACK>() const;

template <Color c>
int Position::pieceCount(const PieceType pt) const {
    if constexpr (c == Color::WHITE)
        return (pieceTypeBB(pt) & occupancies(Color::WHITE)).bitCount();
    else
        return (pieceTypeBB(pt) & occupancies(Color::BLACK)).bitCount();
}

template int Position::pieceCount<Color::WHITE>(PieceType pt) const;
template int Position::pieceCount<Color::BLACK>(PieceType pt) const;

Bitboards::Bitboard Position::attacksToKing(const Square kingSquare, const Color c) const {
    const auto &oppOccupancies   = occupancies(~c);
    const auto &oppPawns         = pieceTypeBB(PieceType::PAWN) & oppOccupancies;
    const auto &oppKnights       = pieceTypeBB(PieceType::KNIGHT) & oppOccupancies;
    const auto &oppQueens        = pieceTypeBB(PieceType::QUEEN) & oppOccupancies;
    const auto &oppBishopsQueens = (pieceTypeBB(PieceType::BISHOP) & oppOccupancies) | oppQueens;
    const auto &oppRooksQueens   = (pieceTypeBB(PieceType::ROOK) & oppOccupancies) | oppQueens;
    const auto &oppKing          = pieceTypeBB(PieceType::KING) & oppOccupancies;

    const auto &blockers = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    return (Bitboards::Attacks::getPawnAttacks(kingSquare, c) & oppPawns)
         | (Bitboards::Attacks::getKnightAttacks(kingSquare) & oppKnights)
         | (Bitboards::Attacks::getBishopAttacks(kingSquare, blockers) & oppBishopsQueens)
         | (Bitboards::Attacks::getRookAttacks(kingSquare, blockers) & oppRooksQueens)
         | (Bitboards::Attacks::getKingAttacks(kingSquare) & oppKing);
}

Square Position::kingSquare(const Color c) const {
    return static_cast<Square>((pieceTypeBB(PieceType::KING) & occupancies(c)).popLSB());
}

void Position::setPiece(const Piece p, const Square sq) {
    m_pieces[std::to_underlying(sq)] = p;
    Bitboards::Bitboard::setBit(
        m_pieceBB[std::to_underlying(Pieces::pieceToPieceType[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::setBit(
        m_occupiedBB[std::to_underlying(Pieces::pieceColor[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::setBit(m_pieceBB[std::to_underlying(Pieces::pieceToPieceType(p))], sq);
    Bitboards::Bitboard::setBit(m_occupiedBB[std::to_underlying(Pieces::pieceColor(p))], sq);
}

void Position::removePiece(const Piece p, const Square sq) {
    m_pieces[std::to_underlying(sq)] = Piece::NO_PIECE;
    Bitboards::Bitboard::clearBit(
        m_pieceBB[std::to_underlying(Pieces::pieceToPieceType[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::clearBit(
        m_occupiedBB[std::to_underlying(Pieces::pieceColor[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::clearBit(m_pieceBB[std::to_underlying(Pieces::pieceToPieceType(p))], sq);
    Bitboards::Bitboard::clearBit(m_occupiedBB[std::to_underlying(Pieces::pieceColor(p))], sq);
}

void Position::movePiece(const Piece p, const Square from, const Square to) {
    removePiece(p, from);
    setPiece(p, to);
}

void Position::makeMove(const Moves::Move move) {
    ++m_halfMoveClock;

    const Square    from        = move.from();
    const Square    to          = move.to();
    const Piece     movingPiece = pieceOn(from);
    const Direction offset      = m_stm == Color::WHITE ? Direction::NORTH : Direction::SOUTH;

    if (move.isCapture()) {
        const auto targetSquare = move.isEnPassant() ? m_epSq - offset : to;
        removePiece(pieceOn(targetSquare), targetSquare);
        m_halfMoveClock = 0;
    }

    removePiece(movingPiece, from);
    setPiece(move.isPromotion() ? move.getPromotedPiece(m_stm) : movingPiece, to);

    if (move.isDoublePush())
        m_epSq = to - offset;
    else
        m_epSq = Square::NONE;

    if (move.isCastling()) {
        switch (to) {
        case Square::G1:
            movePiece(pieceOn(Square::H1), Square::H1, Square::F1);
            break;
        case Square::C1:
            movePiece(pieceOn(Square::A1), Square::A1, Square::D1);
            break;
        case Square::G8:
            movePiece(pieceOn(Square::H8), Square::H8, Square::F8);
            break;
        case Square::C8:
            movePiece(pieceOn(Square::A8), Square::A8, Square::D8);
            break;
        default:
            throw std::runtime_error("Castling to invalid square!\n");
        }
    }

    m_castling &= Util::castlingRightsUpdate[std::to_underlying(from)];
    m_castling &= Util::castlingRightsUpdate[std::to_underlying(to)];

    m_fullMoveNumber += m_stm == Color::BLACK;

    if (Pieces::pieceToPieceType(movingPiece) == PieceType::PAWN)
        m_halfMoveClock = 0;

    m_stm        = ~m_stm;
    m_checkersBB = attacksToKing(kingSquare(m_stm), m_stm);
}

void Position::resetToStartPos() {
    m_checkersBB     = Bitboards::Util::EMPTY_BB;
    m_stm            = Color::WHITE;
    m_epSq           = Square::NONE;
    m_castling       = CastlingRights(CastlingRights::Flags::ALL);
    m_halfMoveClock  = 0;
    m_fullMoveNumber = 1;

    m_pieceBB[std::to_underlying(PieceType::PAWN)]   = Bitboards::Bitboard(0xFF00000000FF00ULL);
    m_pieceBB[std::to_underlying(PieceType::KNIGHT)] = Bitboards::Bitboard(0x4200000000000042ULL);
    m_pieceBB[std::to_underlying(PieceType::BISHOP)] = Bitboards::Bitboard(0x2400000000000024ULL);
    m_pieceBB[std::to_underlying(PieceType::ROOK)]   = Bitboards::Bitboard(0x8100000000000081ULL);
    m_pieceBB[std::to_underlying(PieceType::QUEEN)]  = Bitboards::Bitboard(0x800000000000008ULL);
    m_pieceBB[std::to_underlying(PieceType::KING)]   = Bitboards::Bitboard(0x1000000000000010ULL);

    m_occupiedBB[std::to_underlying(Color::WHITE)] = Bitboards::Bitboard(0xFFFFULL);
    m_occupiedBB[std::to_underlying(Color::BLACK)] = Bitboards::Bitboard(0xFFFF000000000000ULL);

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

bool Position::isSquareAttackedBy(const Square sq, const Color c) const {
    const auto &ourPieces  = occupancies(c);
    const auto &ourPawns   = pieceTypeBB(PieceType::PAWN) & ourPieces;
    const auto &ourKnights = pieceTypeBB(PieceType::KNIGHT) & ourPieces;
    const auto &ourBishops = pieceTypeBB(PieceType::BISHOP) & ourPieces;
    const auto &ourRooks   = pieceTypeBB(PieceType::ROOK) & ourPieces;
    const auto &ourKing    = pieceTypeBB(PieceType::KING) & ourPieces;
    const auto &ourQueens  = pieceTypeBB(PieceType::QUEEN) & ourPieces;

    const auto &blockers = occupancies(Color::WHITE) | occupancies(Color::BLACK);

    if (Bitboards::Attacks::getPawnAttacks(sq, ~c) & ourPawns)
        return true;

    if (Bitboards::Attacks::getKnightAttacks(sq) & ourKnights)
        return true;

    if (Bitboards::Attacks::getBishopAttacks(sq, blockers) & ourBishops)
        return true;

    if (Bitboards::Attacks::getRookAttacks(sq, blockers) & ourRooks)
        return true;

    if (Bitboards::Attacks::getKingAttacks(sq) & ourKing)
        return true;

    if (Bitboards::Attacks::getQueenAttacks(sq, blockers) & ourQueens)
        return true;

    return false;
}

bool Position::isValid() const {
    if (pieceTypeBB(PieceType::KING).bitCount() != 2) {
        std::cerr << std::format("There must be 2 kings on the board.") << std::endl;
        return false;
    }

    const auto &whiteOccupancies = occupancies(Color::WHITE);

    if (whiteOccupancies.bitCount() > 16) {
        std::cerr << std::format("White must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto &blackOccupancies = occupancies(Color::BLACK);

    if (blackOccupancies.bitCount() > 16) {
        std::cerr << std::format("Black must have 16 or less pieces.") << std::endl;
        return false;
    }

    const auto &pawns = pieceTypeBB(PieceType::PAWN);

    if ((pawns & whiteOccupancies).bitCount() > 8) {
        std::cerr << std::format("White must have 8 or less pawns.") << std::endl;
        return false;
    }

    if ((pawns & blackOccupancies).bitCount() > 8) {
        std::cerr << std::format("Black must have 8 or less pawns.") << std::endl;
        return false;
    }

    if (isSquareAttackedBy(kingSquare(~m_stm), m_stm)) {
        std::cerr << std::format(
            "The king of the player whose turn it is not to move must not be in check.")
                  << std::endl;
        return false;
    }

    return true;
}

bool Position::wasLegal() const { return !isSquareAttackedBy(kingSquare(~m_stm), m_stm); }

std::string Position::toFen() const {
    std::string fen;

    for (int rank = 7; rank >= 0; --rank) {
        u16 emptySquares = 0;

        for (int file = 0; file < 8; ++file) {
            const Square sq           = squareOf(file, rank);
            const Piece  currentPiece = pieceOn(sq);

            if (currentPiece != Piece::NONE) {
                if (emptySquares > 0)
                    fen += std::to_string(emptySquares);
                fen += Pieces::pieceToChar(currentPiece);
                emptySquares = 0;
            }
            else
                ++emptySquares;
        }

        if (emptySquares > 0)
            fen += std::to_string(emptySquares);

        fen += rank == 0 ? ' ' : '/';
    }

    fen += std::format("{} {} {} {} {}", m_stm == Color::WHITE ? "w" : "b",
                       castlingRights().toString(),
                       m_epSq != Square::NONE ? Util::sqToCoords[std::to_underlying(m_epSq)] : "-",
                       m_halfMoveClock, m_fullMoveNumber);

    return fen;
}

void printBoard(const Position &pos) {
    std::cout << std::format("\n+---+---+---+---+---+---+---+---+") << std::endl;

    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            const auto  sq           = squareOf(file, rank);
            const Piece currentPiece = pos.pieceOn(sq);
            std::cout << std::format(
                "| {}", currentPiece == Piece::NONE ? ' ' : Pieces::pieceToChar(currentPiece));

            if (file != 7)
                std::cout << std::format(" ");
        }
        std::cout << std::format(" | {}\n+---+---+---+---+---+---+---+---+", rank + 1) << std::endl;
    }

    std::cout << std::format("  A   B   C   D   E   F   G   H\n") << std::endl;
    std::cout << std::format("Side to move    : {}",
                             pos.sideToMove() == Color::WHITE ? "white" : "black")
              << std::endl;

    const auto enpassant = pos.epSquare();

    std::cout << std::format(
        "En passant      : {}",
        enpassant != Square::NONE ? Util::sqToCoords[std::to_underlying(enpassant)] : "-")
              << std::endl;
    std::cout << std::format("Castling rights : {}", pos.castlingRights().toString()) << std::endl;
    std::cout << std::format("Halfmove clock  : {}", pos.fiftyMoveRule()) << std::endl;
    std::cout << std::format("Fullmove number : {}", pos.fullMoves()) << std::endl;
    std::cout << std::format("FEN             : {}\n", pos.toFen()) << std::endl;
}

} // namespace Board