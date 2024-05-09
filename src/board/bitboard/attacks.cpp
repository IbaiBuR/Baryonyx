#include "attacks.hpp"

namespace Board::Bitboards::Attacks {

std::array<std::array<Bitboard, maxBishopBlockersConfig>, static_cast<U8>(Square::SQUARE_NB)>
    bishopAttacks;
std::array<std::array<Bitboard, maxRookBlockersConfig>, static_cast<U8>(Square::SQUARE_NB)>
    rookAttacks;

} // namespace Board::Bitboards::Attacks