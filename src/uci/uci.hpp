#pragma once

#include <string>
#include <vector>

#include "../board/position.hpp"

namespace UCI {

constexpr auto engineAuthor  = "Ibai Burgos";
constexpr auto engineName    = "Baryonyx";
constexpr auto engineVersion = "0.1";

class CommandHandler {
    public:
        void loop();

    private:
        Board::Position pos;

        void        handleBoard() const;
        void        handleEval() const;
        static void handleIsReady();
        void        handleGo(const std::vector<std::string> &command);
        void        handlePosition(const std::vector<std::string> &command);
        static void handleUci();
        void        handleUciNewGame();
};

namespace Util {

Moves::Move fromUCI(const Board::Position &pos, const std::string &move);

}

} // namespace UCI