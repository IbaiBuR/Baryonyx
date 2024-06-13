#pragma once

#include <chrono>
#include <random>
#include <ranges>
#include <string>
#include <vector>

#include "types.hpp"

namespace Utils {

/// @brief Splits a string based on a delimiter
/// @param str String to split
/// @param delim Delimiter
/// @returns A vector of strings
inline std::vector<std::string> splitString(const std::string &str, const char delim) {
    std::vector<std::string> tokens;

    for (const auto token : std::views::split(str, delim))
        tokens.emplace_back(std::string_view(token));

    return tokens;
}

inline u64 getTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

/// @brief Generates a pseudo-random unsigned 64-bit number using a Mersenne Twister PRNG
/// with a fixed seed
inline u64 randomU64() {
    static std::mt19937_64                    prng(979484151);
    static std::uniform_int_distribution<u64> dis;

    return dis(prng);
}

inline u64 randomMagic() { return randomU64() & randomU64() & randomU64(); }

} // namespace Utils