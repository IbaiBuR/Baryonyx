#include "../src/perft/perft.hpp"
#include "../src/utils/split.hpp"
#include "doctest/doctest.hpp"

TEST_SUITE("Perft tests") {
    // clang-format off
    const std::array perft_suite = {
        #include "./resources/perft_suite.txt"
    };
    // clang-format on

    TEST_CASE("perft suite") {
        for (const auto& test : perft_suite) {
            const auto      perft_test = utils::split::split_string(test, ';');
            board::Position test_pos(perft_test[0]);

            for (usize i = 1; i < perft_test.size(); ++i) {
                const auto test_data      = utils::split::split_string(perft_test[i], ' ');
                const int  test_depth     = std::stoi(test_data[0].substr(1));
                const u64  expected_nodes = std::stoi(test_data[1]);

                CHECK(perft(test_pos, test_depth) == expected_nodes);
            }
        }
    }
}