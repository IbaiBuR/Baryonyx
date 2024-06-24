#include "bitboard.hpp"

#include <format>
#include <iostream>

namespace board::bitboards {

void print_bb(const bitboard& bitboard) {
    std::cout << std::endl;

    for (int rank = constants::num_ranks - 1; rank >= 0; --rank) {
        std::cout << std::format("{} ", rank + 1);

        for (u8 file = 0; file < constants::num_files; ++file) {
            std::cout << std::format(
                "{} ", bitboard::is_bit_set(bitboard, square_of(file, rank)) ? '1' : '0');
        }

        std::cout << std::endl;
    }
    std::cout << std::format("\n   a b c d e f g h\n") << std::endl;
    std::cout << std::format("   Bitboard: 0x{:016X}\n", bitboard.as_u64()) << std::endl;
}

} // namespace board::bitboards
