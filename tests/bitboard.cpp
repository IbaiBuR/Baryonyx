#include "../src/board/bitboard/bitboard.hpp"
#include "doctest/doctest.hpp"

using namespace board::bitboards;

TEST_SUITE("Bitboards Tests") {
    TEST_CASE("file_of") {
        CHECK_EQ(file_of(square::a1), file::file_a);
        CHECK_EQ(file_of(square::h1), file::file_h);
    }

    TEST_CASE("rank_of") {
        CHECK_EQ(rank_of(square::a1), rank::rank_1);
        CHECK_EQ(rank_of(square::h8), rank::rank_8);
    }

    TEST_CASE("square_of") {
        CHECK_EQ(square_of(std::to_underlying(file::file_a), std::to_underlying(rank::rank_1)),
                 square::a1);
        CHECK_EQ(square_of(std::to_underlying(file::file_h), std::to_underlying(rank::rank_8)),
                 square::h8);
    }

    TEST_CASE("bit_count") {
        auto b = bitboard(0x5ULL);
        CHECK_EQ(b.bit_count(), 2);

        b = bitboard(0xFULL);
        CHECK_EQ(b.bit_count(), 4);
    }

    TEST_CASE("get_msb") {
        auto b = bitboard(0x8100000000000081ULL);
        CHECK_EQ(b.get_msb(), 63);

        b = bitboard(0x4000000000000000ULL);
        CHECK_EQ(b.get_msb(), 62);
    }

    TEST_CASE("get_lsb") {
        auto b = bitboard(0x8100000000000081ULL);
        CHECK_EQ(b.get_lsb(), 0);

        b = bitboard(0x0000000000000002ULL);
        CHECK_EQ(b.get_lsb(), 1);
    }

    TEST_CASE("pop_lsb") {
        auto b = bitboard(0x8100000000000081ULL);
        CHECK_EQ(b.pop_lsb(), 0);
        CHECK_EQ(b.as_u64(), 0x8100000000000080ULL);

        b = bitboard(0x42ULL);
        CHECK_EQ(b.pop_lsb(), 1);
        CHECK_EQ(b.as_u64(), 0x40ULL);
    }

    TEST_CASE("from_square") {
        CHECK_EQ(bitboard::from_square(square::a1), bitboard(0x1ULL));
        CHECK_EQ(bitboard::from_square(square::h8), bitboard(0x8000000000000000ULL));
    }

    TEST_CASE("shift") {
        SUBCASE("shift north") {
            CHECK_EQ(shift<direction::north>(bitboard::from_square(square::e4)),
                     bitboard(0x1000000000ULL));
            CHECK_EQ(shift<direction::north>(bitboard::from_square(square::e8)), bitboard(0x0ULL));

            SUBCASE("shift north east") {
                CHECK_EQ(shift<direction::north_east>(bitboard::from_square(square::e4)),
                         bitboard(0x2000000000ULL));
                CHECK_EQ(shift<direction::north_east>(bitboard::from_square(square::h4)),
                         bitboard(0x0ULL));
            }

            SUBCASE("shift north west") {
                CHECK_EQ(shift<direction::north_west>(bitboard::from_square(square::e4)),
                         bitboard(0x800000000ULL));
                CHECK_EQ(shift<direction::north_west>(bitboard::from_square(square::a4)),
                         bitboard(0x0ULL));
            }
        }

        SUBCASE("shift south") {
            CHECK_EQ(shift<direction::south>(bitboard::from_square(square::e4)),
                     bitboard(0x100000ULL));
            CHECK_EQ(shift<direction::south>(bitboard::from_square(square::e1)), bitboard(0x0ULL));

            SUBCASE("shift south east") {
                CHECK_EQ(shift<direction::south_east>(bitboard::from_square(square::e4)),
                         bitboard(0x200000ULL));
                CHECK_EQ(shift<direction::south_east>(bitboard::from_square(square::h4)),
                         bitboard(0x0ULL));
            }

            SUBCASE("shift south west") {
                CHECK_EQ(shift<direction::south_west>(bitboard::from_square(square::e4)),
                         bitboard(0x80000ULL));
                CHECK_EQ(shift<direction::south_west>(bitboard::from_square(square::a4)),
                         bitboard(0x0ULL));
            }
        }

        SUBCASE("shift east") {
            CHECK_EQ(shift<direction::east>(bitboard::from_square(square::e4)),
                     bitboard(0x20000000ULL));
            CHECK_EQ(shift<direction::east>(bitboard::from_square(square::h4)), bitboard(0x0ULL));
        }

        SUBCASE("shift west") {
            CHECK_EQ(shift<direction::west>(bitboard::from_square(square::e4)),
                     bitboard(0x8000000ULL));
            CHECK_EQ(shift<direction::west>(bitboard::from_square(square::a4)), bitboard(0x0ULL));
        }
    }
}