#pragma once

#include "../chess.hpp"
#include "../board/position.hpp"

u64  perft(const board::Position& pos, int depth);
void split_perft(const board::Position& pos, int depth);
