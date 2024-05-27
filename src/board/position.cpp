#include "position.hpp"

#include <iostream>
#include <print>

#include "bitboard/attacks.hpp"
#include "piece.hpp"
#include "../utils.hpp"

namespace Board {

Position::Position(const std::string &fen) :
    pieces() {
    this->pieces.fill(Piece::NO_PIECE);
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

        U8 fileIndex = 0;

        for (const auto c : rank) {
            if (fileIndex >= 8)
                throw std::invalid_argument("Invalid FEN string: too many files.\n");

            if (std::isdigit(c))
                fileIndex += c - '0';
            else {
                const auto  sq    = Bitboards::Util::squareOf(fileIndex, rankIndex);
                const Piece piece = Pieces::charToPiece.at(c);
                setPiece(piece, sq);
                ++fileIndex;
            }
        }
        --rankIndex;
    }

    this->stm      = tokens[1] == "w" ? Color::WHITE : Color::BLACK;
    this->castling = CastlingRights(tokens[2]);

    const std::string &enpassant = tokens[3];

    this->epSq = enpassant == "-"
                   ? Square::NO_SQ
                   : Bitboards::Util::squareOf(enpassant[0] - 'a', enpassant[1] - 1 - '0');

    this->halfMoveClock  = std::stoi(tokens[4]);
    this->fullMoveNumver = std::stoi(tokens[5]);

    this->checkersBB = attacksToKing(kingSquare(this->stm), this->stm);

