#pragma once

#include "movelist.hpp"
#include "../board/position.hpp"

namespace moves {

void generate_all_quiets(const board::Position& pos, MoveList& move_list);
void generate_all_captures(const board::Position& pos, MoveList& move_list);
void generate_all_moves(const board::Position& pos, MoveList& move_list);

} // namespace moves