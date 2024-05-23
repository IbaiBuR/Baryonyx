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

    if (tokens.size() > 6)
        throw std::invalid_argument("Invalid FEN string: too many fields.\n");
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


bool Position::isSquareAttacked(const Square sq, const Color c) const {
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

    if (isSquareAttacked(kingSquare(static_cast<Color>(std::to_underlying(this->stm) ^ 1)),
                         this->stm)) {
        std::println(std::cerr,
                     "The king of the player whose turn it is not to move must not be in check.");
        return false;
    }

    return true;
}

bool Position::wasLegal() const { return !isSquareAttackedBy(kingSquare(~stm), stm); }

void printBoard(const Position &pos) {
    std::println("\n +---+---+---+---+---+---+---+---+");

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            const auto  sq           = Bitboards::Util::squareOf(file, rank);
            const Piece currentPiece = pos.pieceOn(sq);
            std::print(" | {}", currentPiece == Piece::NO_PIECE
                                    ? ' '
                                    : Pieces::pieceToChar[std::to_underlying(currentPiece)]);
        }
        std::println(" | {}\n +---+---+---+---+---+---+---+---+", rank + 1);
    }

    std::println("   A   B   C   D   E   F   G   H\n");
    std::println(" Side to move    : {}", pos.sideToMove() == Color::WHITE ? "white" : "black");

    const auto enpassant = pos.epSquare();

    std::println(" En passant      : {}", enpassant != Square::NO_SQ
                                              ? Util::sqToCoords[std::to_underlying(enpassant)]
                                              : "-");
    std::println(" Castling rights : {}", pos.castlingRights().toString());
    std::println(" Halfmove clock  : {}", pos.fiftyMoveRule());
    std::println(" Fullmove number : {}", pos.fullMoves());
}


} // namespace Board