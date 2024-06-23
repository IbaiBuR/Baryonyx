#pragma once

#include <array>
#include <utility>

#include "../chess.hpp"
#include "../board/position.hpp"
#include "../utils/random.hpp"

/// @brief Zobrish Hashing implementation heavily inspired from Stormphrax
/// @note See https://github.com/Ciekce/Stormphrax/blob/main/src/keys.h for reference
namespace tt::zobrist {

constexpr usize piece_keys     = 12 * 64;
constexpr usize castling_keys  = 16;
constexpr usize en_passant_keys = 8;
constexpr usize side_to_move_key = 1;
constexpr usize total_keys     = piece_keys + castling_keys + en_passant_keys + side_to_move_key;

constexpr usize castling_keys_offset  = piece_keys;
constexpr usize en_passant_keys_offset = castling_keys_offset + castling_keys;
constexpr usize side_to_move_key_offset = en_passant_keys_offset + en_passant_keys;

extern const std::array<ZobristKey, total_keys> allKeys;

inline ZobristKey get_piece_key(const Piece p, const Square sq) {
    return p == Piece::NONE || sq == Square::NONE
             ? 0ULL
             : allKeys[std::to_underlying(p) + std::to_underlying(sq) * 12];
}

inline ZobristKey get_castling_key(const board::CastlingRights& castlingRights) {
    return allKeys[castling_keys_offset + castlingRights.as_u8()];
}

inline ZobristKey get_en_passant_key(const File f) {
    return allKeys[en_passant_keys_offset + std::to_underlying(f)];
}

inline ZobristKey get_en_passant_key(const Square epSq) {
    return epSq == Square::NONE ? 0ULL : get_en_passant_key(file_of(epSq));
}

inline ZobristKey get_side_key(const Color c) {
    return c == Color::WHITE ? 0ULL : allKeys[side_to_move_key_offset];
}

} // namespace tt::zobrist