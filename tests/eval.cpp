#include "../src/eval/evalterms.hpp"
#include "doctest/doctest.hpp"

using namespace Eval;

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
}