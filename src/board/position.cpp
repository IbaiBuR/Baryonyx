#include "position.hpp"

#include <iostream>
#include <print>

#include "bitboard/attacks.hpp"
#include "../utils.hpp"

namespace Board {

Position::Position(const std::string &fen) {
    this->pieces.fill(Piece::NO_PIECE);
    const auto tokens = Utils::splitString(fen, ' ');

    if (tokens.size() > 6)
        throw std::invalid_argument("Invalid FEN string: too many fields.\n");
    if (tokens.size() < 6)
        throw std::invalid_argument("Invalid FEN string: missing fields.\n");

    const auto ranks = Utils::splitString(tokens[0], '/');

    U8 rankIndex = 0;

    for (const auto &rank : ranks) {
        if (rankIndex >= 8)
            throw std::invalid_argument("Invalid FEN string: too many ranks.\n");

        U8 fileIndex = 0;

        for (const auto c : rank) {
            if (fileIndex >= 8)
                throw std::invalid_argument("Invalid FEN string: too many files.\n");

            if (std::isdigit(c))
                fileIndex += c - '0';
            else {
                const auto  sq         = Bitboards::Util::squareOf(fileIndex, rankIndex);
                const Color pieceColor = std::islower(c) ? Color::BLACK : Color::WHITE;
                const Piece piece      = Pieces::charToPiece.at(c);
                setPiece(piece, sq, pieceColor);
                ++fileIndex;
            }
        }
        ++rankIndex;
    }

    this->stm      = tokens[1] == "w" ? Color::WHITE : Color::BLACK;
    this->castling = CastlingRights(tokens[2]);

    const std::string &enpassant = tokens[3];

    this->epSq = enpassant == "-"
                   ? Square::NO_SQ
                   : Bitboards::Util::squareOf(enpassant[0] - 'a', 8 - (enpassant[1] - '0'));

    this->halfMoveClock  = std::stoi(tokens[4]);
    this->fullMoveNumver = std::stoi(tokens[5]);

    assert(this->isValid());
}

Bitboards::Bitboard Position::attacksToKing(const Square kingSquare, const Color c) const {
    const auto &oppOccupancies   = occupiedBB[std::to_underlying(c) ^ 1];
    const auto &oppPawns         = pieceTypeBB(PieceType::PAWN) & oppOccupancies;
    const auto &oppKnights       = pieceTypeBB(PieceType::KNIGHT) & oppOccupancies;
    const auto &oppQueens        = pieceTypeBB(PieceType::QUEEN) & oppOccupancies;
    const auto &oppBishopsQueens = (pieceTypeBB(PieceType::BISHOP) & oppOccupancies) | oppQueens;
    const auto &oppRooksQueens   = (pieceTypeBB(PieceType::ROOK) & oppOccupancies) | oppQueens;
    const auto &oppKing          = pieceTypeBB(PieceType::KING) & oppOccupancies;

    const auto &blockers =
        occupiedBB[std::to_underlying(Color::WHITE)] | occupiedBB[std::to_underlying(Color::BLACK)];

    return (Bitboards::Attacks::getPawnAttacks(kingSquare, c) & oppPawns)
         | (Bitboards::Attacks::getKnightAttacks(kingSquare) & oppKnights)
         | (Bitboards::Attacks::getBishopAttacks(kingSquare, blockers) & oppBishopsQueens)
         | (Bitboards::Attacks::getRookAttacks(kingSquare, blockers) & oppRooksQueens)
         | (Bitboards::Attacks::getKingAttacks(kingSquare) & oppKing);
}

Square Position::kingSquare(const Color c) const {
    const auto &kingBB = pieceTypeBB(PieceType::KING) & occupiedBB[std::to_underlying(c)];
    return static_cast<Square>(kingBB.getLSB());
}

void Position::setPiece(const Piece p, const Square sq, const Color c) {
    pieces[std::to_underlying(sq)] = p;
    Bitboards::Bitboard::setBit(pieceBB[std::to_underlying(Pieces::pieceToPieceType.at(p))], sq);
    Bitboards::Bitboard::setBit(occupiedBB[std::to_underlying(c)], sq);
}

bool Position::isValid() const {
    if (const int kingCount = pieceBB[std::to_underlying(PieceType::KING)].bitCount();
        kingCount > 2 || kingCount < 2) {
        std::println(std::cerr, "There must be 2 kings in the board.");
        return false;
    }

    const auto &whiteOccupancies = occupiedBB[std::to_underlying(Color::WHITE)];

    if (whiteOccupancies.bitCount() > 16) {
        std::println(std::cerr, "White must have 16 or less pieces.");
        return false;
    }

    const auto &blackOccupancies = occupiedBB[std::to_underlying(Color::BLACK)];

    if (blackOccupancies.bitCount() > 16) {
        std::println(std::cerr, "Black must have 16 or less pieces.");
        return false;
    }

    const auto &pawns = pieceBB[std::to_underlying(PieceType::PAWN)];

    if ((pawns & whiteOccupancies).bitCount() > 8) {
        std::println(std::cerr, "White must have 8 or less pawns");
        return false;
    }

    if ((pawns & blackOccupancies).bitCount() > 8) {
        std::println(std::cerr, "Black must have 8 or less pawns");
        return false;
    }

    return true;
}

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
        std::println(" | {}\n +---+---+---+---+---+---+---+---+", 8 - rank);
    }

    std::println("   A   B   C   D   E   F   G   H\n");
    std::println(" Side to move    : {}", pos.sideToMove() == Color::WHITE ? "white" : "black");

    const auto enpassant = pos.epSquare();

    std::println(" En passant      : {}",
                 enpassant != Square::NO_SQ
                     ? Util::squareToCoordinates[std::to_underlying(enpassant)]
                     : "-");
    std::println(" Castling rights : {}", pos.castlingRights().toString());
    std::println(" Halfmove clock  : {}", pos.fiftyMoveRule());
    std::println(" Fullmove number : {}", pos.fullMoves());
}


} // namespace Board