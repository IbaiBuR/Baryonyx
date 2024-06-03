#include "perft.hpp"

#include <format>
#include <iostream>

#include "../moves/movegen.hpp"
#include "../moves/movelist.hpp"
#include "../utils.hpp"

u64 perft(const Board::Position &pos, const int depth) {
    if (depth == 0)
        return 1ULL;

    u64             nodes = 0ULL;
    Moves::MoveList moveList;
    generateAllMoves(pos, moveList);

    for (u32 i = 0; i < moveList.size(); ++i) {
        Board::Position copy = pos;
        copy.makeMove(moveList.moveAt(i));

        if (!copy.wasLegal())
            continue;

        nodes += perft(copy, depth - 1);
    }

    return nodes;
}

void splitPerft(const Board::Position &pos, const int depth) {
    std::cout << std::format("\nRunning performance test...\n") << std::endl;

    Moves::MoveList moveList;
    generateAllMoves(pos, moveList);

    u64        totalNodes = 0ULL;
    const auto startTime  = Utils::getTimeMs();

    for (u32 i = 0; i < moveList.size(); ++i) {
        Board::Position   copy        = pos;
        const Moves::Move currentMove = moveList.moveAt(i);
        copy.makeMove(currentMove);

        if (!copy.wasLegal())
            continue;

        const u64 value = perft(copy, depth - 1);
        totalNodes += value;

        std::cout << std::format("{}: {}", currentMove.toString(), value) << std::endl;
    }

    const auto elapsed = Utils::getTimeMs() - startTime;

    std::cout << std::format("\nDepth           : {}", depth) << std::endl;
    std::cout << std::format("Total nodes     : {}", totalNodes) << std::endl;
    std::cout << std::format("Total time      : {} ms", elapsed) << std::endl;
    std::cout << std::format("Nodes per second: {}\n", totalNodes / std::max(1UL, elapsed) * 1000)
              << std::endl;
}
