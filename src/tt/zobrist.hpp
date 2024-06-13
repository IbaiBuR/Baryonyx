#pragma once

#include <array>

#include "../chess.hpp"

namespace TT::Zobrist {

extern std::array<std::array<ZobristKey, 64>, 12> pieceKeys;
extern std::array<ZobristKey, 16>                 castlingRightsKeys;
extern std::array<ZobristKey, 8>                  enPassantKeys;
extern ZobristKey                                 sideToMoveKey;

void init();

} // namespace TT::Zobrist