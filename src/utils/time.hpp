#pragma once

#include <chrono>

#include "../types.hpp"

namespace utils::time {

inline u64 get_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

} // namespace utils::time