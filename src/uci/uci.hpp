#pragma once

#include <string>
#include <vector>

#include "../board/position.hpp"
#include "../search/search.hpp"

namespace uci {

class CommandHandler {
    public:
        void loop();

    private:
        static constexpr auto author  = "Ibai Burgos";
        static constexpr auto name    = "Baryonyx";
        static constexpr auto version = "0.1.6";

        search::Searcher m_searcher;

        static void handle_d(const board::Position& pos);
        static void handle_eval(const board::Position& pos);
        static void handle_is_ready();
        void        handle_go(const std::vector<std::string>& command, const board::Position& pos);
        static void handle_position(const std::vector<std::string>& command, board::Position& pos);
        static void handle_uci();
        static void handle_uci_new_game(board::Position& pos);
};

namespace util {

moves::Move from_uci(const board::Position& pos, const std::string& move);

}

} // namespace uci