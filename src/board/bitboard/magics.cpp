#include "magics.hpp"

namespace Board::Bitboards::Magics {

Bitboard setBlockers(const int index, Bitboard mask) {
    Bitboard blockers;

    for (int i = 0; i < mask.bitCount(); i++)
        if (index & (1 << i))
            blockers |= Bitboard::fromSquare(static_cast<Square>(mask.popLSB()));

    return blockers;
}

} // namespace Board::Bitboards::Magics
