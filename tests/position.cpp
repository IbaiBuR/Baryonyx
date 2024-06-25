#include "../src/board/position.hpp"
#include "doctest/doctest.hpp"

#include <stdexcept>

using namespace board;
using namespace moves;

TEST_SUITE("Position Tests") {
    TEST_CASE("fen parsing") {
        SUBCASE("start position") {
            position pos(util::start_pos_fen);

            CHECK_EQ(pos.piece_type_bb(piece_type::pawn), bitboards::bitboard(0xFF00000000FF00ULL));
            CHECK_EQ(pos.piece_type_bb(piece_type::knight),
                     bitboards::bitboard(0x4200000000000042ULL));
            CHECK_EQ(pos.piece_type_bb(piece_type::king),
                     bitboards::bitboard(0x1000000000000010ULL));
            CHECK_EQ(pos.piece_type_bb(piece_type::bishop),
                     bitboards::bitboard(0x2400000000000024ULL));
            CHECK_EQ(pos.piece_type_bb(piece_type::rook),
                     bitboards::bitboard(0x8100000000000081ULL));
            CHECK_EQ(pos.piece_type_bb(piece_type::queen),
                     bitboards::bitboard(0x800000000000008ULL));

            CHECK_EQ(pos.side_to_move(), color::white);
            CHECK_EQ(pos.castling(), castling_rights(castling_rights::castling_flag::all));
            CHECK_EQ(pos.ep_square(), square::none);
            CHECK_EQ(pos.fifty_move_rule(), 0);
            CHECK_EQ(pos.full_moves(), 1);
            CHECK_EQ(pos.checkers(), bitboards::util::empty_bb);
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
            CHECK_EQ(position(util::start_pos_fen).to_fen(), util::start_pos_fen);
        }

        SUBCASE("fen 1") {
            constexpr auto input_fen =
                "r1bqk1nr/p1p3bp/2n3p1/1p1pPp2/4P3/P1P2NP1/1P3P1P/RNBQKB1R w KQkq f6 0 8";

            CHECK_EQ(position(input_fen).to_fen(), input_fen);
        }

        SUBCASE("fen 2") {
            constexpr auto input_fen =
                "r2q1knr/1bp3bp/p1n3p1/1p1pPp2/1P2P3/P1P2NP1/3N1PBP/R1BQK1R1 b Q - 0 11";

            CHECK_EQ(position(input_fen).to_fen(), input_fen);
        }
    }

    TEST_CASE("zobrist hashing") {
        SUBCASE("start position") {
            CHECK_EQ(position(util::start_pos_fen).key(), 0x63FB272195DEE353ULL);
        }

        SUBCASE("after double push") {
            position pos(util::start_pos_fen);
            pos.make_move<false>(move(square::e2, square::e4, move::move_flag::double_push));

            CHECK_EQ(pos.key(),
                     position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1").key());
        }

        SUBCASE("after castling") {
            position pos("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
            pos.make_move<false>(move(square::e1, square::g1, move::move_flag::castle));

            CHECK_EQ(pos.key(),
                     position("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4")
                         .key());
        }

        SUBCASE("after en passant") {
            position pos("rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
            pos.make_move<false>(move(square::e5, square::d6, move::move_flag::en_passant));

            CHECK_EQ(pos.key(),
                     position("rnbqkbnr/ppp2ppp/3Pp3/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3").key());
        }
    }

    TEST_CASE("repetition detection") {
        SUBCASE("position 1 ") {
            position pos("4r1k1/p2r1p2/1p3BpQ/2p5/4p1PP/2P3qB/PP6/5R1K b - - 5 30");

            pos.make_move<true>(move(square::g3, square::h3, move::move_flag::capture));
            pos.make_move<true>(move(square::h1, square::g1, move::move_flag::quiet));
            pos.make_move<true>(move(square::h3, square::g3, move::move_flag::quiet));
            pos.make_move<true>(move(square::g1, square::h1, move::move_flag::quiet));
            pos.make_move<true>(move(square::g3, square::h3, move::move_flag::quiet));
            pos.make_move<true>(move(square::h1, square::g1, move::move_flag::quiet));
            pos.make_move<true>(move(square::h3, square::g3, move::move_flag::quiet));

            CHECK_EQ(pos.has_repeated(), true);
        }

        SUBCASE("position 2") {
            position pos("r3kr2/p1R5/1p5Q/7p/3P3q/P1P5/1P4P1/R6K w - - 0 30");

            pos.make_move<true>(move(square::h1, square::g1, move::move_flag::quiet));
            pos.make_move<true>(move(square::h4, square::f2, move::move_flag::quiet));
            pos.make_move<true>(move(square::g1, square::h1, move::move_flag::quiet));
            pos.make_move<true>(move(square::f2, square::h4, move::move_flag::quiet));
            pos.make_move<true>(move(square::h1, square::g1, move::move_flag::quiet));
            pos.make_move<true>(move(square::h4, square::f2, move::move_flag::quiet));
            pos.make_move<true>(move(square::g1, square::h1, move::move_flag::quiet));

            CHECK_EQ(pos.has_repeated(), true);
        }
    }
}