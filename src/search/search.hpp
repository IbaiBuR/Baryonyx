#pragma once

#include <algorithm>
#include <cstring>
#include <format>
#include <vector>

#include "../timeman.hpp"

#include "../board/position.hpp"
#include "../utils/mdarray.hpp"

namespace search {

namespace move_ordering {

inline constexpr score mvv_lva_base_bonus      = 10000;
inline constexpr score killer_moves_base_bonus = mvv_lva_base_bonus - 1;
inline constexpr score max_history             = killer_moves_base_bonus - 2;

} // namespace move_ordering

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

struct search_data {
        utils::mdarray<moves::move, 2, constants::max_ply>                  killer_moves;
        utils::mdarray<u16, constants::num_squares, constants::num_squares> quiet_history;

        search_data() { clear(); }

        void clear() {
            clear_killers();
            clear_quiet_history();
        }

        void clear_killers() { killer_moves = {}; }

        void clear_quiet_history() { quiet_history = {}; }

        void update_killers(const moves::move move, const int ply) {
            if (move != killer_moves[0, ply]) {
                killer_moves[1, ply] = killer_moves[0, ply];
                killer_moves[0, ply] = move;
            }
        }

        void update_quiet_history(const moves::move move, const int depth) {
            const auto from          = move.from();
            const auto to            = move.to();
            const auto history_value = quiet_history_value(move);

            const int history_bonus =
                std::min(history_value + depth * depth, move_ordering::max_history);

            quiet_history[std::to_underlying(from), std::to_underlying(to)] = history_bonus;
        }

        [[nodiscard]] auto first_killer(const int ply) const { return killer_moves[0, ply]; }
        [[nodiscard]] auto second_killer(const int ply) const { return killer_moves[1, ply]; }

        [[nodiscard]] u16 quiet_history_value(const moves::move m) const {
            return quiet_history[std::to_underlying(m.from()), std::to_underlying(m.to())];
        }
};

class searcher {
    public:
        [[nodiscard]] u64 searched_nodes() const { return m_info.searched_nodes; }

        void reset();
        void set_limits(u64 nodes_limit, u64 time_limit, u32 depth_limit);
        void set_start_time(u64 time);
        void parse_time_control(const std::vector<std::string>& command, color stm);

        /// @brief Main entrypoint for the search function
        /// @param pos Position to search from
        void main_search(const board::position& pos);

    private:
        search_data   m_data;
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