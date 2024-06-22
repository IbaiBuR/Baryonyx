#include "attacks.hpp"

namespace board::bitboards::attacks {

std::array<std::array<Bitboard, maxBishopBlockersConfig>, std::to_underlying(Square::SQUARE_NB)>
    bishopAttacks;

std::array<std::array<Bitboard, maxRookBlockersConfig>, std::to_underlying(Square::SQUARE_NB)>
    rookAttacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam pt Piece type (Slider)
template <PieceType pt>
void init_sliders() {
    constexpr bool isBishop = pt == PieceType::BISHOP;

    for (u8 sq = 0; sq < std::to_underlying(Square::SQUARE_NB); ++sq) {
        auto [mask, magic, shift] = isBishop ? magics::bishopMagics[sq] : magics::rookMagics[sq];
        const int nBits           = mask.bit_count();
        const int numOccupancies  = 1 << nBits;

        for (int i = 0; i < numOccupancies; i++) {
            const Bitboard occupied = magics::set_blockers(i, nBits, mask);
#ifndef USE_PEXT
            const auto magicIndex = (occupied.as_u64() * magic) >> shift;
#else
            const auto magicIndex = _pext_u64(occupied.asU64(), mask.asU64());
#endif

            if constexpr (isBishop)
                bishopAttacks[sq][magicIndex] = gen_sliding<pt>(static_cast<Square>(sq), occupied);
            else
                rookAttacks[sq][magicIndex] = gen_sliding<pt>(static_cast<Square>(sq), occupied);
        }
    }
}

void init() {
    init_sliders<PieceType::BISHOP>();
    init_sliders<PieceType::ROOK>();
}

} // namespace Board::Bitboards::Attacks