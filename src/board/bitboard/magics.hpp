#pragma once

#include <array>
#include <utility>

#include "bitboard.hpp"

namespace Board::Bitboards::Magics {

struct MagicEntry {
        Bitboard mask;
        U64      magic;
        int      shift;
};

constexpr std::array<Bitboard, std::to_underlying(Square::SQUARE_NB)> bishopMasks = {
    {
     Bitboard(0x0040201008040200ULL), Bitboard(0x0000402010080400ULL),
     Bitboard(0x0000004020100A00ULL), Bitboard(0x0000000040221400ULL),
     Bitboard(0x0000000002442800ULL), Bitboard(0x0000000204085000ULL),
     Bitboard(0x0000020408102000ULL), Bitboard(0x0002040810204000ULL),
     Bitboard(0x0020100804020000ULL), Bitboard(0x0040201008040000ULL),
     Bitboard(0x00004020100A0000ULL), Bitboard(0x0000004022140000ULL),
     Bitboard(0x0000000244280000ULL), Bitboard(0x0000020408500000ULL),
     Bitboard(0x0002040810200000ULL), Bitboard(0x0004081020400000ULL),
     Bitboard(0x0010080402000200ULL), Bitboard(0x0020100804000400ULL),
     Bitboard(0x004020100A000A00ULL), Bitboard(0x0000402214001400ULL),
     Bitboard(0x0000024428002800ULL), Bitboard(0x0002040850005000ULL),
     Bitboard(0x0004081020002000ULL), Bitboard(0x0008102040004000ULL),
     Bitboard(0x0008040200020400ULL), Bitboard(0x0010080400040800ULL),
     Bitboard(0x0020100A000A1000ULL), Bitboard(0x0040221400142200ULL),
     Bitboard(0x0002442800284400ULL), Bitboard(0x0004085000500800ULL),
     Bitboard(0x0008102000201000ULL), Bitboard(0x0010204000402000ULL),
     Bitboard(0x0004020002040800ULL), Bitboard(0x0008040004081000ULL),
     Bitboard(0x00100A000A102000ULL), Bitboard(0x0022140014224000ULL),
     Bitboard(0x0044280028440200ULL), Bitboard(0x0008500050080400ULL),
     Bitboard(0x0010200020100800ULL), Bitboard(0x0020400040201000ULL),
     Bitboard(0x0002000204081000ULL), Bitboard(0x0004000408102000ULL),
     Bitboard(0x000A000A10204000ULL), Bitboard(0x0014001422400000ULL),
     Bitboard(0x0028002844020000ULL), Bitboard(0x0050005008040200ULL),
     Bitboard(0x0020002010080400ULL), Bitboard(0x0040004020100800ULL),
     Bitboard(0x0000020408102000ULL), Bitboard(0x0000040810204000ULL),
     Bitboard(0x00000A1020400000ULL), Bitboard(0x0000142240000000ULL),
     Bitboard(0x0000284402000000ULL), Bitboard(0x0000500804020000ULL),
     Bitboard(0x0000201008040200ULL), Bitboard(0x0000402010080400ULL),
     Bitboard(0x0002040810204000ULL), Bitboard(0x0004081020400000ULL),
     Bitboard(0x000A102040000000ULL), Bitboard(0x0014224000000000ULL),
     Bitboard(0x0028440200000000ULL), Bitboard(0x0050080402000000ULL),
     Bitboard(0x0020100804020000ULL), Bitboard(0x0040201008040200ULL),
     }
};

constexpr std::array<Bitboard, std::to_underlying(Square::SQUARE_NB)> rookMasks = {
    {
     Bitboard(0x000101010101017EULL), Bitboard(0x000202020202027CULL),
     Bitboard(0x000404040404047AULL), Bitboard(0x0008080808080876ULL),
     Bitboard(0x001010101010106EULL), Bitboard(0x002020202020205EULL),
     Bitboard(0x004040404040403EULL), Bitboard(0x008080808080807EULL),
     Bitboard(0x0001010101017E00ULL), Bitboard(0x0002020202027C00ULL),
     Bitboard(0x0004040404047A00ULL), Bitboard(0x0008080808087600ULL),
     Bitboard(0x0010101010106E00ULL), Bitboard(0x0020202020205E00ULL),
     Bitboard(0x0040404040403E00ULL), Bitboard(0x0080808080807E00ULL),
     Bitboard(0x00010101017E0100ULL), Bitboard(0x00020202027C0200ULL),
     Bitboard(0x00040404047A0400ULL), Bitboard(0x0008080808760800ULL),
     Bitboard(0x00101010106E1000ULL), Bitboard(0x00202020205E2000ULL),
     Bitboard(0x00404040403E4000ULL), Bitboard(0x00808080807E8000ULL),
     Bitboard(0x000101017E010100ULL), Bitboard(0x000202027C020200ULL),
     Bitboard(0x000404047A040400ULL), Bitboard(0x0008080876080800ULL),
     Bitboard(0x001010106E101000ULL), Bitboard(0x002020205E202000ULL),
     Bitboard(0x004040403E404000ULL), Bitboard(0x008080807E808000ULL),
     Bitboard(0x0001017E01010100ULL), Bitboard(0x0002027C02020200ULL),
     Bitboard(0x0004047A04040400ULL), Bitboard(0x0008087608080800ULL),
     Bitboard(0x0010106E10101000ULL), Bitboard(0x0020205E20202000ULL),
     Bitboard(0x0040403E40404000ULL), Bitboard(0x0080807E80808000ULL),
     Bitboard(0x00017E0101010100ULL), Bitboard(0x00027C0202020200ULL),
     Bitboard(0x00047A0404040400ULL), Bitboard(0x0008760808080800ULL),
     Bitboard(0x00106E1010101000ULL), Bitboard(0x00205E2020202000ULL),
     Bitboard(0x00403E4040404000ULL), Bitboard(0x00807E8080808000ULL),
     Bitboard(0x007E010101010100ULL), Bitboard(0x007C020202020200ULL),
     Bitboard(0x007A040404040400ULL), Bitboard(0x0076080808080800ULL),
     Bitboard(0x006E101010101000ULL), Bitboard(0x005E202020202000ULL),
     Bitboard(0x003E404040404000ULL), Bitboard(0x007E808080808000ULL),
     Bitboard(0x7E01010101010100ULL), Bitboard(0x7C02020202020200ULL),
     Bitboard(0x7A04040404040400ULL), Bitboard(0x7608080808080800ULL),
     Bitboard(0x6E10101010101000ULL), Bitboard(0x5E20202020202000ULL),
     Bitboard(0x3E40404040404000ULL), Bitboard(0x7E80808080808000ULL),
     }
};

constexpr std::array<MagicEntry, std::to_underlying(Square::SQUARE_NB)> bishopMagics{
    {MagicEntry(Bitboard(0x0040201008040200ULL), 0x020A262403220200ULL, 58),
     MagicEntry(Bitboard(0x0000402010080400ULL), 0x4004810801010082ULL, 59),
     MagicEntry(Bitboard(0x0000004020100A00ULL), 0x1316040902089000ULL, 59),
     MagicEntry(Bitboard(0x0000000040221400ULL), 0x0004040880000000ULL, 59),
     MagicEntry(Bitboard(0x0000000002442800ULL), 0x0004042040400001ULL, 59),
     MagicEntry(Bitboard(0x0000000204085000ULL), 0x8800880440080400ULL, 59),
     MagicEntry(Bitboard(0x0000020408102000ULL), 0x184082102260202AULL, 59),
     MagicEntry(Bitboard(0x0002040810204000ULL), 0x020300404A084080ULL, 58),
     MagicEntry(Bitboard(0x0020100804020000ULL), 0x0103440588120400ULL, 59),
     MagicEntry(Bitboard(0x0040201008040000ULL), 0x020004A1281A0081ULL, 59),
     MagicEntry(Bitboard(0x00004020100A0000ULL), 0x4080100C106D424CULL, 59),
     MagicEntry(Bitboard(0x0000004022140000ULL), 0x00C1A0890108CC21ULL, 59),
     MagicEntry(Bitboard(0x0000000244280000ULL), 0x38000C0CA0426000ULL, 59),
     MagicEntry(Bitboard(0x0000020408500000ULL), 0x2004010120504010ULL, 59),
     MagicEntry(Bitboard(0x0002040810200000ULL), 0x489004C406203084ULL, 59),
     MagicEntry(Bitboard(0x0004081020400000ULL), 0x1081004042282008ULL, 59),
     MagicEntry(Bitboard(0x0010080402000200ULL), 0x100821C011010206ULL, 59),
     MagicEntry(Bitboard(0x0020100804000400ULL), 0x00600588180080A8ULL, 59),
     MagicEntry(Bitboard(0x004020100A000A00ULL), 0x020A262403220200ULL, 57),
     MagicEntry(Bitboard(0x0000402214001400ULL), 0x801802428A024010ULL, 57),
     MagicEntry(Bitboard(0x0000024428002800ULL), 0x000A100401201000ULL, 57),
     MagicEntry(Bitboard(0x0002040850005000ULL), 0x6600800308200200ULL, 57),
     MagicEntry(Bitboard(0x0004081020002000ULL), 0x4010800042101001ULL, 59),
     MagicEntry(Bitboard(0x0008102040004000ULL), 0x010A040820840408ULL, 59),
     MagicEntry(Bitboard(0x0008040200020400ULL), 0x2804840120081000ULL, 59),
     MagicEntry(Bitboard(0x0010080400040800ULL), 0x0081100009101102ULL, 59),
     MagicEntry(Bitboard(0x0020100A000A1000ULL), 0x0408010828220020ULL, 57),
     MagicEntry(Bitboard(0x0040221400142200ULL), 0x0004040218020808ULL, 55),
     MagicEntry(Bitboard(0x0002442800284400ULL), 0x9001010000104008ULL, 55),
     MagicEntry(Bitboard(0x0004085000500800ULL), 0x2090002413008810ULL, 57),
     MagicEntry(Bitboard(0x0008102000201000ULL), 0x0008820080825008ULL, 59),
     MagicEntry(Bitboard(0x0010204000402000ULL), 0x0000809000240402ULL, 59),
     MagicEntry(Bitboard(0x0004020002040800ULL), 0x0212112400C00800ULL, 59),
     MagicEntry(Bitboard(0x0008040004081000ULL), 0x4010900402900400ULL, 59),
     MagicEntry(Bitboard(0x00100A000A102000ULL), 0x1044020100080048ULL, 57),
     MagicEntry(Bitboard(0x0022140014224000ULL), 0x0000020081080082ULL, 55),
     MagicEntry(Bitboard(0x0044280028440200ULL), 0x1810008220020200ULL, 55),
     MagicEntry(Bitboard(0x0008500050080400ULL), 0x0130100040008040ULL, 57),
     MagicEntry(Bitboard(0x0010200020100800ULL), 0x2004308080240C00ULL, 59),
     MagicEntry(Bitboard(0x0020400040201000ULL), 0x0104248A08008841ULL, 59),
     MagicEntry(Bitboard(0x0002000204081000ULL), 0x209508A0102804A0ULL, 59),
     MagicEntry(Bitboard(0x0004000408102000ULL), 0x10C4821092003061ULL, 59),
     MagicEntry(Bitboard(0x000A000A10204000ULL), 0x12000A0802004400ULL, 57),
     MagicEntry(Bitboard(0x0014001422400000ULL), 0x0020604010405200ULL, 57),
     MagicEntry(Bitboard(0x0028002844020000ULL), 0xA02002200A000900ULL, 57),
     MagicEntry(Bitboard(0x0050005008040200ULL), 0x0181021802400A02ULL, 57),
     MagicEntry(Bitboard(0x0020002010080400ULL), 0x022001010E020510ULL, 59),
     MagicEntry(Bitboard(0x0040004020100800ULL), 0x0008120E88240202ULL, 59),
     MagicEntry(Bitboard(0x0000020408102000ULL), 0x184082102260202AULL, 59),
     MagicEntry(Bitboard(0x0000040810204000ULL), 0x1011144610041804ULL, 59),
     MagicEntry(Bitboard(0x00000A1020400000ULL), 0x1000010108260020ULL, 59),
     MagicEntry(Bitboard(0x0000142240000000ULL), 0x0008000020881011ULL, 59),
     MagicEntry(Bitboard(0x0000284402000000ULL), 0x1300011002088104ULL, 59),
     MagicEntry(Bitboard(0x0000500804020000ULL), 0x0103440588120400ULL, 59),
     MagicEntry(Bitboard(0x0000201008040200ULL), 0x2004080888108810ULL, 59),
     MagicEntry(Bitboard(0x0000402010080400ULL), 0x4004810801010082ULL, 59),
     MagicEntry(Bitboard(0x0002040810204000ULL), 0x020300404A084080ULL, 58),
     MagicEntry(Bitboard(0x0004081020400000ULL), 0x1081004042282008ULL, 59),
     MagicEntry(Bitboard(0x000A102040000000ULL), 0x2002A008AC843020ULL, 59),
     MagicEntry(Bitboard(0x0014224000000000ULL), 0x0420004000840420ULL, 59),
     MagicEntry(Bitboard(0x0028440200000000ULL), 0x4104112013220202ULL, 59),
     MagicEntry(Bitboard(0x0050080402000000ULL), 0x6241003821180084ULL, 59),
     MagicEntry(Bitboard(0x0020100804020000ULL), 0x0103440588120400ULL, 59),
     MagicEntry(Bitboard(0x0040201008040200ULL), 0x020A262403220200ULL, 58)}
};

constexpr std::array<MagicEntry, std::to_underlying(Square::SQUARE_NB)> rookMagics{
    {MagicEntry(Bitboard(0x000101010101017EULL), 0x00800284A050C000ULL, 52),
     MagicEntry(Bitboard(0x000202020202027CULL), 0x0840021008200240ULL, 53),
     MagicEntry(Bitboard(0x000404040404047AULL), 0x0100084020001100ULL, 53),
     MagicEntry(Bitboard(0x0008080808080876ULL), 0x0100040900201000ULL, 53),
     MagicEntry(Bitboard(0x001010101010106EULL), 0x0200081002000420ULL, 53),
     MagicEntry(Bitboard(0x002020202020205EULL), 0x0200080190044200ULL, 53),
     MagicEntry(Bitboard(0x004040404040403EULL), 0x0400223010810804ULL, 53),
     MagicEntry(Bitboard(0x008080808080807EULL), 0x4600004400812B02ULL, 52),
     MagicEntry(Bitboard(0x0001010101017E00ULL), 0x0803800080400422ULL, 53),
     MagicEntry(Bitboard(0x0002020202027C00ULL), 0x010280200040018CULL, 54),
     MagicEntry(Bitboard(0x0004040404047A00ULL), 0x1106002200408210ULL, 54),
     MagicEntry(Bitboard(0x0008080808087600ULL), 0x1000808010000800ULL, 54),
     MagicEntry(Bitboard(0x0010101010106E00ULL), 0x0445000800500502ULL, 54),
     MagicEntry(Bitboard(0x0020202020205E00ULL), 0x3000800200040080ULL, 54),
     MagicEntry(Bitboard(0x0040404040403E00ULL), 0x2041000401000200ULL, 54),
     MagicEntry(Bitboard(0x0080808080807E00ULL), 0x0101002081000042ULL, 53),
     MagicEntry(Bitboard(0x00010101017E0100ULL), 0x0080208000904000ULL, 53),
     MagicEntry(Bitboard(0x00020202027C0200ULL), 0x0000404000201004ULL, 54),
     MagicEntry(Bitboard(0x00040404047A0400ULL), 0x4160018020100080ULL, 54),
     MagicEntry(Bitboard(0x0008080808760800ULL), 0x1000808010000800ULL, 54),
     MagicEntry(Bitboard(0x00101010106E1000ULL), 0x0000828008000401ULL, 54),
     MagicEntry(Bitboard(0x00202020205E2000ULL), 0x0004008002008004ULL, 54),
     MagicEntry(Bitboard(0x00404040403E4000ULL), 0x000A440001020810ULL, 54),
     MagicEntry(Bitboard(0x00808080807E8000ULL), 0x0C10020000611084ULL, 53),
     MagicEntry(Bitboard(0x000101017E010100ULL), 0x0080628280094004ULL, 53),
     MagicEntry(Bitboard(0x000202027C020200ULL), 0x4000200240100140ULL, 54),
     MagicEntry(Bitboard(0x000404047A040400ULL), 0x0000200280100080ULL, 54),
     MagicEntry(Bitboard(0x0008080876080800ULL), 0x4050080180100080ULL, 54),
     MagicEntry(Bitboard(0x001010106E101000ULL), 0x000100D100050800ULL, 54),
     MagicEntry(Bitboard(0x002020205E202000ULL), 0x0464020080800400ULL, 54),
     MagicEntry(Bitboard(0x004040403E404000ULL), 0x8203000100040200ULL, 54),
     MagicEntry(Bitboard(0x008080807E808000ULL), 0x8010040200004081ULL, 53),
     MagicEntry(Bitboard(0x0001017E01010100ULL), 0xA000824002800220ULL, 53),
     MagicEntry(Bitboard(0x0002027C02020200ULL), 0x000043E002401004ULL, 54),
     MagicEntry(Bitboard(0x0004047A04040400ULL), 0x0820001000808020ULL, 54),
     MagicEntry(Bitboard(0x0008087608080800ULL), 0x1000800800801001ULL, 54),
     MagicEntry(Bitboard(0x0010106E10101000ULL), 0x0000080101000410ULL, 54),
     MagicEntry(Bitboard(0x0020205E20202000ULL), 0x0104800200800400ULL, 54),
     MagicEntry(Bitboard(0x0040403E40404000ULL), 0x2041000401000200ULL, 54),
     MagicEntry(Bitboard(0x0080807E80808000ULL), 0x0101002081000042ULL, 53),
     MagicEntry(Bitboard(0x00017E0101010100ULL), 0x0020802040008004ULL, 53),
     MagicEntry(Bitboard(0x00027C0202020200ULL), 0x4010002000404000ULL, 54),
     MagicEntry(Bitboard(0x00047A0404040400ULL), 0xC002008014420020ULL, 54),
     MagicEntry(Bitboard(0x0008760808080800ULL), 0x0010200A00420010ULL, 54),
     MagicEntry(Bitboard(0x00106E1010101000ULL), 0x0011006802050010ULL, 54),
     MagicEntry(Bitboard(0x00205E2020202000ULL), 0x0004008002008004ULL, 54),
     MagicEntry(Bitboard(0x00403E4040404000ULL), 0x0020081002040001ULL, 54),
     MagicEntry(Bitboard(0x00807E8080808000ULL), 0x2160204400820011ULL, 53),
     MagicEntry(Bitboard(0x007E010101010100ULL), 0x000D800620C10100ULL, 53),
     MagicEntry(Bitboard(0x007C020202020200ULL), 0x0100201000400040ULL, 54),
     MagicEntry(Bitboard(0x007A040404040400ULL), 0x3802002010884200ULL, 54),
     MagicEntry(Bitboard(0x0076080808080800ULL), 0x0941112201094200ULL, 54),
     MagicEntry(Bitboard(0x006E101010101000ULL), 0x0008004D00102900ULL, 54),
     MagicEntry(Bitboard(0x005E202020202000ULL), 0x3000800200040080ULL, 54),
     MagicEntry(Bitboard(0x003E404040404000ULL), 0x110002C810110400ULL, 54),
     MagicEntry(Bitboard(0x007E808080808000ULL), 0x0000008100441A00ULL, 53),
     MagicEntry(Bitboard(0x7E01010101010100ULL), 0x08084410A1008001ULL, 52),
     MagicEntry(Bitboard(0x7C02020202020200ULL), 0x8844400018802101ULL, 53),
     MagicEntry(Bitboard(0x7A04040404040400ULL), 0x5810108022000842ULL, 53),
     MagicEntry(Bitboard(0x7608080808080800ULL), 0x0444100020090105ULL, 53),
     MagicEntry(Bitboard(0x6E10101010101000ULL), 0x2082000920100402ULL, 53),
     MagicEntry(Bitboard(0x5E20202020202000ULL), 0x0002001008040102ULL, 53),
     MagicEntry(Bitboard(0x3E40404040404000ULL), 0x00000200F0080104ULL, 53),
     MagicEntry(Bitboard(0x7E80808080808000ULL), 0x0821000825420081ULL, 52)}
};

void printMagics();
int  magicIndex(const MagicEntry &entry, Bitboard &occupied);

Bitboard setBlockers(int index, int nBits, Bitboard mask);

} // namespace Board::Bitboards::Magics