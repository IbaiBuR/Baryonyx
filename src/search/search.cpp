#include "search.hpp"

#include <format>
#include <iostream>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../utils/time.hpp"
#include "tt.hpp"

namespace search {

constexpr int rfp_depth_limit = 6;
constexpr int rfp_margin      = 70;

constexpr int nmp_base_reduction = 3;

void searcher::reset() {
    m_info.stopped        = false;
    m_info.searched_nodes = 0ULL;

    m_info.pv.clear();
    m_data.clear_killers();
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

    for (auto it = command.begin() + 1; it < command.end(); ++it) {
        if (stm == color::white) {
            if (*it == "wtime") {
                base_time = std::stoull(*(it + 1));
                set_limits(UINT64_MAX, base_time, constants::max_depth);
            }
            if (*it == "winc")
                increment = std::stoi(*(it + 1));
        }
        else {
            if (*it == "btime") {
                base_time = std::stoull(*(it + 1));
                set_limits(UINT64_MAX, base_time, constants::max_depth);
            }
            if (*it == "binc")
                increment = std::stoi(*(it + 1));
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
    pv.length = 0;

    if (m_info.stopped)
        return 0;

    if (should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    if (depth <= 0)
        return qsearch<pv_node>(pos, alpha, beta, ply);

    const bool root_node = ply == 0;

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
        if (depth <= rfp_depth_limit && static_eval - rfp_margin * depth >= beta)
            return static_eval;

        // Null Move Pruning: If after making a null move (forfeiting the side to move) we still
        // have a strong enough position to produce a cutoff, we cut the search returning the null
        // move score from a shallower search

        if (!pos.last_move_was_null() && !pos.has_no_pawns(pos.side_to_move())
            && static_eval >= beta) {
            const int r = nmp_base_reduction + depth / nmp_base_reduction;

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

        if (legal_moves == 1)
            // Search the first move with a full window
            current_score = -negamax<pv_node>(copy, -beta, -alpha, depth - 1, ply + 1, child_pv);
        else {
            // Do a null window search to see if we find a better move
            current_score = -negamax<false>(copy, -alpha - 1, -alpha, depth - 1, ply + 1, child_pv);

            if (current_score > alpha && pv_node)
                // If we found a better move, do a full re-search
                current_score = -negamax<true>(copy, -beta, -alpha, depth - 1, ply + 1, child_pv);
        }

        if (current_score > best_score) {
            best_score = current_score;

            if (current_score > alpha) {
                alpha     = current_score;
                best_move = current_move;

                if constexpr (pv_node)
                    pv.update(current_move, child_pv);

                if (alpha >= beta) {
                    if (best_move.is_quiet())
                        m_data.update_killers(best_move, ply);

                    break;
                }
            }
        }

        // Double-check if search stopped to make sure we don't exceed the search limits
        if (m_info.stopped)
            return 0;
    }

    // Checkmate / stalemate detection
    if (!legal_moves) {
        return in_check ? -constants::score_mate + ply : 0;
    }

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

void searcher::report_info(u64 elapsed, int depth, score score, const pv_line& pv) const {
    std::cout << std::format("info depth {} score cp {} time {} nodes {} nps {} hashfull {} pv{}",
                             depth, score, elapsed, m_info.searched_nodes,
                             m_info.searched_nodes / std::max<u64>(1, elapsed) * 1000,
                             tt::global_tt.hashfull(), pv.to_string())
              << std::endl;
}

} // namespace search
