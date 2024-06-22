#pragma once

#include "../board/position.hpp"

namespace eval {

constexpr std::array gamePhaseIncrements = {0, 1, 1, 2, 4, 0};
constexpr int        maxGamePhase        = 24;

Score evaluate(const board::Position& pos);

} // namespace eval