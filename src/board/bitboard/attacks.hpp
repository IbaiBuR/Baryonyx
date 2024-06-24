#pragma once

#include <array>

#include "bitboard.hpp"
#include "magics.hpp"

namespace board::bitboards::attacks {

inline constexpr int max_bishop_blockers_config = 1 << 9;
inline constexpr int max_rook_blockers_config   = 1 << 12;

extern std::array<std::array<bitboard, max_bishop_blockers_config>, constants::num_squares>
    bishop_attacks;
extern std::array<std::array<bitboard, max_rook_blockers_config>, constants::num_squares>
    rook_attacks;

inline constexpr std::array<bitboard, constants::num_squares> white_pawn_attacks = {
    {bitboard(0x200ULL),
     bitboard(0x500ULL),
     bitboard(0xA00ULL),
     bitboard(0x1400ULL),
     bitboard(0x2800ULL),
     bitboard(0x5000ULL),
     bitboard(0xA000ULL),
     bitboard(0x4000ULL),
     bitboard(0x20000ULL),
     bitboard(0x50000ULL),
     bitboard(0xA0000ULL),
     bitboard(0x140000ULL),
     bitboard(0x280000ULL),
     bitboard(0x500000ULL),
     bitboard(0xA00000ULL),
     bitboard(0x400000ULL),
     bitboard(0x2000000ULL),
     bitboard(0x5000000ULL),
     bitboard(0xA000000ULL),
     bitboard(0x14000000ULL),
     bitboard(0x28000000ULL),
     bitboard(0x50000000ULL),
     bitboard(0xA0000000ULL),
     bitboard(0x40000000ULL),
     bitboard(0x200000000ULL),
     bitboard(0x500000000ULL),
     bitboard(0xA00000000ULL),
     bitboard(0x1400000000ULL),
     bitboard(0x2800000000ULL),
     bitboard(0x5000000000ULL),
     bitboard(0xA000000000ULL),
     bitboard(0x4000000000ULL),
     bitboard(0x20000000000ULL),
     bitboard(0x50000000000ULL),
     bitboard(0xA0000000000ULL),
     bitboard(0x140000000000ULL),
     bitboard(0x280000000000ULL),
     bitboard(0x500000000000ULL),
     bitboard(0xA00000000000ULL),
     bitboard(0x400000000000ULL),
     bitboard(0x2000000000000ULL),
     bitboard(0x5000000000000ULL),
     bitboard(0xA000000000000ULL),
     bitboard(0x14000000000000ULL),
     bitboard(0x28000000000000ULL),
     bitboard(0x50000000000000ULL),
     bitboard(0xA0000000000000ULL),
     bitboard(0x40000000000000ULL),
     bitboard(0x200000000000000ULL),
     bitboard(0x500000000000000ULL),
     bitboard(0xA00000000000000ULL),
     bitboard(0x1400000000000000ULL),
     bitboard(0x2800000000000000ULL),
     bitboard(0x5000000000000000ULL),
     bitboard(0xA000000000000000ULL),
     bitboard(0x4000000000000000ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL)}
};

inline constexpr std::array<bitboard, constants::num_squares> black_pawn_attacks = {
    {bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x0ULL),
     bitboard(0x2ULL),
     bitboard(0x5ULL),
     bitboard(0xAULL),
     bitboard(0x14ULL),
     bitboard(0x28ULL),
     bitboard(0x50ULL),
     bitboard(0xA0ULL),
     bitboard(0x40ULL),
     bitboard(0x200ULL),
     bitboard(0x500ULL),
     bitboard(0xA00ULL),
     bitboard(0x1400ULL),
     bitboard(0x2800ULL),
     bitboard(0x5000ULL),
     bitboard(0xA000ULL),
     bitboard(0x4000ULL),
     bitboard(0x20000ULL),
     bitboard(0x50000ULL),
     bitboard(0xA0000ULL),
     bitboard(0x140000ULL),
     bitboard(0x280000ULL),
     bitboard(0x500000ULL),
     bitboard(0xA00000ULL),
     bitboard(0x400000ULL),
     bitboard(0x2000000ULL),
     bitboard(0x5000000ULL),
     bitboard(0xA000000ULL),
     bitboard(0x14000000ULL),
     bitboard(0x28000000ULL),
     bitboard(0x50000000ULL),
     bitboard(0xA0000000ULL),
     bitboard(0x40000000ULL),
     bitboard(0x200000000ULL),
     bitboard(0x500000000ULL),
     bitboard(0xA00000000ULL),
     bitboard(0x1400000000ULL),
     bitboard(0x2800000000ULL),
     bitboard(0x5000000000ULL),
     bitboard(0xA000000000ULL),
     bitboard(0x4000000000ULL),
     bitboard(0x20000000000ULL),
     bitboard(0x50000000000ULL),
     bitboard(0xA0000000000ULL),
     bitboard(0x140000000000ULL),
     bitboard(0x280000000000ULL),
     bitboard(0x500000000000ULL),
     bitboard(0xA00000000000ULL),
     bitboard(0x400000000000ULL),
     bitboard(0x2000000000000ULL),
     bitboard(0x5000000000000ULL),
     bitboard(0xA000000000000ULL),
     bitboard(0x14000000000000ULL),
     bitboard(0x28000000000000ULL),
     bitboard(0x50000000000000ULL),
     bitboard(0xA0000000000000ULL),
     bitboard(0x40000000000000ULL)}
};

