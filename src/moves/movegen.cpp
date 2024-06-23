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
void generate_pawn_pushes(const board::Position& pos, MoveList& move_list) {
    constexpr Color     us     = c;
    constexpr Color     them   = ~us;
    constexpr Direction offset = us == Color::WHITE ? Direction::NORTH : Direction::SOUTH;

    const auto& our_pawns   = pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us);
    const auto& empty       = ~(pos.occupancies(us) | pos.occupancies(them));
    auto        single_push = single_pawn_push<us>(our_pawns, empty);
    auto        double_push = double_pawn_push<us>(our_pawns, empty);

    while (!single_push.empty()) {
        const auto to          = static_cast<Square>(single_push.pop_lsb());
        const auto from        = to - offset;
        const auto moving_rank = rank_of(to);

        if (moving_rank != Rank::RANK_1 && moving_rank != Rank::RANK_8)
            move_list.push(Move(from, to, Move::MoveFlag::QUIET));
        else {
            move_list.push(Move(from, to, Move::MoveFlag::KNIGHT_PROMO));
            move_list.push(Move(from, to, Move::MoveFlag::BISHOP_PROMO));
            move_list.push(Move(from, to, Move::MoveFlag::ROOK_PROMO));
            move_list.push(Move(from, to, Move::MoveFlag::QUEEN_PROMO));
        }
    }

    while (!double_push.empty()) {
        const auto to   = static_cast<Square>(double_push.pop_lsb());
        const auto from = to - offset * 2;
        move_list.push(Move(from, to, Move::MoveFlag::DOUBLEPUSH));
    }
}

template <Color c>
void generate_pawn_captures(const board::Position& pos, MoveList& move_list) {
    constexpr Color us        = c;
    constexpr Color them      = ~us;
    auto            our_pawns = pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us);

    while (!our_pawns.empty()) {
        const auto from = static_cast<Square>(our_pawns.pop_lsb());
        auto       possible_pawn_captures =
            bb::attacks::get_pawn_attacks(from, us) & pos.occupancies(them);

        while (!possible_pawn_captures.empty()) {
            const auto to          = static_cast<Square>(possible_pawn_captures.pop_lsb());
            const auto moving_rank = rank_of(to);

            if (moving_rank != Rank::RANK_1 && moving_rank != Rank::RANK_8)
                move_list.push(Move(from, to, Move::MoveFlag::CAPTURE));
            else {
                move_list.push(Move(from, to, Move::MoveFlag::KNIGHT_CAPTURE_PROMO));
                move_list.push(Move(from, to, Move::MoveFlag::BISHOP_CATURE_PROMO));
                move_list.push(Move(from, to, Move::MoveFlag::ROOK_CAPTURE_PROMO));
                move_list.push(Move(from, to, Move::MoveFlag::QUEEN_CAPTURE_PROMO));
            }
        }
    }

    if (pos.ep_square() != Square::NONE) {
        auto ep_pawns = bb::attacks::get_pawn_attacks(pos.ep_square(), them)
                      & (pos.piece_type_bb(PieceType::PAWN) & pos.occupancies(us));

        while (!ep_pawns.empty()) {
            const auto from = static_cast<Square>(ep_pawns.pop_lsb());
            move_list.push(Move(from, pos.ep_square(), Move::MoveFlag::ENPASSANT));
        }
    }
}

template <Color c>
void generate_quiets_by_piece_type(const board::Position& pos,
                                   MoveList&              move_list,
                                   const PieceType        pt) {
    constexpr Color us         = c;
    constexpr Color them       = ~us;
    const auto&     occupied   = pos.occupancies(us) | pos.occupancies(them);
    auto            our_pieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!our_pieces.empty()) {
        const auto from = static_cast<Square>(our_pieces.pop_lsb());
        auto       possible_piece_moves =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & ~occupied;

        while (!possible_piece_moves.empty()) {
            const auto to = static_cast<Square>(possible_piece_moves.pop_lsb());
            move_list.push(Move(from, to, Move::MoveFlag::QUIET));
        }
    }
}

