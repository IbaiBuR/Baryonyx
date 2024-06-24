#pragma once

#include <array>
#include <cassert>

#include "move.hpp"

namespace moves {

struct scored_move {
        score move_score;
        move  move_value;
};

class move_list {
    public:
        void push(const move move) {
            assert(m_count + 1 < constants::max_moves);
            m_moves[m_count++].move_value = move;
        }

        void clear() { m_count = 0; }

        [[nodiscard]] u32 size() const { return m_count; }

        [[nodiscard]] move move_at(const u32 index) const {
            assert(index < constants::max_moves);
            return m_moves[index].move_value;
        }

        [[nodiscard]] score score_at(const u32 index) const {
            assert(index < constants::max_moves);
            return m_moves[index].move_score;
        }

    private:
        std::array<scored_move, constants::max_moves> m_moves{};
        u32                                           m_count{};
};

void print_move_list(const move_list& move_list);

} // namespace moves