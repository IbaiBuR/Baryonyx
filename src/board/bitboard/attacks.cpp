#include "attacks.hpp"

namespace board::bitboards::attacks {

std::array<std::array<Bitboard, max_bishop_blockers_config>, constants::num_squares> bishop_attacks;
std::array<std::array<Bitboard, max_rook_blockers_config>, constants::num_squares>   rook_attacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam pt Piece type (Slider)
template <PieceType pt>
void init_sliders() {
    constexpr bool is_bishop = pt == PieceType::BISHOP;

    for (u8 sq = 0; sq < constants::num_squares; ++sq) {
        auto [mask, magic, shift] = is_bishop ? magics::bishop_magics[sq] : magics::rook_magics[sq];
        const int n_bits          = mask.bit_count();
        const int num_occupancies = 1 << n_bits;

        for (int i = 0; i < num_occupancies; ++i) {
            const Bitboard occupied = magics::set_blockers(i, n_bits, mask);
#ifndef USE_PEXT
            const auto magic_index = (occupied.as_u64() * magic) >> shift;
#else
            const auto magic_index = _pext_u64(occupied.as_u64(), mask.as_u64());
#endif

            if constexpr (is_bishop)
                bishop_attacks[sq][magic_index] =
                    gen_sliding<pt>(static_cast<Square>(sq), occupied);
            else
                rook_attacks[sq][magic_index] = gen_sliding<pt>(static_cast<Square>(sq), occupied);
        }
    }
}

void init() {
    init_sliders<PieceType::BISHOP>();
    init_sliders<PieceType::ROOK>();
}

} // namespace board::bitboards::attacks