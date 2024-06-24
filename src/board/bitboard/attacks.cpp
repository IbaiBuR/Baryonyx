#include "attacks.hpp"

namespace board::bitboards::attacks {

std::array<std::array<bitboard, max_bishop_blockers_config>, constants::num_squares> bishop_attacks;
std::array<std::array<bitboard, max_rook_blockers_config>, constants::num_squares>   rook_attacks;

/// @brief Initializes the sliding attacks lookup tables
/// @tparam PieceType Piece type (Slider)
template <piece_type PieceType>
void init_sliders() {
    constexpr bool is_bishop = PieceType == piece_type::bishop;

    for (u8 sq = 0; sq < constants::num_squares; ++sq) {
        auto [mask, magic, shift] = is_bishop ? magics::bishop_magics[sq] : magics::rook_magics[sq];
        const int n_bits          = mask.bit_count();
        const int num_occupancies = 1 << n_bits;

        for (int i = 0; i < num_occupancies; ++i) {
            const bitboard occupied = magics::set_blockers(i, n_bits, mask);
#ifndef USE_PEXT
            const auto magic_index = (occupied.as_u64() * magic) >> shift;
#else
            const auto magic_index = _pext_u64(occupied.as_u64(), mask.as_u64());
#endif

            if constexpr (is_bishop)
                bishop_attacks[sq][magic_index] =
                    gen_sliding<PieceType>(static_cast<square>(sq), occupied);
            else
                rook_attacks[sq][magic_index] = gen_sliding<PieceType>(static_cast<square>(sq), occupied);
        }
    }
}

void init() {
    init_sliders<piece_type::bishop>();
    init_sliders<piece_type::rook>();
}

} // namespace board::bitboards::attacks