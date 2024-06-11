#include "magics.hpp"

#include <format>
#include <iostream>
#include <random>

#include "attacks.hpp"

namespace Board::Bitboards::Magics {

/// @brief Utility used to generate all the blocker combinations of a mask
/// @param index Index for blocker configuration
/// @param nBits Number of bits set in the mask
/// @param mask Attack mask
/// @returns The generated blocker configuration for the index
Bitboard setBlockers(const int index, const int nBits, Bitboard mask) {
    Bitboard blockers;

    for (int i = 0; i < nBits; ++i) {
        const auto sq = static_cast<Square>(mask.popLSB());

        if (index & (1 << i))
            Bitboard::setBit(blockers, sq);
    }

    return blockers;
}

/// @brief Tests if the magic number produces index collisions
/// @param magic Magic number
/// @param shift MagicEntry shift
/// @param numOccupancies Number of possible combinations of occupied bits in the mask
/// @param blockers Blockers
/// @param attacks Pre-calculated attacks
/// @returns True if no collisions found while trying to fill in the attack table
bool tryMagic(const u64                    magic,
              const int                    shift,
              const int                    numOccupancies,
              const std::vector<Bitboard> &blockers,
              const std::vector<Bitboard> &attacks) {
    std::vector<Bitboard> attackTable(numOccupancies);
    bool                  collision = false;

    for (int i = 0; !collision && i < numOccupancies; ++i) {
        const u64 magicIndex = (blockers[i].asU64() * magic) >> shift;

        if (attackTable[magicIndex] == Util::EMPTY_BB)
            attackTable[magicIndex] = attacks[i];
        else if (attackTable[magicIndex] != attacks[i])
            collision = true;
    }

    return !collision;
}

/// @brief Generates a MagicEntry for the given square depending on piece type
/// @tparam pt Piece type
/// @param sq Square
/// @returns The MagicEntry (empty if it fails to find a valid candidate)
template <PieceType pt>
constexpr MagicEntry findMagic(const Square sq) {
    assert(pt == PieceType::BISHOP || pt == PieceType::ROOK);

    constexpr bool isBishop = pt == PieceType::BISHOP;
    constexpr int  maxBlockersConfig =
        isBishop ? Attacks::maxBishopBlockersConfig : Attacks::maxRookBlockersConfig;

    const Bitboard mask =
        isBishop ? bishopMasks[std::to_underlying(sq)] : rookMasks[std::to_underlying(sq)];
    const int relevantBits   = mask.bitCount();
    const int numOccupancies = 1 << relevantBits;

    std::vector<Bitboard> blockers(maxBlockersConfig);
    std::vector<Bitboard> attacks(maxBlockersConfig);

    for (int i = 0; i < numOccupancies; ++i) {
        blockers[i] = setBlockers(i, relevantBits, mask);
        attacks[i]  = Attacks::genSliding<pt>(sq, blockers[i]);
    }

    // RNG using a fixed seed generated with an std::random_device
    std::mt19937_64 magicsRng(979484151);
    const auto      randomMagic = [&magicsRng]() {
        std::uniform_int_distribution<u64> dis;
        return dis(magicsRng) & dis(magicsRng) & dis(magicsRng);
    };

    // Find the magic numbers by trial and error
    for (int i = 0; i < 10000000; ++i) {
        const u64 magicCandidate = randomMagic();

        // Skip bad magics
        if (std::popcount((mask.asU64() * magicCandidate) & 0xFF00000000000000) < 6)
            continue;

        const int shift = static_cast<int>(Square::SQUARE_NB) - relevantBits;

        if (tryMagic(magicCandidate, shift, numOccupancies, blockers, attacks))
            return MagicEntry(mask, magicCandidate, shift);
    }

    return {};
}

template <PieceType pt>
void printMagicsByPieceType() {
    std::cout << std::format(
        "constexpr std::array<MagicEntry, std::to_underlying(Square::SQUARE_NB)> {} = {}\n {}",
        pt == PieceType::BISHOP ? "bishopMagics" : "rookMagics", "{", "{")
              << std::endl;

    for (u8 sq = 0; sq < 64; ++sq) {
        auto [mask, magic, shift] = findMagic<pt>(static_cast<Square>(sq));
        std::cout << std::format(" MagicEntry(Bitboard(0x{:016X}ULL), 0x{:016X}ULL, {})",
                                 mask.asU64(), magic, shift);

        if (sq < 63)
            std::cout << std::format(",");

        std::cout << std::endl;
    }
    std::cout << std::format(" {}\n{};\n", "}", "}") << std::endl;
}

void printMagics() {
    printMagicsByPieceType<PieceType::BISHOP>();
    printMagicsByPieceType<PieceType::ROOK>();
}

} // namespace Board::Bitboards::Magics
