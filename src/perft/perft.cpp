#include "perft.hpp"

#include <print>

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
    std::println("\nRunning performance test...\n");

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

        std::println("{0}: {1}", currentMove.toString(), value);
    }

    const auto elapsed = Utils::getTimeMs() - startTime;

    std::println("\nDepth           : {}", depth);
    std::println("Total nodes     : {}", totalNodes);
    std::println("Total time      : {} ms", elapsed);
    std::println("Nodes per second: {}\n", totalNodes / std::max(1UL, elapsed) * 1000);
}