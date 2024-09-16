#pragma once

#include <cassert>
#include <cmath>

#include "../chess.hpp"

namespace utils::score {

constexpr bool is_mate(const i32 score) {
    return constants::score_mate - std::abs(score) <= constants::max_ply;
}

constexpr int mate_in(const i32 score) {
    assert(is_mate(score));

    return (score > 0 ? constants::score_mate - score + 1 : -constants::score_mate - score) / 2;
}

} // namespace utils::score