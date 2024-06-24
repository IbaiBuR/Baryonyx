#include "movegen.hpp"

#include "../board/bitboard/attacks.hpp"

namespace moves {

namespace bb = board::bitboards;

template <color C>
constexpr bb::bitboard single_pawn_push(const bb::bitboard& pawns, const bb::bitboard& empty) {
    if constexpr (C == color::white)
        return bb::shift<direction::north>(pawns) & empty;
    else
        return bb::shift<direction::south>(pawns) & empty;
}

template <color C>
constexpr bb::bitboard double_pawn_push(const bb::bitboard& pawns, const bb::bitboard& empty) {
    if constexpr (C == color::white)
        return bb::shift<direction::north>(single_pawn_push<C>(pawns, empty)) & empty
             & bb::util::rank_4_bb;
    else
        return bb::shift<direction::south>(single_pawn_push<C>(pawns, empty)) & empty
             & bb::util::rank_5_bb;
}

template <color Stm>
void generate_pawn_pushes(const board::position& pos, move_list& move_list) {
    constexpr color     us     = Stm;
    constexpr color     them   = ~us;
    constexpr direction offset = us == color::white ? direction::north : direction::south;

    const auto& our_pawns   = pos.piece_type_bb(piece_type::pawn) & pos.occupancies(us);
    const auto& empty       = ~(pos.occupancies(us) | pos.occupancies(them));
    auto        single_push = single_pawn_push<us>(our_pawns, empty);
    auto        double_push = double_pawn_push<us>(our_pawns, empty);

    while (!single_push.empty()) {
        const auto to          = static_cast<square>(single_push.pop_lsb());
        const auto from        = to - offset;
        const auto moving_rank = rank_of(to);

        if (moving_rank != rank::rank_1 && moving_rank != rank::rank_8)
            move_list.push(move(from, to, move::move_flag::quiet));
        else {
            move_list.push(move(from, to, move::move_flag::knight_promo));
            move_list.push(move(from, to, move::move_flag::bishop_promo));
            move_list.push(move(from, to, move::move_flag::rook_promo));
            move_list.push(move(from, to, move::move_flag::queen_promo));
        }
    }

    while (!double_push.empty()) {
        const auto to   = static_cast<square>(double_push.pop_lsb());
        const auto from = to - offset * 2;
        move_list.push(move(from, to, move::move_flag::double_push));
    }
}

template <color Stm>
void generate_pawn_captures(const board::position& pos, move_list& move_list) {
    constexpr color us        = Stm;
    constexpr color them      = ~us;
    auto            our_pawns = pos.piece_type_bb(piece_type::pawn) & pos.occupancies(us);

    while (!our_pawns.empty()) {
        const auto from = static_cast<square>(our_pawns.pop_lsb());
        auto       possible_pawn_captures =
            bb::attacks::get_pawn_attacks(from, us) & pos.occupancies(them);

        while (!possible_pawn_captures.empty()) {
            const auto to          = static_cast<square>(possible_pawn_captures.pop_lsb());
            const auto moving_rank = rank_of(to);

            if (moving_rank != rank::rank_1 && moving_rank != rank::rank_8)
                move_list.push(move(from, to, move::move_flag::capture));
            else {
                move_list.push(move(from, to, move::move_flag::knight_capture_promo));
                move_list.push(move(from, to, move::move_flag::bishop_cature_promo));
                move_list.push(move(from, to, move::move_flag::rook_capture_promo));
                move_list.push(move(from, to, move::move_flag::queen_capture_promo));
            }
        }
    }

    if (pos.ep_square() != square::none) {
        auto ep_pawns = bb::attacks::get_pawn_attacks(pos.ep_square(), them)
                      & (pos.piece_type_bb(piece_type::pawn) & pos.occupancies(us));

        while (!ep_pawns.empty()) {
            const auto from = static_cast<square>(ep_pawns.pop_lsb());
            move_list.push(move(from, pos.ep_square(), move::move_flag::en_passant));
        }
    }
}

template <color Stm>
void generate_quiets_by_piece_type(const board::position& pos,
                                   move_list&             move_list,
                                   const piece_type       pt) {
    constexpr color us         = Stm;
    constexpr color them       = ~us;
    const auto&     occupied   = pos.occupancies(us) | pos.occupancies(them);
    auto            our_pieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!our_pieces.empty()) {
        const auto from = static_cast<square>(our_pieces.pop_lsb());
        auto       possible_piece_moves =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & ~occupied;

        while (!possible_piece_moves.empty()) {
            const auto to = static_cast<square>(possible_piece_moves.pop_lsb());
            move_list.push(move(from, to, move::move_flag::quiet));
        }
    }
}