template <Color c>
void generate_captures_by_piece_type(const board::Position& pos,
                                     MoveList&              move_list,
                                     const PieceType        pt) {
    constexpr Color us         = c;
    constexpr Color them       = ~us;
    const auto&     occupied   = pos.occupancies(us) | pos.occupancies(them);
    auto            our_pieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!our_pieces.empty()) {
        const auto from = static_cast<Square>(our_pieces.pop_lsb());
        auto       possible_piece_captures =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & pos.occupancies(them);

        while (!possible_piece_captures.empty()) {
            const auto to = static_cast<Square>(possible_piece_captures.pop_lsb());
            move_list.push(Move(from, to, Move::MoveFlag::CAPTURE));
        }
    }
}

template <Color c>
void generate_castling_moves(const board::Position& pos, MoveList& move_list) {
    if constexpr (c == Color::WHITE) {
        if (pos.can_castle_king_side<Color::WHITE>())
            move_list.push(Move(Square::E1, Square::G1, Move::MoveFlag::CASTLE));
        if (pos.can_castle_queen_side<Color::WHITE>())
            move_list.push(Move(Square::E1, Square::C1, Move::MoveFlag::CASTLE));
    }
    else {
        if (pos.can_castle_king_side<Color::BLACK>())
            move_list.push(Move(Square::E8, Square::G8, Move::MoveFlag::CASTLE));
        if (pos.can_castle_queen_side<Color::BLACK>())
            move_list.push(Move(Square::E8, Square::C8, Move::MoveFlag::CASTLE));
    }
}

void generate_all_quiets(const board::Position& pos, MoveList& move_list) {
    if (pos.side_to_move() == Color::WHITE) {
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_quiets_by_piece_type<Color::WHITE>(pos, move_list, pt);
    }
    else {
        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_quiets_by_piece_type<Color::BLACK>(pos, move_list, pt);
    }
}

void generate_all_captures(const board::Position& pos, MoveList& move_list) {
    if (pos.side_to_move() == Color::WHITE) {
        generate_pawn_captures<Color::WHITE>(pos, move_list);

        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_captures_by_piece_type<Color::WHITE>(pos, move_list, pt);
    }
    else {
        generate_pawn_captures<Color::BLACK>(pos, move_list);

        for (const PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK,
                                   PieceType::QUEEN, PieceType::KING})
            generate_captures_by_piece_type<Color::BLACK>(pos, move_list, pt);
    }
}

void generate_all_moves(const board::Position& pos, MoveList& move_list) {
    const int num_checkers = pos.checkers().bit_count();

    if (num_checkers == 0) {
        generate_all_captures(pos, move_list);

        if (pos.side_to_move() == Color::WHITE) {
            generate_pawn_pushes<Color::WHITE>(pos, move_list);
            generate_castling_moves<Color::WHITE>(pos, move_list);
        }
        else {
            generate_pawn_pushes<Color::BLACK>(pos, move_list);
            generate_castling_moves<Color::BLACK>(pos, move_list);
        }

        generate_all_quiets(pos, move_list);
    }
    else if (num_checkers == 1) {
        generate_all_captures(pos, move_list);

        if (pos.side_to_move() == Color::WHITE)
            generate_pawn_pushes<Color::WHITE>(pos, move_list);
        else {
            generate_pawn_pushes<Color::BLACK>(pos, move_list);
        }

        generate_all_quiets(pos, move_list);
    }
    else {
        if (pos.side_to_move() == Color::WHITE) {
            generate_captures_by_piece_type<Color::WHITE>(pos, move_list, PieceType::KING);
            generate_quiets_by_piece_type<Color::WHITE>(pos, move_list, PieceType::KING);
        }
        else {
            generate_captures_by_piece_type<Color::BLACK>(pos, move_list, PieceType::KING);
            generate_quiets_by_piece_type<Color::BLACK>(pos, move_list, PieceType::KING);
        }
    }
}

} // namespace moves