#include "perft.hpp"

#include <format>
#include <iostream>

#include "../moves/movegen.hpp"
#include "../moves/movelist.hpp"
#include "../utils/time.hpp"

u64 perft(const board::position& pos, const int depth) {
    if (depth == 0)
        return 1ULL;

    u64              nodes = 0ULL;
    moves::move_list move_list;
    generate_all_moves(pos, move_list);

    for (u32 i = 0; i < move_list.size(); ++i) {
        board::position copy = pos;
        copy.make_move<false>(move_list.move_at(i));

        if (!copy.was_legal())
            continue;

        nodes += perft(copy, depth - 1);
    }

    return nodes;
}

void split_perft(const board::position& pos, const int depth) {
    std::cout << std::format("\nRunning performance test...\n") << std::endl;

    const auto       base_pos = board::position::copy_without_hash_history(pos);
    moves::move_list move_list;
    generate_all_moves(base_pos, move_list);

    u64       total_nodes = 0ULL;
    const u64 start_time  = utils::time::get_time_ms();

    for (u32 i = 0; i < move_list.size(); ++i) {
        auto       copy         = base_pos;
        const auto current_move = move_list.move_at(i);
        copy.make_move<false>(current_move);

        if (!copy.was_legal())
            continue;

        const u64 value = perft(copy, depth - 1);
        total_nodes += value;

        std::cout << std::format("{}: {}", current_move.to_string(), value) << std::endl;
    }

    const auto elapsed = utils::time::get_time_ms() - start_time;

    std::cout << std::format("\nDepth           : {}", depth) << std::endl;
    std::cout << std::format("Total nodes     : {}", total_nodes) << std::endl;
    std::cout << std::format("Total time      : {} ms", elapsed) << std::endl;
    std::cout << std::format("Nodes per second: {}\n",
                             total_nodes / std::max<u64>(1, elapsed) * 1000)
              << std::endl;
}
