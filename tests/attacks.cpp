#include "../src/board/bitboard/attacks.hpp"
#include "doctest/doctest.hpp"

using namespace Board::Bitboards;

TEST_SUITE("Attacks Test") {
    TEST_CASE("pawn attacks") {
        SUBCASE("white") {
            CHECK(Attacks::pawnAttacks[std::to_underlying(Color::WHITE)]
                                      [std::to_underlying(Square::E4)]
                  == Bitboard(0x2800000000ULL));
            CHECK(Attacks::pawnAttacks[std::to_underlying(Color::WHITE)]
                                      [std::to_underlying(Square::A8)]
                  == Bitboard(0x0ULL));
        }

        SUBCASE("black") {
            CHECK(Attacks::pawnAttacks[std::to_underlying(Color::BLACK)]
                                      [std::to_underlying(Square::E4)]
                  == Bitboard(0x280000ULL));
            CHECK(Attacks::pawnAttacks[std::to_underlying(Color::BLACK)]
                                      [std::to_underlying(Square::A1)]
                  == Bitboard(0x0ULL));
        }
    }

    TEST_CASE("knight attacks") {
        CHECK(Attacks::knightAttacks[std::to_underlying(Square::A1)] == Bitboard(0x20400ULL));
        CHECK(Attacks::knightAttacks[std::to_underlying(Square::H1)] == Bitboard(0x402000ULL));
        CHECK(Attacks::knightAttacks[std::to_underlying(Square::A8)]
              == Bitboard(0x4020000000000ULL));
        CHECK(Attacks::knightAttacks[std::to_underlying(Square::H8)]
              == Bitboard(0x20400000000000ULL));
    }

    TEST_CASE("king attacks") {
        CHECK(Attacks::kingAttacks[std::to_underlying(Square::A1)] == Bitboard(0x302ULL));
        CHECK(Attacks::kingAttacks[std::to_underlying(Square::H1)] == Bitboard(0xC040ULL));
        CHECK(Attacks::kingAttacks[std::to_underlying(Square::A8)]
              == Bitboard(0x203000000000000ULL));
        CHECK(Attacks::kingAttacks[std::to_underlying(Square::H8)]
              == Bitboard(0x40C0000000000000ULL));
    }

    TEST_CASE("sliding attacks") {
        Attacks::init();
        SUBCASE("bishop attacks") {
            CHECK(Attacks::genSliding<PieceType::BISHOP>(Square::E4, Bitboard(0x440000004400ULL))
                  == Bitboard(0x442800284400ULL));
            CHECK(Attacks::genSliding<PieceType::BISHOP>(Square::D4, Bitboard(0x40020000002200ULL))
                  == Bitboard(0x40221400142200ULL));
            CHECK(Attacks::getBishopAttacks(Square::E4, Bitboard(0x440000004400ULL))
                  == Bitboard(0x442800284400ULL));
            CHECK(Attacks::getBishopAttacks(Square::D4, Bitboard(0x40020000002200ULL))
                  == Bitboard(0x40221400142200ULL));
        }

        SUBCASE("rook attacks") {
            CHECK(Attacks::genSliding<PieceType::ROOK>(Square::E4, Bitboard(0x10000042001000ULL))
                  == Bitboard(0x1010106E101000ULL));
            CHECK(Attacks::genSliding<PieceType::ROOK>(Square::E2, Bitboard(0x10000000004200ULL))
                  == Bitboard(0x10101010106E10ULL));
            CHECK(Attacks::getRookAttacks(Square::E4, Bitboard(0x10000042001000ULL))
                  == Bitboard(0x1010106E101000ULL));
            CHECK(Attacks::getRookAttacks(Square::E2, Bitboard(0x10000000004200ULL))
                  == Bitboard(0x10101010106E10ULL));
        }
    }
}