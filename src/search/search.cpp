#include "search.hpp"

#include <cmath>
#include <format>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "tt.hpp"

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../utils/mdarray.hpp"
#include "../utils/parsing.hpp"
#include "../utils/time.hpp"
#include "../utils/score.hpp"

namespace search {

namespace heuristics {

constexpr int rfp_depth_limit = 6;
constexpr int rfp_margin      = 70;

constexpr int nmp_base_reduction = 3;

constexpr auto lmr_quiet_factor  = 1.00;
constexpr auto lmr_quiet_divisor = 2.00;

static const auto lmr_table = [] {
    utils::mdarray<int, constants::max_depth, constants::max_moves> lmr_table;

    for (int i = 1; i < constants::max_depth; ++i) {
        for (int j = 1; j < constants::max_moves; ++j) {
            lmr_table[i, j] =
                static_cast<int>(lmr_quiet_factor + std::log(i) * std::log(j) / lmr_quiet_divisor);
        }
    }

    return lmr_table;
}();

constexpr int lmr_min_depth      = 2;
constexpr int lmr_move_threshold = 3;

} // namespace heuristics

void searcher::reset() {
    m_info.stopped        = false;
    m_info.searched_nodes = 0ULL;

    m_info.pv.clear();
    m_data.clear();
}

void searcher::set_limits(const u64 nodes_limit, const u64 time_limit, const u32 depth_limit) {
    m_limits.nodes_limit = nodes_limit;
    m_limits.time_limit  = time_limit;
    m_limits.depth_limit = depth_limit;
}

void searcher::set_start_time(const u64 time) { m_timer.set_start_time(time); }

void searcher::parse_time_control(const std::vector<std::string>& command, const color stm) {
    u64 base_time{};
    u16 increment{};

    auto set_base_time_and_limits = [&](const std::optional<u64> parsed_base_time) -> void {
        if (parsed_base_time) {
            base_time = parsed_base_time.value();
            set_limits(std::numeric_limits<u64>::max(), base_time, constants::max_depth);
        }
        else {
            throw std::runtime_error("Failed to parse base time.\n");
        }
    };

    auto set_increment = [&](const std::optional<u16> parsed_increment) -> void {
        if (parsed_increment)
            increment = parsed_increment.value();
        else
            throw std::runtime_error("Failed to parse increment.\n");
    };

    for (auto it = command.begin() + 1; it < command.end(); ++it) {
        if (stm == color::white) {
            if (*it == "wtime")
                set_base_time_and_limits(utils::parsing::to_number<u64>(*(it + 1)));

            if (*it == "winc")
                set_increment(utils::parsing::to_number<u16>(*(it + 1)));
        }
        else {
            if (*it == "btime")
                set_base_time_and_limits(utils::parsing::to_number<u64>(*(it + 1)));

            if (*it == "binc")
                set_increment(utils::parsing::to_number<u16>(*(it + 1)));
        }
    }
    m_timer = time_manager(utils::time::get_time_ms(), base_time, increment);
}

void searcher::main_search(const board::position& pos) {
    reset();
    auto best_move = moves::move::null();

    // Iterative deepening loop
    for (int current_depth = 1; current_depth <= m_limits.depth_limit; ++current_depth) {
        const score best_score =
            negamax<true>(pos, -constants::score_infinite, constants::score_infinite, current_depth,
                          0, m_info.pv);

        if (m_info.stopped) {
            // If search stopped too early and we don't have a best move, we update it in order to
            // avoid sending illegal moves to GUI
            if (best_move == moves::move::null())
                best_move = m_info.pv.best_move();

            break;
        }

        // Ensure we only update the best move if search was not cancelled. Otherwise, our best
        // move may be terrible
        best_move = m_info.pv.best_move();
        report_info(m_timer.elapsed(), current_depth, best_score, m_info.pv);
    }
    std::cout << std::format("bestmove {}", best_move.to_string()) << std::endl;
}

template <bool pv_node>
score searcher::qsearch(const board::position& pos, score alpha, const score beta, const int ply) {
    ++m_info.searched_nodes;

    if (m_info.stopped)
        return 0;

    if (should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    tt::tt_entry entry;

    // Probe the tranposition table and retrieve information from previous searches if possible
    const bool tt_hit   = tt::global_tt.probe(pos.key(), entry);
    const auto tt_score = tt_hit ? tt::score_from_tt(entry.value(), ply) : constants::score_none;
    const auto tt_move  = tt_hit ? entry.move() : moves::move::null();

    // TT cutoff: If we are not in a pv-node and we get a tt hit with a
    // usable score, cut the search returning the score from the tt
    if (!pv_node && tt_score != constants::score_none && entry.can_use_score(alpha, beta))
        return tt_score;

    const score static_eval = eval::evaluate(pos);

    if (ply >= constants::max_ply)
        return static_eval;

    if (static_eval >= beta)
        return static_eval;

    if (static_eval > alpha)
        alpha = static_eval;

    score best_score = static_eval;
    auto  best_move  = moves::move::null();

    moves::move_list move_list;
    generate_all_captures(pos, move_list);
    move_list.score_moves(tt_move, pos, m_data, ply);
    move_list.sort();

    for (usize i = 0; i < move_list.size(); i++) {
        const auto current_move = move_list.move_at(i);

        auto copy = pos;
        copy.make_move<true>(current_move);

        if (!copy.was_legal())
            continue;

        const score current_score = -qsearch<pv_node>(copy, -beta, -alpha, ply + 1);

        if (current_score > best_score) {
            best_score = current_score;

            if (current_score > alpha) {
                alpha     = current_score;
                best_move = current_move;

                if (alpha >= beta)
                    break;
            }
        }

        if (m_info.stopped)
            return 0;
    }

    const auto tt_flag = best_score >= beta ? tt::tt_entry::tt_flag::lower_bound
                                            : tt::tt_entry::tt_flag::upper_bound;

    tt::global_tt.store(pos.key(), tt::tt_entry(pos.key(), best_move,
                                                tt::score_to_tt(best_score, ply), 0, tt_flag));

    return best_score;
}

template <bool pv_node>
score searcher::negamax(const board::position& pos,
                        score                  alpha,
                        const score            beta,
                        const int              depth,
                        const int              ply,
                        pv_line&               pv) {
    ++m_info.searched_nodes;

    if (m_info.stopped)
        return 0;

    const bool root_node = ply == 0;

    if (!root_node && should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    if (depth <= 0)
        return qsearch<pv_node>(pos, alpha, beta, ply);

    if (!root_node && pos.has_repeated())
        return 0;

    tt::tt_entry entry;

    // Probe the tranposition table and retrieve information from previous searches if possible
    const bool tt_hit   = tt::global_tt.probe(pos.key(), entry);
    const auto tt_score = tt_hit ? tt::score_from_tt(entry.value(), ply) : constants::score_none;
    const auto tt_move  = tt_hit ? entry.move() : moves::move::null();
    const u8   tt_depth = entry.depth();

    // TT cutoff: If we are not in a pv-node and we get a tt hit with a high enough depth and a
    // usable score, cut the search returning the score from the tt
    if (!pv_node && tt_score != constants::score_none && tt_depth >= depth
        && entry.can_use_score(alpha, beta))
        return tt_score;

    pv_line child_pv;

    const score static_eval = eval::evaluate(pos);
    const bool  in_check    = pos.checkers().bit_count() > 0;

    if (!in_check && !pv_node) {
        // Reverse Futility Pruning
        if (depth <= heuristics::rfp_depth_limit
            && static_eval - heuristics::rfp_margin * depth >= beta)
            return static_eval;

        // Null Move Pruning: If after making a null move (forfeiting the side to move) we still
        // have a strong enough position to produce a cutoff, we cut the search returning the null
        // move score from a shallower search
        if (!pos.last_move_was_null() && !pos.has_no_pawns(pos.side_to_move())
            && static_eval >= beta) {
            const int r = heuristics::nmp_base_reduction + depth / heuristics::nmp_base_reduction;

            auto copy = pos;
            copy.make_null_move();

            const score null_move_score =
                -negamax<false>(copy, -beta, -beta + 1, depth - r, ply + 1, child_pv);

            if (null_move_score >= beta)
                return null_move_score;
        }
    }

    if (ply >= constants::max_ply)
        return static_eval;

    u16         legal_moves{};
    score       best_score     = -constants::score_infinite;
    auto        best_move      = moves::move::null();
    const score original_alpha = alpha;

    moves::move_list move_list;
    generate_all_moves(pos, move_list);
    move_list.score_moves(tt_move, pos, m_data, ply);
    move_list.sort();

    for (usize i = 0; i < move_list.size(); ++i) {
        const auto current_move = move_list.move_at(i);

        auto copy = pos;
        copy.make_move<true>(current_move);

        if (!copy.was_legal())
            continue;

        ++legal_moves;

        if constexpr (pv_node)
            child_pv.length = 0;

        score current_score;

        // Search the first move with a full window
        if (legal_moves == 1)
            current_score = -negamax<pv_node>(copy, -beta, -alpha, depth - 1, ply + 1, child_pv);
        else {
            // Apply LMR: Search moves that are late in move ordering with reduced depth
            const auto reduction = depth > heuristics::lmr_min_depth
                                        && legal_moves > heuristics::lmr_move_threshold
                                        && current_move.is_quiet()
                                     ? heuristics::lmr_table[depth, legal_moves]
                                     : 0;

            // Ensure the reduced depth is not negative
            const auto new_depth     = depth - 1;
            const auto reduced_depth = std::clamp(new_depth - reduction, 0, new_depth);

            // Perform a null window search at reduced depth
            current_score =
                -negamax<false>(copy, -alpha - 1, -alpha, reduced_depth, ply + 1, child_pv);

            // Full depth search
            if (current_score > alpha && reduced_depth < new_depth)
                current_score =
                    -negamax<false>(copy, -alpha - 1, -alpha, new_depth, ply + 1, child_pv);

            // If we found a better move, do a full window search
            if (current_score > alpha && pv_node)
                current_score = -negamax<true>(copy, -beta, -alpha, new_depth, ply + 1, child_pv);
        }

        if (current_score > best_score) {
            best_score = current_score;

            if (current_score > alpha) {
                alpha     = current_score;
                best_move = current_move;

                if constexpr (pv_node)
                    pv.update(current_move, child_pv);

                if (alpha >= beta) {
                    if (best_move.is_quiet()) {
                        m_data.update_killers(best_move, ply);
                        m_data.update_quiet_history(best_move, depth);
                    }

                    break;
                }
            }
        }

        // Double-check if search stopped to make sure we don't exceed the search limits
        if (m_info.stopped)
            return 0;
    }

    // Checkmate / stalemate detection
    if (!legal_moves)
        return in_check ? -constants::score_mate + ply : 0;

    const auto tt_flag = best_score <= original_alpha ? tt::tt_entry::tt_flag::upper_bound
                       : best_score >= beta           ? tt::tt_entry::tt_flag::lower_bound
                                                      : tt::tt_entry::tt_flag::exact;

    tt::global_tt.store(pos.key(), tt::tt_entry(pos.key(), best_move,
                                                tt::score_to_tt(best_score, ply), depth, tt_flag));

    return best_score;
}

bool searcher::should_stop() const {
    if (m_info.searched_nodes % 1024 != 0)
        return false;

    const u64 elapsed = m_timer.elapsed();

    return m_info.searched_nodes >= m_limits.nodes_limit || elapsed >= m_limits.time_limit
        || elapsed >= m_timer.optimum_time();
}

void searcher::report_info(const u64      elapsed,
                           const int      depth,
                           const score    score,
                           const pv_line& pv) const {
    std::cout << std::format("info depth {} score {} time {} nodes {} nps {} hashfull {} pv{}",
                             depth, utils::score::to_string(score), elapsed, m_info.searched_nodes,
                             m_info.searched_nodes / std::max<u64>(1, elapsed) * 1000,
                             tt::global_tt.hashfull(), pv.to_string())
              << std::endl;
}

} // namespace search
