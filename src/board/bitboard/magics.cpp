#include "magics.hpp"

#include <format>
#include <iostream>

#include "attacks.hpp"
#include "../../utils/random.hpp"

namespace board::bitboards::magics {

Bitboard set_blockers(const int index, const int nBits, Bitboard mask) {
    Bitboard blockers;

    for (int i = 0; i < nBits; ++i) {
        const auto sq = static_cast<Square>(mask.pop_lsb());

        if (index & (1 << i))
            Bitboard::set_bit(blockers, sq);
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
bool try_magic(const u64                    magic,
               const int                    shift,
               const int                    numOccupancies,
               const std::vector<Bitboard>& blockers,
               const std::vector<Bitboard>& attacks) {
    std::vector<Bitboard> attackTable(numOccupancies);
    bool                  collision = false;

    for (int i = 0; !collision && i < numOccupancies; ++i) {
        const u64 magicIndex = (blockers[i].as_u64() * magic) >> shift;

        if (attackTable[magicIndex] == util::empty_bb)
            attackTable[magicIndex] = attacks[i];
        else if (attackTable[magicIndex] != attacks[i])
            collision = true;
    }

    return !collision;
}

/// @brief Generates a MagicEntry for the given square depending on piece type
/// @tparam pt Piece type (Slider)
/// @param sq Square
/// @returns The MagicEntry (empty if it fails to find a valid candidate)
template <PieceType pt>
constexpr MagicEntry find_magic(const Square sq) {
    assert(pt == PieceType::BISHOP || pt == PieceType::ROOK);

    constexpr bool isBishop = pt == PieceType::BISHOP;
    constexpr int  maxBlockersConfig =
        isBishop ? attacks::max_bishop_blockers_config : attacks::max_rook_blockers_config;

    const Bitboard mask =
        isBishop ? bishop_masks[std::to_underlying(sq)] : rook_masks[std::to_underlying(sq)];
    const int relevantBits   = mask.bit_count();
    const int numOccupancies = 1 << relevantBits;

    std::vector<Bitboard> blockers(maxBlockersConfig);
    std::vector<Bitboard> attacks(maxBlockersConfig);

    for (int i = 0; i < numOccupancies; ++i) {
        blockers[i] = set_blockers(i, relevantBits, mask);
        attacks[i]  = attacks::gen_sliding<pt>(sq, blockers[i]);
    }

    static utils::random::Sfc64Rng prng;

    auto random_magic = [&]() -> u64 {
        return prng.next_u64() & prng.next_u64() & prng.next_u64();
    };

    // Find the magic numbers by trial and error
    for (int i = 0; i < 10000000; ++i) {
        const u64 magicCandidate = random_magic();

        // Skip bad magics
        if (std::popcount((mask.as_u64() * magicCandidate) & 0xFF00000000000000) < 6)
            continue;

        const int shift = static_cast<int>(constants::num_squares) - relevantBits;

        if (try_magic(magicCandidate, shift, numOccupancies, blockers, attacks))
            return MagicEntry(mask, magicCandidate, shift);
    }

    return {};
}

template <PieceType pt>
void print_magics_by_piece_type() {
    std::cout << std::format(
        "constexpr std::array<MagicEntry, constants::num_squares> {} = {}\n {}",
        pt == PieceType::BISHOP ? "bishopMagics" : "rookMagics", "{", "{")
              << std::endl;

    for (u8 sq = 0; sq < constants::num_squares; ++sq) {
        auto [mask, magic, shift] = find_magic<pt>(static_cast<Square>(sq));
        std::cout << std::format(" MagicEntry(Bitboard(0x{:016X}ULL), 0x{:016X}ULL, {})",
                                 mask.as_u64(), magic, shift);

        if (sq < constants::num_squares - 1)
            std::cout << std::format(",");

        std::cout << std::endl;
    }
    std::cout << std::format(" {}\n{};\n", "}", "}") << std::endl;
}

void print_magics() {
    print_magics_by_piece_type<PieceType::BISHOP>();
    print_magics_by_piece_type<PieceType::ROOK>();
}

} // namespace board::bitboards::magics
