#include "zobrist.hpp"

#include "../utils.hpp"

namespace TT::Zobrist {

std::array<std::array<ZobristKey, 64>, 12> pieceKeys;
std::array<ZobristKey, 16>                 castlingRightsKeys;
std::array<ZobristKey, 8>                  enPassantKeys;
ZobristKey                                 sideToMoveKey;

void init() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 64; ++j) {
            pieceKeys[i][j] = Utils::randomU64();
        }
    }

    for (int i = 0; i < 16; ++i)
        castlingRightsKeys[i] = Utils::randomU64();

    for (int i = 0; i < 8; ++i)
        enPassantKeys[i] = Utils::randomU64();

    sideToMoveKey = Utils::randomU64();
}

} // namespace TT::Zobrist
