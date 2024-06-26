#include "search.hpp"

#include <format>
#include <iostream>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../utils/time.hpp"

namespace search {

void searcher::reset_info() {
    m_info.stopped        = false;
    m_info.searched_nodes = 0ULL;
    m_info.pv.clear();
}

void searcher::set_limits(const u64 nodes_limit, const u64 time_limit, const u32 depth_limit) {
    m_limits.nodes_limit = nodes_limit;
    m_limits.time_limit  = time_limit;
    m_limits.depth_limit = depth_limit;
}

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

/// @brief Main entrypoint for the search function
void searcher::main_search(const board::position& pos) {
    m_timer.set_start_time(utils::time::get_time_ms());
    reset_info();
    auto best_move = moves::move::null();

    // Iterative deepening loop
    for (int current_depth = 1; current_depth <= m_limits.depth_limit; ++current_depth) {
        const score best_score =
            negamax(pos, -score_infinite, score_infinite, current_depth, 0, m_info.pv);

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

/// @brief Quiescence search, to get rid of the horizon effect
score searcher::qsearch(const board::position& pos, score alpha, const score beta, const int ply) {
    ++m_info.searched_nodes;

    if (m_info.stopped)
        return 0;

    if (should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    const score static_eval = eval::evaluate(pos);

    if (ply >= constants::max_ply)
        return static_eval;

    if (static_eval >= beta)
        return static_eval;

    if (static_eval > alpha)
        alpha = static_eval;

    score            best_score = static_eval;
    moves::move_list move_list;
    generate_all_captures(pos, move_list);
    move_list.score_moves(pos);
    move_list.sort();

    for (usize i = 0; i < move_list.size(); i++) {
        const auto current_move = move_list.move_at(i);

        board::position copy = pos;
        copy.make_move<true>(current_move);

        if (!copy.was_legal())
            continue;

        const score current_score = -qsearch(copy, -beta, -alpha, ply + 1);

        if (m_info.stopped)
            return 0;

        if (current_score > best_score)
            best_score = current_score;

        if (current_score > alpha)
            alpha = current_score;

        if (alpha >= beta)
            break;
    }
    return best_score;
}

/// @brief Fail-soft negamax algorithm with alpha-beta pruning
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
        return qsearch(pos, alpha, beta, ply);

    if (ply > 0 && pos.has_repeated())
        return 0;

    if (ply >= constants::max_ply)
        return eval::evaluate(pos);

    u16              legal_moves{};
    pv_line          child_pv{};
    score            best_score = -score_infinite;
    moves::move_list move_list;
    generate_all_moves(pos, move_list);
    move_list.score_moves(pos);
    move_list.sort();

    for (usize i = 0; i < move_list.size(); ++i) {
        const auto current_move = move_list.move_at(i);

        board::position copy = pos;
        copy.make_move<true>(current_move);

        if (!copy.was_legal())
            continue;

        ++legal_moves;

        const score current_score = -negamax(copy, -beta, -alpha, depth - 1, ply + 1, child_pv);

        if (current_score > best_score)
            best_score = current_score;

        if (current_score > alpha) {
            alpha = current_score;
            pv.update(current_move, child_pv);
        }

        // Double-check if search stopped to make sure we don't exceed the search limits
        if (m_info.stopped)
            return 0;

        if (alpha >= beta)
            break;
    }

    // Checkmate / stalemate detection
    if (!legal_moves) {
        return pos.checkers().bit_count() > 0 ? -score_mate + ply : 0;
    }

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
    std::cout << std::format("info depth {} score cp {} time {} nodes {} nps {} pv{}", depth,
                             score, elapsed, m_info.searched_nodes,
                             m_info.searched_nodes / std::max<u64>(1, elapsed) * 1000,
                             pv.to_string())
              << std::endl;
}

} // namespace search
