#include "../src/eval/eval.hpp"
#include "../src/eval/evalterms.hpp"
#include "doctest/doctest.hpp"

using namespace eval;

TEST_SUITE("Eval Tests") {
    TEST_CASE("packed score") {
        SUBCASE("midgame") {
            CHECK(PSQT::pawnTable[0].midgame() == 0);
            CHECK(PSQT::pawnTable[63].midgame() == 0);
            CHECK(PSQT::knightTable[0].midgame() == -167);
            CHECK(PSQT::knightTable[63].midgame() == -23);
        }

        SUBCASE("endgame") {
            CHECK(PSQT::pawnTable[0].endgame() == 0);
            CHECK(PSQT::pawnTable[63].endgame() == 0);
            CHECK(PSQT::knightTable[0].endgame() == -58);
            CHECK(PSQT::knightTable[63].endgame() == -64);
        }
    }

    TEST_CASE("static eval") {
        SUBCASE("start position") {
            CHECK(evaluate(board::Position(board::util::start_pos_fen)) == 0);
        }

        SUBCASE("3k4/8/8/8/8/8/8/3K3R w - - 0 1") {
            CHECK(evaluate(board::Position("3k4/8/8/8/8/8/8/3K3R w - - 0 1")) == 488);
        }

        SUBCASE("3k3r/8/8/8/8/8/8/3K4 w - - 0 1") {
            CHECK(evaluate(board::Position("3k3r/8/8/8/8/8/8/3K4 w - - 0 1")) == -488);
        }

        SUBCASE("3k4/8/8/8/8/3N4/8/3K4 w - - 0 1") {
            CHECK(evaluate(board::Position("3k4/8/8/8/8/3N4/8/3K4 w - - 0 1")) == 298);
        }

        SUBCASE("3k4/8/3n4/8/8/8/8/3K4 w - - 0 1") {
            CHECK(evaluate(board::Position("3k4/8/3n4/8/8/8/8/3K4 w - - 0 1")) == -298);
        }
    }
}