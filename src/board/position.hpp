#pragma once

#include <array>
#include <string>

#include "bitboard/bitboard.hpp"
#include "../moves/move.hpp"

namespace Board {

class CastlingRights {
    public:
        enum class Flags : U8 {
            NONE,
            WK  = 1,
            WQ  = 2,
            BK  = 4,
            BQ  = 8,
            ALL = 15
        };

        constexpr CastlingRights() :
            castlingFlags(Flags::NONE) {}

        constexpr explicit CastlingRights(const Flags flag) :
            castlingFlags(flag) {}

        constexpr explicit CastlingRights(const std::string &flags) :
            castlingFlags(Flags::NONE) {
            for (const char c : flags) {
                switch (c) {
                case 'K':
                    *this |= CastlingRights(Flags::WK);
                    break;
                case 'Q':
                    *this |= CastlingRights(Flags::WQ);
                    break;
                case 'k':
                    *this |= CastlingRights(Flags::BK);
                    break;
                case 'q':
                    *this |= CastlingRights(Flags::BQ);
                    break;
                default:
                    break;
                }
            }
        }

        constexpr bool operator==(const CastlingRights &other) const {
            return std::to_underlying(castlingFlags) == std::to_underlying(other.castlingFlags);
        }

        constexpr bool operator!=(const CastlingRights &other) const {
            return std::to_underlying(castlingFlags) != std::to_underlying(other.castlingFlags);
        }

        constexpr CastlingRights operator|(const CastlingRights &other) const {
            return CastlingRights(static_cast<Flags>(std::to_underlying(castlingFlags)
                                                     | std::to_underlying(other.castlingFlags)));
        }

        constexpr CastlingRights &operator|=(const CastlingRights &other) {
            castlingFlags = static_cast<Flags>(std::to_underlying(castlingFlags)
                                               | std::to_underlying(other.castlingFlags));
            return *this;
        }

        constexpr CastlingRights operator&(const int update) const {
            return CastlingRights(static_cast<Flags>(std::to_underlying(castlingFlags) & update));
        }

        constexpr CastlingRights &operator&=(const int update) {
            castlingFlags = static_cast<Flags>(std::to_underlying(castlingFlags) & update);
            return *this;
        }

        template <Color c>
        [[nodiscard]] bool kingSideAvailable() const {
            if constexpr (c == Color::WHITE)
                return std::to_underlying(castlingFlags) & std::to_underlying(Flags::WK);
            else
                return std::to_underlying(castlingFlags) & std::to_underlying(Flags::BK);
        }

        template <Color c>
        [[nodiscard]] bool queenSideAvailable() const {
            if constexpr (c == Color::WHITE)
                return std::to_underlying(castlingFlags) & std::to_underlying(Flags::WQ);
            else
                return std::to_underlying(castlingFlags) & std::to_underlying(Flags::BQ);
        }

        [[nodiscard]] std::string toString() const {
            std::string result;

            if (kingSideAvailable<Color::WHITE>())
                result += 'K';
            if (queenSideAvailable<Color::WHITE>())
                result += 'Q';
            if (kingSideAvailable<Color::BLACK>())
                result += 'k';
            if (queenSideAvailable<Color::BLACK>())
                result += 'q';

            if (result.empty())
                result = '-';

            return result;
        }

    private:
        Flags castlingFlags;
};

class Position {
    public:
        Position() :
            stm(Color::WHITE),
            epSq(Square::NO_SQ),
            halfMoveClock(0),
            fullMoveNumver(1),
            pieces() {
            pieces.fill(Piece::NO_PIECE);
        }

        explicit Position(const std::string &fen);

        [[nodiscard]] Bitboards::Bitboard checkers() const { return checkersBB; }
        [[nodiscard]] Color               sideToMove() const { return stm; }
        [[nodiscard]] Square              epSquare() const { return epSq; }
        [[nodiscard]] CastlingRights      castlingRights() const { return castling; }
        [[nodiscard]] U8                  fiftyMoveRule() const { return halfMoveClock; }
        [[nodiscard]] U16                 fullMoves() const { return fullMoveNumver; }

