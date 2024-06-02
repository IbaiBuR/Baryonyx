#include "eval.hpp"

#include <utility>

#include "evalterms.hpp"
#include "../board/piece.hpp"

namespace Eval {

int getGamePhase(const Board::Position &pos) {
    const int gamePhase = gamePhaseIncrements[std::to_underlying(PieceType::KNIGHT)]
                            * pos.pieceTypeBB(PieceType::KNIGHT).bitCount()
                        + gamePhaseIncrements[std::to_underlying(PieceType::BISHOP)]
                              * pos.pieceTypeBB(PieceType::BISHOP).bitCount()
                        + gamePhaseIncrements[std::to_underlying(PieceType::ROOK)]
                              * pos.pieceTypeBB(PieceType::ROOK).bitCount()
                        + gamePhaseIncrements[std::to_underlying(PieceType::QUEEN)]
                              * pos.pieceTypeBB(PieceType::QUEEN).bitCount();

    return std::min(gamePhase, maxGamePhase);
}

template <Color stm>
PackedScore evaluateMaterial(const Board::Position &pos) {
    constexpr Color us   = stm;
    constexpr Color them = ~us;

    const int pawnCount =
        pos.pieceCount<us>(PieceType::PAWN) - pos.pieceCount<them>(PieceType::PAWN);
    const int knightCount =
        pos.pieceCount<us>(PieceType::KNIGHT) - pos.pieceCount<them>(PieceType::KNIGHT);
    const int bishopCount =
        pos.pieceCount<us>(PieceType::BISHOP) - pos.pieceCount<them>(PieceType::BISHOP);
    const int rookCount =
        pos.pieceCount<us>(PieceType::ROOK) - pos.pieceCount<them>(PieceType::ROOK);
    const int queenCount =
        pos.pieceCount<us>(PieceType::QUEEN) - pos.pieceCount<them>(PieceType::QUEEN);

    PackedScore materialScore;

    materialScore += pieceValues[std::to_underlying(PieceType::PAWN)] * pawnCount;
    materialScore += pieceValues[std::to_underlying(PieceType::KNIGHT)] * knightCount;
    materialScore += pieceValues[std::to_underlying(PieceType::BISHOP)] * bishopCount;
    materialScore += pieceValues[std::to_underlying(PieceType::ROOK)] * rookCount;
    materialScore += pieceValues[std::to_underlying(PieceType::QUEEN)] * queenCount;

    return materialScore;
}

template <Color stm>
PackedScore evaluatePSQT(const Board::Position &pos) {
    constexpr Color us   = stm;
    constexpr Color them = ~us;

    PackedScore psqtScore;

    auto ourPieces   = pos.occupancies(us);
    auto theirPieces = pos.occupancies(them);

    while (!ourPieces.empty()) {
        const auto      sq = static_cast<Square>(ourPieces.popLSB());
        const PieceType pt = Board::Pieces::pieceToPieceType[std::to_underlying(pos.pieceOn(sq))];
        psqtScore +=
            PSQT::allPSQT[std::to_underlying(pt)][std::to_underlying(relativeSquare<us>(sq))];
    }

    while (!theirPieces.empty()) {
        const auto      sq = static_cast<Square>(theirPieces.popLSB());
        const PieceType pt = Board::Pieces::pieceToPieceType[std::to_underlying(pos.pieceOn(sq))];
        psqtScore -=
            PSQT::allPSQT[std::to_underlying(pt)][std::to_underlying(relativeSquare<them>(sq))];
    }

    return psqtScore;
}

template <Color stm>
Score evaluate(const Board::Position &pos) {
    const PackedScore score     = evaluateMaterial<stm>(pos) + evaluatePSQT<stm>(pos);
    const int         gamePhase = getGamePhase(pos);
    const Score       eval =
        (score.midgame() * gamePhase + score.endgame() * (maxGamePhase - gamePhase)) / maxGamePhase;

    return eval;
}

Score evaluate(const Board::Position &pos) {
    return pos.sideToMove() == Color::WHITE ? evaluate<Color::WHITE>(pos)
                                            : evaluate<Color::BLACK>(pos);
}

} // namespace Eval