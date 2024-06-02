#pragma once

#include "../board/position.hpp"

namespace Eval {

constexpr std::array gamePhaseIncrements = {0, 1, 1, 2, 4, 0};
constexpr int        maxGamePhase        = 24;

Score evaluate(const Board::Position &pos);

} // namespace Eval