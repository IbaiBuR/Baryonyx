#pragma once

#include "types.hpp"

enum class File : u8 {
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
    FILE_NB
};

enum class Rank : u8 {
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_NB
};

// clang-format off
enum class Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    SQUARE_NB, NO_SQ
};
// clang-format on

enum class Color : u8 {
    WHITE,
    BLACK,
    COLOR_NB
};

enum class PieceType : u8 {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    PIECETYPE_NB
};

enum class Piece : i8 {
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    PIECE_NB,
    NO_PIECE = -1
};

enum class Direction : i8 {
    NORTH      = 8,
    SOUTH      = -NORTH,
    EAST       = 1,
    WEST       = -EAST,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};

constexpr Color operator~(const Color c) { return static_cast<Color>(static_cast<int>(c) ^ 1); }

constexpr Square &operator++(Square &sq) {
    return sq = static_cast<Square>(static_cast<int>(sq) + 1);
}

constexpr Square &operator--(Square &sq) {
    return sq = static_cast<Square>(static_cast<int>(sq) - 1);
}

constexpr Square operator+(const Square sq, const Direction dir) {
    return static_cast<Square>(static_cast<int>(sq) + static_cast<int>(dir));
}

constexpr Square operator-(const Square sq, const Direction dir) {
    return static_cast<Square>(static_cast<int>(sq) - static_cast<int>(dir));
}

constexpr Square flip(const Square sq) { return static_cast<Square>(static_cast<int>(sq) ^ 56); }

constexpr Direction operator*(const Direction dir, const int mul) {
    return static_cast<Direction>(static_cast<int>(dir) * mul);
}