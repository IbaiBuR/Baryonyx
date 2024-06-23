#include "../src/board/position.hpp"
#include "doctest/doctest.hpp"

#include <stdexcept>

using namespace board;

TEST_SUITE("Position Tests") {
    TEST_CASE("fen parsing") {
        SUBCASE("start position") {
            Position pos(util::start_pos_fen);

            CHECK(pos.piece_type_bb(PieceType::PAWN) == bitboards::Bitboard(0xFF00000000FF00ULL));
            CHECK(pos.piece_type_bb(PieceType::KNIGHT) == bitboards::Bitboard(0x4200000000000042ULL));
            CHECK(pos.piece_type_bb(PieceType::KING) == bitboards::Bitboard(0x1000000000000010ULL));
            CHECK(pos.piece_type_bb(PieceType::BISHOP) == bitboards::Bitboard(0x2400000000000024ULL));
            CHECK(pos.piece_type_bb(PieceType::ROOK) == bitboards::Bitboard(0x8100000000000081ULL));
            CHECK(pos.piece_type_bb(PieceType::QUEEN) == bitboards::Bitboard(0x800000000000008ULL));

            CHECK(pos.side_to_move() == Color::WHITE);
            CHECK(pos.castling_rights() == CastlingRights(CastlingRights::Flags::ALL));
            CHECK(pos.ep_square() == Square::NONE);
            CHECK(pos.fifty_move_rule() == 0);
            CHECK(pos.full_moves() == 1);
            CHECK(pos.checkers() == bitboards::util::empty_bb);
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
            CHECK(Position(util::start_pos_fen).to_fen()
                  == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }

        SUBCASE("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8") {
            CHECK(
                Position("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8")
                    .to_fen()
                == "r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8");
        }

        SUBCASE("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11") {
            CHECK(Position("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11")
                      .to_fen()
                  == "r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11");
        }
    }
}