/// @brief Pre-calculated lookup table for pawn attacks
inline constexpr std::array pawn_attacks = {white_pawn_attacks, black_pawn_attacks};

/// @brief Pre-calculated lookup table for knight attacks
inline constexpr std::array<bitboard, constants::num_squares> knight_attacks = {
    {bitboard(0x20400ULL),
     bitboard(0x50800ULL),
     bitboard(0xA1100ULL),
     bitboard(0x142200ULL),
     bitboard(0x284400ULL),
     bitboard(0x508800ULL),
     bitboard(0xA01000ULL),
     bitboard(0x402000ULL),
     bitboard(0x2040004ULL),
     bitboard(0x5080008ULL),
     bitboard(0xA110011ULL),
     bitboard(0x14220022ULL),
     bitboard(0x28440044ULL),
     bitboard(0x50880088ULL),
     bitboard(0xA0100010ULL),
     bitboard(0x40200020ULL),
     bitboard(0x204000402ULL),
     bitboard(0x508000805ULL),
     bitboard(0xA1100110AULL),
     bitboard(0x1422002214ULL),
     bitboard(0x2844004428ULL),
     bitboard(0x5088008850ULL),
     bitboard(0xA0100010A0ULL),
     bitboard(0x4020002040ULL),
     bitboard(0x20400040200ULL),
     bitboard(0x50800080500ULL),
     bitboard(0xA1100110A00ULL),
     bitboard(0x142200221400ULL),
     bitboard(0x284400442800ULL),
     bitboard(0x508800885000ULL),
     bitboard(0xA0100010A000ULL),
     bitboard(0x402000204000ULL),
     bitboard(0x2040004020000ULL),
     bitboard(0x5080008050000ULL),
     bitboard(0xA1100110A0000ULL),
     bitboard(0x14220022140000ULL),
     bitboard(0x28440044280000ULL),
     bitboard(0x50880088500000ULL),
     bitboard(0xA0100010A00000ULL),
     bitboard(0x40200020400000ULL),
     bitboard(0x204000402000000ULL),
     bitboard(0x508000805000000ULL),
     bitboard(0xA1100110A000000ULL),
     bitboard(0x1422002214000000ULL),
     bitboard(0x2844004428000000ULL),
     bitboard(0x5088008850000000ULL),
     bitboard(0xA0100010A0000000ULL),
     bitboard(0x4020002040000000ULL),
     bitboard(0x400040200000000ULL),
     bitboard(0x800080500000000ULL),
     bitboard(0x1100110A00000000ULL),
     bitboard(0x2200221400000000ULL),
     bitboard(0x4400442800000000ULL),
     bitboard(0x8800885000000000ULL),
     bitboard(0x100010A000000000ULL),
     bitboard(0x2000204000000000ULL),
     bitboard(0x4020000000000ULL),
     bitboard(0x8050000000000ULL),
     bitboard(0x110A0000000000ULL),
     bitboard(0x22140000000000ULL),
     bitboard(0x44280000000000ULL),
     bitboard(0x88500000000000ULL),
     bitboard(0x10A00000000000ULL),
     bitboard(0x20400000000000ULL)}
};

