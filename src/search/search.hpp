#pragma once

#include <format>
#include <vector>

#include "../board/position.hpp"
#include "../timeman.hpp"

namespace search {

struct pv_line {
        std::array<moves::move, constants::max_moves> moves{};
        usize                                         length{};

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

            for (usize i = 0; i < length; ++i)
                result += std::format(" {}", moves[i].to_string());

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
        void set_start_time(u64 time);
        void parse_time_control(const std::vector<std::string>& command, color stm);

        /// @brief Main entrypoint for the search function
        /// @param pos Position to search from
        void main_search(const board::position& pos);

    private:
        search_info   m_info{};
        search_limits m_limits{};
        time_manager  m_timer{};

        /// @brief Quiescence search, to get rid of the horizon effect
        /// @tparam pv_node Indicates if the current node is from the principal variation
        /// @param pos Position to search from
        /// @param alpha Best score for the maximizing player
        /// @param beta Best score for the minimizing player
        /// @param ply Internal depth of the search tree (seldepth)
        /// @returns The best score found
        /// @note See https://en.wikipedia.org/wiki/Quiescence_search for reference
        template <bool pv_node>
        score qsearch(const board::position& pos, score alpha, score beta, int ply);

        /// @brief Fail-soft negamax algorithm with alpha-beta pruning
        /// @tparam pv_node Indicates if the current node is from the principal variation
        /// @param pos Position to search from
        /// @param alpha Best score for the maximizing player
        /// @param beta Best score for the minimizing player
        /// @param depth Depth to start searching from
        /// @param ply Internal depth of the search tree (seldepth)
        /// @param pv PV-List on the stack, to keep track of the principal variation
        /// @returns The best score found
        /// @note See https://en.wikipedia.org/wiki/Negamax for reference
        template <bool pv_node>
        score negamax(
            const board::position& pos, score alpha, score beta, int depth, int ply, pv_line& pv);

        /// @brief Determines if search should stop according to the search limits
        /// @returns true if time is up or the nodes or time limit is exceeded
        [[nodiscard]] bool should_stop() const;

        /// @brief Reports uci-compliant info about the search tree
        /// @param elapsed Elapsed time since the start of the search, in milliseconds
        /// @param depth Depth of the search tree
        /// @param score Score of the current position
        /// @param pv Principal variation line from the current position
        void report_info(u64 elapsed, int depth, score score, const pv_line& pv) const;
};

} // namespace search