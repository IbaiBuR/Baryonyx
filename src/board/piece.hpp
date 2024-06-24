#pragma once

#include "../chess.hpp"

namespace board::pieces {

constexpr char piece_to_char(const piece p) {
    switch (p) {
    case piece::w_pawn:
        return 'P';
    case piece::w_knight:
        return 'N';
    case piece::w_bishop:
        return 'B';
    case piece::w_rook:
        return 'R';
    case piece::w_queen:
        return 'Q';
    case piece::w_king:
        return 'K';
    case piece::b_pawn:
        return 'p';
    case piece::b_knight:
        return 'n';
    case piece::b_bishop:
        return 'b';
    case piece::b_rook:
        return 'r';
    case piece::b_queen:
        return 'q';
    case piece::b_king:
        return 'k';
    default:
        return ' ';
    }
}

constexpr piece char_to_piece(const char c) {
    switch (c) {
    case 'P':
        return piece::w_pawn;
    case 'N':
        return piece::w_knight;
    case 'B':
        return piece::w_bishop;
    case 'R':
        return piece::w_rook;
    case 'Q':
        return piece::w_queen;
    case 'K':
        return piece::w_king;
    case 'p':
        return piece::b_pawn;
    case 'n':
        return piece::b_knight;
    case 'b':
        return piece::b_bishop;
    case 'r':
        return piece::b_rook;
    case 'q':
        return piece::b_queen;
    case 'k':
        return piece::b_king;
    default:
        return piece::none;
    }
}

constexpr piece_type piece_to_piece_type(const piece p) {
    switch (p) {
    case piece::w_pawn:
    case piece::b_pawn:
        return piece_type::pawn;
    case piece::w_knight:
    case piece::b_knight:
        return piece_type::knight;
    case piece::w_bishop:
    case piece::b_bishop:
        return piece_type::bishop;
    case piece::w_rook:
    case piece::b_rook:
        return piece_type::rook;
    case piece::w_queen:
    case piece::b_queen:
        return piece_type::queen;
    case piece::w_king:
    case piece::b_king:
        return piece_type::king;
    default:
        return piece_type::none;
    }
}

constexpr color piece_color(const piece p) {
    switch (p) {
    case piece::w_pawn:
    case piece::w_knight:
    case piece::w_bishop:
    case piece::w_rook:
    case piece::w_queen:
    case piece::w_king:
        return color::white;
    case piece::b_pawn:
    case piece::b_knight:
    case piece::b_bishop:
    case piece::b_rook:
    case piece::b_queen:
    case piece::b_king:
        return color::black;
    default:
        return color::none;
    }
}

} // namespace board::pieces
