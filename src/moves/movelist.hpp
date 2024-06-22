#pragma once

#include <array>
#include <cassert>

#include "move.hpp"

namespace moves {

struct ScoredMove {
        Score score;
        Move  move;
};

class MoveList {
    public:
        void push(const Move move) {
            assert(m_count + 1 < MAX_MOVES);
            m_moves[m_count++].move = move;
        }

        void clear() { m_count = 0; }

        [[nodiscard]] u32 size() const { return m_count; }

        [[nodiscard]] Move move_at(const u32 index) const {
            assert(index < MAX_MOVES);
            return m_moves[index].move;
        }

        [[nodiscard]] Score score_at(const u32 index) const {
            assert(index < MAX_MOVES);
            return m_moves[index].score;
        }

    private:
        std::array<ScoredMove, MAX_MOVES> m_moves{};
        u32                               m_count{};
};

void printMoveList(const MoveList &moveList);

} // namespace Moves