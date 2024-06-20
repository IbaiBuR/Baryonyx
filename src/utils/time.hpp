#pragma once

#include <chrono>

#include "../types.hpp"

namespace Utils::Time {

inline u64 getTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

} // namespace Utils::Time