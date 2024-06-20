#include "bench.hpp"

#include <format>
#include <iostream>

#include "../utils/time.hpp"

void Search::Bench::run(Searcher &searcher, const u32 depth) {
    // clang-format off
    const std::array benchFens = {
        #include "./resources/bench.csv"
    };
    // clang-format on

    const u64 startTime = Utils::Time::getTimeMs();
    u64       totalNodes{};
    searcher.setLimits(UINT64_MAX, UINT64_MAX, depth);

    for (const auto &fen : benchFens) {
        Board::Position pos(fen);

        searcher.mainSearch(pos);

        totalNodes += searcher.searchedNodes();
    }

    const u64  elapsed = Utils::Time::getTimeMs() - startTime;
    const auto seconds = elapsed / 1000;

    std::cout << std::format("\ninfo string {} seconds", seconds) << std::endl;
    std::cout << std::format("{} nodes {} nps", totalNodes, totalNodes / seconds) << std::endl;
}
