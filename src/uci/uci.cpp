#include "uci.hpp"

#include <format>
#include <iostream>
#include <numeric>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../perft/perft.hpp"
#include "../utils.hpp"

namespace UCI {

void CommandHandler::handleD(const Board::Position &pos) { printBoard(pos); }

void CommandHandler::handleEval(const Board::Position &pos) {
    std::cout << std::format("\nStatic evaluation: {}", Eval::evaluate(pos)) << std::endl;
}

void CommandHandler::handleIsReady() { std::cout << std::format("readyok") << std::endl; }

void CommandHandler::handleGo(const std::vector<std::string> &command, const Board::Position &pos) {
    if (command[1] == "depth")
        m_searcher.setLimits(UINT64_MAX, UINT64_MAX, std::stoi(command[2]));
    else if (command[1] == "perft") {
        splitPerft(pos, std::stoi(command[2]));
        return;
    }
    else if (command[1] == "movetime")
        m_searcher.setLimits(UINT64_MAX, std::stoull(command[2]), MAX_DEPTH);
    else if (command[1] == "nodes")
        m_searcher.setLimits(std::stoull(command[2]), UINT64_MAX, MAX_DEPTH);
    else if (command[1] == "infinite")
        m_searcher.setLimits(UINT64_MAX, UINT64_MAX, MAX_DEPTH);
    else if (command[1] == "wtime" || command[1] == "btime") {
        m_searcher.parseTimeControl(command, pos.sideToMove());
    }
    else
        std::cout << std::format("Unhandled go command: {}", command[1]) << std::endl;

    m_searcher.mainSearch(pos);
}

void CommandHandler::handlePosition(const std::vector<std::string> &command, Board::Position &pos) {
    if (command[1] == "startpos") {
        pos.resetToStartPos();
    }
    else if (command[1] == "fen") {
        const auto &fen = command | std::views::drop(2) | std::views::take(6)
                        | std::views::transform([](const std::string &s) { return s + " "; });
        const std::string &fenStr = std::accumulate(fen.begin(), fen.end(), std::string{});
        pos                       = Board::Position(fenStr);
    }

    if (auto offset = std::ranges::find(command.begin(), command.end(), "moves");
        offset != command.end()) {

        ++offset;

        for (auto it = offset; it != command.end(); ++it) {
            const Moves::Move parsedMove = Util::fromUCI(pos, *it);

            if (parsedMove == Moves::Move::none())
                break;

            pos.makeMove(parsedMove);
        }
    }
}

void CommandHandler::handleUci() {
    std::cout << std::format("id name {} {}", engineName, engineVersion) << std::endl;
    std::cout << std::format("id author {}", engineAuthor) << std::endl;
    std::cout << std::format("option name Hash type spin default 1 min 1 max 1") << std::endl;
    std::cout << std::format("option name Threads type spin default 1 min 1 max 1") << std::endl;
    std::cout << std::format("uciok") << std::endl;
}

void CommandHandler::handleUciNewGame(Board::Position &pos) { pos.resetToStartPos(); }

void CommandHandler::loop() {
    std::string input;
    auto        pos = Board::Position(Board::Util::startPosFen);

    while (std::getline(std::cin, input)) {
        const auto command = Utils::splitString(input, ' ');

        if (command.empty())
            continue;

        if (command[0] == "d")
            handleD(pos);
        else if (command[0] == "eval")
            handleEval(pos);
        else if (command[0] == "isready")
            handleIsReady();
        else if (command[0] == "go")
            handleGo(command, pos);
        else if (command[0] == "position")
            handlePosition(command, pos);
        else if (command[0] == "quit")
            break;
        else if (command[0] == "uci")
            handleUci();
        else if (command[0] == "ucinewgame")
            handleUciNewGame(pos);
        else {
            std::cout << std::format("Unknown command: {}", command[0]) << std::endl;
        }
    }
}

namespace Util {

Moves::Move fromUCI(const Board::Position &pos, const std::string &move) {
    Moves::MoveList moveList;
    generateAllMoves(pos, moveList);

    for (u32 i = 0; i < moveList.size(); ++i) {
        if (const Moves::Move currentMove = moveList.moveAt(i); move == currentMove.toString())
            return currentMove;
    }

    return Moves::Move::none();
}

} // namespace Util

} // namespace UCI