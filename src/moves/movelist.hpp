#pragma once

#include <array>
#include <cassert>

#include "move.hpp"

namespace Moves {

constexpr int maxMoves = 256;

struct ScoredMove {
        Score score;
        Move  move;
};

class MoveList {
    public:
        void push(const Move move) {
            assert(count + 1 < maxMoves);
            moves[count++].move = move;
        }

        void clear() { count = 0; }

        [[nodiscard]] u32 size() const { return count; }

        [[nodiscard]] Move moveAt(const u32 index) const {
            assert(index < maxMoves);
            return moves[index].move;
        }

        [[nodiscard]] Score scoreAt(const u32 index) const {
            assert(index < maxMoves);
            return moves[index].score;
        }

    private:
        std::array<ScoredMove, maxMoves> moves{};
        u32                              count{};
};

void printMoveList(const MoveList &moveList);

} // namespace Moves