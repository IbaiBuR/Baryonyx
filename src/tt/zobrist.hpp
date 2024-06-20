#pragma once

#include <array>
#include <utility>

#include "../chess.hpp"
#include "../board/position.hpp"
#include "../utils/random.hpp"

/// @brief Zobrish Hashing implementation heavily inspired from Stormphrax
/// @note See https://github.com/Ciekce/Stormphrax/blob/main/src/keys.h for reference
namespace TT::Zobrist {

constexpr usize pieceKeys     = 12 * 64;
constexpr usize castlingKeys  = 16;
constexpr usize enPassantKeys = 8;
constexpr usize sideToMoveKey = 1;
constexpr usize totalKeys     = pieceKeys + castlingKeys + enPassantKeys + sideToMoveKey;

constexpr usize castlingKeysOffset  = pieceKeys;
constexpr usize enPassantKeysOffset = castlingKeysOffset + castlingKeys;
constexpr usize sideToMoveKeyOffset = enPassantKeysOffset + enPassantKeys;

extern const std::array<ZobristKey, totalKeys> allKeys;

inline ZobristKey getPieceKey(const Piece p, const Square sq) {
    return p == Piece::NONE || sq == Square::NONE
             ? 0ULL
             : allKeys[std::to_underlying(p) + std::to_underlying(sq) * 12];
}

inline ZobristKey getCastlingKey(const Board::CastlingRights &castlingRights) {
    return allKeys[castlingKeysOffset + castlingRights.asU8()];
}

inline ZobristKey getEnPassantKey(const File f) {
    return allKeys[enPassantKeysOffset + std::to_underlying(f)];
}

inline ZobristKey getEnPassantKey(const Square epSq) {
    return epSq == Square::NONE ? 0ULL : getEnPassantKey(fileOf(epSq));
}

inline ZobristKey getSideKey(const Color c) {
    return c == Color::WHITE ? 0ULL : allKeys[sideToMoveKeyOffset];
}

} // namespace TT::Zobrist