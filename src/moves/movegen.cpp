#include "movegen.hpp"

#include "../board/bitboard/attacks.hpp"

namespace moves {

namespace bb = board::bitboards;

template <Color c>
constexpr bb::Bitboard single_pawn_push(const bb::Bitboard& pawns, const bb::Bitboard& empty) {
    if constexpr (c == Color::WHITE)
        return bb::shift<Direction::NORTH>(pawns) & empty;
    else
        return bb::shift<Direction::SOUTH>(pawns) & empty;
}

template <Color c>
constexpr bb::Bitboard double_pawn_push(const bb::Bitboard& pawns, const bb::Bitboard& empty) {
    if constexpr (c == Color::WHITE)
        return bb::shift<Direction::NORTH>(single_pawn_push<c>(pawns, empty)) & empty
             & bb::util::rank_4_bb;
    else
        return bb::shift<Direction::SOUTH>(single_pawn_push<c>(pawns, empty)) & empty
             & bb::util::rank_5_bb;
}

template <Color c>
void generate_pawn_pushes(const board::Position& pos, MoveList& moveList) {
    constexpr Color     us       = c;
    constexpr Color     them     = ~us;
    constexpr Direction offset   = us == Color::WHITE ? Direction::NORTH : Direction::SOUTH;
    const bb::Bitboard& ourPawns = pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us);
    const bb::Bitboard& empty    = ~(pos.occupancies(us) | pos.occupancies(them));

    bb::Bitboard singlePush = single_pawn_push<us>(ourPawns, empty);
    bb::Bitboard doublePush = double_pawn_push<us>(ourPawns, empty);

    while (!singlePush.empty()) {
        const auto to         = static_cast<Square>(singlePush.pop_lsb());
        const auto from       = to - offset;
        const auto movingRank = rank_of(to);

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
        const auto to   = static_cast<Square>(doublePush.pop_lsb());
        const auto from = to - offset * 2;
        moveList.push(Move(from, to, Move::MoveFlag::DOUBLEPUSH));
    }
}

template <Color c>
void generate_pawn_captures(const board::Position& pos, MoveList& moveList) {
    constexpr Color us       = c;
    constexpr Color them     = ~us;
    bb::Bitboard    ourPawns = pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us);

    while (!ourPawns.empty()) {
        const auto   from = static_cast<Square>(ourPawns.pop_lsb());
        bb::Bitboard possiblePawnCaptures =
            bb::attacks::get_pawn_attacks(from, us) & pos.occupancies(them);

        while (!possiblePawnCaptures.empty()) {
            const auto to         = static_cast<Square>(possiblePawnCaptures.pop_lsb());
            const auto movingRank = rank_of(to);

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

    if (pos.ep_square() != Square::NONE) {
        bb::Bitboard epPawns = bb::attacks::get_pawn_attacks(pos.ep_square(), them)
                             & (pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us));

        while (!epPawns.empty()) {
            const auto from = static_cast<Square>(epPawns.pop_lsb());
            moveList.push(Move(from, pos.ep_square(), Move::MoveFlag::ENPASSANT));
        }
    }
}

template <Color c>
void generate_quiets_by_piece_type(const board::Position& pos,
                                   MoveList&              moveList,
                                   const PieceType        pt) {
    constexpr Color     us        = c;
    constexpr Color     them      = ~us;
    const bb::Bitboard& occupied  = pos.occupancies(us) | pos.occupancies(them);
    bb::Bitboard        ourPieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!ourPieces.empty()) {
        const auto   from = static_cast<Square>(ourPieces.pop_lsb());
        bb::Bitboard possiblePieceMoves =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & ~occupied;

        while (!possiblePieceMoves.empty()) {
            const auto to = static_cast<Square>(possiblePieceMoves.pop_lsb());
            moveList.push(Move(from, to, Move::MoveFlag::QUIET));
        }
    }
}

