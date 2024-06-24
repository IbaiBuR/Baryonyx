#pragma once

#include <bit>
#include <cassert>
#include <utility>

#include "../../chess.hpp"

namespace board::bitboards {

class bitboard {
    public:
        constexpr bitboard() :
            m_data(0ULL) {}
        constexpr explicit bitboard(const u64 bb) :
            m_data(bb) {}

        static constexpr bitboard from_square(const square sq) {
            return bitboard(1ULL << std::to_underlying(sq));
        }

        static constexpr bool is_bit_set(const bitboard& bitboard, const square sq) {
            return static_cast<bool>(bitboard.m_data & 1ULL << std::to_underlying(sq));
        }

        static constexpr void set_bit(bitboard& bitboard, const square sq) {
            bitboard.m_data |= 1ULL << std::to_underlying(sq);
        }

        static constexpr void clear_bit(bitboard& bitboard, const square sq) {
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

        constexpr bool operator==(const bitboard& other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const bitboard& other) const { return m_data != other.m_data; }

        constexpr bitboard operator&(const bitboard& other) const {
            return bitboard(m_data & other.m_data);
        }

        constexpr bitboard operator|(const bitboard& other) const {
            return bitboard(m_data | other.m_data);
        }

        constexpr bitboard operator^(const bitboard& other) const {
            return bitboard(m_data ^ other.m_data);
        }

        constexpr bitboard operator~() const { return bitboard(~m_data); }

        constexpr bitboard operator<<(const bitboard& other) const {
            return bitboard(m_data << other.m_data);
        }

        constexpr bitboard operator>>(const bitboard& other) const {
            return bitboard(m_data >> other.m_data);
        }

        constexpr bitboard& operator&=(const bitboard& other) {
            m_data &= other.m_data;
            return *this;
        }

        constexpr bitboard& operator*=(const u64 value) {
            m_data *= value;
            return *this;
        }

        constexpr bitboard& operator|=(const bitboard& other) {
            m_data |= other.m_data;
            return *this;
        }

        constexpr bitboard& operator^=(const bitboard& other) {
            m_data ^= other.m_data;
            return *this;
        }

        constexpr bitboard& operator<<=(const bitboard& other) {
            m_data <<= other.m_data;
            return *this;
        }

        constexpr bitboard& operator>>=(const bitboard& other) {
            m_data >>= other.m_data;
            return *this;
        }

        constexpr bitboard operator<<(const int shift) const { return bitboard(m_data << shift); }
        constexpr bitboard operator>>(const int shift) const { return bitboard(m_data >> shift); }

    private:
        u64 m_data;
};

namespace util {

inline constexpr auto empty_bb = bitboard(0ULL);

inline constexpr auto file_a_bb = bitboard(0x101010101010101ULL);
inline constexpr auto file_b_bb = bitboard(0x202020202020202ULL);
inline constexpr auto file_c_bb = bitboard(0x404040404040404ULL);
inline constexpr auto file_d_bb = bitboard(0x808080808080808ULL);
inline constexpr auto file_e_bb = bitboard(0x1010101010101010ULL);
inline constexpr auto file_f_bb = bitboard(0x2020202020202020ULL);
inline constexpr auto file_g_bb = bitboard(0x4040404040404040ULL);
inline constexpr auto file_h_bb = bitboard(0x8080808080808080ULL);

inline constexpr auto rank_1_bb = bitboard(0x00000000000000FFULL);
inline constexpr auto rank_2_bb = bitboard(0x000000000000FF00ULL);
inline constexpr auto rank_3_bb = bitboard(0x0000000000FF0000ULL);
inline constexpr auto rank_4_bb = bitboard(0x00000000FF000000ULL);
inline constexpr auto rank_5_bb = bitboard(0x000000FF00000000ULL);
inline constexpr auto rank_6_bb = bitboard(0x0000FF0000000000ULL);
inline constexpr auto rank_7_bb = bitboard(0x00FF000000000000ULL);
inline constexpr auto rank_8_bb = bitboard(0xFF00000000000000ULL);

} // namespace util

void print_bb(const bitboard& bitboard);

/// @brief Shifts a bitboard to the specified direction
/// @tparam D Direction to shift the bitboard to
/// @param bb Bitboard to shift
/// @returns The shifted bitboard
template <direction D>
constexpr bitboard shift(const bitboard& bb) {
    if constexpr (D == direction::north)
        return bitboard(bb << 8);
    else if constexpr (D == direction::south)
        return bitboard(bb >> 8);
    else if constexpr (D == direction::east)
        return bitboard(bb << 1) & ~util::file_a_bb;
    else if constexpr (D == direction::west)
        return bitboard(bb >> 1) & ~util::file_h_bb;
    else if constexpr (D == direction::north_east)
        return bitboard(bb << 9) & ~util::file_a_bb;
    else if constexpr (D == direction::north_west)
        return bitboard(bb << 7) & ~util::file_h_bb;
    else if constexpr (D == direction::south_east)
        return bitboard(bb >> 7) & ~util::file_a_bb;
    else if constexpr (D == direction::south_west)
        return bitboard(bb >> 9) & ~util::file_h_bb;
    else
        std::unreachable();
}

} // namespace board::bitboards