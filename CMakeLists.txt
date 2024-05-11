cmake_minimum_required(VERSION 3.27)
project(NibbleCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SRCS
        "src/*.hpp" "src/*.cpp"
        "src/board/*.hpp" "src/board/*.cpp"
        "src/board/bitboard/*.hpp" "src/board/bitboard/*.cpp")

add_executable(NibbleCpp ${SRCS})