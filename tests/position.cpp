#include "../src/board/position.hpp"
#include "doctest/doctest.hpp"

#include <stdexcept>

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
            CHECK(pos.checkers() == Bitboards::Util::EMPTY_BB);
        }

        SUBCASE("illegal position: too many kings") {
            CHECK_THROWS_AS(Position("2k1k3/8/8/8/8/8/2K1K3/8 w - - 0 1"), std::invalid_argument);
        }

        SUBCASE("illegal position: too many pieces") {
            CHECK_THROWS_AS(
                Position("rnbqkbnr/pppppppp/8/8/8/1N4N1/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
                std::invalid_argument);
        }

        SUBCASE("illegal position: too many pawns") {
            CHECK_THROWS_AS(Position("rnbqkbnr/pppppppp/8/8/8/2PPP3/PPPPPPPP/R2QK2R w KQkq - 0 1"),
                            std::invalid_argument);
        }

        SUBCASE("illegal position: side not to move in check") {
            CHECK_THROWS_AS(
                Position("rnbqkbnr/ppp2ppp/8/1B1pp3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 1 3"),
                std::invalid_argument);
        }
    }

    TEST_CASE("board to fen") {
        SUBCASE("start position") {
            CHECK(Position(Util::startPosFen).toFen()
                  == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }

        SUBCASE("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8") {
            CHECK(
                Position("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8")
                    .toFen()
                == "r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8");
        }

        SUBCASE("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11") {
            CHECK(Position("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11")
                      .toFen()
                  == "r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11");
        }
    }
}