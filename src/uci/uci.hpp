#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "../board/position.hpp"
#include "../search/search.hpp"

namespace uci {

class command_handler {
    public:
        void loop();

    private:
        static constexpr std::string_view author  = "Ibai Burgos";
        static constexpr std::string_view name    = "Baryonyx";
        static constexpr std::string_view version = "0.1.17";

        search::searcher m_searcher;

        static void handle_d(const board::position& pos);
        static void handle_eval(const board::position& pos);
        static void handle_is_ready();
        void        handle_go(const std::vector<std::string>& command, const board::position& pos);
        static void handle_position(const std::vector<std::string>& command, board::position& pos);
        static void handle_setoption(const std::vector<std::string>& command);
        static void handle_uci();
        static void handle_uci_new_game(board::position& pos);
};

namespace util {

moves::move from_uci(const board::position& pos, const std::string& move);

}

} // namespace uci