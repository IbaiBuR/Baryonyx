#include "search.hpp"

#include <format>
#include <iostream>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../utils.hpp"

namespace Search {

void Searcher::resetInfo() {
    m_info.stopped       = false;
    m_info.searchedNodes = 0ULL;
    m_info.pv.clear();
}

void Searcher::setLimits(const u64 nodesLimit, const u64 timeLimit, const u32 depthLimit) {
    m_limits.nodesLimit = nodesLimit;
    m_limits.timeLimit  = timeLimit;
    m_limits.depthLimit = depthLimit;
}

void Searcher::parseTimeControl(const std::vector<std::string> &command, const Color stm) {
    u64 baseTime{};
    u16 increment{};

    for (auto it = command.begin() + 1; it < command.end(); ++it) {
        if (stm == Color::WHITE) {
            if (*it == "wtime") {
                baseTime = std::stoull(*(it + 1));
                setLimits(UINT64_MAX, baseTime, MAX_DEPTH);
            }
            if (*it == "winc")
                increment = std::stoi(*(it + 1));
        }
        else {
            if (*it == "btime") {
                baseTime = std::stoull(*(it + 1));
                setLimits(UINT64_MAX, baseTime, MAX_DEPTH);
            }
            if (*it == "binc")
                increment = std::stoi(*(it + 1));
        }
    }
    m_timer = TimeManager(Utils::getTimeMs(), baseTime, increment);
}

/// @brief Main entrypoint for the search function
void Searcher::mainSearch(const Board::Position &pos) {
    m_timer.setStartTime(Utils::getTimeMs());
    resetInfo();
    auto bestMove = Moves::Move::none();

    // Iterative deepening loop
    for (int currentDepth = 1; currentDepth <= m_limits.depthLimit; ++currentDepth) {
        const Score bestScore =
            negamax(pos, -SCORE_INFINITE, SCORE_INFINITE, currentDepth, 0, m_info.pv);

        if (m_info.stopped) {
            break;
        }

        // Ensure we only update the best move if the search was not cancelled
        // otherwise our best move may be terrible
        bestMove = m_info.pv.bestMove();

        reportInfo(m_timer.elapsed(), currentDepth, bestScore, m_info.pv);
    }
    std::cout << std::format("bestmove {}", bestMove.toString()) << std::endl;
}

/// @brief Fail-soft negamax algorithm with alpha-beta pruning
Score Searcher::negamax(const Board::Position &pos,
                        Score                  alpha,
                        const Score            beta,
                        const int              depth,
                        const int              ply,
                        PVLine                &pv) {

    if (depth == 0)
        return Eval::evaluate(pos);

    ++m_info.searchedNodes;

    if (depth > 1 && shouldStop()) {
        m_info.stopped = true;
        return 0;
    }

    u16             legalMoves{};
    PVLine          childPV{};
    Score           bestScore = -SCORE_INFINITE;
    Moves::MoveList moveList;
    generateAllMoves(pos, moveList);

    for (u32 i = 0; i < moveList.size(); ++i) {
        const auto currentMove = moveList.moveAt(i);

        Board::Position copy = pos;
        copy.makeMove(currentMove);

        if (!copy.wasLegal())
            continue;

        ++legalMoves;
        const Score score = -negamax(copy, -beta, -alpha, depth - 1, ply + 1, childPV);

        // Double-check if search stopped to make sure we don't exceed the search limits
        if (depth > 1 && m_info.stopped)
            return 0;

        if (score > bestScore)
            bestScore = score;

        if (score > alpha) {
            alpha = score;
            pv.update(currentMove, childPV);
        }

        if (alpha >= beta)
            break;
    }

    // Checkmate / stalemate detection
    if (!legalMoves) {
        if (pos.checkers().bitCount() > 0)
            // Take the shortest available mate
            return -SCORE_MATE + ply;
        else
            // Stalemate
            return 0;
    }

    return bestScore;
}

bool Searcher::shouldStop() const {
    if (m_info.searchedNodes % 1024 != 0)
        return false;

    const u64 elapsed = m_timer.elapsed();

    return m_info.searchedNodes >= m_limits.nodesLimit || elapsed >= m_limits.timeLimit
        || elapsed >= m_timer.optimumTime();
}

void Searcher::reportInfo(u64 elapsed, int depth, Score score, const PVLine &pv) const {
    std::cout << std::format("info depth {} score cp {} time {} nodes {} nps {} pv {}", depth,
                             score, elapsed, m_info.searchedNodes,
                             m_info.searchedNodes / std::max<u64>(1, elapsed) * 1000, pv.toString())
              << std::endl;
}

} // namespace Search
