#include "uci.hpp"

#include <format>
#include <iostream>
#include <limits>
#include <numeric>

#include "../eval/eval.hpp"
#include "../moves/movegen.hpp"
#include "../perft/perft.hpp"
#include "../utils/split.hpp"
#include "../search/tt.hpp"
#include "../utils/parsing.hpp"
#include "../utils/time.hpp"

namespace uci {

void command_handler::handle_d(const board::position& pos) { print_board(pos); }

void command_handler::handle_eval(const board::position& pos) {
    std::cout << std::format("\nStatic evaluation: {}", eval::evaluate(pos)) << std::endl;
}

void command_handler::handle_is_ready() { std::cout << "readyok" << std::endl; }

void command_handler::handle_go(const std::vector<std::string>& command,
                                const board::position&          pos) {
    if (command[1] == "depth") {
        if (const auto parsed_depth = utils::parsing::to_number<u32>(command[2]))
            m_searcher.set_limits(std::numeric_limits<u64>::max(), std::numeric_limits<u64>::max(),
                                  parsed_depth.value());

        m_searcher.set_start_time(utils::time::get_time_ms());
    }
    else if (command[1] == "perft") {
        if (const auto parsed_perft_depth = utils::parsing::to_number<int>(command[2]))
            split_perft(pos, parsed_perft_depth.value());

        return;
    }
    else if (command[1] == "movetime") {
        if (const auto parsed_move_time = utils::parsing::to_number<u64>(command[2]))
            m_searcher.set_limits(std::numeric_limits<u64>::max(), parsed_move_time.value(),
                                  constants::max_depth);

        m_searcher.set_start_time(utils::time::get_time_ms());
    }
    else if (command[1] == "nodes") {
        if (const auto parsed_nodes = utils::parsing::to_number<u64>(command[2]))
            m_searcher.set_limits(parsed_nodes.value(), std::numeric_limits<u64>::max(),
                                  constants::max_depth);

        m_searcher.set_start_time(utils::time::get_time_ms());
    }
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

    if (const auto offset = std::ranges::find(command.begin(), command.end(), "moves");
        offset != command.end()) {
        for (auto it = offset + 1; it != command.end(); ++it) {
            const auto parsed_move = util::from_uci(pos, *it);

            if (parsed_move == moves::move::none())
                break;

            pos.make_move<true>(parsed_move);
        }
    }
}

void command_handler::handle_setoption(const std::vector<std::string>& command) {
    if (command[2] == "Hash") {
        search::tt::global_tt.resize(std::stoul(command[4]));
    }
}

void command_handler::handle_uci() {
    std::cout << std::format("id name {} {}", name, version) << std::endl;
    std::cout << std::format("id author {}", author) << std::endl;
    std::cout << "option name Hash type spin default 16 min 1 max 1024" << std::endl;
    std::cout << "option name Threads type spin default 1 min 1 max 1" << std::endl;
    std::cout << "uciok" << std::endl;
}

void command_handler::handle_uci_new_game(board::position& pos) {
    pos.reset_to_start_pos();
    search::tt::global_tt.clear();
}

void command_handler::loop() {
    std::string input;
    auto        pos = board::position(board::util::start_pos_fen);

    while (std::getline(std::cin, input)) {
        const auto& command = utils::split::split_string(input, ' ');

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
        else if (command[0] == "setoption")
            handle_setoption(command);
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

    for (usize i = 0; i < move_list.size(); ++i) {
        if (const auto current_move = move_list.move_at(i); move == current_move.to_string())
            return current_move;
    }

    return moves::move::none();
}

} // namespace util

} // namespace uci