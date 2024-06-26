#pragma once

#include <array>
#include <utility>

#include "../chess.hpp"
#include "../board/position.hpp"

/// @brief Zobrish Hashing implementation heavily inspired from Stormphrax
/// @note See https://github.com/Ciekce/Stormphrax/blob/main/src/keys.h for reference
namespace utils::zobrist {

constexpr usize piece_keys       = constants::num_pieces * constants::num_squares;
constexpr usize castling_keys    = 1 << 4;
constexpr usize en_passant_keys  = constants::num_files;
constexpr usize side_to_move_key = 1;
constexpr usize total_keys       = piece_keys + castling_keys + en_passant_keys + side_to_move_key;

constexpr usize castling_keys_offset    = piece_keys;
constexpr usize en_passant_keys_offset  = castling_keys_offset + castling_keys;
constexpr usize side_to_move_key_offset = en_passant_keys_offset + en_passant_keys;

extern const std::array<zobrist_key, total_keys> all_keys;

inline zobrist_key get_piece_key(const piece p, const square sq) {
    return p == piece::none || sq == square::none
             ? 0ULL
             : all_keys[std::to_underlying(p) + std::to_underlying(sq) * 12];
}

inline zobrist_key get_castling_key(const board::castling_rights castling_rights) {
    return all_keys[castling_keys_offset + castling_rights.as_u8()];
}

inline zobrist_key get_en_passant_key(const file f) {
    return all_keys[en_passant_keys_offset + std::to_underlying(f)];
}

inline zobrist_key get_en_passant_key(const square ep_sq) {
    return ep_sq == square::none ? 0ULL : get_en_passant_key(file_of(ep_sq));
}

inline zobrist_key get_side_key(const color c) {
    return c == color::white ? 0ULL : all_keys[side_to_move_key_offset];
}

} // namespace utils::zobrist