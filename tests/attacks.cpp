#include "../src/board/bitboard/attacks.hpp"
#include "doctest/doctest.hpp"

using namespace board::bitboards;

TEST_SUITE("Attacks Test") {
    TEST_CASE("pawn attacks") {
        SUBCASE("white") {
            CHECK(attacks::pawnAttacks[std::to_underlying(Color::WHITE)]
                                      [std::to_underlying(Square::E4)]
                  == Bitboard(0x2800000000ULL));
            CHECK(attacks::pawnAttacks[std::to_underlying(Color::WHITE)]
                                      [std::to_underlying(Square::A8)]
                  == Bitboard(0x0ULL));
        }

        SUBCASE("black") {
            CHECK(attacks::pawnAttacks[std::to_underlying(Color::BLACK)]
                                      [std::to_underlying(Square::E4)]
                  == Bitboard(0x280000ULL));
            CHECK(attacks::pawnAttacks[std::to_underlying(Color::BLACK)]
                                      [std::to_underlying(Square::A1)]
                  == Bitboard(0x0ULL));
        }
    }

    TEST_CASE("knight attacks") {
        CHECK(attacks::knightAttacks[std::to_underlying(Square::A1)] == Bitboard(0x20400ULL));
        CHECK(attacks::knightAttacks[std::to_underlying(Square::H1)] == Bitboard(0x402000ULL));
        CHECK(attacks::knightAttacks[std::to_underlying(Square::A8)]
              == Bitboard(0x4020000000000ULL));
        CHECK(attacks::knightAttacks[std::to_underlying(Square::H8)]
              == Bitboard(0x20400000000000ULL));
    }

    TEST_CASE("king attacks") {
        CHECK(attacks::kingAttacks[std::to_underlying(Square::A1)] == Bitboard(0x302ULL));
        CHECK(attacks::kingAttacks[std::to_underlying(Square::H1)] == Bitboard(0xC040ULL));
        CHECK(attacks::kingAttacks[std::to_underlying(Square::A8)]
              == Bitboard(0x203000000000000ULL));
        CHECK(attacks::kingAttacks[std::to_underlying(Square::H8)]
              == Bitboard(0x40C0000000000000ULL));
    }

    TEST_CASE("sliding attacks") {
        attacks::init();
        SUBCASE("bishop attacks") {
            CHECK(attacks::gen_sliding<PieceType::BISHOP>(Square::E4, Bitboard(0x440000004400ULL))
                  == Bitboard(0x442800284400ULL));
            CHECK(attacks::gen_sliding<PieceType::BISHOP>(Square::D4, Bitboard(0x40020000002200ULL))
                  == Bitboard(0x40221400142200ULL));
            CHECK(attacks::get_bishop_attacks(Square::E4, Bitboard(0x440000004400ULL))
                  == Bitboard(0x442800284400ULL));
            CHECK(attacks::get_bishop_attacks(Square::D4, Bitboard(0x40020000002200ULL))
                  == Bitboard(0x40221400142200ULL));
        }

        SUBCASE("rook attacks") {
            CHECK(attacks::gen_sliding<PieceType::ROOK>(Square::E4, Bitboard(0x10000042001000ULL))
                  == Bitboard(0x1010106E101000ULL));
            CHECK(attacks::gen_sliding<PieceType::ROOK>(Square::E2, Bitboard(0x10000000004200ULL))
                  == Bitboard(0x10101010106E10ULL));
            CHECK(attacks::get_rook_attacks(Square::E4, Bitboard(0x10000042001000ULL))
                  == Bitboard(0x1010106E101000ULL));
            CHECK(attacks::get_rook_attacks(Square::E2, Bitboard(0x10000000004200ULL))
                  == Bitboard(0x10101010106E10ULL));
        }
    }
}