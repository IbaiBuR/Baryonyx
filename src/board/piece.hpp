#pragma once

#include <array>
#include <unordered_map>

#include "../chess.hpp"

namespace Board::Pieces {

constexpr std::array pieceToChar = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

const std::unordered_map<char, Piece> charToPiece = {
    {'P',   Piece::W_PAWN},
    {'N', Piece::W_KNIGHT},
    {'B', Piece::W_BISHOP},
    {'R',   Piece::W_ROOK},
    {'Q',  Piece::W_QUEEN},
    {'K',   Piece::W_KING},
    {'p',   Piece::B_PAWN},
    {'n', Piece::B_KNIGHT},
    {'b', Piece::B_BISHOP},
    {'r',   Piece::B_ROOK},
    {'q',  Piece::B_QUEEN},
    {'k',   Piece::B_KING}
};

constexpr std::array pieceToPieceType = {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP,
                                         PieceType::ROOK, PieceType::QUEEN,  PieceType::KING,
                                         PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP,
                                         PieceType::ROOK, PieceType::QUEEN,  PieceType::KING};

constexpr std::array pieceColor = {Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,
                                   Color::WHITE, Color::WHITE, Color::BLACK, Color::BLACK,
                                   Color::BLACK, Color::BLACK, Color::BLACK, Color::BLACK};

} // namespace Board::Pieces
