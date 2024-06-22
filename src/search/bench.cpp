#include "bench.hpp"

#include <format>
#include <iostream>

#include "../utils/time.hpp"

void search::bench::run(Searcher &searcher, const u32 depth) {
    // clang-format off
    const std::array benchFens = {
        #include "./resources/bench.csv"
    };
    // clang-format on

    const u64 startTime = utils::time::get_time_ms();
    u64       totalNodes{};
    searcher.set_limits(UINT64_MAX, UINT64_MAX, depth);

    for (const auto &fen : benchFens) {
        board::Position pos(fen);

        searcher.main_search(pos);

        totalNodes += searcher.searched_nodes();
    }

    const u64  elapsed = utils::time::get_time_ms() - startTime;
    const auto seconds = elapsed / 1000;

    std::cout << std::format("\ninfo string {} seconds", seconds) << std::endl;
    std::cout << std::format("{} nodes {} nps", totalNodes, totalNodes / seconds) << std::endl;
}
