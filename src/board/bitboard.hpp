#pragma once

#include "../types.hpp"


namespace Board::Bitboards {

namespace Util {

constexpr U64 FileABB = 0x101010101010101ULL;
constexpr U64 FileBBB = 0x202020202020202ULL;
constexpr U64 FileCBB = 0x404040404040404ULL;
constexpr U64 FileDBB = 0x808080808080808ULL;
constexpr U64 FileEBB = 0x1010101010101010ULL;
constexpr U64 FileFBB = 0x2020202020202020ULL;
constexpr U64 FileGBB = 0x4040404040404040ULL;
constexpr U64 FileHBB = 0x8080808080808080ULL;

constexpr U64 Rank1BB = 0xFF00000000000000ULL;
constexpr U64 Rank2BB = 0x00FF000000000000ULL;
constexpr U64 Rank3BB = 0x0000FF0000000000ULL;
constexpr U64 Rank4BB = 0x000000FF00000000ULL;
constexpr U64 Rank5BB = 0x00000000FF000000ULL;
constexpr U64 Rank6BB = 0x0000000000FF0000ULL;
constexpr U64 Rank7BB = 0x000000000000FF00ULL;
constexpr U64 Rank8BB = 0x00000000000000FFULL;

constexpr File fileOf(const Square sq) { return static_cast<File>(static_cast<U8>(sq) & 7); }
constexpr Rank rankOf(const Square sq) { return static_cast<Rank>(static_cast<U8>(sq) >> 3); }

constexpr Square squareOf(const U8 file, const U8 rank) {
    return static_cast<Square>((rank << 3) + file);
}

} // namespace Util

class Bitboard {
    public:
        constexpr Bitboard() :
            data(0ULL){};
        constexpr explicit Bitboard(const U64 bb) :
            data(bb){};

        static constexpr Bitboard fromSquare(const Square sq) {
            return Bitboard(1ULL << static_cast<U64>(sq));
        }

        static constexpr bool isBitSet(const Bitboard &bitboard, const Square sq) {
            return static_cast<bool>(bitboard.data & 1ULL << static_cast<U64>(sq));
        }

        [[nodiscard]] constexpr U64 asU64() const { return data; }

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

        constexpr Bitboard operator&=(const Bitboard &other) {
            data &= other.data;
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

    private:
        U64 data;
};

void printBB(Bitboard bitboard);
} // namespace Board::Bitboards