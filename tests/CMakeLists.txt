cmake_minimum_required(VERSION 3.27)
project(BaryonyxTests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -flto -DNDEBUG -std=c++23 -march=native -mtune=native -Wall -Wextra")

file(GLOB SRCS "*.cpp"
        "../src/*.hpp"
        "../src/perft/perft.cpp"
        "../src/moves/*.hpp"
        "../src/moves/*.cpp"
        "../src/board/bitboard/*.hpp"
        "../src/board/bitboard/*.cpp"
        "../src/board/*.cpp"
        "../src/board/*.hpp"
        "../src/eval/*.hpp"
        "../src/eval/*.cpp")

add_executable(BaryonyxTests ${SRCS})