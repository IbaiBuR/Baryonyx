#include "movelist.hpp"

#include <format>
#include <iostream>

namespace moves {

void print_move_list(const move_list& move_list) {
    for (u32 i = 0; i < move_list.size(); ++i)
        std::cout << std::format("{0:3}. Move: {1:<5} Score {2:<5}", i + 1,
                                 move_list.move_at(i).to_string(), move_list.score_at(i))
                  << std::endl;
}

} // namespace moves