/// @brief Pre-calculated lookup table for king attacks
inline constexpr std::array<bitboard, constants::num_squares> king_attacks = {
    {bitboard(0x302ULL),
     bitboard(0x705ULL),
     bitboard(0xE0AULL),
     bitboard(0x1C14ULL),
     bitboard(0x3828ULL),
     bitboard(0x7050ULL),
     bitboard(0xE0A0ULL),
     bitboard(0xC040ULL),
     bitboard(0x30203ULL),
     bitboard(0x70507ULL),
     bitboard(0xE0A0EULL),
     bitboard(0x1C141CULL),
     bitboard(0x382838ULL),
     bitboard(0x705070ULL),
     bitboard(0xE0A0E0ULL),
     bitboard(0xC040C0ULL),
     bitboard(0x3020300ULL),
     bitboard(0x7050700ULL),
     bitboard(0xE0A0E00ULL),
     bitboard(0x1C141C00ULL),
     bitboard(0x38283800ULL),
     bitboard(0x70507000ULL),
     bitboard(0xE0A0E000ULL),
     bitboard(0xC040C000ULL),
     bitboard(0x302030000ULL),
     bitboard(0x705070000ULL),
     bitboard(0xE0A0E0000ULL),
     bitboard(0x1C141C0000ULL),
     bitboard(0x3828380000ULL),
     bitboard(0x7050700000ULL),
     bitboard(0xE0A0E00000ULL),
     bitboard(0xC040C00000ULL),
     bitboard(0x30203000000ULL),
     bitboard(0x70507000000ULL),
     bitboard(0xE0A0E000000ULL),
     bitboard(0x1C141C000000ULL),
     bitboard(0x382838000000ULL),
     bitboard(0x705070000000ULL),
     bitboard(0xE0A0E0000000ULL),
     bitboard(0xC040C0000000ULL),
     bitboard(0x3020300000000ULL),
     bitboard(0x7050700000000ULL),
     bitboard(0xE0A0E00000000ULL),
     bitboard(0x1C141C00000000ULL),
     bitboard(0x38283800000000ULL),
     bitboard(0x70507000000000ULL),
     bitboard(0xE0A0E000000000ULL),
     bitboard(0xC040C000000000ULL),
     bitboard(0x302030000000000ULL),
     bitboard(0x705070000000000ULL),
     bitboard(0xE0A0E0000000000ULL),
     bitboard(0x1C141C0000000000ULL),
     bitboard(0x3828380000000000ULL),
     bitboard(0x7050700000000000ULL),
     bitboard(0xE0A0E00000000000ULL),
     bitboard(0xC040C00000000000ULL),
     bitboard(0x203000000000000ULL),
     bitboard(0x507000000000000ULL),
     bitboard(0xA0E000000000000ULL),
     bitboard(0x141C000000000000ULL),
     bitboard(0x2838000000000000ULL),
     bitboard(0x5070000000000000ULL),
     bitboard(0xA0E0000000000000ULL),
     bitboard(0x40C0000000000000ULL)}
};

void init();

template <piece_type PieceType>
constexpr int get_max_blockers_config() {
    if constexpr (PieceType == piece_type::bishop)
        return max_bishop_blockers_config;
    else
        return max_rook_blockers_config;
}

