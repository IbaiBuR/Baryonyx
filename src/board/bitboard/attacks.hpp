#pragma once

#include <array>

#include "bitboard.hpp"

namespace Board::Bitboards::Attacks {

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> whitePawnAttacks = {
    {
     Bitboard(0x200ULL),
     Bitboard(0x500ULL),
     Bitboard(0xA00ULL),
     Bitboard(0x1400ULL),
     Bitboard(0x2800ULL),
     Bitboard(0x5000ULL),
     Bitboard(0xA000ULL),
     Bitboard(0x4000ULL),
     Bitboard(0x20000ULL),
     Bitboard(0x50000ULL),
     Bitboard(0xA0000ULL),
     Bitboard(0x140000ULL),
     Bitboard(0x280000ULL),
     Bitboard(0x500000ULL),
     Bitboard(0xA00000ULL),
     Bitboard(0x400000ULL),
     Bitboard(0x2000000ULL),
     Bitboard(0x5000000ULL),
     Bitboard(0xA000000ULL),
     Bitboard(0x14000000ULL),
     Bitboard(0x28000000ULL),
     Bitboard(0x50000000ULL),
     Bitboard(0xA0000000ULL),
     Bitboard(0x40000000ULL),
     Bitboard(0x200000000ULL),
     Bitboard(0x500000000ULL),
     Bitboard(0xA00000000ULL),
     Bitboard(0x1400000000ULL),
     Bitboard(0x2800000000ULL),
     Bitboard(0x5000000000ULL),
     Bitboard(0xA000000000ULL),
     Bitboard(0x4000000000ULL),
     Bitboard(0x20000000000ULL),
     Bitboard(0x50000000000ULL),
     Bitboard(0xA0000000000ULL),
     Bitboard(0x140000000000ULL),
     Bitboard(0x280000000000ULL),
     Bitboard(0x500000000000ULL),
     Bitboard(0xA00000000000ULL),
     Bitboard(0x400000000000ULL),
     Bitboard(0x2000000000000ULL),
     Bitboard(0x5000000000000ULL),
     Bitboard(0xA000000000000ULL),
     Bitboard(0x14000000000000ULL),
     Bitboard(0x28000000000000ULL),
     Bitboard(0x50000000000000ULL),
     Bitboard(0xA0000000000000ULL),
     Bitboard(0x40000000000000ULL),
     Bitboard(0x200000000000000ULL),
     Bitboard(0x500000000000000ULL),
     Bitboard(0xA00000000000000ULL),
     Bitboard(0x1400000000000000ULL),
     Bitboard(0x2800000000000000ULL),
     Bitboard(0x5000000000000000ULL),
     Bitboard(0xA000000000000000ULL),
     Bitboard(0x4000000000000000ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     }
};

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> blackPawnAttacks = {
    {
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x0ULL),
     Bitboard(0x2ULL),
     Bitboard(0x5ULL),
     Bitboard(0xAULL),
     Bitboard(0x14ULL),
     Bitboard(0x28ULL),
     Bitboard(0x50ULL),
     Bitboard(0xA0ULL),
     Bitboard(0x40ULL),
     Bitboard(0x200ULL),
     Bitboard(0x500ULL),
     Bitboard(0xA00ULL),
     Bitboard(0x1400ULL),
     Bitboard(0x2800ULL),
     Bitboard(0x5000ULL),
     Bitboard(0xA000ULL),
     Bitboard(0x4000ULL),
     Bitboard(0x20000ULL),
     Bitboard(0x50000ULL),
     Bitboard(0xA0000ULL),
     Bitboard(0x140000ULL),
     Bitboard(0x280000ULL),
     Bitboard(0x500000ULL),
     Bitboard(0xA00000ULL),
     Bitboard(0x400000ULL),
     Bitboard(0x2000000ULL),
     Bitboard(0x5000000ULL),
     Bitboard(0xA000000ULL),
     Bitboard(0x14000000ULL),
     Bitboard(0x28000000ULL),
     Bitboard(0x50000000ULL),
     Bitboard(0xA0000000ULL),
     Bitboard(0x40000000ULL),
     Bitboard(0x200000000ULL),
     Bitboard(0x500000000ULL),
     Bitboard(0xA00000000ULL),
     Bitboard(0x1400000000ULL),
     Bitboard(0x2800000000ULL),
     Bitboard(0x5000000000ULL),
     Bitboard(0xA000000000ULL),
     Bitboard(0x4000000000ULL),
     Bitboard(0x20000000000ULL),
     Bitboard(0x50000000000ULL),
     Bitboard(0xA0000000000ULL),
     Bitboard(0x140000000000ULL),
     Bitboard(0x280000000000ULL),
     Bitboard(0x500000000000ULL),
     Bitboard(0xA00000000000ULL),
     Bitboard(0x400000000000ULL),
     Bitboard(0x2000000000000ULL),
     Bitboard(0x5000000000000ULL),
     Bitboard(0xA000000000000ULL),
     Bitboard(0x14000000000000ULL),
     Bitboard(0x28000000000000ULL),
     Bitboard(0x50000000000000ULL),
     Bitboard(0xA0000000000000ULL),
     Bitboard(0x40000000000000ULL),
     }
};

constexpr std::array pawnAttacks = {whitePawnAttacks, blackPawnAttacks};

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> knightAttacks = {
    {
     Bitboard(0x20400ULL),
     Bitboard(0x50800ULL),
     Bitboard(0xA1100ULL),
     Bitboard(0x142200ULL),
     Bitboard(0x284400ULL),
     Bitboard(0x508800ULL),
     Bitboard(0xA01000ULL),
     Bitboard(0x402000ULL),
     Bitboard(0x2040004ULL),
     Bitboard(0x5080008ULL),
     Bitboard(0xA110011ULL),
     Bitboard(0x14220022ULL),
     Bitboard(0x28440044ULL),
     Bitboard(0x50880088ULL),
     Bitboard(0xA0100010ULL),
     Bitboard(0x40200020ULL),
     Bitboard(0x204000402ULL),
     Bitboard(0x508000805ULL),
     Bitboard(0xA1100110AULL),
     Bitboard(0x1422002214ULL),
     Bitboard(0x2844004428ULL),
     Bitboard(0x5088008850ULL),
     Bitboard(0xA0100010A0ULL),
     Bitboard(0x4020002040ULL),
     Bitboard(0x20400040200ULL),
     Bitboard(0x50800080500ULL),
     Bitboard(0xA1100110A00ULL),
     Bitboard(0x142200221400ULL),
     Bitboard(0x284400442800ULL),
     Bitboard(0x508800885000ULL),
     Bitboard(0xA0100010A000ULL),
     Bitboard(0x402000204000ULL),
     Bitboard(0x2040004020000ULL),
     Bitboard(0x5080008050000ULL),
     Bitboard(0xA1100110A0000ULL),
     Bitboard(0x14220022140000ULL),
     Bitboard(0x28440044280000ULL),
     Bitboard(0x50880088500000ULL),
     Bitboard(0xA0100010A00000ULL),
     Bitboard(0x40200020400000ULL),
     Bitboard(0x204000402000000ULL),
     Bitboard(0x508000805000000ULL),
     Bitboard(0xA1100110A000000ULL),
     Bitboard(0x1422002214000000ULL),
     Bitboard(0x2844004428000000ULL),
     Bitboard(0x5088008850000000ULL),
     Bitboard(0xA0100010A0000000ULL),
     Bitboard(0x4020002040000000ULL),
     Bitboard(0x400040200000000ULL),
     Bitboard(0x800080500000000ULL),
     Bitboard(0x1100110A00000000ULL),
     Bitboard(0x2200221400000000ULL),
     Bitboard(0x4400442800000000ULL),
     Bitboard(0x8800885000000000ULL),
     Bitboard(0x100010A000000000ULL),
     Bitboard(0x2000204000000000ULL),
     Bitboard(0x4020000000000ULL),
     Bitboard(0x8050000000000ULL),
     Bitboard(0x110A0000000000ULL),
     Bitboard(0x22140000000000ULL),
     Bitboard(0x44280000000000ULL),
     Bitboard(0x88500000000000ULL),
     Bitboard(0x10A00000000000ULL),
     Bitboard(0x20400000000000ULL),
     }
};

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> kingAttacks = {
    {
     Bitboard(0x302ULL),
     Bitboard(0x705ULL),
     Bitboard(0xE0AULL),
     Bitboard(0x1C14ULL),
     Bitboard(0x3828ULL),
     Bitboard(0x7050ULL),
     Bitboard(0xE0A0ULL),
     Bitboard(0xC040ULL),
     Bitboard(0x30203ULL),
     Bitboard(0x70507ULL),
     Bitboard(0xE0A0EULL),
     Bitboard(0x1C141CULL),
     Bitboard(0x382838ULL),
     Bitboard(0x705070ULL),
     Bitboard(0xE0A0E0ULL),
     Bitboard(0xC040C0ULL),
     Bitboard(0x3020300ULL),
     Bitboard(0x7050700ULL),
     Bitboard(0xE0A0E00ULL),
     Bitboard(0x1C141C00ULL),
     Bitboard(0x38283800ULL),
     Bitboard(0x70507000ULL),
     Bitboard(0xE0A0E000ULL),
     Bitboard(0xC040C000ULL),
     Bitboard(0x302030000ULL),
     Bitboard(0x705070000ULL),
     Bitboard(0xE0A0E0000ULL),
     Bitboard(0x1C141C0000ULL),
     Bitboard(0x3828380000ULL),
     Bitboard(0x7050700000ULL),
     Bitboard(0xE0A0E00000ULL),
     Bitboard(0xC040C00000ULL),
     Bitboard(0x30203000000ULL),
     Bitboard(0x70507000000ULL),
     Bitboard(0xE0A0E000000ULL),
     Bitboard(0x1C141C000000ULL),
     Bitboard(0x382838000000ULL),
     Bitboard(0x705070000000ULL),
     Bitboard(0xE0A0E0000000ULL),
     Bitboard(0xC040C0000000ULL),
     Bitboard(0x3020300000000ULL),
     Bitboard(0x7050700000000ULL),
     Bitboard(0xE0A0E00000000ULL),
     Bitboard(0x1C141C00000000ULL),
     Bitboard(0x38283800000000ULL),
     Bitboard(0x70507000000000ULL),
     Bitboard(0xE0A0E000000000ULL),
     Bitboard(0xC040C000000000ULL),
     Bitboard(0x302030000000000ULL),
     Bitboard(0x705070000000000ULL),
     Bitboard(0xE0A0E0000000000ULL),
     Bitboard(0x1C141C0000000000ULL),
     Bitboard(0x3828380000000000ULL),
     Bitboard(0x7050700000000000ULL),
     Bitboard(0xE0A0E00000000000ULL),
     Bitboard(0xC040C00000000000ULL),
     Bitboard(0x203000000000000ULL),
     Bitboard(0x507000000000000ULL),
     Bitboard(0xA0E000000000000ULL),
     Bitboard(0x141C000000000000ULL),
     Bitboard(0x2838000000000000ULL),
     Bitboard(0x5070000000000000ULL),
     Bitboard(0xA0E0000000000000ULL),
     Bitboard(0x40C0000000000000ULL),
     }
};

template <Direction d>
constexpr Bitboard slidingAttacks(const Square sq, const Bitboard &occupied) {
    Bitboard attacks;

    for (Bitboard b = shift<d>(Bitboard::fromSquare(sq)); !b.empty(); b = shift<d>(b))
    {
        attacks |= b;

        if (b & occupied)
            break;
    }

    return attacks;
}

template <PieceType pt>
constexpr Bitboard genSliding(const Square sq, const Bitboard &occupied) {
    assert(pt == PieceType::BISHOP || pt == PieceType::ROOK);

    if constexpr (pt == PieceType::BISHOP)
    {
        return slidingAttacks<Direction::NORTH_EAST>(sq, occupied)
             | slidingAttacks<Direction::NORTH_WEST>(sq, occupied)
             | slidingAttacks<Direction::SOUTH_EAST>(sq, occupied)
             | slidingAttacks<Direction::SOUTH_WEST>(sq, occupied);
    }
    else
    {
        return slidingAttacks<Direction::NORTH>(sq, occupied)
             | slidingAttacks<Direction::SOUTH>(sq, occupied)
             | slidingAttacks<Direction::EAST>(sq, occupied)
             | slidingAttacks<Direction::WEST>(sq, occupied);
    }
}

} // namespace Board::Bitboards::Attacks
