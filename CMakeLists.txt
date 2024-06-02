cmake_minimum_required(VERSION 3.27)
project(NibbleCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -flto -DNDEBUG -std=c++23 -march=native -mtune=native -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SRCS
        "src/*.hpp" "src/*.cpp"
        "src/board/*.hpp" "src/board/*.cpp"
        "src/board/bitboard/*.hpp" "src/board/bitboard/*.cpp"
        "src/moves/*.hpp" "src/moves/*.cpp"
        "src/perft/*.hpp" "src/perft/*.cpp"
        "src/uci/*.hpp" "src/uci/*.cpp"
        "src/eval/*.hpp" "src/eval/*.cpp")

add_executable(NibbleCpp ${SRCS})