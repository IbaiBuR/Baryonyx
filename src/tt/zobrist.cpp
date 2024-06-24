#include "zobrist.hpp"

namespace tt::zobrist {

/// @brief Generates the Zobrist keys at compile time
/// @tparam T Type of the array
/// @tparam Size Size of the array
/// @returns An array of random Zobrist keys
template <typename T, usize Size>
consteval std::array<T, Size> generate_keys() {
    std::array<T, Size>      keys{};
    utils::random::sfc64_rng prng{};

    std::ranges::for_each(keys.begin(), keys.end(), [&](auto& key) { key = prng.next_u64(); });

    return keys;
}

constexpr std::array<zobrist_key, total_keys> all_keys = generate_keys<zobrist_key, total_keys>();

} // namespace tt::zobrist
