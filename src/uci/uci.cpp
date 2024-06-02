#include "uci.hpp"

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../perft/perft.hpp"
#include "../utils.hpp"

#include <iostream>
#include <numeric>
#include <print>

namespace UCI {

void CommandHandler::handleBoard() const { printBoard(pos); }

void CommandHandler::handleEval() const {
    std::println("\nStatic evaluation: {}", Eval::evaluate(pos));
}

void CommandHandler::handleIsReady() {
    std::print("readyok");
    std::cout << std::endl;
}

void CommandHandler::handleGo(const std::vector<std::string> &command) {
    if (command[1] == "perft")
        splitPerft(pos, std::stoi(command[2]));
}

void CommandHandler::handlePosition(const std::vector<std::string> &command) {
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
    std::println("id name {} {}", engineName, engineVersion);
    std::println("id author {}", engineAuthor);
    std::print("uciok");
    std::cout << std::endl;
}

void CommandHandler::handleUciNewGame() { pos.resetToStartPos(); }

void CommandHandler::loop() {
    std::string input;

    while (std::getline(std::cin, input)) {
        const auto command = Utils::splitString(input, ' ');

        if (command.empty())
            continue;

        if (command[0] == "board")
            handleBoard();
        else if (command[0] == "eval")
            handleEval();
        else if (command[0] == "isready")
            handleIsReady();
        else if (command[0] == "go")
            handleGo(command);
        else if (command[0] == "position")
            handlePosition(command);
        else if (command[0] == "quit")
            break;
        else if (command[0] == "uci")
            handleUci();
        else if (command[0] == "ucinewgame")
            handleUciNewGame();
        else {
            std::print("Unknown command: {}", command[0]);
            std::cout << std::endl;
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