        [[nodiscard]] Piece pieceOn(const Square sq) const {
            return pieces[std::to_underlying(sq)];
        }

        [[nodiscard]] Bitboards::Bitboard occupancies(const Color c) const {
            return occupiedBB[std::to_underlying(c)];
        }

        [[nodiscard]] Bitboards::Bitboard pieceTypeBB(const PieceType pt) const {
            return pieceBB[std::to_underlying(pt)];
        }

        template <Color c>
        [[nodiscard]] bool canCastleKingSide() const {
            const Bitboards::Bitboard &occupied =
                occupancies(Color::WHITE) | occupancies(Color::BLACK);

            if constexpr (c == Color::WHITE)
                return castlingRights().kingSideAvailable<Color::WHITE>()
                    && !isSquareAttackedBy(Square::F1, Color::BLACK)
                    && !isSquareAttackedBy(Square::G1, Color::BLACK)
                    && !(occupied & Bitboards::Bitboard(0x60ULL));
            else
                return castlingRights().kingSideAvailable<Color::BLACK>()
                    && !isSquareAttackedBy(Square::F8, Color::WHITE)
                    && !isSquareAttackedBy(Square::G8, Color::WHITE)
                    && !(occupied & Bitboards::Bitboard(0x6000000000000000ULL));
        }

        template <Color c>
        [[nodiscard]] bool canCastleQueenSide() const {
            const Bitboards::Bitboard &occupied =
                occupancies(Color::WHITE) | occupancies(Color::BLACK);

            if constexpr (c == Color::WHITE)
                return castlingRights().queenSideAvailable<Color::WHITE>()
                    && !isSquareAttackedBy(Square::D1, Color::BLACK)
                    && !isSquareAttackedBy(Square::C1, Color::BLACK)
                    && !(occupied & Bitboards::Bitboard(0xEULL));
            else
                return castlingRights().queenSideAvailable<Color::BLACK>()
                    && !isSquareAttackedBy(Square::D8, Color::WHITE)
                    && !isSquareAttackedBy(Square::C8, Color::WHITE)
                    && !(occupied & Bitboards::Bitboard(0xE00000000000000ULL));
        }

        [[nodiscard]] Bitboards::Bitboard attacksToKing(Square kingSquare, Color c) const;

        [[nodiscard]] Square kingSquare(Color c) const;

        void setPiece(Piece p, Square sq);

        void removePiece(Piece p, Square sq);

        void movePiece(Piece p, Square from, Square to);

        void makeMove(Moves::Move move);

        void resetToStartPos();

        [[nodiscard]] bool isSquareAttackedBy(Square sq, Color c) const;

        [[nodiscard]] bool isValid() const;

        [[nodiscard]] bool wasLegal() const;

        [[nodiscard]] std::string toFen() const;

    private:
        Bitboards::Bitboard                                                          checkersBB;
        Color                                                                        stm;
        Square                                                                       epSq;
        CastlingRights                                                               castling;
        U8                                                                           halfMoveClock;
        U16                                                                          fullMoveNumver;
        std::array<Bitboards::Bitboard, std::to_underlying(PieceType::PIECETYPE_NB)> pieceBB;
        std::array<Bitboards::Bitboard, std::to_underlying(Color::COLOR_NB)>         occupiedBB;
        std::array<Piece, std::to_underlying(Square::SQUARE_NB)>                     pieces;
};

namespace Util {

constexpr auto startPosFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// clang-format off
constexpr std::array<std::string_view, std::to_underlying(Square::SQUARE_NB)> sqToCoords = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

constexpr std::array castlingRightsUpdate = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11
};
// clang-format on

} // namespace Util

void printBoard(const Position &pos);

} // namespace Board
