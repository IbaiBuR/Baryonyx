#include "../src/eval/eval.hpp"
#include "doctest/doctest.hpp"

using namespace eval;

TEST_SUITE("Eval Tests") {
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