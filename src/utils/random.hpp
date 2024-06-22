#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <random>

#include "../types.hpp"

namespace utils::random {

using seed_type = std::array<u64, 3>;

/// @brief Default fixed seed to be used by the PRNG in order to be deterministic
/// @note This seed was generated using the generate_seed function below
constexpr seed_type defaultSeed = {
    {0x4A4380B6E784A3BFULL, 0xF0A5F659292015A9ULL, 0x0158BFEDFD98A85DULL}
};

/// @brief A small chaotic RNG combined with a counter, designed by Chris Doty-Humphrey
/// @note See http://pracrand.sourceforge.net/ for reference and
/// https://github.com/pitdicker/small-rngs/blob/master/src/sfc.rs for the rust implementation
/// from which I took the code
class Sfc64Rng {
    public:
        using result_type = u64;

        constexpr Sfc64Rng() :
            Sfc64Rng(defaultSeed) {}

        constexpr explicit Sfc64Rng(const seed_type &seed) :
            m_a(seed[0]),
            m_b(seed[1]),
            m_c(seed[2]),
            m_counter(1ULL) {

            for (int i = 0; i < 18; ++i) {
                next_u64();
            }
        }

        constexpr result_type next_u64() {
            static constexpr u32 barrel_shift = 24;
            static constexpr u32 r_shift      = 11;
            static constexpr u32 l_shift      = 3;

            const u64 tmp = m_a + m_b + m_counter;

            ++m_counter;

            m_a = m_b ^ (m_b >> r_shift);
            m_b = m_c + (m_c << l_shift);
            m_c = std::rotl(m_c, barrel_shift) + tmp;

            return tmp;
        }

        constexpr u32 next_u32() { return static_cast<u32>(next_u64() >> 32); }

        constexpr result_type operator()() { return next_u64(); }

    private:
        u64 m_a{};
        u64 m_b{};
        u64 m_c{};
        u64 m_counter{};
};

inline seed_type generate_seed() {
    std::random_device rd;
    seed_type          seed;

    std::ranges::for_each(seed.begin(), seed.end(), [&](auto &seed_part) {
        seed_part = static_cast<u64>(rd()) << 32 | rd();
    });

    return seed;
}

} // namespace Utils::Random