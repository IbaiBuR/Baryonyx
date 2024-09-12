#pragma once

#include <algorithm>
#include <array>
#include <cassert>

#include "move.hpp"

#include "../board/piece.hpp"
#include "../board/position.hpp"
#include "../search/search.hpp"

namespace moves {

struct scored_move {
        score move_score;
        move  move_value;
};

class move_list {
    public:
        [[nodiscard]] auto begin() { return m_moves.begin(); }
        [[nodiscard]] auto end() { return m_moves.begin() + m_size; }

        [[nodiscard]] auto begin() const { return m_moves.begin(); }
        [[nodiscard]] auto end() const { return m_moves.begin() + m_size; }

        void push(const move move) {
            assert(m_size + 1 < constants::max_moves);
            m_moves[m_size++].move_value = move;
        }

        void clear() { m_size = 0; }

        void score_moves(move                       tt_move,
                         const board::position&     pos,
                         const search::search_data& search_data,
                         int                        ply);

        void sort() {
            std::stable_sort(begin(), end(), [](const scored_move a, const scored_move b) {
                return std::greater()(a.move_score, b.move_score);
            });
        }

        [[nodiscard]] usize size() const { return m_size; }

        [[nodiscard]] move move_at(const usize index) const {
            assert(index < constants::max_moves);
            return m_moves[index].move_value;
        }

        [[nodiscard]] score score_at(const usize index) const {
            assert(index < constants::max_moves);
            return m_moves[index].move_score;
        }

    private:
        std::array<scored_move, constants::max_moves> m_moves{};
        usize                                         m_size{};
};

void print_move_list(const move_list& move_list);

} // namespace moves