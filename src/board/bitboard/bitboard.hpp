#pragma once

#include <bit>
#include <cassert>
#include <utility>

#include "../../chess.hpp"

namespace Board::Bitboards {

class Bitboard {
    public:
        constexpr Bitboard() :
            data(0ULL){};
        constexpr explicit Bitboard(const U64 bb) :
            data(bb){};

        static constexpr Bitboard fromSquare(const Square sq) {
            return Bitboard(1ULL << std::to_underlying(sq));
        }

        static constexpr bool isBitSet(const Bitboard &bitboard, const Square sq) {
            return static_cast<bool>(bitboard.data & 1ULL << std::to_underlying(sq));
        }

        static constexpr void setBit(Bitboard &bitboard, const Square sq) {
            bitboard.data |= 1ULL << std::to_underlying(sq);
        }

        static constexpr void clearBit(Bitboard &bitboard, const Square sq) {
            bitboard.data &= ~(1ULL << std::to_underlying(sq));
        }

        [[nodiscard]] constexpr U64 asU64() const { return data; }

        [[nodiscard]] constexpr bool empty() const { return data == 0ULL; }

        [[nodiscard]] constexpr int bitCount() const { return std::popcount(data); }

        [[nodiscard]] constexpr int getLSB() const {
            assert(data);
            return std::countr_zero(data);
        }

        [[nodiscard]] constexpr int getMSB() const { return 63 - std::countl_zero(data); }

        [[nodiscard]] constexpr int popLSB() {
            const int lsbIndex = getLSB();
            data &= data - 1;
            return lsbIndex;
        }

        constexpr explicit operator bool() const { return data != 0ULL; }

        constexpr bool operator==(const Bitboard &other) const { return data == other.data; }
        constexpr bool operator!=(const Bitboard &other) const { return data != other.data; }

        constexpr Bitboard operator&(const Bitboard &other) const {
            return Bitboard(data & other.data);
        }

        constexpr Bitboard operator|(const Bitboard &other) const {
            return Bitboard(data | other.data);
        }

        constexpr Bitboard operator^(const Bitboard &other) const {
            return Bitboard(data ^ other.data);
        }

        constexpr Bitboard operator~() const { return Bitboard(~data); }

        constexpr Bitboard operator<<(const Bitboard &other) const {
            return Bitboard(data << other.data);
        }

        constexpr Bitboard operator>>(const Bitboard &other) const {
            return Bitboard(data >> other.data);
        }

        constexpr Bitboard operator&=(const Bitboard &other) {
            data &= other.data;
            return *this;
        }

        constexpr Bitboard operator*=(const U64 value) {
            data *= value;
            return *this;
        }

        constexpr Bitboard operator|=(const Bitboard &other) {
            data |= other.data;
            return *this;
        }

        constexpr Bitboard operator^=(const Bitboard &other) {
            data ^= other.data;
            return *this;
        }

        constexpr Bitboard operator<<=(const Bitboard &other) {
            data <<= other.data;
            return *this;
        }

        constexpr Bitboard operator>>=(const Bitboard &other) {
            data >>= other.data;
            return *this;
        }

        constexpr Bitboard operator<<(const int shift) const { return Bitboard(data << shift); }
        constexpr Bitboard operator>>(const int shift) const { return Bitboard(data >> shift); }

    private:
        U64 data;
};

namespace Util {

constexpr auto EmptyBB = Bitboard(0ULL);

constexpr auto FileABB = Bitboard(0x101010101010101ULL);
constexpr auto FileBBB = Bitboard(0x202020202020202ULL);
constexpr auto FileCBB = Bitboard(0x404040404040404ULL);
constexpr auto FileDBB = Bitboard(0x808080808080808ULL);
constexpr auto FileEBB = Bitboard(0x1010101010101010ULL);
constexpr auto FileFBB = Bitboard(0x2020202020202020ULL);
constexpr auto FileGBB = Bitboard(0x4040404040404040ULL);
constexpr auto FileHBB = Bitboard(0x8080808080808080ULL);

constexpr auto Rank1BB = Bitboard(0x00000000000000FFULL);
constexpr auto Rank2BB = Bitboard(0x000000000000FF00ULL);
constexpr auto Rank3BB = Bitboard(0x0000000000FF0000ULL);
constexpr auto Rank4BB = Bitboard(0x00000000FF000000ULL);
constexpr auto Rank5BB = Bitboard(0x000000FF00000000ULL);
constexpr auto Rank6BB = Bitboard(0x0000FF0000000000ULL);
constexpr auto Rank7BB = Bitboard(0x00FF000000000000ULL);
constexpr auto Rank8BB = Bitboard(0xFF00000000000000ULL);

constexpr File fileOf(const Square sq) { return static_cast<File>(std::to_underlying(sq) & 7); }
constexpr Rank rankOf(const Square sq) { return static_cast<Rank>(std::to_underlying(sq) >> 3); }

constexpr Square squareOf(const U8 file, const U8 rank) {
    return static_cast<Square>((rank << 3) + file);
}

} // namespace Util

void printBB(Bitboard bitboard);

/// @brief Shifts a bitboard to the specified direction
/// @tparam d Direction to shift the bitboard to
/// @param bb Bitboard to shift
/// @returns The shifted bitboard
template <Direction d>
constexpr Bitboard shift(const Bitboard &bb) {
    switch (d) {
    case Direction::NORTH:
        return Bitboard(bb << 8);
        break;
    case Direction::SOUTH:
        return Bitboard(bb >> 8);
        break;
    case Direction::EAST:
        return Bitboard(bb << 1) & ~Util::FileABB;
        break;
    case Direction::WEST:
        return Bitboard(bb >> 1) & ~Util::FileHBB;
        break;
    case Direction::NORTH_EAST:
        return Bitboard(bb << 9) & ~Util::FileABB;
        break;
    case Direction::NORTH_WEST:
        return Bitboard(bb << 7) & ~Util::FileHBB;
        break;
    case Direction::SOUTH_EAST:
        return Bitboard(bb >> 7) & ~Util::FileABB;
        break;
    case Direction::SOUTH_WEST:
        return Bitboard(bb >> 9) & ~Util::FileHBB;
        break;
    default:
        return Bitboard(0ULL);
        break;
    }
}

} // namespace Board::Bitboards