#include "zobrist.hpp"

namespace tt::zobrist {

/// @brief Generates the Zobrist keys at compile time
/// @tparam T Type of the array
/// @tparam size Size of the array
/// @returns An array of random Zobrist keys
template <typename T, usize size>
consteval std::array<T, size> generate_keys() {
    std::array<T, size>     keys{};
    utils::random::Sfc64Rng prng{};

    std::ranges::for_each(keys.begin(), keys.end(), [&](auto& key) { key = prng.next_u64(); });

    return keys;
}

constexpr std::array<ZobristKey, total_keys> allKeys = generate_keys<ZobristKey, total_keys>();

} // namespace tt::zobrist
