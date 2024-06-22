#include "movelist.hpp"

#include <format>
#include <iostream>

namespace moves {

void printMoveList(const MoveList& moveList) {
    for (u32 i = 0; i < moveList.size(); ++i)
        std::cout << std::format("{0:3}. Move: {1:<5} Score {2:<5}", i + 1,
                                 moveList.move_at(i).to_string(), moveList.score_at(i))
                  << std::endl;
}

} // namespace moves