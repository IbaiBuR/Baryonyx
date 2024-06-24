#include "uci.hpp"

#include <format>
#include <iostream>
#include <numeric>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../perft/perft.hpp"
#include "../utils/split.hpp"

namespace uci {

void command_handler::handle_d(const board::position& pos) { print_board(pos); }

void command_handler::handle_eval(const board::position& pos) {
    std::cout << std::format("\nStatic evaluation: {}", eval::evaluate(pos)) << std::endl;
}

void command_handler::handle_is_ready() { std::cout << std::format("readyok") << std::endl; }

void command_handler::handle_go(const std::vector<std::string>& command,
                                const board::position&          pos) {
    if (command[1] == "depth")
        m_searcher.set_limits(UINT64_MAX, UINT64_MAX, std::stoi(command[2]));
    else if (command[1] == "perft") {
        split_perft(pos, std::stoi(command[2]));
        return;
    }
    else if (command[1] == "movetime")
        m_searcher.set_limits(UINT64_MAX, std::stoull(command[2]), constants::max_depth);
    else if (command[1] == "nodes")
        m_searcher.set_limits(std::stoull(command[2]), UINT64_MAX, constants::max_depth);
    else if (command[1] == "infinite")
        m_searcher.set_limits(UINT64_MAX, UINT64_MAX, constants::max_depth);
    else if (command[1] == "wtime" || command[1] == "btime") {
        m_searcher.parse_time_control(command, pos.side_to_move());
    }
    else
        std::cout << std::format("Unhandled go command: {}", command[1]) << std::endl;

    m_searcher.main_search(pos);
}

void command_handler::handle_position(const std::vector<std::string>& command,
                                      board::position&                pos) {
    if (command[1] == "startpos") {
        pos.reset_to_start_pos();
    }
    else if (command[1] == "fen") {
        const auto& fen = command | std::views::drop(2) | std::views::take(6)
                        | std::views::transform([](const std::string& s) { return s + " "; });
        const std::string& fen_str = std::accumulate(fen.begin(), fen.end(), std::string{});
        pos                        = board::position(fen_str);
    }

    if (auto offset = std::ranges::find(command.begin(), command.end(), "moves");
        offset != command.end()) {

        ++offset;

        for (auto it = offset; it != command.end(); ++it) {
            const auto parsed_move = util::from_uci(pos, *it);

            if (parsed_move == moves::move::none())
                break;

            pos.make_move(parsed_move);
        }
    }
}

void command_handler::handle_uci() {
    std::cout << std::format("id name {} {}", name, version) << std::endl;
    std::cout << std::format("id author {}", author) << std::endl;
    std::cout << std::format("option name Hash type spin default 1 min 1 max 1") << std::endl;
    std::cout << std::format("option name Threads type spin default 1 min 1 max 1") << std::endl;
    std::cout << std::format("uciok") << std::endl;
}

void command_handler::handle_uci_new_game(board::position& pos) { pos.reset_to_start_pos(); }

void command_handler::loop() {
    std::string input;
    auto        pos = board::position(board::util::start_pos_fen);

    while (std::getline(std::cin, input)) {
        const auto command = utils::split::split_string(input, ' ');

        if (command.empty())
            continue;

        if (command[0] == "d")
            handle_d(pos);
        else if (command[0] == "eval")
            handle_eval(pos);
        else if (command[0] == "isready")
            handle_is_ready();
        else if (command[0] == "go")
            handle_go(command, pos);
        else if (command[0] == "position")
            handle_position(command, pos);
        else if (command[0] == "quit")
            break;
        else if (command[0] == "uci")
            handle_uci();
        else if (command[0] == "ucinewgame")
            handle_uci_new_game(pos);
        else {
            std::cout << std::format("Unknown command: {}", command[0]) << std::endl;
        }
    }
}

namespace util {

moves::move from_uci(const board::position& pos, const std::string& move) {
    moves::move_list move_list;
    generate_all_moves(pos, move_list);

    for (u32 i = 0; i < move_list.size(); ++i) {
        if (const auto current_move = move_list.move_at(i); move == current_move.to_string())
            return current_move;
    }

    return moves::move::none();
}

} // namespace util

} // namespace uci