#include <cstring>

#include "board/bitboard/attacks.hpp"
#include "search/bench.hpp"
#include "uci/uci.hpp"

int main(const int argc, const char *argv[]) {
    board::bitboards::attacks::init();

    if (argc > 1 && !strcmp(argv[1], "bench")) {
        constexpr int benchDepth = 5;

        search::Searcher searcher;
        search::bench::run(searcher, benchDepth);

        return 0;
    }

    uci::CommandHandler commandHandler;
    commandHandler.loop();

    return 0;
}