template <Color c>
void generate_captures_by_piece_type(const board::Position& pos,
                                     MoveList&              moveList,
                                     const PieceType        pt) {
    constexpr Color     us        = c;
    constexpr Color     them      = ~us;
    const bb::Bitboard& occupied  = pos.occupancies(us) | pos.occupancies(them);
    bb::Bitboard        ourPieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!ourPieces.empty()) {
        const auto   from = static_cast<Square>(ourPieces.pop_lsb());
        bb::Bitboard possiblePieceCaptures =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & pos.occupancies(them);

        while (!possiblePieceCaptures.empty()) {
            const auto to = static_cast<Square>(possiblePieceCaptures.pop_lsb());
            moveList.push(Move(from, to, Move::MoveFlag::CAPTURE));
        }
    }
}

template <Color c>
void generate_castling_moves(const board::Position& pos, MoveList& moveList) {
    if constexpr (c == Color::WHITE) {
        if (pos.can_castle_king_side<Color::WHITE>())
            moveList.push(Move(Square::E1, Square::G1, Move::MoveFlag::CASTLE));
        if (pos.can_castle_queen_side<Color::WHITE>())
            moveList.push(Move(Square::E1, Square::C1, Move::MoveFlag::CASTLE));
    }
    else {
        if (pos.can_castle_king_side<Color::BLACK>())
            moveList.push(Move(Square::E8, Square::G8, Move::MoveFlag::CASTLE));
        if (pos.can_castle_queen_side<Color::BLACK>())
            moveList.push(Move(Square::E8, Square::C8, Move::MoveFlag::CASTLE));
    }
}

void generate_all_quiets(const board::Position& pos, MoveList& moveList) {
    if (pos.side_to_move() == Color::WHITE) {
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_quiets_by_piece_type<Color::WHITE>(pos, moveList, pt);
    }
    else {
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_quiets_by_piece_type<Color::BLACK>(pos, moveList, pt);
    }
}

void generate_all_captures(const board::Position& pos, MoveList& moveList) {
    if (pos.side_to_move() == Color::WHITE) {
        generate_pawn_captures<Color::WHITE>(pos, moveList);
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_captures_by_piece_type<Color::WHITE>(pos, moveList, pt);
    }
    else {
        generate_pawn_captures<Color::BLACK>(pos, moveList);
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_captures_by_piece_type<Color::BLACK>(pos, moveList, pt);
    }
}

void generate_all_moves(const board::Position& pos, MoveList& moveList) {
    const int numCheckers = pos.checkers().bit_count();

    if (numCheckers == 0) {
        generate_all_captures(pos, moveList);

        if (pos.side_to_move() == Color::WHITE) {
            generate_pawn_pushes<Color::WHITE>(pos, moveList);
            generate_castling_moves<Color::WHITE>(pos, moveList);
        }
        else {
            generate_pawn_pushes<Color::BLACK>(pos, moveList);
            generate_castling_moves<Color::BLACK>(pos, moveList);
        }

        generate_all_quiets(pos, moveList);
    }
    else if (numCheckers == 1) {
        generate_all_captures(pos, moveList);

        if (pos.side_to_move() == Color::WHITE)
            generate_pawn_pushes<Color::WHITE>(pos, moveList);
        else {
            generate_pawn_pushes<Color::BLACK>(pos, moveList);
        }

        generate_all_quiets(pos, moveList);
    }
    else {
        if (pos.side_to_move() == Color::WHITE) {
            generate_captures_by_piece_type<Color::WHITE>(pos, moveList, PieceType::KING);
            generate_quiets_by_piece_type<Color::WHITE>(pos, moveList, PieceType::KING);
        }
        else {
            generate_captures_by_piece_type<Color::BLACK>(pos, moveList, PieceType::KING);
            generate_quiets_by_piece_type<Color::BLACK>(pos, moveList, PieceType::KING);
        }
    }
}

} // namespace moves