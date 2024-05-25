CXX = $(COMP)
SRCS = src/*.cpp src/board/*.cpp src/board/bitboard/*.cpp src/moves/*.cpp src/uci/*.cpp
HEADERS = src/*.hpp src/board/*.hpp src/board/bitboard/*.hpp src/moves/*.hpp src/uci/*.hpp
CXXFLAGS = -O3 -DNDEBUG -std=c++23 -march=native -Wall -Wextra
DEBUGFLAGS = -g -std=c++23 -Wall -Wextra
EXE = nibblecpp

.PHONY: all clean debug format

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXE)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CXX) $(DEBUGFLAGS) $(SRCS) -o $(EXE)

clean:
	rm -f nibblecpp
