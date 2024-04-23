#include "bitboard.hpp"

#include <iomanip>
#include <iostream>

namespace Board::Bitboards {

void printBB(const Bitboard bitboard) {
    std::cout << std::endl;

    for (U8 rank = 0; rank < 8; rank++)
    {
        std::cout << " " << 8 - rank << " ";

        for (U8 file = 0; file < 8; file++)
        {
            std::cout << " "
                      << (Bitboard::isBitSet(bitboard, Util::squareOf(file, rank)) ? '1' : '0');
        }

        std::cout << std::endl;
    }
    std::cout << std::endl << "    a b c d e f g h" << std::endl << std::endl;
    std::cout << "    Bitboard: 0x" << std::hex << std::setw(16) << std::setfill('0')
              << bitboard.asU64() << "ULL" << std::endl
              << std::endl;
}

} // namespace Board::Bitboards
