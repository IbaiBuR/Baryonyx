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
    {magic_entry(bitboard(0x0040201008040200ULL), 0x10080808A4065200ULL, 58),
     magic_entry(bitboard(0x0000402010080400ULL), 0x08A0841100450C00ULL, 59),
     magic_entry(bitboard(0x0000004020100A00ULL), 0xC506460403001000ULL, 59),
     magic_entry(bitboard(0x0000000040221400ULL), 0x0084050204008001ULL, 59),
     magic_entry(bitboard(0x0000000002442800ULL), 0x024910C101047000ULL, 59),
     magic_entry(bitboard(0x0000000204085000ULL), 0x0201100804109004ULL, 59),
     magic_entry(bitboard(0x0000020408102000ULL), 0x1105080250040050ULL, 59),
     magic_entry(bitboard(0x0002040810204000ULL), 0x0010808410010401ULL, 58),
     magic_entry(bitboard(0x0020100804020000ULL), 0x0101409004410843ULL, 59),
     magic_entry(bitboard(0x0040201008040000ULL), 0x0105040404404200ULL, 59),
     magic_entry(bitboard(0x00004020100A0000ULL), 0x0048040102220000ULL, 59),
     magic_entry(bitboard(0x0000004022140000ULL), 0x4001840410804004ULL, 59),
     magic_entry(bitboard(0x0000000244280000ULL), 0x02C0040420000811ULL, 59),
     magic_entry(bitboard(0x0000020408500000ULL), 0x8C000104204502B0ULL, 59),
     magic_entry(bitboard(0x0002040810200000ULL), 0x0000040108021101ULL, 59),
     magic_entry(bitboard(0x0004081020400000ULL), 0x000013804C166000ULL, 59),
     magic_entry(bitboard(0x0010080402000200ULL), 0x1220404004010212ULL, 59),
     magic_entry(bitboard(0x0020100804000400ULL), 0x0820008214091200ULL, 59),
     magic_entry(bitboard(0x004020100A000A00ULL), 0x0004000208220200ULL, 57),
     magic_entry(bitboard(0x0000402214001400ULL), 0x00940088024008E2ULL, 57),
     magic_entry(bitboard(0x0000024428002800ULL), 0x000400C202112082ULL, 57),
     magic_entry(bitboard(0x0002040850005000ULL), 0x4206080C10020800ULL, 57),
     magic_entry(bitboard(0x0004081020002000ULL), 0x0002000092012022ULL, 59),
     magic_entry(bitboard(0x0008102040004000ULL), 0x21D9202843045001ULL, 59),
     magic_entry(bitboard(0x0008040200020400ULL), 0x9020C80A10102104ULL, 59),
     magic_entry(bitboard(0x0010080400040800ULL), 0x040C1200A0885901ULL, 59),
     magic_entry(bitboard(0x0020100A000A1000ULL), 0x0200240408180220ULL, 57),
     magic_entry(bitboard(0x0040221400142200ULL), 0x0804004024030052ULL, 55),
     magic_entry(bitboard(0x0002442800284400ULL), 0x0301001081004000ULL, 55),
     magic_entry(bitboard(0x0004085000500800ULL), 0x828200210200900CULL, 57),
     magic_entry(bitboard(0x0008102000201000ULL), 0x9489210002080108ULL, 59),
     magic_entry(bitboard(0x0010204000402000ULL), 0x4004044000884410ULL, 59),
     magic_entry(bitboard(0x0004020002040800ULL), 0x0050104440082800ULL, 59),
     magic_entry(bitboard(0x0008040004081000ULL), 0x0408D01040180201ULL, 59),
     magic_entry(bitboard(0x00100A000A102000ULL), 0x00A0119800100040ULL, 57),
     magic_entry(bitboard(0x0022140014224000ULL), 0x0202020080080080ULL, 55),
     magic_entry(bitboard(0x0044280028440200ULL), 0x0004008400020102ULL, 55),
     magic_entry(bitboard(0x0008500050080400ULL), 0x1090C501000A0090ULL, 57),
     magic_entry(bitboard(0x0010200020100800ULL), 0x0024040C01006100ULL, 59),
     magic_entry(bitboard(0x0020400040201000ULL), 0x0001012600002203ULL, 59),
     magic_entry(bitboard(0x0002000204081000ULL), 0x0002220221204081ULL, 59),
     magic_entry(bitboard(0x0004000408102000ULL), 0x00020804020004A0ULL, 59),
     magic_entry(bitboard(0x000A000A10204000ULL), 0x8080101090001808ULL, 57),
     magic_entry(bitboard(0x0014001422400000ULL), 0x0014C0A088001444ULL, 57),
     magic_entry(bitboard(0x0028002844020000ULL), 0x0400049008800400ULL, 57),
     magic_entry(bitboard(0x0050005008040200ULL), 0x0040100441429181ULL, 57),
     magic_entry(bitboard(0x0020002010080400ULL), 0x214CA81084202300ULL, 59),
     magic_entry(bitboard(0x0040004020100800ULL), 0x0102040410800020ULL, 59),
     magic_entry(bitboard(0x0000020408102000ULL), 0x0001009010080500ULL, 59),
     magic_entry(bitboard(0x0000040810204000ULL), 0x1003011801048800ULL, 59),
     magic_entry(bitboard(0x00000A1020400000ULL), 0x0808020508880006ULL, 59),
     magic_entry(bitboard(0x0000142240000000ULL), 0x2110400420880835ULL, 59),
     magic_entry(bitboard(0x0000284402000000ULL), 0x0100589042022001ULL, 59),
     magic_entry(bitboard(0x0000500804020000ULL), 0x0000045830430900ULL, 59),
     magic_entry(bitboard(0x0000201008040200ULL), 0x00040408920C1003ULL, 59),
     magic_entry(bitboard(0x0000402010080400ULL), 0x0402040434084009ULL, 59),
     magic_entry(bitboard(0x0002040810204000ULL), 0x0000420050221008ULL, 58),
     magic_entry(bitboard(0x0004081020400000ULL), 0x0008520203042360ULL, 59),
     magic_entry(bitboard(0x000A102040000000ULL), 0x1220000080680802ULL, 59),
     magic_entry(bitboard(0x0014224000000000ULL), 0x0008080140208802ULL, 59),
     magic_entry(bitboard(0x0028440200000000ULL), 0x008C800204208200ULL, 59),
     magic_entry(bitboard(0x0050080402000000ULL), 0x0601810820080090ULL, 59),
     magic_entry(bitboard(0x0020100804020000ULL), 0x000050101040C080ULL, 59),
     magic_entry(bitboard(0x0040201008040200ULL), 0x001060008102D300ULL, 58)}
};

