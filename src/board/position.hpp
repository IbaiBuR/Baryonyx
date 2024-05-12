#pragma once

#include <array>

#include "bitboard/bitboard.hpp"

namespace Board {

enum class CastlingRights : U8 {
    NONE,
    WK = 1,
    WQ = 2,
    BK = 4,
    BQ = 8,
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
