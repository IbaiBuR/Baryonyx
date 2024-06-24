#include "magics.hpp"

#include <format>
#include <iostream>

#include "attacks.hpp"
#include "../../utils/random.hpp"

namespace board::bitboards::magics {

bitboard set_blockers(const int index, const int n_bits, bitboard mask) {
    bitboard blockers;

    for (int i = 0; i < n_bits; ++i) {
        const auto sq = static_cast<square>(mask.pop_lsb());

        if (index & (1 << i))
            bitboard::set_bit(blockers, sq);
    }

    return blockers;
}

/// @brief Tests if the magic number produces index collisions
/// @param magic Magic number
/// @param shift Magic entry shift
/// @param num_occupancies Number of possible combinations of occupied bits in the mask
/// @param blockers Blockers
/// @param attacks Pre-calculated attacks
/// @returns True if no collisions found while trying to fill in the attack table
bool try_magic(const u64                    magic,
               const int                    shift,
               const int                    num_occupancies,
               const std::vector<bitboard>& blockers,
               const std::vector<bitboard>& attacks) {
    std::vector<bitboard> attack_table(num_occupancies);
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
/// @tparam PieceType Piece type (Slider)
/// @param sq Square
/// @returns The MagicEntry (empty if it fails to find a valid candidate)
template <piece_type PieceType>
constexpr magic_entry find_magic(const square sq) {
    assert(Pt == piece_type::bishop || Pt == piece_type::rook);

    constexpr bool is_bishop = PieceType == piece_type::bishop;
    constexpr int  max_blockers_config =
        is_bishop ? attacks::max_bishop_blockers_config : attacks::max_rook_blockers_config;

    const bitboard mask =
        is_bishop ? bishop_masks[std::to_underlying(sq)] : rook_masks[std::to_underlying(sq)];
    const int relevant_bits   = mask.bit_count();
    const int num_occupancies = 1 << relevant_bits;

    std::vector<bitboard> blockers(max_blockers_config);
    std::vector<bitboard> attacks(max_blockers_config);

    for (int i = 0; i < num_occupancies; ++i) {
        blockers[i] = set_blockers(i, relevant_bits, mask);
        attacks[i]  = attacks::gen_sliding<PieceType>(sq, blockers[i]);
    }

    static utils::random::sfc64_rng prng;

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
            return magic_entry(mask, magic_candidate, shift);
    }

    return {};
}

template <piece_type PieceType>
void print_magics_by_piece_type() {
    std::cout << std::format(
        "constexpr std::array<magic_entry, constants::num_squares> {} = {}\n {}",
        PieceType == piece_type::bishop ? "bishop_magics" : "rook_magics", "{", "{")
              << std::endl;

    for (u8 sq = 0; sq < constants::num_squares; ++sq) {
        auto [mask, magic, shift] = find_magic<PieceType>(static_cast<square>(sq));
        std::cout << std::format(" magic_entry(bitboard(0x{:016X}ULL), 0x{:016X}ULL, {})",
                                 mask.as_u64(), magic, shift);

        if (sq < constants::num_squares - 1)
            std::cout << std::format(",");

        std::cout << std::endl;
    }
    std::cout << std::format(" {}\n{};\n", "}", "}") << std::endl;
}

void print_magics() {
    print_magics_by_piece_type<piece_type::bishop>();
    print_magics_by_piece_type<piece_type::rook>();
}

} // namespace board::bitboards::magics