    if (!this->isValid())
        throw std::invalid_argument("Invalid FEN string: illegal position.\n");
}

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
    pieces[std::to_underlying(sq)] = p;
    Bitboards::Bitboard::setBit(
        pieceBB[std::to_underlying(Pieces::pieceToPieceType[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::setBit(
        occupiedBB[std::to_underlying(Pieces::pieceColor[std::to_underlying(p)])], sq);
}

void Position::removePiece(const Piece p, const Square sq) {
    pieces[std::to_underlying(sq)] = Piece::NO_PIECE;
    Bitboards::Bitboard::clearBit(
        pieceBB[std::to_underlying(Pieces::pieceToPieceType[std::to_underlying(p)])], sq);
    Bitboards::Bitboard::clearBit(
        occupiedBB[std::to_underlying(Pieces::pieceColor[std::to_underlying(p)])], sq);
}

void Position::movePiece(const Piece p, const Square from, const Square to) {
    removePiece(p, from);
    setPiece(p, to);
}

void Position::makeMove(const Moves::Move move) {
    ++halfMoveClock;

    const Square    from        = move.from();
    const Square    to          = move.to();
    const Piece     movingPiece = pieceOn(from);
    const Direction offset      = stm == Color::WHITE ? Direction::NORTH : Direction::SOUTH;

    if (move.isCapture()) {
        const auto targetSquare = move.isEnPassant() ? epSq - offset : to;
        removePiece(pieceOn(targetSquare), targetSquare);
        halfMoveClock = 0;
    }

    removePiece(movingPiece, from);
    setPiece(move.isPromotion() ? move.getPromotedPiece(stm) : movingPiece, to);

    if (move.isDoublePush())
        epSq = to - offset;
    else
        epSq = Square::NO_SQ;

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

    castling &= Util::castlingRightsUpdate[std::to_underlying(from)];
    castling &= Util::castlingRightsUpdate[std::to_underlying(to)];

    fullMoveNumver += stm == Color::BLACK;

    if (Pieces::pieceToPieceType[std::to_underlying(movingPiece)] == PieceType::PAWN)
        halfMoveClock = 0;

    stm        = ~stm;
    checkersBB = attacksToKing(kingSquare(stm), stm);
}

void Position::resetToStartPos() {
    checkersBB     = Bitboards::Util::EmptyBB;
    stm            = Color::WHITE;
    epSq           = Square::NO_SQ;
    castling       = CastlingRights(CastlingRights::Flags::ALL);
    halfMoveClock  = 0;
    fullMoveNumver = 1;

    pieceBB[std::to_underlying(PieceType::PAWN)]   = Bitboards::Bitboard(0xFF00000000FF00ULL);
    pieceBB[std::to_underlying(PieceType::KNIGHT)] = Bitboards::Bitboard(0x4200000000000042ULL);
    pieceBB[std::to_underlying(PieceType::BISHOP)] = Bitboards::Bitboard(0x2400000000000024ULL);
    pieceBB[std::to_underlying(PieceType::ROOK)]   = Bitboards::Bitboard(0x8100000000000081ULL);
    pieceBB[std::to_underlying(PieceType::QUEEN)]  = Bitboards::Bitboard(0x800000000000008ULL);
    pieceBB[std::to_underlying(PieceType::KING)]   = Bitboards::Bitboard(0x1000000000000010ULL);

    occupiedBB[std::to_underlying(Color::WHITE)] = Bitboards::Bitboard(0xFFFFULL);
    occupiedBB[std::to_underlying(Color::BLACK)] = Bitboards::Bitboard(0xFFFF000000000000ULL);

    pieces.fill(Piece::NO_PIECE);

    for (U8 sq = std::to_underlying(Square::A2); sq <= std::to_underlying(Square::H2); ++sq)
        pieces[sq] = Piece::W_PAWN;

    for (U8 sq = std::to_underlying(Square::A7); sq <= std::to_underlying(Square::H7); ++sq)
        pieces[sq] = Piece::B_PAWN;

    pieces[std::to_underlying(Square::B1)] = pieces[std::to_underlying(Square::G1)] =
        Piece::W_KNIGHT;

    pieces[std::to_underlying(Square::B8)] = pieces[std::to_underlying(Square::G8)] =
        Piece::B_KNIGHT;

    pieces[std::to_underlying(Square::C1)] = pieces[std::to_underlying(Square::F1)] =
        Piece::W_BISHOP;

    pieces[std::to_underlying(Square::C8)] = pieces[std::to_underlying(Square::F8)] =
        Piece::B_BISHOP;

    pieces[std::to_underlying(Square::A1)] = pieces[std::to_underlying(Square::H1)] = Piece::W_ROOK;
    pieces[std::to_underlying(Square::A8)] = pieces[std::to_underlying(Square::H8)] = Piece::B_ROOK;

    pieces[std::to_underlying(Square::D1)] = Piece::W_QUEEN;
    pieces[std::to_underlying(Square::D8)] = Piece::B_QUEEN;

    pieces[std::to_underlying(Square::E1)] = Piece::W_KING;
    pieces[std::to_underlying(Square::E8)] = Piece::B_KING;
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
        std::println(std::cerr, "There must be 2 kings on the board.");
        return false;
    }

    const auto &whiteOccupancies = occupancies(Color::WHITE);

    if (whiteOccupancies.bitCount() > 16) {
        std::println(std::cerr, "White must have 16 or less pieces.");
        return false;
    }

    const auto &blackOccupancies = occupancies(Color::BLACK);

    if (blackOccupancies.bitCount() > 16) {
        std::println(std::cerr, "Black must have 16 or less pieces.");
        return false;
    }

    const auto &pawns = pieceTypeBB(PieceType::PAWN);

    if ((pawns & whiteOccupancies).bitCount() > 8) {
        std::println(std::cerr, "White must have 8 or less pawns.");
        return false;
    }

    if ((pawns & blackOccupancies).bitCount() > 8) {
        std::println(std::cerr, "Black must have 8 or less pawns.");
        return false;
    }

    if (isSquareAttackedBy(kingSquare(~stm), stm)) {
        std::println(std::cerr,
                     "The king of the player whose turn it is not to move must not be in check.");
        return false;
    }

    return true;
}

bool Position::wasLegal() const { return !isSquareAttackedBy(kingSquare(~stm), stm); }

std::string Position::toFen() const {
    std::string fen;

    for (int rank = 7; rank >= 0; --rank) {
        U16 emptySquares = 0;

        for (int file = 0; file < 8; ++file) {
            const Square sq           = Bitboards::Util::squareOf(file, rank);
            const Piece  currentPiece = pieceOn(sq);

            if (currentPiece != Piece::NO_PIECE) {
                if (emptySquares > 0)
                    fen += std::to_string(emptySquares);
                fen += Pieces::pieceToChar[std::to_underlying(currentPiece)];
                emptySquares = 0;
            }
            else
                ++emptySquares;
        }

        if (emptySquares > 0)
            fen += std::to_string(emptySquares);

        fen += rank == 0 ? ' ' : '/';
    }

    fen +=
        std::format("{} {} {} {} {}", stm == Color::WHITE ? "w" : "b", castlingRights().toString(),
                    epSq != Square::NO_SQ ? Util::sqToCoords[std::to_underlying(epSq)] : "-",
                    halfMoveClock, fullMoveNumver);

    return fen;
}


void printBoard(const Position &pos) {
    std::println("\n+---+---+---+---+---+---+---+---+");

    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            const auto  sq           = Bitboards::Util::squareOf(file, rank);
            const Piece currentPiece = pos.pieceOn(sq);
            std::print("| {}", currentPiece == Piece::NO_PIECE
                                   ? ' '
                                   : Pieces::pieceToChar[std::to_underlying(currentPiece)]);

            if (file != 7)
                std::print(" ");
        }
        std::println(" | {}\n+---+---+---+---+---+---+---+---+", rank + 1);
    }

    std::println("  A   B   C   D   E   F   G   H\n");
    std::println("Side to move    : {}", pos.sideToMove() == Color::WHITE ? "white" : "black");

    const auto enpassant = pos.epSquare();

    std::println("En passant      : {}", enpassant != Square::NO_SQ
                                             ? Util::sqToCoords[std::to_underlying(enpassant)]
                                             : "-");
    std::println("Castling rights : {}", pos.castlingRights().toString());
    std::println("Halfmove clock  : {}", pos.fiftyMoveRule());
    std::println("Fullmove number : {}", pos.fullMoves());
    std::println("FEN             : {}\n", pos.toFen());
}

} // namespace Board