#pragma once

#include <format>
#include <vector>

#include "../board/position.hpp"
#include "../timeman.hpp"

namespace search {

struct PVLine {
        std::array<moves::Move, MAX_MOVES> moves{};
        u32                                length{};

        [[nodiscard]] auto begin() const { return moves.begin(); }
        [[nodiscard]] auto end() const { return moves.begin() + length; }

        void update(const moves::Move move, const PVLine& childPV) {
            moves[0] = move;
            std::ranges::copy(childPV.begin(), childPV.end(), moves.begin() + 1);
            length = childPV.length + 1;
        }

        void clear() {
            moves.fill(moves::Move::none());
            length = 0;
        }

        [[nodiscard]] moves::Move best_move() const { return moves[0]; }

        [[nodiscard]] std::string to_string() const {
            std::string result;

            for (auto it = begin(); it < end(); ++it) {
                if (it + 1 == end())
                    result += std::format("{}", it->to_string());
                else
                    result += std::format("{} ", it->to_string());
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
        [[nodiscard]] u64 searched_nodes() const { return m_info.searchedNodes; }

        void reset_info();
        void set_limits(u64 nodesLimit, u64 timeLimit, u32 depthLimit);
        void parse_time_control(const std::vector<std::string>& command, Color stm);
        void main_search(const board::Position& pos);

    private:
        SearchInfo   m_info{};
        SearchLimits m_limits{};
        TimeManager  m_timer{};

        Score qsearch(const board::Position& pos, Score alpha, Score beta, int ply);
        Score negamax(
            const board::Position& pos, Score alpha, Score beta, int depth, int ply, PVLine& pv);

        [[nodiscard]] bool should_stop() const;

        void report_info(u64 elapsed, int depth, Score score, const PVLine& pv) const;
};

} // namespace search