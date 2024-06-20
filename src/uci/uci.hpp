#pragma once

#include <string>
#include <vector>

#include "../board/position.hpp"
#include "../search/search.hpp"

namespace UCI {

constexpr auto engineAuthor  = "Ibai Burgos";
constexpr auto engineName    = "Baryonyx";
constexpr auto engineVersion = "0.1.6";

class CommandHandler {
    public:
        void loop();

    private:
        Search::Searcher m_searcher;

        static void handleD(const Board::Position &pos);
        static void handleEval(const Board::Position &pos);
        static void handleIsReady();
        void        handleGo(const std::vector<std::string> &command, const Board::Position &pos);
        static void handlePosition(const std::vector<std::string> &command, Board::Position &pos);
        static void handleUci();
        static void handleUciNewGame(Board::Position &pos);
};

namespace Util {

Moves::Move fromUCI(const Board::Position &pos, const std::string &move);

}

} // namespace UCI