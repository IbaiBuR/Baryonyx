#include "magics.hpp"

#include <format>
#include <iostream>

#include "attacks.hpp"
#include "../../utils/random.hpp"

namespace board::bitboards::magics {

Bitboard set_blockers(const int index, const int n_bits, Bitboard mask) {
    Bitboard blockers;

    for (int i = 0; i < n_bits; ++i) {
        const auto sq = static_cast<Square>(mask.pop_lsb());

        if (index & (1 << i))
            Bitboard::set_bit(blockers, sq);
    }

    return blockers;
}

/// @brief Tests if the magic number produces index collisions
/// @param magic Magic number
/// @param shift MagicEntry shift
/// @param num_occupancies Number of possible combinations of occupied bits in the mask
/// @param blockers Blockers
/// @param attacks Pre-calculated attacks
/// @returns True if no collisions found while trying to fill in the attack table
bool try_magic(const u64                    magic,
               const int                    shift,
               const int                    num_occupancies,
               const std::vector<Bitboard>& blockers,
               const std::vector<Bitboard>& attacks) {
    std::vector<Bitboard> attack_table(num_occupancies);
    bool                  collision = false;

    for (int i = 0; !collision && i < num_occupancies; ++i) {
        const u64 magic_index = (blockers[i].as_u64() * magic) >> shift;

        if (attack_table[magic_index] == util::empty_bb)
            attack_table[magic_index] = attacks[i];
        else if (attack_table[magic_index] != attacks[i])
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

    constexpr bool is_bishop = pt == PieceType::BISHOP;
    constexpr int  max_blockers_config =
        is_bishop ? attacks::max_bishop_blockers_config : attacks::max_rook_blockers_config;

    const Bitboard mask =
        is_bishop ? bishop_masks[std::to_underlying(sq)] : rook_masks[std::to_underlying(sq)];
    const int relevant_bits   = mask.bit_count();
    const int num_occupancies = 1 << relevant_bits;

    std::vector<Bitboard> blockers(max_blockers_config);
    std::vector<Bitboard> attacks(max_blockers_config);

    for (int i = 0; i < num_occupancies; ++i) {
        blockers[i] = set_blockers(i, relevant_bits, mask);
        attacks[i]  = attacks::gen_sliding<pt>(sq, blockers[i]);
    }

    static utils::random::Sfc64Rng prng;

    auto random_magic = [&]() -> u64 {
        return prng.next_u64() & prng.next_u64() & prng.next_u64();
    };

    // Find the magic numbers by trial and error
    for (int i = 0; i < 10000000; ++i) {
        const u64 magic_candidate = random_magic();

        // Skip bad magics
        if (std::popcount((mask.as_u64() * magic_candidate) & 0xFF00000000000000) < 6)
            continue;

        const int shift = static_cast<int>(constants::num_squares) - relevant_bits;

        if (try_magic(magic_candidate, shift, num_occupancies, blockers, attacks))
            return MagicEntry(mask, magic_candidate, shift);
    }

    return {};
}

template <PieceType pt>
void print_magics_by_piece_type() {
    std::cout << std::format(
        "constexpr std::array<MagicEntry, constants::num_squares> {} = {}\n {}",
        pt == PieceType::BISHOP ? "bishop_magics" : "rook_magics", "{", "{")
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
