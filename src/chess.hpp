#pragma once

#include "types.hpp"

namespace constants {

inline constexpr usize num_files   = 8;
inline constexpr usize num_ranks   = 8;
inline constexpr usize num_squares = num_files * num_ranks;

inline constexpr usize num_piece_types = 6;
inline constexpr usize num_colors      = 2;
inline constexpr usize num_pieces      = num_piece_types * num_colors;

inline constexpr int max_depth = 100;
inline constexpr int max_ply   = 200;
inline constexpr int max_moves = 256;

} // namespace constants

enum class file : u8 {
    file_a,
    file_b,
    file_c,
    file_d,
    file_e,
    file_f,
    file_g,
    file_h
};

enum class rank : u8 {
    rank_1,
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8
};

// clang-format off
enum class square : u8 {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    none
};
// clang-format on

enum class color : u8 {
    white,
    black,
    none
};

enum class piece_type : u8 {
    pawn,
    knight,
    bishop,
    rook,
    queen,
    king,
    none
};

enum class piece : u8 {
    w_pawn,
    w_knight,
    w_bishop,
    w_rook,
    w_queen,
    w_king,
    b_pawn,
    b_knight,
    b_bishop,
    b_rook,
    b_queen,
    b_king,
    none
};

enum class direction : i8 {
    north      = 8,
    south      = -north,
    east       = 1,
    west       = -east,
    north_east = north + east,
    north_west = north + west,
    south_east = south + east,
    south_west = south + west
};

constexpr color operator~(const color c) { return static_cast<color>(static_cast<int>(c) ^ 1); }

constexpr square& operator++(square& sq) {
    return sq = static_cast<square>(static_cast<int>(sq) + 1);
}

constexpr square& operator--(square& sq) {
    return sq = static_cast<square>(static_cast<int>(sq) - 1);
}

constexpr square operator+(const square sq, const direction dir) {
    return static_cast<square>(static_cast<int>(sq) + static_cast<int>(dir));
}

constexpr square operator-(const square sq, const direction dir) {
    return static_cast<square>(static_cast<int>(sq) - static_cast<int>(dir));
}

constexpr square flip(const square sq) { return static_cast<square>(static_cast<int>(sq) ^ 56); }

constexpr square square_of(const u8 file, const u8 rank) {
    return static_cast<square>((rank << 3) + file);
}

constexpr file file_of(const square sq) { return static_cast<file>(static_cast<u8>(sq) & 7); }
constexpr rank rank_of(const square sq) { return static_cast<rank>(static_cast<u8>(sq) >> 3); }

template <color C>
constexpr square relative_square(const square sq) {
    if constexpr (C == color::white)
        return flip(sq);
    else
        return sq;
}

constexpr direction operator*(const direction dir, const int mul) {
    return static_cast<direction>(static_cast<int>(dir) * mul);
}