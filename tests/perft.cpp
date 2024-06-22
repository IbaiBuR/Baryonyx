#include "../src/perft/perft.hpp"
#include "../src/utils/split.hpp"
#include "doctest/doctest.hpp"

TEST_SUITE("Perft tests") {
    // clang-format off
    const std::array perftSuite = {
        #include "./resources/perft_suite.txt"
    };
    // clang-format on

    TEST_CASE("perft suite") {
        for (const auto &test : perftSuite) {
            const auto      perftTest = utils::split::splitString(test, ';');
            board::Position testPos(perftTest[0]);

            for (usize i = 1; i < perftTest.size(); ++i) {
                const auto testData      = utils::split::splitString(perftTest[i], ' ');
                const int  testDepth     = std::stoi(testData[0].substr(1));
                const u64  expectedNodes = std::stoi(testData[1]);

                CHECK(perft(testPos, testDepth) == expectedNodes);
            }
        }
    }
}