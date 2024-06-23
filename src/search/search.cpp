#include "search.hpp"

#include <format>
#include <iostream>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../utils/time.hpp"

namespace search {

void Searcher::reset_info() {
    m_info.stopped       = false;
    m_info.searchedNodes = 0ULL;
    m_info.pv.clear();
}

void Searcher::set_limits(const u64 nodesLimit, const u64 timeLimit, const u32 depthLimit) {
    m_limits.nodesLimit = nodesLimit;
    m_limits.timeLimit  = timeLimit;
    m_limits.depthLimit = depthLimit;
}

void Searcher::parse_time_control(const std::vector<std::string>& command, const Color stm) {
    u64 baseTime{};
    u16 increment{};

    for (auto it = command.begin() + 1; it < command.end(); ++it) {
        if (stm == Color::WHITE) {
            if (*it == "wtime") {
                baseTime = std::stoull(*(it + 1));
                set_limits(UINT64_MAX, baseTime, constants::max_depth);
            }
            if (*it == "winc")
                increment = std::stoi(*(it + 1));
        }
        else {
            if (*it == "btime") {
                baseTime = std::stoull(*(it + 1));
                set_limits(UINT64_MAX, baseTime, constants::max_depth);
            }
            if (*it == "binc")
                increment = std::stoi(*(it + 1));
        }
    }
    m_timer = TimeManager(utils::time::get_time_ms(), baseTime, increment);
}

/// @brief Main entrypoint for the search function
void Searcher::main_search(const board::Position& pos) {
    m_timer.set_start_time(utils::time::get_time_ms());
    reset_info();
    auto bestMove = moves::Move::none();

    // Iterative deepening loop
    for (int currentDepth = 1; currentDepth <= m_limits.depthLimit; ++currentDepth) {
        const Score bestScore =
            negamax(pos, -score_infinite, score_infinite, currentDepth, 0, m_info.pv);

        if (m_info.stopped) {
            // If search stopped and we don't have a bestMove, we update it in order to avoid
            // sending illegal moves to GUI
            if (bestMove == moves::Move::none())
                bestMove = m_info.pv.best_move();
            break;
        }

        // Ensure we only update the best move if search was not cancelled. Otherwise, our best
        // move may be terrible
        bestMove = m_info.pv.best_move();

        report_info(m_timer.elapsed(), currentDepth, bestScore, m_info.pv);
    }
    std::cout << std::format("bestmove {}", bestMove.to_string()) << std::endl;
}

/// @brief Quiescence search, to get rid of the horizon effect
Score Searcher::qsearch(const board::Position& pos, Score alpha, const Score beta, const int ply) {
    ++m_info.searchedNodes;

    if (m_info.stopped)
        return 0;

    if (should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    const Score staticEval = eval::evaluate(pos);

    if (ply >= constants::max_ply)
        return staticEval;

    if (staticEval >= beta)
        return staticEval;

    if (staticEval > alpha)
        alpha = staticEval;

    Score           bestScore = staticEval;
    moves::MoveList moveList;
    generate_all_captures(pos, moveList);

    for (u32 i = 0; i < moveList.size(); i++) {
        const auto currentMove = moveList.move_at(i);

        board::Position copy = pos;
        copy.make_move(currentMove);

        if (!copy.was_legal())
            continue;

        const Score score = -qsearch(copy, -beta, -alpha, ply + 1);

        if (m_info.stopped)
            return 0;

        if (score > bestScore)
            bestScore = score;

        if (score > alpha)
            alpha = score;

        if (alpha >= beta)
            break;
    }
    return bestScore;
}

/// @brief Fail-soft negamax algorithm with alpha-beta pruning
Score Searcher::negamax(const board::Position& pos,
                        Score                  alpha,
                        const Score            beta,
                        const int              depth,
                        const int              ply,
                        PVLine&                pv) {
    ++m_info.searchedNodes;
    pv.length = 0;

    if (m_info.stopped)
        return 0;

    if (should_stop()) {
        m_info.stopped = true;
        return 0;
    }

    if (depth <= 0)
        return qsearch(pos, alpha, beta, ply);

    if (ply >= constants::max_ply)
        return eval::evaluate(pos);

    u16             legalMoves{};
    PVLine          childPV{};
    Score           bestScore = -score_infinite;
    moves::MoveList moveList;
    generate_all_moves(pos, moveList);

    for (u32 i = 0; i < moveList.size(); ++i) {
        const auto currentMove = moveList.move_at(i);

        board::Position copy = pos;
        copy.make_move(currentMove);

        if (!copy.was_legal())
            continue;

        ++legalMoves;
        const Score score = -negamax(copy, -beta, -alpha, depth - 1, ply + 1, childPV);

        if (score > bestScore)
            bestScore = score;

        if (score > alpha) {
            alpha = score;
            pv.update(currentMove, childPV);
        }

        // Double-check if search stopped to make sure we don't exceed the search limits
        if (m_info.stopped)
            return 0;

        if (alpha >= beta)
            break;
    }

    // Checkmate / stalemate detection
    if (!legalMoves) {
        if (pos.checkers().bit_count() > 0)
            // Take the shortest available mate
            return -score_mate + ply;
        else
            // Stalemate
            return 0;
    }

    return bestScore;
}

bool Searcher::should_stop() const {
    if (m_info.searchedNodes % 1024 != 0)
        return false;

    const u64 elapsed = m_timer.elapsed();

    return m_info.searchedNodes >= m_limits.nodesLimit || elapsed >= m_limits.timeLimit
        || elapsed >= m_timer.optimum_time();
}

void Searcher::report_info(u64 elapsed, int depth, Score score, const PVLine& pv) const {
    std::cout << std::format("info depth {} score cp {} time {} nodes {} nps {} pv {}", depth,
                             score, elapsed, m_info.searchedNodes,
                             m_info.searchedNodes / std::max<u64>(1, elapsed) * 1000,
                             pv.to_string())
              << std::endl;
}

} // namespace search
