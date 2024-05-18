#include "../src/board/position.hpp"
#include "doctest/doctest.hpp"

using namespace Board;

TEST_SUITE("Position Tests") {
    TEST_CASE("fen parsing") {
        SUBCASE("start position") {
            Position pos(Util::startPosFen);

            CHECK(pos.pieceTypeBB(PieceType::PAWN) == Bitboards::Bitboard(0xFF00000000FF00ULL));
            CHECK(pos.pieceTypeBB(PieceType::KNIGHT) == Bitboards::Bitboard(0x4200000000000042ULL));
            CHECK(pos.pieceTypeBB(PieceType::KING) == Bitboards::Bitboard(0x1000000000000010ULL));
            CHECK(pos.pieceTypeBB(PieceType::BISHOP) == Bitboards::Bitboard(0x2400000000000024ULL));
            CHECK(pos.pieceTypeBB(PieceType::ROOK) == Bitboards::Bitboard(0x8100000000000081ULL));
            CHECK(pos.pieceTypeBB(PieceType::QUEEN) == Bitboards::Bitboard(0x800000000000008ULL));

            CHECK(pos.sideToMove() == Color::WHITE);
            CHECK(pos.castlingRights() == CastlingRights(CastlingRights::Flags::ALL));
            CHECK(pos.epSquare() == Square::NO_SQ);
            CHECK(pos.fiftyMoveRule() == 0);
            CHECK(pos.fullMoves() == 1);
            CHECK(pos.checkers() == Bitboards::Util::EmptyBB);
        }
    }
}