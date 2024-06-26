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

void move_list::score_moves(const board::position& pos) {
    for (usize i = 0; i < m_size; ++i) {
        const auto current_move = move_at(i);

        if (current_move.is_capture()) {
            const auto attacker_piece_type =
                board::pieces::piece_to_piece_type(pos.piece_on(current_move.from()));
            const auto victim_piece_type =
                board::pieces::piece_to_piece_type(pos.piece_on(current_move.to()));

            m_moves[i].move_score = mvv_lva[std::to_underlying(attacker_piece_type)]
                                           [std::to_underlying(victim_piece_type)];
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