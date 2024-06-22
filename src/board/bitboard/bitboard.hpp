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

        static constexpr bool is_bit_set(const Bitboard &bitboard, const Square sq) {
            return static_cast<bool>(bitboard.m_data & 1ULL << std::to_underlying(sq));
        }

        static constexpr void set_bit(Bitboard &bitboard, const Square sq) {
            bitboard.m_data |= 1ULL << std::to_underlying(sq);
        }

        static constexpr void clear_bit(Bitboard &bitboard, const Square sq) {
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

        constexpr bool operator==(const Bitboard &other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const Bitboard &other) const { return m_data != other.m_data; }

        constexpr Bitboard operator&(const Bitboard &other) const {
            return Bitboard(m_data & other.m_data);
        }

        constexpr Bitboard operator|(const Bitboard &other) const {
            return Bitboard(m_data | other.m_data);
        }

        constexpr Bitboard operator^(const Bitboard &other) const {
            return Bitboard(m_data ^ other.m_data);
        }

        constexpr Bitboard operator~() const { return Bitboard(~m_data); }

        constexpr Bitboard operator<<(const Bitboard &other) const {
            return Bitboard(m_data << other.m_data);
        }

        constexpr Bitboard operator>>(const Bitboard &other) const {
            return Bitboard(m_data >> other.m_data);
        }

        constexpr Bitboard &operator&=(const Bitboard &other) {
            m_data &= other.m_data;
            return *this;
        }

        constexpr Bitboard &operator*=(const u64 value) {
            m_data *= value;
            return *this;
        }

        constexpr Bitboard &operator|=(const Bitboard &other) {
            m_data |= other.m_data;
            return *this;
        }

        constexpr Bitboard &operator^=(const Bitboard &other) {
            m_data ^= other.m_data;
            return *this;
        }

        constexpr Bitboard &operator<<=(const Bitboard &other) {
            m_data <<= other.m_data;
            return *this;
        }

        constexpr Bitboard &operator>>=(const Bitboard &other) {
            m_data >>= other.m_data;
            return *this;
        }

        constexpr Bitboard operator<<(const int shift) const { return Bitboard(m_data << shift); }
        constexpr Bitboard operator>>(const int shift) const { return Bitboard(m_data >> shift); }

    private:
        u64 m_data;
};

namespace util {

constexpr auto EMPTY_BB = Bitboard(0ULL);

constexpr auto FILE_A_BB = Bitboard(0x101010101010101ULL);
constexpr auto FILE_B_BB = Bitboard(0x202020202020202ULL);
constexpr auto FILE_C_BB = Bitboard(0x404040404040404ULL);
constexpr auto FILE_D_BB = Bitboard(0x808080808080808ULL);
constexpr auto FILE_E_BB = Bitboard(0x1010101010101010ULL);
constexpr auto FILE_F_BB = Bitboard(0x2020202020202020ULL);
constexpr auto FILE_G_BB = Bitboard(0x4040404040404040ULL);
constexpr auto FILE_H_BB = Bitboard(0x8080808080808080ULL);

constexpr auto RANK_1_BB = Bitboard(0x00000000000000FFULL);
constexpr auto RANK_2_BB = Bitboard(0x000000000000FF00ULL);
constexpr auto RANK_3_BB = Bitboard(0x0000000000FF0000ULL);
constexpr auto RANK_4_BB = Bitboard(0x00000000FF000000ULL);
constexpr auto RANK_5_BB = Bitboard(0x000000FF00000000ULL);
constexpr auto RANK_6_BB = Bitboard(0x0000FF0000000000ULL);
constexpr auto RANK_7_BB = Bitboard(0x00FF000000000000ULL);
constexpr auto RANK_8_BB = Bitboard(0xFF00000000000000ULL);

} // namespace Util

void print_bb(Bitboard bitboard);

/// @brief Shifts a bitboard to the specified direction
/// @tparam d Direction to shift the bitboard to
/// @param bb Bitboard to shift
/// @returns The shifted bitboard
template <Direction d>
constexpr Bitboard shift(const Bitboard &bb) {
    if constexpr (d == Direction::NORTH)
        return Bitboard(bb << 8);
    else if constexpr (d == Direction::SOUTH)
        return Bitboard(bb >> 8);
    else if constexpr (d == Direction::EAST)
        return Bitboard(bb << 1) & ~util::FILE_A_BB;
    else if constexpr (d == Direction::WEST)
        return Bitboard(bb >> 1) & ~util::FILE_H_BB;
    else if constexpr (d == Direction::NORTH_EAST)
        return Bitboard(bb << 9) & ~util::FILE_A_BB;
    else if constexpr (d == Direction::NORTH_WEST)
        return Bitboard(bb << 7) & ~util::FILE_H_BB;
    else if constexpr (d == Direction::SOUTH_EAST)
        return Bitboard(bb >> 7) & ~util::FILE_A_BB;
    else if constexpr (d == Direction::SOUTH_WEST)
        return Bitboard(bb >> 9) & ~util::FILE_H_BB;
    else
        std::unreachable();
}

} // namespace Board::Bitboards