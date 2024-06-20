#include <cstring>

#include "board/bitboard/attacks.hpp"
#include "search/bench.hpp"
#include "uci/uci.hpp"

int main(const int argc, const char *argv[]) {
    Board::Bitboards::Attacks::init();

    if (argc > 1 && !strcmp(argv[1], "bench")) {
        constexpr int benchDepth = 5;

        Search::Searcher searcher;
        Search::Bench::run(searcher, benchDepth);

        return 0;
    }

    UCI::CommandHandler commandHandler;
    commandHandler.loop();

    return 0;
}
