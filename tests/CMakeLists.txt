cmake_minimum_required(VERSION 3.27)
project(NibbleTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -Wall -Wextra")

file(GLOB SRCS "*.cpp")

add_executable(NibbleTests ${SRCS})