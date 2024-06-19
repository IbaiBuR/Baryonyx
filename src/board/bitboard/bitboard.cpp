#include "bitboard.hpp"

#include <format>
#include <iostream>

namespace Board::Bitboards {

void printBB(const Bitboard bitboard) {
    std::cout << std::endl;

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << std::format("{} ", rank + 1);

        for (u8 file = 0; file < 8; file++) {
            std::cout << std::format(
                "{} ", Bitboard::isBitSet(bitboard, squareOf(file, rank)) ? '1' : '0');
        }

        std::cout << std::endl;
    }
    std::cout << std::format("\n   a b c d e f g h\n") << std::endl;
    std::cout << std::format("   Bitboard: 0x{:016X}\n", bitboard.asU64()) << std::endl;
}

} // namespace Board::Bitboards