template <color Stm>
void generate_captures_by_piece_type(const board::position& pos,
                                     move_list&             move_list,
                                     const piece_type       pt) {
    constexpr color us         = Stm;
    constexpr color them       = ~us;
    const auto&     occupied   = pos.occupancies(us) | pos.occupancies(them);
    auto            our_pieces = pos.piece_type_bb(pt) & pos.occupancies(us);

    while (!our_pieces.empty()) {
        const auto from = static_cast<square>(our_pieces.pop_lsb());
        auto       possible_piece_captures =
            bb::attacks::get_attacks_by_piece_type(pt, from, occupied) & pos.occupancies(them);

        while (!possible_piece_captures.empty()) {
            const auto to = static_cast<square>(possible_piece_captures.pop_lsb());
            move_list.push(move(from, to, move::move_flag::capture));
        }
    }
}

template <color Stm>
void generate_castling_moves(const board::position& pos, move_list& move_list) {
    if constexpr (Stm == color::white) {
        if (pos.can_castle_king_side<color::white>())
            move_list.push(move(square::e1, square::g1, move::move_flag::castle));
        if (pos.can_castle_queen_side<color::white>())
            move_list.push(move(square::e1, square::c1, move::move_flag::castle));
    }
    else {
        if (pos.can_castle_king_side<color::black>())
            move_list.push(move(square::e8, square::g8, move::move_flag::castle));
        if (pos.can_castle_queen_side<color::black>())
            move_list.push(move(square::e8, square::c8, move::move_flag::castle));
    }
}

void generate_all_quiets(const board::position& pos, move_list& move_list) {
    if (pos.side_to_move() == color::white) {
        for (const piece_type pt : {piece_type::knight, piece_type::bishop, piece_type::rook,
                                    piece_type::queen, piece_type::king})
            generate_quiets_by_piece_type<color::white>(pos, move_list, pt);
    }
    else {
        for (const piece_type pt : {piece_type::knight, piece_type::bishop, piece_type::rook,
                                    piece_type::queen, piece_type::king})
            generate_quiets_by_piece_type<color::black>(pos, move_list, pt);
    }
}

void generate_all_captures(const board::position& pos, move_list& move_list) {
    if (pos.side_to_move() == color::white) {
        generate_pawn_captures<color::white>(pos, move_list);

        for (const piece_type pt : {piece_type::knight, piece_type::bishop, piece_type::rook,
                                    piece_type::queen, piece_type::king})
            generate_captures_by_piece_type<color::white>(pos, move_list, pt);
    }
    else {
        generate_pawn_captures<color::black>(pos, move_list);

        for (const piece_type pt : {piece_type::knight, piece_type::bishop, piece_type::rook,
                                    piece_type::queen, piece_type::king})
            generate_captures_by_piece_type<color::black>(pos, move_list, pt);
    }
}

void generate_all_moves(const board::position& pos, move_list& move_list) {
    const int num_checkers = pos.checkers().bit_count();

    if (num_checkers == 0) {
        generate_all_captures(pos, move_list);

        if (pos.side_to_move() == color::white) {
            generate_pawn_pushes<color::white>(pos, move_list);
            generate_castling_moves<color::white>(pos, move_list);
        }
        else {
            generate_pawn_pushes<color::black>(pos, move_list);
            generate_castling_moves<color::black>(pos, move_list);
        }

        generate_all_quiets(pos, move_list);
    }
    else if (num_checkers == 1) {
        generate_all_captures(pos, move_list);

        if (pos.side_to_move() == color::white)
            generate_pawn_pushes<color::white>(pos, move_list);
        else {
            generate_pawn_pushes<color::black>(pos, move_list);
        }

        generate_all_quiets(pos, move_list);
    }
    else {
        if (pos.side_to_move() == color::white) {
            generate_captures_by_piece_type<color::white>(pos, move_list, piece_type::king);
            generate_quiets_by_piece_type<color::white>(pos, move_list, piece_type::king);
        }
        else {
            generate_captures_by_piece_type<color::black>(pos, move_list, piece_type::king);
            generate_quiets_by_piece_type<color::black>(pos, move_list, piece_type::king);
        }
    }
}

} // namespace moves