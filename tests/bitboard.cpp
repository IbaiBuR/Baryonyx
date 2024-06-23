#include "../src/board/bitboard/bitboard.hpp"
#include "doctest/doctest.hpp"

using namespace board::bitboards;

TEST_SUITE("Bitboards Tests") {
    TEST_CASE("file_of") {
        CHECK(file_of(Square::A1) == File::FILE_A);
        CHECK(file_of(Square::H1) == File::FILE_H);
    }

    TEST_CASE("rank_of") {
        CHECK(rank_of(Square::A1) == Rank::RANK_1);
        CHECK(rank_of(Square::H8) == Rank::RANK_8);
    }

    TEST_CASE("square_of") {
        CHECK(square_of(std::to_underlying(File::FILE_A), std::to_underlying(Rank::RANK_1))
              == Square::A1);
        CHECK(square_of(std::to_underlying(File::FILE_H), std::to_underlying(Rank::RANK_8))
              == Square::H8);
    }

    TEST_CASE("bit_count") {
        auto b = Bitboard(0x5ULL);
        CHECK(b.bit_count() == 2);

        b = Bitboard(0xFULL);
        CHECK(b.bit_count() == 4);
    }

    TEST_CASE("get_msb") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.get_msb() == 63);

        b = Bitboard(0x4000000000000000ULL);
        CHECK(b.get_msb() == 62);
    }

    TEST_CASE("get_lsb") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.get_lsb() == 0);

        b = Bitboard(0x0000000000000002ULL);
        CHECK(b.get_lsb() == 1);
    }

    TEST_CASE("pop_lsb") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.pop_lsb() == 0);
        CHECK(b.as_u64() == 0x8100000000000080ULL);

        b = Bitboard(0x42ULL);
        CHECK(b.pop_lsb() == 1);
        CHECK(b.as_u64() == 0x40ULL);
    }

    TEST_CASE("from_square") {
        CHECK(Bitboard::from_square(Square::A1) == Bitboard(0x1ULL));
        CHECK(Bitboard::from_square(Square::H8) == Bitboard(0x8000000000000000ULL));
    }

    TEST_CASE("shift") {
        SUBCASE("shift north") {
            CHECK(shift<Direction::NORTH>(Bitboard::from_square(Square::E4))
                  == Bitboard(0x1000000000ULL));
            CHECK(shift<Direction::NORTH>(Bitboard::from_square(Square::E8)) == Bitboard(0x0ULL));

            SUBCASE("shift north east") {
                CHECK(shift<Direction::NORTH_EAST>(Bitboard::from_square(Square::E4))
                      == Bitboard(0x2000000000ULL));
                CHECK(shift<Direction::NORTH_EAST>(Bitboard::from_square(Square::H4))
                      == Bitboard(0x0ULL));
            }

            SUBCASE("shift north west") {
                CHECK(shift<Direction::NORTH_WEST>(Bitboard::from_square(Square::E4))
                      == Bitboard(0x800000000ULL));
                CHECK(shift<Direction::NORTH_WEST>(Bitboard::from_square(Square::A4))
                      == Bitboard(0x0ULL));
            }
        }

        SUBCASE("shift south") {
            CHECK(shift<Direction::SOUTH>(Bitboard::from_square(Square::E4))
                  == Bitboard(0x100000ULL));
            CHECK(shift<Direction::SOUTH>(Bitboard::from_square(Square::E1)) == Bitboard(0x0ULL));

            SUBCASE("shift south east") {
                CHECK(shift<Direction::SOUTH_EAST>(Bitboard::from_square(Square::E4))
                      == Bitboard(0x200000ULL));
                CHECK(shift<Direction::SOUTH_EAST>(Bitboard::from_square(Square::H4))
                      == Bitboard(0x0ULL));
            }

            SUBCASE("shift south west") {
                CHECK(shift<Direction::SOUTH_WEST>(Bitboard::from_square(Square::E4))
                      == Bitboard(0x80000ULL));
                CHECK(shift<Direction::SOUTH_WEST>(Bitboard::from_square(Square::A4))
                      == Bitboard(0x0ULL));
            }
        }

        SUBCASE("shift east") {
            CHECK(shift<Direction::EAST>(Bitboard::from_square(Square::E4))
                  == Bitboard(0x20000000ULL));
            CHECK(shift<Direction::EAST>(Bitboard::from_square(Square::H4)) == Bitboard(0x0ULL));
        }

        SUBCASE("shift west") {
            CHECK(shift<Direction::WEST>(Bitboard::from_square(Square::E4))
                  == Bitboard(0x8000000ULL));
            CHECK(shift<Direction::WEST>(Bitboard::from_square(Square::A4)) == Bitboard(0x0ULL));
        }
    }
}