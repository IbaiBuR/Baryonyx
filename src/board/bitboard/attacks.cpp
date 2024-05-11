#include "attacks.hpp"

#include "magics.hpp"

namespace Board::Bitboards::Attacks {

std::array<std::array<Bitboard, maxBishopBlockersConfig>, std::to_underlying(Square::SQUARE_NB)>
    bishopAttacks;
std::array<std::array<Bitboard, maxRookBlockersConfig>, std::to_underlying(Square::SQUARE_NB)>
    rookAttacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam pt Piece type (Slider)
template <PieceType pt>
constexpr void initSliders() {
    constexpr bool isBishop = pt == PieceType::BISHOP;

    for (U8 sq = 0; sq < std::to_underlying(Square::SQUARE_NB); sq++)
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

Bitboard getBishopAttacks(const Square sq, Bitboard blockers) {
    return bishopAttacks[std::to_underlying(sq)]
                        [magicIndex(Magics::bishopMagics[std::to_underlying(sq)], blockers)];
}

Bitboard getRookAttacks(const Square sq, Bitboard blockers) {
    return rookAttacks[std::to_underlying(sq)]
                      [magicIndex(Magics::rookMagics[std::to_underlying(sq)], blockers)];
}


} // namespace Board::Bitboards::Attacks