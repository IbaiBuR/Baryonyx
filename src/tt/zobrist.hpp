#pragma once

#include <array>
#include <utility>

#include "../chess.hpp"
#include "../board/position.hpp"
#include "../utils/random.hpp"

/// @brief Zobrish Hashing implementation heavily inspired from Stormphrax
/// @note See https://github.com/Ciekce/Stormphrax/blob/main/src/keys.h for reference
namespace tt::zobrist {

constexpr usize pieceKeys     = 12 * 64;
constexpr usize castlingKeys  = 16;
constexpr usize enPassantKeys = 8;
constexpr usize sideToMoveKey = 1;
constexpr usize totalKeys     = pieceKeys + castlingKeys + enPassantKeys + sideToMoveKey;

constexpr usize castlingKeysOffset  = pieceKeys;
constexpr usize enPassantKeysOffset = castlingKeysOffset + castlingKeys;
constexpr usize sideToMoveKeyOffset = enPassantKeysOffset + enPassantKeys;

extern const std::array<ZobristKey, totalKeys> allKeys;

inline ZobristKey get_piece_key(const Piece p, const Square sq) {
    return p == Piece::NONE || sq == Square::NONE
             ? 0ULL
             : allKeys[std::to_underlying(p) + std::to_underlying(sq) * 12];
}

inline ZobristKey get_castling_key(const board::CastlingRights &castlingRights) {
    return allKeys[castlingKeysOffset + castlingRights.as_u8()];
}

inline ZobristKey get_en_passant_key(const File f) {
    return allKeys[enPassantKeysOffset + std::to_underlying(f)];
}

inline ZobristKey get_en_passant_key(const Square epSq) {
    return epSq == Square::NONE ? 0ULL : get_en_passant_key(file_of(epSq));
}

inline ZobristKey get_side_key(const Color c) {
    return c == Color::WHITE ? 0ULL : allKeys[sideToMoveKeyOffset];
}

} // namespace TT::Zobrist