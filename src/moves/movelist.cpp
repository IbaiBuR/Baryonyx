#include "movelist.hpp"

#include <print>

namespace Moves {

void printMoveList(const MoveList &moveList) {
    for (U32 i = 0; i < moveList.size(); ++i)
        std::println("{0:3}. Move: {1:<5} Score {2:<5}", i + 1, moveList.moveAt(i).toString(),
                     moveList.scoreAt(i));
}

} // namespace Moves