#pragma once

#include <array>
#include <unordered_map>

#include "../types.hpp"

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

const std::unordered_map<Piece, PieceType> pieceToPieceType = {
    {  Piece::W_PAWN,   PieceType::PAWN},
    {Piece::W_KNIGHT, PieceType::KNIGHT},
    {Piece::W_BISHOP, PieceType::BISHOP},
    {  Piece::W_ROOK,   PieceType::ROOK},
    { Piece::W_QUEEN,  PieceType::QUEEN},
    {  Piece::W_KING,   PieceType::KING},
    {  Piece::B_PAWN,   PieceType::PAWN},
    {Piece::B_KNIGHT, PieceType::KNIGHT},
    {Piece::B_BISHOP, PieceType::BISHOP},
    {  Piece::B_ROOK,   PieceType::ROOK},
    { Piece::B_QUEEN,  PieceType::QUEEN},
    {  Piece::B_KING,   PieceType::KING}
};

} // namespace Board::Pieces
