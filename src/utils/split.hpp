#pragma once

#include <ranges>
#include <string>
#include <vector>

namespace utils::split {

/// @brief Splits a string based on a delimiter
/// @param str String to split
/// @param delim Delimiter
/// @returns A vector of strings
inline std::vector<std::string> split_string(const std::string& str, const char delim) {
    std::vector<std::string> tokens;

    for (const auto& token : std::views::split(str, delim))
        tokens.emplace_back(std::string_view(token));

    return tokens;
}

} // namespace utils::split