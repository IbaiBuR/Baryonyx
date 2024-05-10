#include "attacks.hpp"

#include "magics.hpp"

namespace Board::Bitboards::Attacks {

std::array<std::array<Bitboard, maxBishopBlockersConfig>, static_cast<U8>(Square::SQUARE_NB)>
    bishopAttacks;
std::array<std::array<Bitboard, maxRookBlockersConfig>, static_cast<U8>(Square::SQUARE_NB)>
    rookAttacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam pt Piece type (Slider)
template <PieceType pt>
constexpr void initSliders() {
    constexpr bool isBishop = pt == PieceType::BISHOP;

    for (U8 sq = 0; sq < static_cast<U8>(Square::SQUARE_NB); sq++)
    {
        auto [mask, magic, shift] = isBishop ? Magics::bishopMagics[sq] : Magics::rookMagics[sq];
        const int nBits           = mask.bitCount();
        const int numOccupancies  = 1 << nBits;

        for (int i = 0; i < numOccupancies; i++)
        {
            const Bitboard occupied   = Magics::setBlockers(i, nBits, mask);
            const int      magicIndex = static_cast<int>((occupied.asU64() * magic) >> shift);

            if (isBishop)
                bishopAttacks[sq][magicIndex] = genSliding<pt>(static_cast<Square>(sq), occupied);
            else
                rookAttacks[sq][magicIndex] = genSliding<pt>(static_cast<Square>(sq), occupied);
        }
    }
}

void init() {
    initSliders<PieceType::BISHOP>();
    initSliders<PieceType::ROOK>();
}

Bitboard getBishopAttacks(Square sq, Bitboard blockers) {
    return bishopAttacks[static_cast<U8>(sq)]
                        [magicIndex(Magics::bishopMagics[static_cast<U8>(sq)], blockers)];
}

Bitboard getRookAttacks(Square sq, Bitboard blockers) {
    return rookAttacks[static_cast<U8>(sq)]
                      [magicIndex(Magics::rookMagics[static_cast<U8>(sq)], blockers)];
}


} // namespace Board::Bitboards::Attacks