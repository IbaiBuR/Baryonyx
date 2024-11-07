#pragma once

#include <charconv>
#include <optional>
#include <string>
#include <type_traits>

namespace utils::parsing {

namespace internal {

template <typename T>
concept NumberOutput = std::is_integral_v<T> || std::is_floating_point_v<T>;

} // namespace internal

template <internal::NumberOutput Output>
constexpr std::optional<Output> to_number(const std::string& s) {
    Output     value{};
    const auto result = std::from_chars(s.data(), s.data() + s.size(), value);

    if (result.ec == std::errc())
        return value;

    return std::nullopt;
}

} // namespace utils::parsing
