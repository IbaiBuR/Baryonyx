#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.hpp"

int main(const int argc, char *argv[]) {
    doctest::Context ctx;

    ctx.setOption("abort-after", 1);
    ctx.setOption("no-exitcode", false);
    ctx.setOption("success", true);

    ctx.applyCommandLine(argc, argv);

    const int res = ctx.run();

    if (ctx.shouldExit())
        return res;

    return res;
}