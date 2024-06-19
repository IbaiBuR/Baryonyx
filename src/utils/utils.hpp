#pragma once

#include <chrono>
#include <random>

#include "../types.hpp"

namespace Utils {

/// @brief Generates a pseudo-random unsigned 64-bit number using a Mersenne Twister PRNG
/// with a fixed seed
inline u64 randomU64() {
    static std::mt19937_64                    prng(979484151);
    static std::uniform_int_distribution<u64> dis;

    return dis(prng);
}

inline u64 randomMagic() { return randomU64() & randomU64() & randomU64(); }

} // namespace Utils