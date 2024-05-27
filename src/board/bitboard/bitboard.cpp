#include "bitboard.hpp"

#include <print>

namespace Board::Bitboards {

void printBB(const Bitboard bitboard) {
    std::println("");

    for (int rank = 7; rank >= 0; --rank) {
        std::print("{}  ", rank + 1);

        for (U8 file = 0; file < 8; file++) {
            std::print("{} ", Bitboard::isBitSet(bitboard, Util::squareOf(file, rank)) ? '1' : '0');
        }

        std::println("");
    }
    std::println("\n   a b c d e f g h\n");
    std::println("   Bitboard: 0x{:016X}\n", bitboard.asU64());
}

} // namespace Board::Bitboards
