#include "../src/board/bitboard/attacks.hpp"
#include "doctest/doctest.hpp"

using namespace board::bitboards;

TEST_SUITE("Attacks Test") {
    TEST_CASE("pawn attacks") {
        SUBCASE("white") {
            CHECK_EQ(attacks::pawn_attacks[std::to_underlying(color::white)]
                                          [std::to_underlying(square::e4)],
                     bitboard(0x2800000000ULL));
            CHECK_EQ(attacks::pawn_attacks[std::to_underlying(color::white)]
                                          [std::to_underlying(square::a8)],
                     bitboard(0x0ULL));
        }

        SUBCASE("black") {
            CHECK_EQ(attacks::pawn_attacks[std::to_underlying(color::black)]
                                          [std::to_underlying(square::e4)],
                     bitboard(0x280000ULL));
            CHECK_EQ(attacks::pawn_attacks[std::to_underlying(color::black)]
                                          [std::to_underlying(square::a1)],
                     bitboard(0x0ULL));
        }
    }

    TEST_CASE("knight attacks") {
        CHECK_EQ(attacks::knight_attacks[std::to_underlying(square::a1)], bitboard(0x20400ULL));
        CHECK_EQ(attacks::knight_attacks[std::to_underlying(square::h1)], bitboard(0x402000ULL));
        CHECK_EQ(attacks::knight_attacks[std::to_underlying(square::a8)],
                 bitboard(0x4020000000000ULL));
        CHECK_EQ(attacks::knight_attacks[std::to_underlying(square::h8)],
                 bitboard(0x20400000000000ULL));
    }

    TEST_CASE("king attacks") {
        CHECK_EQ(attacks::king_attacks[std::to_underlying(square::a1)], bitboard(0x302ULL));
        CHECK_EQ(attacks::king_attacks[std::to_underlying(square::h1)], bitboard(0xC040ULL));
        CHECK_EQ(attacks::king_attacks[std::to_underlying(square::a8)],
                 bitboard(0x203000000000000ULL));
        CHECK_EQ(attacks::king_attacks[std::to_underlying(square::h8)],
                 bitboard(0x40C0000000000000ULL));
    }

    TEST_CASE("sliding attacks") {
        attacks::init();
        SUBCASE("bishop attacks") {
            CHECK_EQ(
                attacks::gen_sliding<piece_type::bishop>(square::e4, bitboard(0x440000004400ULL)),
                bitboard(0x442800284400ULL));
            CHECK_EQ(
                attacks::gen_sliding<piece_type::bishop>(square::d4, bitboard(0x40020000002200ULL)),
                bitboard(0x40221400142200ULL));
            CHECK_EQ(attacks::get_bishop_attacks(square::e4, bitboard(0x440000004400ULL)),
                     bitboard(0x442800284400ULL));
            CHECK_EQ(attacks::get_bishop_attacks(square::d4, bitboard(0x40020000002200ULL)),
                     bitboard(0x40221400142200ULL));
        }

        SUBCASE("rook attacks") {
            CHECK_EQ(
                attacks::gen_sliding<piece_type::rook>(square::e4, bitboard(0x10000042001000ULL)),
                bitboard(0x1010106E101000ULL));
            CHECK_EQ(
                attacks::gen_sliding<piece_type::rook>(square::e2, bitboard(0x10000000004200ULL)),
                bitboard(0x10101010106E10ULL));
            CHECK_EQ(attacks::get_rook_attacks(square::e4, bitboard(0x10000042001000ULL)),
                     bitboard(0x1010106E101000ULL));
            CHECK_EQ(attacks::get_rook_attacks(square::e2, bitboard(0x10000000004200ULL)),
                     bitboard(0x10101010106E10ULL));
        }
    }
}