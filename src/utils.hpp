#pragma once

#include <chrono>
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

inline U64 getTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

} // namespace Utils