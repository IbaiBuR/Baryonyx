#pragma once

#include <cassert>
#include <cmath>
#include <string>

#include "../chess.hpp"

namespace utils::score {

constexpr bool is_mate(const i32 score) {
    return constants::score_mate - std::abs(score) <= constants::max_ply;
}

constexpr int mate_in(const i32 score) {
    assert(is_mate(score));

    return (score > 0 ? constants::score_mate - score + 1 : -constants::score_mate - score) / 2;
}

inline std::string to_string(const i32 score) {
    return is_mate(score) ? "mate " + std::to_string(mate_in(score))
                          : "cp " + std::to_string(score);
}

} // namespace utils::score