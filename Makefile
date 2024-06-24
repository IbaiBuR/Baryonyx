CXX     = clang++
SRCS    = src/*.cpp src/board/*.cpp src/board/bitboard/*.cpp src/moves/*.cpp src/uci/*.cpp src/eval/*.cpp src/perft/*.cpp src/search/*.cpp src/utils/*.cpp
HEADERS = src/*.hpp src/board/*.hpp src/board/bitboard/*.hpp src/moves/*.hpp src/uci/*.hpp src/eval/*.hpp src/perft/*.hpp src/search/*.hpp src/utils/*.hpp

STD        = -std=c++23
WARNINGS   = -Wall -Wextra -Wpedantic
CXXFLAGS   = -O3 -funroll-loops -flto=auto -DNDEBUG $(STD) $(WARNINGS)
DEBUGFLAGS = -g -O0 $(STD) $(WARNINGS)

NATIVE = -march=native -mtune=native
M64    = -m64 -mpopcnt
SSE2   = $(M64) -msse -msse2
SSSE3  = $(SSE2) -mssse3
AVX2   = $(SSSE3) -msse4.1 -mbmi -mfma -mavx2
BMI2   = $(AVX2) -march=haswell -mbmi2 -DUSE_PEXT

EXE = baryonyx

ifeq ($(OS), Windows_NT)
    NAME := $(EXE).exe
    CXXFLAGS += -static
else
    NAME := $(EXE)
endif

ifeq ($(build), )
    build = native
endif

ifeq ($(build), native)
    CXXFLAGS += $(NATIVE)

    PROPS = $(shell echo | $(CXX) -march=native -E -dM -)
    ifneq ($(findstring __BMI2__, $(PROPS)),)
    	ifeq ($(findstring __znver1, $(PROPS)),)
    		ifeq ($(findstring __znver2, $(PROPS)),)
    			CXXFLAGS += -DUSE_PEXT
    		endif
    	endif
    endif
else ifeq ($(build), x86-64)
    CXXFLAGS += $(M64)
else ifeq ($(build), sse2)
    CXXFLAGS += $(SSE2) -mtune=znver1
else ifeq ($(build), ssse3)
    CXXFLAGS += $(SSSE3) -mtune=znver2
else ifeq ($(build), avx2)
    CXXFLAGS += $(AVX2)
else ifeq ($(build), bmi2)
    CXXFLAGS += $(BMI2)
endif

.PHONY: all clean debug format

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(NAME)

format:
	clang-format -i $(SRCS) $(HEADERS) -style=file

debug:
	$(CXX) $(DEBUGFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME)
