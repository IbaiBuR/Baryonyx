#include "movegen.hpp"

#include "../board/bitboard/attacks.hpp"

namespace Moves {

namespace BB = Board::Bitboards;

template <Color c>
constexpr BB::Bitboard singlePawnPush(const BB::Bitboard &pawns, const BB::Bitboard &empty) {
    return c == Color::WHITE ? BB::shift<Direction::NORTH>(pawns) & empty
                             : BB::shift<Direction::SOUTH>(pawns) & empty;
}

template <Color c>
constexpr BB::Bitboard doublePawnPush(const BB::Bitboard &pawns, const BB::Bitboard &empty) {
    return c == Color::WHITE ? BB::shift<Direction::NORTH>(singlePawnPush<c>(pawns, empty)) & empty
                                   & BB::Util::Rank4BB
                             : BB::shift<Direction::SOUTH>(singlePawnPush<c>(pawns, empty)) & empty
                                   & BB::Util::Rank5BB;
}

template <Color c>
constexpr void generatePawnPushes(const Board::Position &pos, MoveList &moveList) {
    const BB::Bitboard  ourPawns = pos.pieceTypeBB(PieceType::PAWN) & pos.occupancies(c);
    const BB::Bitboard  empty    = ~(pos.occupancies(Color::WHITE) | pos.occupancies(Color::BLACK));
    constexpr Direction offset   = c == Color::WHITE ? Direction::NORTH : Direction::SOUTH;

    BB::Bitboard singlePush = singlePawnPush<c>(ourPawns, empty);
    BB::Bitboard doublePush = doublePawnPush<c>(ourPawns, empty);

    while (!singlePush.empty()) {
        const auto to   = static_cast<Square>(singlePush.popLSB());
        const auto from = static_cast<Square>(std::to_underlying(to) - std::to_underlying(offset));
        const auto movingRank = BB::Util::rankOf(to);

        if (movingRank != Rank::RANK_1 && movingRank != Rank::RANK_8)
            moveList.push(Move(from, to, Move::MoveFlag::QUIET));
        else {
            moveList.push(Move(from, to, Move::MoveFlag::KNIGHT_PROMO));
            moveList.push(Move(from, to, Move::MoveFlag::BISHOP_PROMO));
            moveList.push(Move(from, to, Move::MoveFlag::ROOK_PROMO));
            moveList.push(Move(from, to, Move::MoveFlag::QUEEN_PROMO));
        }
    }

    while (!doublePush.empty()) {
        const auto to = static_cast<Square>(doublePush.popLSB());
        const auto from =
            static_cast<Square>(std::to_underlying(to) - std::to_underlying(offset) * 2);
        moveList.push(Move(from, to, Move::MoveFlag::DOUBLEPUSH));
    }
}

template <Color c>
constexpr void generatePawnCaptures(const Board::Position &pos, MoveList &moveList) {
    constexpr Color us       = c;
    constexpr Color them     = us == Color::WHITE ? Color::BLACK : Color::WHITE;
    BB::Bitboard    ourPawns = pos.pieceTypeBB(PieceType::PAWN) & pos.occupancies(us);

    while (!ourPawns.empty()) {
        const auto   from = static_cast<Square>(ourPawns.popLSB());
        BB::Bitboard possiblePawnCaptures =
            BB::Attacks::getPawnAttacks(from, us) & pos.occupancies(them);

        while (!possiblePawnCaptures.empty()) {
            const auto to         = static_cast<Square>(possiblePawnCaptures.popLSB());
            const auto movingRank = BB::Util::rankOf(to);

            if (movingRank != Rank::RANK_1 && movingRank != Rank::RANK_8)
                moveList.push(Move(from, to, Move::MoveFlag::CAPTURE));
            else {
                moveList.push(Move(from, to, Move::MoveFlag::KNIGHT_CAPTURE_PROMO));
                moveList.push(Move(from, to, Move::MoveFlag::BISHOP_CATURE_PROMO));
                moveList.push(Move(from, to, Move::MoveFlag::ROOK_CAPTURE_PROMO));
                moveList.push(Move(from, to, Move::MoveFlag::QUEEN_CAPTURE_PROMO));
            }
        }
    }

    if (pos.epSquare() != Square::NO_SQ) {
        BB::Bitboard epPawns = BB::Attacks::getPawnAttacks(pos.epSquare(), them)
                             & (pos.pieceTypeBB(PieceType::PAWN) & pos.occupancies(us));

        while (!epPawns.empty()) {
            const auto from = static_cast<Square>(epPawns.popLSB());
            moveList.push(Move(from, pos.epSquare(), Move::MoveFlag::ENPASSANT));
        }
    }
}

void generateAllQuiets(const Board::Position &pos, MoveList &moveList) {
    if (pos.sideToMove() == Color::WHITE) {
        generatePawnPushes<Color::WHITE>(pos, moveList);
    }
    else {
        generatePawnPushes<Color::BLACK>(pos, moveList);
    }
}

void generateAllCaptures(const Board::Position &pos, MoveList &moveList) {
    if (pos.sideToMove() == Color::WHITE) {
        generatePawnCaptures<Color::WHITE>(pos, moveList);
    }
    else {
        generatePawnCaptures<Color::BLACK>(pos, moveList);
    }
}

} // namespace Moves