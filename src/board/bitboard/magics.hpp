#pragma once

#include <array>

#ifdef USE_PEXT
    #include <immintrin.h>
#endif

#include "bitboard.hpp"

namespace board::bitboards::magics {

struct magic_entry {
        bitboard mask;
        u64      magic;
        int      shift;
};

/// @brief Pre-calculated bishop attack masks
inline constexpr std::array<bitboard, constants::num_squares> bishop_masks = {
    {bitboard(0x0040201008040200ULL), bitboard(0x0000402010080400ULL),
     bitboard(0x0000004020100A00ULL), bitboard(0x0000000040221400ULL),
     bitboard(0x0000000002442800ULL), bitboard(0x0000000204085000ULL),
     bitboard(0x0000020408102000ULL), bitboard(0x0002040810204000ULL),
     bitboard(0x0020100804020000ULL), bitboard(0x0040201008040000ULL),
     bitboard(0x00004020100A0000ULL), bitboard(0x0000004022140000ULL),
     bitboard(0x0000000244280000ULL), bitboard(0x0000020408500000ULL),
     bitboard(0x0002040810200000ULL), bitboard(0x0004081020400000ULL),
     bitboard(0x0010080402000200ULL), bitboard(0x0020100804000400ULL),
     bitboard(0x004020100A000A00ULL), bitboard(0x0000402214001400ULL),
     bitboard(0x0000024428002800ULL), bitboard(0x0002040850005000ULL),
     bitboard(0x0004081020002000ULL), bitboard(0x0008102040004000ULL),
     bitboard(0x0008040200020400ULL), bitboard(0x0010080400040800ULL),
     bitboard(0x0020100A000A1000ULL), bitboard(0x0040221400142200ULL),
     bitboard(0x0002442800284400ULL), bitboard(0x0004085000500800ULL),
     bitboard(0x0008102000201000ULL), bitboard(0x0010204000402000ULL),
     bitboard(0x0004020002040800ULL), bitboard(0x0008040004081000ULL),
     bitboard(0x00100A000A102000ULL), bitboard(0x0022140014224000ULL),
     bitboard(0x0044280028440200ULL), bitboard(0x0008500050080400ULL),
     bitboard(0x0010200020100800ULL), bitboard(0x0020400040201000ULL),
     bitboard(0x0002000204081000ULL), bitboard(0x0004000408102000ULL),
     bitboard(0x000A000A10204000ULL), bitboard(0x0014001422400000ULL),
     bitboard(0x0028002844020000ULL), bitboard(0x0050005008040200ULL),
     bitboard(0x0020002010080400ULL), bitboard(0x0040004020100800ULL),
     bitboard(0x0000020408102000ULL), bitboard(0x0000040810204000ULL),
     bitboard(0x00000A1020400000ULL), bitboard(0x0000142240000000ULL),
     bitboard(0x0000284402000000ULL), bitboard(0x0000500804020000ULL),
     bitboard(0x0000201008040200ULL), bitboard(0x0000402010080400ULL),
     bitboard(0x0002040810204000ULL), bitboard(0x0004081020400000ULL),
     bitboard(0x000A102040000000ULL), bitboard(0x0014224000000000ULL),
     bitboard(0x0028440200000000ULL), bitboard(0x0050080402000000ULL),
     bitboard(0x0020100804020000ULL), bitboard(0x0040201008040200ULL)}
};

/// @brief Pre-calculated rook attack masks
inline constexpr std::array<bitboard, constants::num_squares> rook_masks = {
    {bitboard(0x000101010101017EULL), bitboard(0x000202020202027CULL),
     bitboard(0x000404040404047AULL), bitboard(0x0008080808080876ULL),
     bitboard(0x001010101010106EULL), bitboard(0x002020202020205EULL),
     bitboard(0x004040404040403EULL), bitboard(0x008080808080807EULL),
     bitboard(0x0001010101017E00ULL), bitboard(0x0002020202027C00ULL),
     bitboard(0x0004040404047A00ULL), bitboard(0x0008080808087600ULL),
     bitboard(0x0010101010106E00ULL), bitboard(0x0020202020205E00ULL),
     bitboard(0x0040404040403E00ULL), bitboard(0x0080808080807E00ULL),
     bitboard(0x00010101017E0100ULL), bitboard(0x00020202027C0200ULL),
     bitboard(0x00040404047A0400ULL), bitboard(0x0008080808760800ULL),
     bitboard(0x00101010106E1000ULL), bitboard(0x00202020205E2000ULL),
     bitboard(0x00404040403E4000ULL), bitboard(0x00808080807E8000ULL),
     bitboard(0x000101017E010100ULL), bitboard(0x000202027C020200ULL),
     bitboard(0x000404047A040400ULL), bitboard(0x0008080876080800ULL),
     bitboard(0x001010106E101000ULL), bitboard(0x002020205E202000ULL),
     bitboard(0x004040403E404000ULL), bitboard(0x008080807E808000ULL),
     bitboard(0x0001017E01010100ULL), bitboard(0x0002027C02020200ULL),
     bitboard(0x0004047A04040400ULL), bitboard(0x0008087608080800ULL),
     bitboard(0x0010106E10101000ULL), bitboard(0x0020205E20202000ULL),
     bitboard(0x0040403E40404000ULL), bitboard(0x0080807E80808000ULL),
     bitboard(0x00017E0101010100ULL), bitboard(0x00027C0202020200ULL),
     bitboard(0x00047A0404040400ULL), bitboard(0x0008760808080800ULL),
     bitboard(0x00106E1010101000ULL), bitboard(0x00205E2020202000ULL),
     bitboard(0x00403E4040404000ULL), bitboard(0x00807E8080808000ULL),
     bitboard(0x007E010101010100ULL), bitboard(0x007C020202020200ULL),
     bitboard(0x007A040404040400ULL), bitboard(0x0076080808080800ULL),
     bitboard(0x006E101010101000ULL), bitboard(0x005E202020202000ULL),
     bitboard(0x003E404040404000ULL), bitboard(0x007E808080808000ULL),
     bitboard(0x7E01010101010100ULL), bitboard(0x7C02020202020200ULL),
     bitboard(0x7A04040404040400ULL), bitboard(0x7608080808080800ULL),
     bitboard(0x6E10101010101000ULL), bitboard(0x5E20202020202000ULL),
     bitboard(0x3E40404040404000ULL), bitboard(0x7E80808080808000ULL)}
};

inline constexpr std::array<magic_entry, constants::num_squares> bishop_magics = {
    {magic_entry(bitboard(0x0040201008040200ULL), 0x020A262403220200ULL, 58),
     magic_entry(bitboard(0x0000402010080400ULL), 0x8020040100411809ULL, 59),
     magic_entry(bitboard(0x0000004020100A00ULL), 0xC122021042008110ULL, 59),
     magic_entry(bitboard(0x0000000040221400ULL), 0x2B080A0020200011ULL, 59),
     magic_entry(bitboard(0x0000000002442800ULL), 0x8524042002010610ULL, 59),
     magic_entry(bitboard(0x0000000204085000ULL), 0x1058280808000000ULL, 59),
     magic_entry(bitboard(0x0000020408102000ULL), 0x8000889008204000ULL, 59),
     magic_entry(bitboard(0x0002040810204000ULL), 0x0287008080884121ULL, 58),
     magic_entry(bitboard(0x0020100804020000ULL), 0x03020428D8010C00ULL, 59),
     magic_entry(bitboard(0x0040201008040000ULL), 0x010020120A204100ULL, 59),
     magic_entry(bitboard(0x00004020100A0000ULL), 0x2000080081320000ULL, 59),
     magic_entry(bitboard(0x0000004022140000ULL), 0x0084040408810244ULL, 59),
     magic_entry(bitboard(0x0000000244280000ULL), 0x00A2440421140002ULL, 59),
     magic_entry(bitboard(0x0000020408500000ULL), 0xC000008220204418ULL, 59),
     magic_entry(bitboard(0x0002040810200000ULL), 0x8000020804144600ULL, 59),
     magic_entry(bitboard(0x0004081020400000ULL), 0x8C01004044242000ULL, 59),
     magic_entry(bitboard(0x0010080402000200ULL), 0x1420A14005010200ULL, 59),
     magic_entry(bitboard(0x0020100804000400ULL), 0x80201002422C5100ULL, 59),
     magic_entry(bitboard(0x004020100A000A00ULL), 0x0102002104040A80ULL, 57),
     magic_entry(bitboard(0x0000402214001400ULL), 0x2108000082004024ULL, 57),
     magic_entry(bitboard(0x0000024428002800ULL), 0x0004010C80A00840ULL, 57),
     magic_entry(bitboard(0x0002040850005000ULL), 0x00090000B0180400ULL, 57),
     magic_entry(bitboard(0x0004081020002000ULL), 0x0624100202020241ULL, 59),
     magic_entry(bitboard(0x0008102040004000ULL), 0x109D080200849440ULL, 59),
     magic_entry(bitboard(0x0008040200020400ULL), 0x0408202042020220ULL, 59),
     magic_entry(bitboard(0x0010080400040800ULL), 0x1030824850140520ULL, 59),
     magic_entry(bitboard(0x0020100A000A1000ULL), 0x0424010010070420ULL, 57),
     magic_entry(bitboard(0x0040221400142200ULL), 0x4049080003004100ULL, 55),
     magic_entry(bitboard(0x0002442800284400ULL), 0x0101010100104004ULL, 55),
     magic_entry(bitboard(0x0004085000500800ULL), 0x040802000C4106C4ULL, 57),
     magic_entry(bitboard(0x0008102000201000ULL), 0x0104009034024100ULL, 59),
     magic_entry(bitboard(0x0010204000402000ULL), 0x8002124082006601ULL, 59),
     magic_entry(bitboard(0x0004020002040800ULL), 0x0001184088614480ULL, 59),
     magic_entry(bitboard(0x0008040004081000ULL), 0x0000C42080100208ULL, 59),
     magic_entry(bitboard(0x00100A000A102000ULL), 0x0412280404880820ULL, 57),
     magic_entry(bitboard(0x0022140014224000ULL), 0x0388020080880082ULL, 55),
     magic_entry(bitboard(0x0044280028440200ULL), 0x0001010100100404ULL, 55),
     magic_entry(bitboard(0x0008500050080400ULL), 0x80080809A0031000ULL, 57),
     magic_entry(bitboard(0x0010200020100800ULL), 0x00043800809210B0ULL, 59),
     magic_entry(bitboard(0x0020400040201000ULL), 0x4044004084004400ULL, 59),
     magic_entry(bitboard(0x0002000204081000ULL), 0x00080A1320081100ULL, 59),
     magic_entry(bitboard(0x0004000408102000ULL), 0x06004422A8002001ULL, 59),
     magic_entry(bitboard(0x000A000A10204000ULL), 0x0002020422040C00ULL, 57),
     magic_entry(bitboard(0x0014001422400000ULL), 0x84200202140A1200ULL, 57),
     magic_entry(bitboard(0x0028002844020000ULL), 0x010002060E000400ULL, 57),
     magic_entry(bitboard(0x0050005008040200ULL), 0x0020210A1A000088ULL, 57),
     magic_entry(bitboard(0x0020002010080400ULL), 0x00A8080080904420ULL, 59),
     magic_entry(bitboard(0x0040004020100800ULL), 0x4004694041000202ULL, 59),
     magic_entry(bitboard(0x0000020408102000ULL), 0x2C0041501010E010ULL, 59),
     magic_entry(bitboard(0x0000040810204000ULL), 0x0685041202020080ULL, 59),
     magic_entry(bitboard(0x00000A1020400000ULL), 0x20050B0049100C04ULL, 59),
     magic_entry(bitboard(0x0000142240000000ULL), 0x0900020084110488ULL, 59),
     magic_entry(bitboard(0x0000284402000000ULL), 0x4003204188220008ULL, 59),
     magic_entry(bitboard(0x0000500804020000ULL), 0x8010200830032201ULL, 59),
     magic_entry(bitboard(0x0000201008040200ULL), 0x0804900288050040ULL, 59),
     magic_entry(bitboard(0x0000402010080400ULL), 0x2820020420408006ULL, 59),
     magic_entry(bitboard(0x0002040810204000ULL), 0x0010404400884000ULL, 58),
     magic_entry(bitboard(0x0004081020400000ULL), 0x2800104844100804ULL, 59),
     magic_entry(bitboard(0x000A102040000000ULL), 0x0000000201012100ULL, 59),
     magic_entry(bitboard(0x0014224000000000ULL), 0x4080450502840418ULL, 59),
     magic_entry(bitboard(0x0028440200000000ULL), 0x0002000008210101ULL, 59),
     magic_entry(bitboard(0x0050080402000000ULL), 0x021A000408504100ULL, 59),
     magic_entry(bitboard(0x0020100804020000ULL), 0x2088420401140100ULL, 59),
     magic_entry(bitboard(0x0040201008040200ULL), 0x082104100A002120ULL, 58)}
};

inline constexpr std::array<magic_entry, constants::num_squares> rook_magics = {
    {magic_entry(bitboard(0x000101010101017EULL), 0x2080004000142088ULL, 52),
     magic_entry(bitboard(0x000202020202027CULL), 0x4840001000200048ULL, 53),
     magic_entry(bitboard(0x000404040404047AULL), 0x0880200208801000ULL, 53),
     magic_entry(bitboard(0x0008080808080876ULL), 0x0100100009000420ULL, 53),
     magic_entry(bitboard(0x001010101010106EULL), 0x0200040821100200ULL, 53),
     magic_entry(bitboard(0x002020202020205EULL), 0x9080040080010200ULL, 53),
     magic_entry(bitboard(0x004040404040403EULL), 0x0200480082000104ULL, 53),
     magic_entry(bitboard(0x008080808080807EULL), 0x0200102201088044ULL, 52),
     magic_entry(bitboard(0x0001010101017E00ULL), 0x00038000804000AAULL, 53),
     magic_entry(bitboard(0x0002020202027C00ULL), 0x8084804000200080ULL, 54),
     magic_entry(bitboard(0x0004040404047A00ULL), 0x2000801000200080ULL, 54),
     magic_entry(bitboard(0x0008080808087600ULL), 0x2009001001000820ULL, 54),
     magic_entry(bitboard(0x0010101010106E00ULL), 0x2000800800040080ULL, 54),
     magic_entry(bitboard(0x0020202020205E00ULL), 0x2002000805620030ULL, 54),
     magic_entry(bitboard(0x0040404040403E00ULL), 0x0004000201044810ULL, 54),
     magic_entry(bitboard(0x0080808080807E00ULL), 0x01C2000094204102ULL, 53),
     magic_entry(bitboard(0x00010101017E0100ULL), 0x0040008020800040ULL, 53),
     magic_entry(bitboard(0x00020202027C0200ULL), 0x2A40010020408100ULL, 54),
     magic_entry(bitboard(0x00040404047A0400ULL), 0x00A1828020021004ULL, 54),
     magic_entry(bitboard(0x0008080808760800ULL), 0x0000420020100A01ULL, 54),
     magic_entry(bitboard(0x00101010106E1000ULL), 0x2490818004010800ULL, 54),
     magic_entry(bitboard(0x00202020205E2000ULL), 0x1000808002000400ULL, 54),
     magic_entry(bitboard(0x00404040403E4000ULL), 0x0000040001081002ULL, 54),
     magic_entry(bitboard(0x00808080807E8000ULL), 0x0100920009826504ULL, 53),
     magic_entry(bitboard(0x000101017E010100ULL), 0x06148164800A4000ULL, 53),
     magic_entry(bitboard(0x000202027C020200ULL), 0x4320100040204004ULL, 54),
     magic_entry(bitboard(0x000404047A040400ULL), 0x0830080020002400ULL, 54),
     magic_entry(bitboard(0x0008080876080800ULL), 0x0000080080100082ULL, 54),
     magic_entry(bitboard(0x001010106E101000ULL), 0x0002050100080010ULL, 54),
     magic_entry(bitboard(0x002020205E202000ULL), 0x0000040080020080ULL, 54),
     magic_entry(bitboard(0x004040403E404000ULL), 0x0051000100040200ULL, 54),
     magic_entry(bitboard(0x008080807E808000ULL), 0x0805B406000088D1ULL, 53),
     magic_entry(bitboard(0x0001017E01010100ULL), 0x035226400080008CULL, 53),
     magic_entry(bitboard(0x0002027C02020200ULL), 0x0300802000804003ULL, 54),
     magic_entry(bitboard(0x0004047A04040400ULL), 0x80A0004101002810ULL, 54),
     magic_entry(bitboard(0x0008087608080800ULL), 0x0200801004800800ULL, 54),
     magic_entry(bitboard(0x0010106E10101000ULL), 0x0200800800800401ULL, 54),
     magic_entry(bitboard(0x0020205E20202000ULL), 0x2202000852001430ULL, 54),
     magic_entry(bitboard(0x0040403E40404000ULL), 0x0025000401000200ULL, 54),
     magic_entry(bitboard(0x0080807E80808000ULL), 0x0200004402000081ULL, 53),
     magic_entry(bitboard(0x00017E0101010100ULL), 0x001E806040108000ULL, 53),
     magic_entry(bitboard(0x00027C0202020200ULL), 0x0010002000504000ULL, 54),
     magic_entry(bitboard(0x00047A0404040400ULL), 0x0101002000110040ULL, 54),
     magic_entry(bitboard(0x0008760808080800ULL), 0x0000100008008080ULL, 54),
     magic_entry(bitboard(0x00106E1010101000ULL), 0x0A04008040080800ULL, 54),
     magic_entry(bitboard(0x00205E2020202000ULL), 0x2043104004080120ULL, 54),
     magic_entry(bitboard(0x00403E4040404000ULL), 0x4200010802040010ULL, 54),
     magic_entry(bitboard(0x00807E8080808000ULL), 0x0080008064020001ULL, 53),
     magic_entry(bitboard(0x007E010101010100ULL), 0x04048222084D0200ULL, 53),
     magic_entry(bitboard(0x007C020202020200ULL), 0x2450002000400140ULL, 54),
     magic_entry(bitboard(0x007A040404040400ULL), 0x1800200010008080ULL, 54),
     magic_entry(bitboard(0x0076080808080800ULL), 0x0012004110240A00ULL, 54),
     magic_entry(bitboard(0x006E101010101000ULL), 0x8124004800800480ULL, 54),
     magic_entry(bitboard(0x005E202020202000ULL), 0x0040040002008080ULL, 54),
     magic_entry(bitboard(0x003E404040404000ULL), 0x0400109238110400ULL, 54),
     magic_entry(bitboard(0x007E808080808000ULL), 0x4248410400409200ULL, 53),
     magic_entry(bitboard(0x7E01010101010100ULL), 0x00F0224101800031ULL, 52),
     magic_entry(bitboard(0x7C02020202020200ULL), 0x0240144020860102ULL, 53),
     magic_entry(bitboard(0x7A04040404040400ULL), 0x0001020841A00091ULL, 53),
     magic_entry(bitboard(0x7608080808080800ULL), 0x0101001002200409ULL, 53),
     magic_entry(bitboard(0x6E10101010101000ULL), 0x0402002024100856ULL, 53),
     magic_entry(bitboard(0x5E20202020202000ULL), 0x0201000812040025ULL, 53),
     magic_entry(bitboard(0x3E40404040404000ULL), 0x000A0010A1040842ULL, 53),
     magic_entry(bitboard(0x7E80808080808000ULL), 0x0000040100804022ULL, 52)}
};

void print_magics();

/// @brief Utility used to generate all the blocker combinations of a mask
/// @param index Index for blocker configuration
/// @param n_bits Number of bits set in the mask
/// @param mask Attack mask
/// @returns The generated blocker configuration for the index
bitboard set_blockers(int index, int n_bits, bitboard mask);

template <piece_type PieceType>
constexpr bitboard get_mask(const square sq) {
    if constexpr (PieceType == piece_type::bishop)
        return bishop_masks[std::to_underlying(sq)];
    else
        return rook_masks[std::to_underlying(sq)];
}

/// @brief Generates an index to map an attack table
/// @param entry Magic entry
/// @param occupied Bitboard of occupied squares on the board (Blockers)
/// @returns The index
/// @note If USE_PEXT is defined, the function uses the _pext_u64 intrinsic function for
/// efficient bit manipulation. This function is available on CPUs that support the BMI2
/// instruction set, which can significantly speed up the operation
inline auto magic_index(const magic_entry& entry, bitboard& occupied) {
#ifndef USE_PEXT
    occupied &= entry.mask;
    occupied *= entry.magic;
    return occupied.as_u64() >> entry.shift;
#else
    return _pext_u64(occupied.as_u64(), entry.mask.as_u64());
#endif
}

} // namespace board::bitboards::magics