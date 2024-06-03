CXX = clang++
SRCS = src/*.cpp src/board/*.cpp src/board/bitboard/*.cpp src/moves/*.cpp src/uci/*.cpp src/eval/*.cpp src/perft/*.cpp
HEADERS = src/*.hpp src/board/*.hpp src/board/bitboard/*.hpp src/moves/*.hpp src/uci/*.hpp src/eval/*.hpp src/perft/*.hpp
CXXFLAGS = -O3 -funroll-loops -flto -DNDEBUG -std=c++23 -march=native -mtune=native -Wall -Wextra
DEBUGFLAGS = -g -O0 -std=c++23 -Wall -Wextra
EXE = baryonyx

.PHONY: all clean debug format

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CXX) $(DEBUGFLAGS) $(SRCS) -o $(EXE)

clean:
	rm -f $(EXE)
