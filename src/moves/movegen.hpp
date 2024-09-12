#pragma once

#include "movelist.hpp"

#include "../board/position.hpp"

namespace moves {

void generate_all_quiets(const board::position& pos, move_list& move_list);
void generate_all_captures(const board::position& pos, move_list& move_list);
void generate_all_moves(const board::position& pos, move_list& move_list);

} // namespace moves