#pragma once

#include <array>
#include <string>

#include "bitboard/bitboard.hpp"

namespace Board {

enum class CastlingRights : U8 {
    NONE,
    WK = 1,
    WQ = 2,
    BK = 4,
    BQ = 8,
    ALL = 15
};

class Position {
    public:
        Position() :
            stm(Color::WHITE),
            epSq(Square::NO_SQ),
            castling(CastlingRights::NONE),
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

} // namespace Board