inline constexpr std::array<magic_entry, constants::num_squares> rook_magics = {
    {magic_entry(bitboard(0x000101010101017EULL), 0x5080004000108020ULL, 52),
     magic_entry(bitboard(0x000202020202027CULL), 0x00C0400010002000ULL, 53),
     magic_entry(bitboard(0x000404040404047AULL), 0x2100084104200010ULL, 53),
     magic_entry(bitboard(0x0008080808080876ULL), 0x0480049000080080ULL, 53),
     magic_entry(bitboard(0x001010101010106EULL), 0x3001001080200840ULL, 53),
     magic_entry(bitboard(0x002020202020205EULL), 0x0500010002240028ULL, 53),
     magic_entry(bitboard(0x004040404040403EULL), 0x0080008001000200ULL, 53),
     magic_entry(bitboard(0x008080808080807EULL), 0x0200005102008024ULL, 52),
     magic_entry(bitboard(0x0001010101017E00ULL), 0xC840800040002084ULL, 53),
     magic_entry(bitboard(0x0002020202027C00ULL), 0x0020808040002000ULL, 54),
     magic_entry(bitboard(0x0004040404047A00ULL), 0x0100801000802004ULL, 54),
     magic_entry(bitboard(0x0008080808087600ULL), 0x2280801000800800ULL, 54),
     magic_entry(bitboard(0x0010101010106E00ULL), 0x1251000800851100ULL, 54),
     magic_entry(bitboard(0x0020202020205E00ULL), 0x40EA000482001008ULL, 54),
     magic_entry(bitboard(0x0040404040403E00ULL), 0x6083000401001200ULL, 54),
     magic_entry(bitboard(0x0080808080807E00ULL), 0x04010010418A0500ULL, 53),
     magic_entry(bitboard(0x00010101017E0100ULL), 0x3813A48000401082ULL, 53),
     magic_entry(bitboard(0x00020202027C0200ULL), 0x1020004000205000ULL, 54),
     magic_entry(bitboard(0x00040404047A0400ULL), 0x9000848020001006ULL, 54),
     magic_entry(bitboard(0x0008080808760800ULL), 0x12021200400A0020ULL, 54),
     magic_entry(bitboard(0x00101010106E1000ULL), 0x0001010004080010ULL, 54),
     magic_entry(bitboard(0x00202020205E2000ULL), 0x1004008004020080ULL, 54),
     magic_entry(bitboard(0x00404040403E4000ULL), 0xA8000400C2102801ULL, 54),
     magic_entry(bitboard(0x00808080807E8000ULL), 0x0032020000408104ULL, 53),
     magic_entry(bitboard(0x000101017E010100ULL), 0x3000400080208000ULL, 53),
     magic_entry(bitboard(0x000202027C020200ULL), 0x4001600140005000ULL, 54),
     magic_entry(bitboard(0x000404047A040400ULL), 0x9403100480200080ULL, 54),
     magic_entry(bitboard(0x0008080876080800ULL), 0x0000100080080080ULL, 54),
     magic_entry(bitboard(0x001010106E101000ULL), 0x2800080080040080ULL, 54),
     magic_entry(bitboard(0x002020205E202000ULL), 0x6004004040020100ULL, 54),
     magic_entry(bitboard(0x004040403E404000ULL), 0x0000080400021081ULL, 54),
     magic_entry(bitboard(0x008080807E808000ULL), 0x0400802180014100ULL, 53),
     magic_entry(bitboard(0x0001017E01010100ULL), 0x8000400020800089ULL, 53),
     magic_entry(bitboard(0x0002027C02020200ULL), 0x4820406000C01000ULL, 54),
     magic_entry(bitboard(0x0004047A04040400ULL), 0x9200802000801008ULL, 54),
     magic_entry(bitboard(0x0008087608080800ULL), 0x08100101910009A0ULL, 54),
     magic_entry(bitboard(0x0010106E10101000ULL), 0x2888004004040020ULL, 54),
     magic_entry(bitboard(0x0020205E20202000ULL), 0x081A002004040010ULL, 54),
     magic_entry(bitboard(0x0040403E40404000ULL), 0x5000280254001001ULL, 54),
     magic_entry(bitboard(0x0080807E80808000ULL), 0x9802089052001405ULL, 53),
     magic_entry(bitboard(0x00017E0101010100ULL), 0x0180004020004000ULL, 53),
     magic_entry(bitboard(0x00027C0202020200ULL), 0x88E0004000810100ULL, 54),
     magic_entry(bitboard(0x00047A0404040400ULL), 0x80D0040028012002ULL, 54),
     magic_entry(bitboard(0x0008760808080800ULL), 0x1810010008210010ULL, 54),
     magic_entry(bitboard(0x00106E1010101000ULL), 0x1140040008008080ULL, 54),
     magic_entry(bitboard(0x00205E2020202000ULL), 0x100A001104020008ULL, 54),
     magic_entry(bitboard(0x00403E4040404000ULL), 0x00001008110400E2ULL, 54),
     magic_entry(bitboard(0x00807E8080808000ULL), 0x4201108100420004ULL, 53),
     magic_entry(bitboard(0x007E010101010100ULL), 0x0800400080022180ULL, 53),
     magic_entry(bitboard(0x007C020202020200ULL), 0x2A20004000802080ULL, 54),
     magic_entry(bitboard(0x007A040404040400ULL), 0x0800200110008180ULL, 54),
     magic_entry(bitboard(0x0076080808080800ULL), 0x0000801000080280ULL, 54),
     magic_entry(bitboard(0x006E101010101000ULL), 0x0080040080080080ULL, 54),
     magic_entry(bitboard(0x005E202020202000ULL), 0x0002001088450200ULL, 54),
     magic_entry(bitboard(0x003E404040404000ULL), 0x0022000104080200ULL, 54),
     magic_entry(bitboard(0x007E808080808000ULL), 0x20080400A0410200ULL, 53),
     magic_entry(bitboard(0x7E01010101010100ULL), 0x0300904901208001ULL, 52),
     magic_entry(bitboard(0x7C02020202020200ULL), 0x0100128040022901ULL, 53),
     magic_entry(bitboard(0x7A04040404040400ULL), 0x0082084020010211ULL, 53),
     magic_entry(bitboard(0x7608080808080800ULL), 0x050A8910004D0021ULL, 53),
     magic_entry(bitboard(0x6E10101010101000ULL), 0x0012002810216402ULL, 53),
     magic_entry(bitboard(0x5E20202020202000ULL), 0x0801000208440041ULL, 53),
     magic_entry(bitboard(0x3E40404040404000ULL), 0x8124011000822804ULL, 53),
     magic_entry(bitboard(0x7E80808080808000ULL), 0x0084208020440106ULL, 52)}
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