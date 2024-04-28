#include "../src/board/bitboard/bitboard.hpp"
#include "doctest/doctest.hpp"

using namespace Board::Bitboards;

TEST_SUITE("Bitboards Tests") {
    TEST_CASE("fileOf") {
        CHECK(Util::fileOf(Square::A1) == File::FILE_A);
        CHECK(Util::fileOf(Square::H1) == File::FILE_H);
    }

    TEST_CASE("rankOf") {
        CHECK(Util::rankOf(Square::A1) == Rank::RANK_1);
        CHECK(Util::rankOf(Square::H8) == Rank::RANK_8);
    }

    TEST_CASE("squareOf") {
        CHECK(Util::squareOf(static_cast<U8>(File::FILE_A), static_cast<U8>(Rank::RANK_1))
              == Square::A1);
        CHECK(Util::squareOf(static_cast<U8>(File::FILE_H), static_cast<U8>(Rank::RANK_8))
              == Square::H8);
    }

    TEST_CASE("bitCount") {
        auto b = Bitboard(0x5ULL);
        CHECK(b.bitCount() == 2);

        b = Bitboard(0xFULL);
        CHECK(b.bitCount() == 4);
    }

    TEST_CASE("getMSB") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.getMSB() == 63);

        b = Bitboard(0x4000000000000000ULL);
        CHECK(b.getMSB() == 62);
    }

    TEST_CASE("getLSB") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.getLSB() == 0);

        b = Bitboard(0x0000000000000002ULL);
        CHECK(b.getLSB() == 1);
    }

    TEST_CASE("popLSB") {
        auto b = Bitboard(0x8100000000000081ULL);
        CHECK(b.popLSB() == 0);
        CHECK(b.asU64() == 0x8100000000000080);

        b = Bitboard(0x42ULL);
        CHECK(b.popLSB() == 1);
        CHECK(b.asU64() == 0x40ULL);
    }

    TEST_CASE("fromSquare") {
        CHECK(Bitboard::fromSquare(Square::A1) == Bitboard(0x1ULL));
        CHECK(Bitboard::fromSquare(Square::H8) == Bitboard(0x8000000000000000ULL));
    }

    TEST_CASE("shift") {
        SUBCASE("shift north") {
            CHECK(shift<Direction::NORTH>(Bitboard::fromSquare(Square::E4))
                  == Bitboard(0x1000000000ULL));
            CHECK(shift<Direction::NORTH>(Bitboard::fromSquare(Square::E8)) == Bitboard(0x0ULL));

            SUBCASE("shift north east") {
                CHECK(shift<Direction::NORTH_EAST>(Bitboard::fromSquare(Square::E4))
                      == Bitboard(0x2000000000ULL));
                CHECK(shift<Direction::NORTH_EAST>(Bitboard::fromSquare(Square::H4))
                      == Bitboard(0x0ULL));
            }

            SUBCASE("shift north west") {
                CHECK(shift<Direction::NORTH_WEST>(Bitboard::fromSquare(Square::E4))
                      == Bitboard(0x800000000ULL));
                CHECK(shift<Direction::NORTH_WEST>(Bitboard::fromSquare(Square::A4))
                      == Bitboard(0x0ULL));
            }
        }

        SUBCASE("shift south") {
            CHECK(shift<Direction::SOUTH>(Bitboard::fromSquare(Square::E4))
                  == Bitboard(0x100000ULL));
            CHECK(shift<Direction::SOUTH>(Bitboard::fromSquare(Square::E1)) == Bitboard(0x0ULL));

            SUBCASE("shift south east") {
                CHECK(shift<Direction::SOUTH_EAST>(Bitboard::fromSquare(Square::E4))
                      == Bitboard(0x200000ULL));
                CHECK(shift<Direction::SOUTH_EAST>(Bitboard::fromSquare(Square::H4))
                      == Bitboard(0x0ULL));
            }

            SUBCASE("shift south west") {
                CHECK(shift<Direction::SOUTH_WEST>(Bitboard::fromSquare(Square::E4))
                      == Bitboard(0x80000ULL));
                CHECK(shift<Direction::SOUTH_WEST>(Bitboard::fromSquare(Square::A4))
                      == Bitboard(0x0ULL));
            }
        }

        SUBCASE("shift east") {
            CHECK(shift<Direction::EAST>(Bitboard::fromSquare(Square::E4))
                  == Bitboard(0x20000000ULL));
            CHECK(shift<Direction::EAST>(Bitboard::fromSquare(Square::H4)) == Bitboard(0x0ULL));
        }

        SUBCASE("shift west") {
            CHECK(shift<Direction::WEST>(Bitboard::fromSquare(Square::E4))
                  == Bitboard(0x8000000ULL));
            CHECK(shift<Direction::WEST>(Bitboard::fromSquare(Square::A4)) == Bitboard(0x0ULL));
        }
    }
}