#include "bench.hpp"

#include <format>
#include <iostream>

#include "../utils/time.hpp"

void search::bench::run(searcher& searcher, const u32 depth) {
    // clang-format off
    const std::array bench_fens = {
        #include "./resources/bench.csv"
    };
    // clang-format on

    const u64 start_time = utils::time::get_time_ms();
    u64       total_nodes{};
    searcher.set_limits(UINT64_MAX, UINT64_MAX, depth);

    for (const auto& fen : bench_fens) {
        board::position pos(fen);

        searcher.main_search(pos);

        total_nodes += searcher.searched_nodes();
    }

    const u64  elapsed = utils::time::get_time_ms() - start_time;
    const auto seconds = elapsed / 1000;

    std::cout << std::format("\ninfo string {} seconds", seconds) << std::endl;
    std::cout << std::format("{} nodes {} nps", total_nodes, total_nodes / seconds) << std::endl;
}
