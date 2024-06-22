#include "eval.hpp"

#include <utility>

#include "evalterms.hpp"
#include "../board/piece.hpp"

namespace eval {

int get_game_phase(const board::Position& pos) {
    const int gamePhase = gamePhaseIncrements[std::to_underlying(PieceType::KNIGHT)]
                            * pos.piece_type_bb(PieceType::KNIGHT).bit_count()
                        + gamePhaseIncrements[std::to_underlying(PieceType::BISHOP)]
                              * pos.piece_type_bb(PieceType::BISHOP).bit_count()
                        + gamePhaseIncrements[std::to_underlying(PieceType::ROOK)]
                              * pos.piece_type_bb(PieceType::ROOK).bit_count()
                        + gamePhaseIncrements[std::to_underlying(PieceType::QUEEN)]
                              * pos.piece_type_bb(PieceType::QUEEN).bit_count();

    return std::min(gamePhase, maxGamePhase);
}

template <Color stm>
PackedScore evaluate_material(const board::Position& pos) {
    constexpr Color us   = stm;
    constexpr Color them = ~us;

    const int pawnCount =
        pos.piece_count<us>(PieceType::PAWN) - pos.piece_count<them>(PieceType::PAWN);
    const int knightCount =
        pos.piece_count<us>(PieceType::KNIGHT) - pos.piece_count<them>(PieceType::KNIGHT);
    const int bishopCount =
        pos.piece_count<us>(PieceType::BISHOP) - pos.piece_count<them>(PieceType::BISHOP);
    const int rookCount =
        pos.piece_count<us>(PieceType::ROOK) - pos.piece_count<them>(PieceType::ROOK);
    const int queenCount =
        pos.piece_count<us>(PieceType::QUEEN) - pos.piece_count<them>(PieceType::QUEEN);

    PackedScore materialScore;

    materialScore += pieceValues[std::to_underlying(PieceType::PAWN)] * pawnCount;
    materialScore += pieceValues[std::to_underlying(PieceType::KNIGHT)] * knightCount;
    materialScore += pieceValues[std::to_underlying(PieceType::BISHOP)] * bishopCount;
    materialScore += pieceValues[std::to_underlying(PieceType::ROOK)] * rookCount;
    materialScore += pieceValues[std::to_underlying(PieceType::QUEEN)] * queenCount;

    return materialScore;
}

template <Color stm>
PackedScore evaluate_psqt(const board::Position& pos) {
    constexpr Color us   = stm;
    constexpr Color them = ~us;

    PackedScore psqtScore;

    auto ourPieces   = pos.occupancies(us);
    auto theirPieces = pos.occupancies(them);

    while (!ourPieces.empty()) {
        const auto      sq = static_cast<Square>(ourPieces.pop_lsb());
        const PieceType pt = board::pieces::piece_to_piece_type(pos.piece_on(sq));
        psqtScore +=
            PSQT::allPSQT[std::to_underlying(pt)][std::to_underlying(relative_square<us>(sq))];
    }

    while (!theirPieces.empty()) {
        const auto      sq = static_cast<Square>(theirPieces.pop_lsb());
        const PieceType pt = board::pieces::piece_to_piece_type(pos.piece_on(sq));
        psqtScore -=
            PSQT::allPSQT[std::to_underlying(pt)][std::to_underlying(relative_square<them>(sq))];
    }

    return psqtScore;
}

template <Color stm>
Score evaluate(const board::Position& pos) {
    const PackedScore score     = evaluate_material<stm>(pos) + evaluate_psqt<stm>(pos);
    const int         gamePhase = get_game_phase(pos);
    const Score       eval =
        (score.midgame() * gamePhase + score.endgame() * (maxGamePhase - gamePhase)) / maxGamePhase;

    return eval;
}

Score evaluate(const board::Position& pos) {
    return pos.side_to_move() == Color::WHITE ? evaluate<Color::WHITE>(pos)
                                              : evaluate<Color::BLACK>(pos);
}

} // namespace eval