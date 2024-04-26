#pragma once

#include <array>

#include "bitboard.hpp"

namespace Board::Bitboards::Magics {

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> bishopMasks = {
    {
     Bitboard(0x0040201008040200ULL), Bitboard(0x0000402010080400ULL),
     Bitboard(0x0000004020100a00ULL), Bitboard(0x0000000040221400ULL),
     Bitboard(0x0000000002442800ULL), Bitboard(0x0000000204085000ULL),
     Bitboard(0x0000020408102000ULL), Bitboard(0x0002040810204000ULL),
     Bitboard(0x0020100804020000ULL), Bitboard(0x0040201008040000ULL),
     Bitboard(0x00004020100a0000ULL), Bitboard(0x0000004022140000ULL),
     Bitboard(0x0000000244280000ULL), Bitboard(0x0000020408500000ULL),
     Bitboard(0x0002040810200000ULL), Bitboard(0x0004081020400000ULL),
     Bitboard(0x0010080402000200ULL), Bitboard(0x0020100804000400ULL),
     Bitboard(0x004020100a000a00ULL), Bitboard(0x0000402214001400ULL),
     Bitboard(0x0000024428002800ULL), Bitboard(0x0002040850005000ULL),
     Bitboard(0x0004081020002000ULL), Bitboard(0x0008102040004000ULL),
     Bitboard(0x0008040200020400ULL), Bitboard(0x0010080400040800ULL),
     Bitboard(0x0020100a000a1000ULL), Bitboard(0x0040221400142200ULL),
     Bitboard(0x0002442800284400ULL), Bitboard(0x0004085000500800ULL),
     Bitboard(0x0008102000201000ULL), Bitboard(0x0010204000402000ULL),
     Bitboard(0x0004020002040800ULL), Bitboard(0x0008040004081000ULL),
     Bitboard(0x00100a000a102000ULL), Bitboard(0x0022140014224000ULL),
     Bitboard(0x0044280028440200ULL), Bitboard(0x0008500050080400ULL),
     Bitboard(0x0010200020100800ULL), Bitboard(0x0020400040201000ULL),
     Bitboard(0x0002000204081000ULL), Bitboard(0x0004000408102000ULL),
     Bitboard(0x000a000a10204000ULL), Bitboard(0x0014001422400000ULL),
     Bitboard(0x0028002844020000ULL), Bitboard(0x0050005008040200ULL),
     Bitboard(0x0020002010080400ULL), Bitboard(0x0040004020100800ULL),
     Bitboard(0x0000020408102000ULL), Bitboard(0x0000040810204000ULL),
     Bitboard(0x00000a1020400000ULL), Bitboard(0x0000142240000000ULL),
     Bitboard(0x0000284402000000ULL), Bitboard(0x0000500804020000ULL),
     Bitboard(0x0000201008040200ULL), Bitboard(0x0000402010080400ULL),
     Bitboard(0x0002040810204000ULL), Bitboard(0x0004081020400000ULL),
     Bitboard(0x000a102040000000ULL), Bitboard(0x0014224000000000ULL),
     Bitboard(0x0028440200000000ULL), Bitboard(0x0050080402000000ULL),
     Bitboard(0x0020100804020000ULL), Bitboard(0x0040201008040200ULL),
     }
};

constexpr std::array<Bitboard, static_cast<U8>(Square::SQUARE_NB)> rookMasks = {
    {
     Bitboard(0x000101010101017eULL), Bitboard(0x000202020202027cULL),
     Bitboard(0x000404040404047aULL), Bitboard(0x0008080808080876ULL),
     Bitboard(0x001010101010106eULL), Bitboard(0x002020202020205eULL),
     Bitboard(0x004040404040403eULL), Bitboard(0x008080808080807eULL),
     Bitboard(0x0001010101017e00ULL), Bitboard(0x0002020202027c00ULL),
     Bitboard(0x0004040404047a00ULL), Bitboard(0x0008080808087600ULL),
     Bitboard(0x0010101010106e00ULL), Bitboard(0x0020202020205e00ULL),
     Bitboard(0x0040404040403e00ULL), Bitboard(0x0080808080807e00ULL),
     Bitboard(0x00010101017e0100ULL), Bitboard(0x00020202027c0200ULL),
     Bitboard(0x00040404047a0400ULL), Bitboard(0x0008080808760800ULL),
     Bitboard(0x00101010106e1000ULL), Bitboard(0x00202020205e2000ULL),
     Bitboard(0x00404040403e4000ULL), Bitboard(0x00808080807e8000ULL),
     Bitboard(0x000101017e010100ULL), Bitboard(0x000202027c020200ULL),
     Bitboard(0x000404047a040400ULL), Bitboard(0x0008080876080800ULL),
     Bitboard(0x001010106e101000ULL), Bitboard(0x002020205e202000ULL),
     Bitboard(0x004040403e404000ULL), Bitboard(0x008080807e808000ULL),
     Bitboard(0x0001017e01010100ULL), Bitboard(0x0002027c02020200ULL),
     Bitboard(0x0004047a04040400ULL), Bitboard(0x0008087608080800ULL),
     Bitboard(0x0010106e10101000ULL), Bitboard(0x0020205e20202000ULL),
     Bitboard(0x0040403e40404000ULL), Bitboard(0x0080807e80808000ULL),
     Bitboard(0x00017e0101010100ULL), Bitboard(0x00027c0202020200ULL),
     Bitboard(0x00047a0404040400ULL), Bitboard(0x0008760808080800ULL),
     Bitboard(0x00106e1010101000ULL), Bitboard(0x00205e2020202000ULL),
     Bitboard(0x00403e4040404000ULL), Bitboard(0x00807e8080808000ULL),
     Bitboard(0x007e010101010100ULL), Bitboard(0x007c020202020200ULL),
     Bitboard(0x007a040404040400ULL), Bitboard(0x0076080808080800ULL),
     Bitboard(0x006e101010101000ULL), Bitboard(0x005e202020202000ULL),
     Bitboard(0x003e404040404000ULL), Bitboard(0x007e808080808000ULL),
     Bitboard(0x7e01010101010100ULL), Bitboard(0x7c02020202020200ULL),
     Bitboard(0x7a04040404040400ULL), Bitboard(0x7608080808080800ULL),
     Bitboard(0x6e10101010101000ULL), Bitboard(0x5e20202020202000ULL),
     Bitboard(0x3e40404040404000ULL), Bitboard(0x7e80808080808000ULL),
     }
};

} // namespace Board::Bitboards::Magics