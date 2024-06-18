cmake_minimum_required(VERSION 3.27)
project(Baryonyx)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -flto=auto -DNDEBUG -std=c++23 -march=native -mtune=native -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SRCS
        "src/*.cpp"
        "src/board/*.cpp"
        "src/board/bitboard/*.cpp"
        "src/moves/*.cpp"
        "src/perft/*.cpp"
        "src/uci/*.cpp"
        "src/eval/*.cpp"
        "src/search/*.cpp"
        "src/tt/*.cpp")

add_executable(Baryonyx ${SRCS})