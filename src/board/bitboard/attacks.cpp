#include "attacks.hpp"

namespace board::bitboards::attacks {

std::array<std::array<Bitboard, max_bishop_blockers_config>, constants::num_squares> bishop_attacks;
std::array<std::array<Bitboard, max_rook_blockers_config>, constants::num_squares>   rook_attacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam pt Piece type (Slider)
template <PieceType pt>
void init_sliders() {
    constexpr bool isBishop = pt == PieceType::BISHOP;

    for (u8 sq = 0; sq < constants::num_squares; ++sq) {
        auto [mask, magic, shift] = isBishop ? magics::bishop_magics[sq] : magics::rook_magics[sq];
        const int nBits           = mask.bit_count();
        const int numOccupancies  = 1 << nBits;

        for (int i = 0; i < numOccupancies; i++) {
            const Bitboard occupied = magics::set_blockers(i, nBits, mask);
#ifndef USE_PEXT
            const auto magicIndex = (occupied.as_u64() * magic) >> shift;
#else
            const auto magicIndex = _pext_u64(occupied.as_u64(), mask.as_u64());
#endif

            if constexpr (isBishop)
                bishop_attacks[sq][magicIndex] = gen_sliding<pt>(static_cast<Square>(sq), occupied);
            else
                rook_attacks[sq][magicIndex] = gen_sliding<pt>(static_cast<Square>(sq), occupied);
        }
    }
}

void init() {
    init_sliders<PieceType::BISHOP>();
    init_sliders<PieceType::ROOK>();
}

} // namespace board::bitboards::attacks