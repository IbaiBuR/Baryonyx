cmake_minimum_required(VERSION 3.27)
project(NibbleTests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -Wall -Wextra")

file(GLOB SRCS "*.cpp"
        "../src/board/bitboard/*.cpp"
        "../src/board/*.cpp"
        "../src/board/*.hpp")

add_executable(NibbleTests ${SRCS})