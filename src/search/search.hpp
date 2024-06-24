#pragma once

#include <format>
#include <vector>

#include "../board/position.hpp"
#include "../timeman.hpp"

namespace search {

constexpr score score_infinite = 32000;
constexpr score score_mate     = 31500;

struct pv_line {
        std::array<moves::move, constants::max_moves> moves{};
        u32                                           length{};

        [[nodiscard]] auto begin() const { return moves.begin(); }
        [[nodiscard]] auto end() const { return moves.begin() + length; }

        void update(const moves::move move, const pv_line& child_pv) {
            moves[0] = move;
            std::ranges::copy(child_pv.begin(), child_pv.end(), moves.begin() + 1);
            length = child_pv.length + 1;
        }

        void clear() {
            moves.fill(moves::move::null());
            length = 0;
        }

        [[nodiscard]] moves::move best_move() const { return moves[0]; }

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

struct search_limits {
        u64 nodes_limit;
        u64 time_limit;
        u8  depth_limit;
};

struct search_info {
        u64     searched_nodes;
        pv_line pv;
        bool    stopped;
};

class searcher {
    public:
        [[nodiscard]] u64 searched_nodes() const { return m_info.searched_nodes; }

        void reset_info();
        void set_limits(u64 nodes_limit, u64 time_limit, u32 depth_limit);
        void parse_time_control(const std::vector<std::string>& command, color stm);
        void main_search(const board::position& pos);

    private:
        search_info   m_info{};
        search_limits m_limits{};
        time_manager  m_timer{};

        score qsearch(const board::position& pos, score alpha, score beta, int ply);
        score negamax(
            const board::position& pos, score alpha, score beta, int depth, int ply, pv_line& pv);

        [[nodiscard]] bool should_stop() const;

        void report_info(u64 elapsed, int depth, score score, const pv_line& pv) const;
};

} // namespace search