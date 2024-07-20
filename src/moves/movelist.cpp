#include "movelist.hpp"

#include <format>
#include <iostream>

namespace moves {

// clang-format off
/// @brief MVV-LVA (Most Valuable Victim - Least Valuable Attacker) table, used for move
/// ordering and indexed by [attacker][victim]
constexpr std::array<std::array<score, constants::num_piece_types>, constants::num_piece_types> mvv_lva = {
    {
        {105, 205, 305, 405, 505, 0}, // attacker -> PAWN
        {104, 204, 303, 404, 504, 0}, // attacker -> KNIGHT
        {103, 203, 303, 403, 503, 0}, // attacker -> BISHOP
        {102, 202, 302, 402, 502, 0}, // attacker -> ROOK
        {101, 201, 301, 401, 501, 0}, // attacker -> QUEEN
        {100, 200, 300, 400, 500, 0}  // attacker -> KING
    }
};
// clang-format on

constexpr int mvv_lva_base_bonus      = 1000;
constexpr int killer_moves_base_bonus = mvv_lva_base_bonus - 1;

void move_list::score_moves(const move                 tt_move,
                            const board::position&     pos,
                            const search::search_data& search_data,
                            const int                  ply) {
    for (usize i = 0; i < m_size; ++i) {
        const auto current_move = move_at(i);

        if (tt_move != move::null() && current_move == tt_move) {
            m_moves[i].move_score = constants::score_infinite;
            continue;
        }

        if (current_move.is_capture()) {
            const auto attacker_piece_type =
                board::pieces::piece_to_piece_type(pos.piece_on(current_move.from()));
            const auto victim_piece_type =
                board::pieces::piece_to_piece_type(pos.piece_on(current_move.to()));

            m_moves[i].move_score = mvv_lva[std::to_underlying(attacker_piece_type)]
                                           [std::to_underlying(victim_piece_type)]
                                  + mvv_lva_base_bonus;
        }
        else {
            if (current_move == search_data.first_killer(ply))
                m_moves[i].move_score = killer_moves_base_bonus;
            else if (current_move == search_data.second_killer(ply))
                m_moves[i].move_score = killer_moves_base_bonus - 1;
            else
                m_moves[i].move_score = search_data.quiet_history_value(current_move);
        }
    }
}

void print_move_list(const move_list& move_list) {
    for (usize i = 0; i < move_list.size(); ++i) {
        std::cout << std::format("{0:3}. Move: {1:<5} Score {2:<5}", i + 1,
                                 move_list.move_at(i).to_string(), move_list.score_at(i))
                  << std::endl;
    }
}

} // namespace moves