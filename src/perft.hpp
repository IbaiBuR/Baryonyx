#pragma once

#include "chess.hpp"
#include "board/position.hpp"

U64  perft(const Board::Position &pos, int depth);
void splitPerft(const Board::Position &pos, int depth);
