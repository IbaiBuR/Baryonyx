#pragma once

#include "../chess.hpp"

#include "../board/position.hpp"

u64  perft(const board::position& pos, int depth);
void split_perft(const board::position& pos, int depth);
