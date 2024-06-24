cmake_minimum_required(VERSION 3.27)
project(BaryonyxTests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -flto=auto -DNDEBUG -std=c++23 -march=native -mtune=native -Wall -Wextra -Wpedantic")

file(GLOB SRCS "*.cpp"
        "../src/*.hpp"
        "../src/perft/perft.cpp"
        "../src/moves/*.cpp"
        "../src/board/bitboard/*.cpp"
        "../src/board/*.cpp"
        "../src/eval/*.cpp"
        "../src/utils/*.cpp")

add_executable(BaryonyxTests ${SRCS})