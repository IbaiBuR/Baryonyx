CC = $(COMP)
SRCS = src/*.cpp src/board/*.cpp src/board/bitboard/*.cpp
HEADERS = src/*.hpp src/board/*.hpp src/board/bitboard/*.hpp
CXXFLAGS = -O3 -DNDEBUG -std=c++20 -march=native -Wall -Wextra
EXE = nibblecpp

.PHONY: all clean debug format

all:
	$(CC) $(CXXFLAGS) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CC) -g -std=c++20 $(SRCS) -o $(EXE)

clean:
	rm -f nibblecpp