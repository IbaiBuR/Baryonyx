#pragma once

#include <array>

#include "../types.hpp"
#include "bitboard/bitboard.hpp"

namespace Board {

class CastlingRights {
    public:
        CastlingRights() :
            rights(0){};

    private:
        U8 rights;
};

class Position {
    public:
        Position() :
            stm(Color::WHITE),
            epSq(Square::NO_SQ),
            halfMoveClock(0),
            fullMoveNumver(1),
            pieces() {
            pieceBB.fill(Bitboards::Util::EmptyBB);
            occupiedBB.fill(Bitboards::Util::EmptyBB);
            pieces.fill(Piece::NO_PIECE);
        }

    private:
        Bitboards::Bitboard                                                       checkersBB;
        Color                                                                     stm;
        Square                                                                    epSq;
        CastlingRights                                                            castling;
        U16                                                                       halfMoveClock;
        U32                                                                       fullMoveNumver;
        std::array<Bitboards::Bitboard, static_cast<U8>(PieceType::PIECETYPE_NB)> pieceBB;
        std::array<Bitboards::Bitboard, static_cast<U8>(Color::COLOR_NB)>         occupiedBB;
        std::array<Piece, static_cast<U8>(Square::SQUARE_NB)>                     pieces;
};

} // namespace Board
