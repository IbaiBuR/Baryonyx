#pragma once

#include "../chess.hpp"
#include "../board/position.hpp"

u64  perft(const Board::Position &pos, int depth);
void splitPerft(const Board::Position &pos, int depth);
