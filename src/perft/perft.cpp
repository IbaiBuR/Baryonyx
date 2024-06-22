#include "perft.hpp"

#include <format>
#include <iostream>

#include "../moves/movegen.hpp"
#include "../moves/movelist.hpp"
#include "../utils/time.hpp"

u64 perft(const board::Position& pos, const int depth) {
    if (depth == 0)
        return 1ULL;

    u64             nodes = 0ULL;
    moves::MoveList moveList;
    generate_all_moves(pos, moveList);

    for (u32 i = 0; i < moveList.size(); ++i) {
        board::Position copy = pos;
        copy.make_move(moveList.move_at(i));

        if (!copy.was_legal())
            continue;

        nodes += perft(copy, depth - 1);
    }

    return nodes;
}

void split_perft(const board::Position& pos, const int depth) {
    std::cout << std::format("\nRunning performance test...\n") << std::endl;

    moves::MoveList moveList;
    generate_all_moves(pos, moveList);

    u64        totalNodes = 0ULL;
    const auto startTime  = utils::time::get_time_ms();

    for (u32 i = 0; i < moveList.size(); ++i) {
        board::Position   copy        = pos;
        const moves::Move currentMove = moveList.move_at(i);
        copy.make_move(currentMove);

        if (!copy.was_legal())
            continue;

        const u64 value = perft(copy, depth - 1);
        totalNodes += value;

        std::cout << std::format("{}: {}", currentMove.to_string(), value) << std::endl;
    }

    const auto elapsed = utils::time::get_time_ms() - startTime;

    std::cout << std::format("\nDepth           : {}", depth) << std::endl;
    std::cout << std::format("Total nodes     : {}", totalNodes) << std::endl;
    std::cout << std::format("Total time      : {} ms", elapsed) << std::endl;
    std::cout << std::format("Nodes per second: {}\n",
                             totalNodes / std::max<u64>(1, elapsed) * 1000)
              << std::endl;
}
