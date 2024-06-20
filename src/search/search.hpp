#pragma once

#include <format>
#include <vector>

#include "../board/position.hpp"
#include "../timeman.hpp"

namespace Search {

struct PVLine {
        std::array<Moves::Move, MAX_MOVES> moves{};
        u32                                length{};

        [[nodiscard]] auto begin() const { return moves.begin(); }
        [[nodiscard]] auto end() const { return moves.begin() + length; }

        void update(const Moves::Move move, const PVLine &childPV) {
            moves[0] = move;
            std::ranges::copy(childPV.begin(), childPV.end(), moves.begin() + 1);
            length = childPV.length + 1;
        }

        void clear() {
            moves.fill(Moves::Move::none());
            length = 0;
        }

        [[nodiscard]] Moves::Move bestMove() const { return moves[0]; }

        [[nodiscard]] std::string toString() const {
            std::string result;

            for (auto it = begin(); it < end(); ++it) {
                if (it + 1 == end())
                    result += std::format("{}", it->toString());
                else
                    result += std::format("{} ", it->toString());
            }

            return result;
        }
};

struct SearchLimits {
        u64 nodesLimit;
        u64 timeLimit;
        u8  depthLimit;
};

struct SearchInfo {
        u64    searchedNodes{};
        PVLine pv;
        bool   stopped{};
};

class Searcher {
    public:
        [[nodiscard]] u64 searchedNodes() const { return m_info.searchedNodes; }

        void resetInfo();
        void setLimits(u64 nodesLimit, u64 timeLimit, u32 depthLimit);
        void parseTimeControl(const std::vector<std::string> &command, Color stm);
        void mainSearch(const Board::Position &pos);

    private:
        SearchInfo   m_info{};
        SearchLimits m_limits{};
        TimeManager  m_timer{};

        Score qsearch(const Board::Position &pos, Score alpha, Score beta, int ply);
        Score negamax(
            const Board::Position &pos, Score alpha, Score beta, int depth, int ply, PVLine &pv);

        [[nodiscard]] bool shouldStop() const;

        void reportInfo(u64 elapsed, int depth, Score score, const PVLine &pv) const;
};

} // namespace Search