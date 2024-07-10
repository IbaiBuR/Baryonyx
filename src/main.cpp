#include <cstring>

#include "board/bitboard/attacks.hpp"
#include "search/bench.hpp"
#include "uci/uci.hpp"

int main(const int argc, const char *argv[]) {
    board::bitboards::attacks::init();

    if (argc > 1 && !strcmp(argv[1], "bench")) {
        constexpr int bench_depth = 8;

        search::searcher searcher;
        search::bench::run(searcher, bench_depth);

        return 0;
    }

    uci::command_handler uci_handler;
    uci_handler.loop();

    return 0;
}
