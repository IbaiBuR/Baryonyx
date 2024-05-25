#include "board/bitboard/attacks.hpp"
#include "uci/uci.hpp"

int main() {
    Board::Bitboards::Attacks::init();

    UCI::CommandHandler commandHandler;
    commandHandler.loop();

    return 0;
}
