#pragma once

#include "../chess.hpp"

namespace board::pieces {

constexpr char piece_to_char(const Piece p) {
    switch (p) {
    case Piece::W_PAWN:
        return 'P';
    case Piece::W_KNIGHT:
        return 'N';
    case Piece::W_BISHOP:
        return 'B';
    case Piece::W_ROOK:
        return 'R';
    case Piece::W_QUEEN:
        return 'Q';
    case Piece::W_KING:
        return 'K';
    case Piece::B_PAWN:
        return 'p';
    case Piece::B_KNIGHT:
        return 'n';
    case Piece::B_BISHOP:
        return 'b';
    case Piece::B_ROOK:
        return 'r';
    case Piece::B_QUEEN:
        return 'q';
    case Piece::B_KING:
        return 'k';
    default:
        return ' ';
    }
}

constexpr Piece char_to_piece(const char c) {
    switch (c) {
    case 'P':
        return Piece::W_PAWN;
    case 'N':
        return Piece::W_KNIGHT;
    case 'B':
        return Piece::W_BISHOP;
    case 'R':
        return Piece::W_ROOK;
    case 'Q':
        return Piece::W_QUEEN;
    case 'K':
        return Piece::W_KING;
    case 'p':
        return Piece::B_PAWN;
    case 'n':
        return Piece::B_KNIGHT;
    case 'b':
        return Piece::B_BISHOP;
    case 'r':
        return Piece::B_ROOK;
    case 'q':
        return Piece::B_QUEEN;
    case 'k':
        return Piece::B_KING;
    default:
        return Piece::NONE;
    }
}

constexpr PieceType piece_to_piece_type(const Piece p) {
    switch (p) {
    case Piece::W_PAWN:
    case Piece::B_PAWN:
        return PieceType::PAWN;
    case Piece::W_KNIGHT:
    case Piece::B_KNIGHT:
        return PieceType::KNIGHT;
    case Piece::W_BISHOP:
    case Piece::B_BISHOP:
        return PieceType::BISHOP;
    case Piece::W_ROOK:
    case Piece::B_ROOK:
        return PieceType::ROOK;
    case Piece::W_QUEEN:
    case Piece::B_QUEEN:
        return PieceType::QUEEN;
    case Piece::W_KING:
    case Piece::B_KING:
        return PieceType::KING;
    default:
        return PieceType::NONE;
    }
}

constexpr Color piece_color(const Piece p) {
    switch (p) {
    case Piece::W_PAWN:
    case Piece::W_KNIGHT:
    case Piece::W_BISHOP:
    case Piece::W_ROOK:
    case Piece::W_QUEEN:
    case Piece::W_KING:
        return Color::WHITE;
    case Piece::B_PAWN:
    case Piece::B_KNIGHT:
    case Piece::B_BISHOP:
    case Piece::B_ROOK:
    case Piece::B_QUEEN:
    case Piece::B_KING:
        return Color::BLACK;
    default:
        return Color::NONE;
    }
}

} // namespace board::pieces
