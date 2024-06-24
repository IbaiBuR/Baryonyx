#include "../src/board/position.hpp"
#include "doctest/doctest.hpp"

#include <stdexcept>

using namespace board;
using namespace moves;

TEST_SUITE("Position Tests") {
    TEST_CASE("fen parsing") {
        SUBCASE("start position") {
            position pos(util::start_pos_fen);

            CHECK(pos.piece_type_bb(piece_type::pawn) == bitboards::bitboard(0xFF00000000FF00ULL));
            CHECK(pos.piece_type_bb(piece_type::knight)
                  == bitboards::bitboard(0x4200000000000042ULL));
            CHECK(pos.piece_type_bb(piece_type::king)
                  == bitboards::bitboard(0x1000000000000010ULL));
            CHECK(pos.piece_type_bb(piece_type::bishop)
                  == bitboards::bitboard(0x2400000000000024ULL));
            CHECK(pos.piece_type_bb(piece_type::rook)
                  == bitboards::bitboard(0x8100000000000081ULL));
            CHECK(pos.piece_type_bb(piece_type::queen)
                  == bitboards::bitboard(0x800000000000008ULL));

            CHECK(pos.side_to_move() == color::white);
            CHECK(pos.castling() == castling_rights(castling_rights::castling_flag::all));
            CHECK(pos.ep_square() == square::none);
            CHECK(pos.fifty_move_rule() == 0);
            CHECK(pos.full_moves() == 1);
            CHECK(pos.checkers() == bitboards::util::empty_bb);
        }

        SUBCASE("illegal position: too many kings") {
            CHECK_THROWS_AS(position("2k1k3/8/8/8/8/8/2K1K3/8 w - - 0 1"), std::invalid_argument);
        }

        SUBCASE("illegal position: too many pieces") {
            CHECK_THROWS_AS(
                position("rnbqkbnr/pppppppp/8/8/8/1N4N1/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
                std::invalid_argument);
        }

        SUBCASE("illegal position: too many pawns") {
            CHECK_THROWS_AS(position("rnbqkbnr/pppppppp/8/8/8/2PPP3/PPPPPPPP/R2QK2R w KQkq - 0 1"),
                            std::invalid_argument);
        }

        SUBCASE("illegal position: side not to move in check") {
            CHECK_THROWS_AS(
                position("rnbqkbnr/ppp2ppp/8/1B1pp3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 1 3"),
                std::invalid_argument);
        }
    }

    TEST_CASE("board to fen") {
        SUBCASE("start position") {
            CHECK(position(util::start_pos_fen).to_fen()
                  == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }

        SUBCASE("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8") {
            CHECK(
                position("r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8")
                    .to_fen()
                == "r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8");
        }

        SUBCASE("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11") {
            CHECK(position("r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11")
                      .to_fen()
                  == "r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11");
        }
    }

    TEST_CASE("zobrist hashing") {
        SUBCASE("start position") {
            CHECK(position(util::start_pos_fen).key() == 0x63FB272195DEE353ULL);
        }

        SUBCASE("after double push") {
            position pos(util::start_pos_fen);
            pos.make_move(move(square::e2, square::e4, move::move_flag::double_push));

            CHECK(pos.key()
                  == position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1").key());
        }

        SUBCASE("after castling") {
            position pos("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
            pos.make_move(move(square::e1, square::g1, move::move_flag::castle));

            CHECK(pos.key()
                  == position("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4")
                         .key());
        }

        SUBCASE("after en passant") {
            position pos("rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
            pos.make_move(move(square::e5, square::d6, move::move_flag::en_passant));

            CHECK(pos.key()
                  == position("rnbqkbnr/ppp2ppp/3Pp3/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3").key());
        }
    }
}