#include "bitboard.hpp"

#include <format>
#include <iostream>

namespace Board::Bitboards {

void printBB(const Bitboard bitboard) {
    std::cout << std::endl;

    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << " " << rank + 1 << " ";

        for (U8 file = 0; file < 8; file++)
        {
            std::cout << " "
                      << (Bitboard::isBitSet(bitboard, Util::squareOf(file, rank)) ? '1' : '0');
        }

        std::cout << std::endl;
    }
    std::cout << std::endl << "    a b c d e f g h" << std::endl << std::endl;
    std::cout << std::format("    Bitboard: 0x{:016X}", bitboard.asU64()) << std::endl << std::endl;
}

} // namespace Board::Bitboards
