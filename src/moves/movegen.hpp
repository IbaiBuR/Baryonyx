#pragma once

#include "movelist.hpp"
#include "../board/position.hpp"

namespace Moves {

void generateAllQuiets(const Board::Position &pos, MoveList &moveList);
void generateAllCaptures(const Board::Position &pos, MoveList &moveList);
void generateAllMoves(const Board::Position &pos, MoveList &moveList);

} // namespace Moves