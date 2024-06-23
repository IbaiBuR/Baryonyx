#pragma once

#include <bit>
#include <cassert>
#include <utility>

#include "../../chess.hpp"

namespace board::bitboards {

class Bitboard {
    public:
        constexpr Bitboard() :
            m_data(0ULL) {}
        constexpr explicit Bitboard(const u64 bb) :
            m_data(bb) {}

        static constexpr Bitboard from_square(const Square sq) {
            return Bitboard(1ULL << std::to_underlying(sq));
        }

        static constexpr bool is_bit_set(const Bitboard& bitboard, const Square sq) {
            return static_cast<bool>(bitboard.m_data & 1ULL << std::to_underlying(sq));
        }

        static constexpr void set_bit(Bitboard& bitboard, const Square sq) {
            bitboard.m_data |= 1ULL << std::to_underlying(sq);
        }

        static constexpr void clear_bit(Bitboard& bitboard, const Square sq) {
            bitboard.m_data &= ~(1ULL << std::to_underlying(sq));
        }

        [[nodiscard]] constexpr u64 as_u64() const { return m_data; }

        [[nodiscard]] constexpr bool empty() const { return m_data == 0ULL; }

        [[nodiscard]] constexpr int bit_count() const { return std::popcount(m_data); }

        [[nodiscard]] constexpr int get_lsb() const {
            assert(m_data);
            return std::countr_zero(m_data);
        }

        [[nodiscard]] constexpr int get_msb() const { return 63 - std::countl_zero(m_data); }

        [[nodiscard]] constexpr int pop_lsb() {
            const int lsbIndex = get_lsb();
            m_data &= m_data - 1;
            return lsbIndex;
        }

        constexpr explicit operator bool() const { return m_data != 0ULL; }

        constexpr bool operator==(const Bitboard& other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const Bitboard& other) const { return m_data != other.m_data; }

        constexpr Bitboard operator&(const Bitboard& other) const {
            return Bitboard(m_data & other.m_data);
        }

        constexpr Bitboard operator|(const Bitboard& other) const {
            return Bitboard(m_data | other.m_data);
        }

        constexpr Bitboard operator^(const Bitboard& other) const {
            return Bitboard(m_data ^ other.m_data);
        }

        constexpr Bitboard operator~() const { return Bitboard(~m_data); }

        constexpr Bitboard operator<<(const Bitboard& other) const {
            return Bitboard(m_data << other.m_data);
        }

        constexpr Bitboard operator>>(const Bitboard& other) const {
            return Bitboard(m_data >> other.m_data);
        }

        constexpr Bitboard& operator&=(const Bitboard& other) {
            m_data &= other.m_data;
            return *this;
        }

        constexpr Bitboard& operator*=(const u64 value) {
            m_data *= value;
            return *this;
        }

        constexpr Bitboard& operator|=(const Bitboard& other) {
            m_data |= other.m_data;
            return *this;
        }

        constexpr Bitboard& operator^=(const Bitboard& other) {
            m_data ^= other.m_data;
            return *this;
        }

        constexpr Bitboard& operator<<=(const Bitboard& other) {
            m_data <<= other.m_data;
            return *this;
        }

        constexpr Bitboard& operator>>=(const Bitboard& other) {
            m_data >>= other.m_data;
            return *this;
        }

        constexpr Bitboard operator<<(const int shift) const { return Bitboard(m_data << shift); }
        constexpr Bitboard operator>>(const int shift) const { return Bitboard(m_data >> shift); }

    private:
        u64 m_data;
};

namespace util {

inline constexpr auto empty_bb = Bitboard(0ULL);

inline constexpr auto file_a_bb = Bitboard(0x101010101010101ULL);
inline constexpr auto file_b_bb = Bitboard(0x202020202020202ULL);
inline constexpr auto file_c_bb = Bitboard(0x404040404040404ULL);
inline constexpr auto file_d_bb = Bitboard(0x808080808080808ULL);
inline constexpr auto file_e_bb = Bitboard(0x1010101010101010ULL);
inline constexpr auto file_f_bb = Bitboard(0x2020202020202020ULL);
inline constexpr auto file_g_bb = Bitboard(0x4040404040404040ULL);
inline constexpr auto file_h_bb = Bitboard(0x8080808080808080ULL);

inline constexpr auto rank_1_bb = Bitboard(0x00000000000000FFULL);
inline constexpr auto rank_2_bb = Bitboard(0x000000000000FF00ULL);
inline constexpr auto rank_3_bb = Bitboard(0x0000000000FF0000ULL);
inline constexpr auto rank_4_bb = Bitboard(0x00000000FF000000ULL);
inline constexpr auto rank_5_bb = Bitboard(0x000000FF00000000ULL);
inline constexpr auto rank_6_bb = Bitboard(0x0000FF0000000000ULL);
inline constexpr auto rank_7_bb = Bitboard(0x00FF000000000000ULL);
inline constexpr auto rank_8_bb = Bitboard(0xFF00000000000000ULL);

} // namespace util

void print_bb(Bitboard bitboard);

/// @brief Shifts a bitboard to the specified direction
/// @tparam d Direction to shift the bitboard to
/// @param bb Bitboard to shift
/// @returns The shifted bitboard
template <Direction d>
constexpr Bitboard shift(const Bitboard& bb) {
    if constexpr (d == Direction::NORTH)
        return Bitboard(bb << 8);
    else if constexpr (d == Direction::SOUTH)
        return Bitboard(bb >> 8);
    else if constexpr (d == Direction::EAST)
        return Bitboard(bb << 1) & ~util::file_a_bb;
    else if constexpr (d == Direction::WEST)
        return Bitboard(bb >> 1) & ~util::file_h_bb;
    else if constexpr (d == Direction::NORTH_EAST)
        return Bitboard(bb << 9) & ~util::file_a_bb;
    else if constexpr (d == Direction::NORTH_WEST)
        return Bitboard(bb << 7) & ~util::file_h_bb;
    else if constexpr (d == Direction::SOUTH_EAST)
        return Bitboard(bb >> 7) & ~util::file_a_bb;
    else if constexpr (d == Direction::SOUTH_WEST)
        return Bitboard(bb >> 9) & ~util::file_h_bb;
    else
        std::unreachable();
}

} // namespace board::bitboards