inline bitboard get_pawn_attacks(const square sq, const color c) {
    return pawn_attacks[std::to_underlying(c)][std::to_underlying(sq)];
}

inline bitboard get_knight_attacks(const square sq) {
    return knight_attacks[std::to_underlying(sq)];
}

inline bitboard get_bishop_attacks(const square sq, bitboard blockers) {
    return bishop_attacks[std::to_underlying(sq)]
                         [magic_index(magics::bishop_magics[std::to_underlying(sq)], blockers)];
}

inline bitboard get_rook_attacks(const square sq, bitboard blockers) {
    return rook_attacks[std::to_underlying(sq)]
                       [magic_index(magics::rook_magics[std::to_underlying(sq)], blockers)];
}

inline bitboard get_king_attacks(const square sq) { return king_attacks[std::to_underlying(sq)]; }

inline bitboard get_queen_attacks(const square sq, const bitboard& blockers) {
    return get_bishop_attacks(sq, blockers) | get_rook_attacks(sq, blockers);
}

/// @brief Creates the sliding attacks for the specified direction
/// @tparam D Direction to create the attacks
/// @param sq Square to generate the attacks from
/// @param occupied Bitboard of occupied squares on the board (Blockers)
/// @returns A bitboard with the sliding attacks in the given direction
template <direction D>
constexpr bitboard sliding_attacks(const square sq, const bitboard& occupied) {
    bitboard attacks;

    for (bitboard b = shift<D>(bitboard::from_square(sq)); !b.empty(); b = shift<D>(b)) {
        attacks |= b;

        if (b & occupied)
            break;
    }

    return attacks;
}

/// @brief Generates the possible moves for sliding pieces
/// @tparam PieceType Piece type (Slider)
/// @param sq Square to generate the attacks from
/// @param occupied Bitboard of occupied squares on the board (Blockers)
/// @returns A bitboard with the generated sliding attacks
/// @note An assert is used to ensure the function is only used with sliders. The queen is not
/// included since we can obtain it by combining the attacks of the bishop and the rook
template <piece_type PieceType>
constexpr bitboard gen_sliding(const square sq, const bitboard& occupied) {
    assert(PieceType == piece_type::bishop || PieceType == piece_type::rook);

    if constexpr (PieceType == piece_type::bishop) {
        return sliding_attacks<direction::north_east>(sq, occupied)
             | sliding_attacks<direction::north_west>(sq, occupied)
             | sliding_attacks<direction::south_east>(sq, occupied)
             | sliding_attacks<direction::south_west>(sq, occupied);
    }
    else {
        return sliding_attacks<direction::north>(sq, occupied)
             | sliding_attacks<direction::south>(sq, occupied)
             | sliding_attacks<direction::east>(sq, occupied)
             | sliding_attacks<direction::west>(sq, occupied);
    }
}

/// @brief Generates the possible attacks for a given piece type on a given square
/// @param pt Piece type
/// @param sq Square to generate the attacks from
/// @param occupied Bitboard of occupied squares on the board (Blockers)
/// @returns A bitboard representing the squares that the piece can attack from the given square
/// @note Pawns are not taken into account since this function is primarily used during movegen
/// and pawns are treated separately there
constexpr bitboard
get_attacks_by_piece_type(const piece_type pt, const square sq, const bitboard& occupied) {
    switch (pt) {
    case piece_type::knight:
        return knight_attacks[std::to_underlying(sq)];
    case piece_type::king:
        return king_attacks[std::to_underlying(sq)];
    case piece_type::bishop:
        return get_bishop_attacks(sq, occupied);
    case piece_type::rook:
        return get_rook_attacks(sq, occupied);
    case piece_type::queen:
        return get_bishop_attacks(sq, occupied) | get_rook_attacks(sq, occupied);
    default:
        std::unreachable();
    }
}

} // namespace board::bitboards::attacks
