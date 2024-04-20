#pragma once

#include "../types.hpp"

class Bitboard {
  public:
    constexpr Bitboard() :
        data(0ULL){};
    constexpr explicit Bitboard(const U64 bb) :
        data(bb){};

  private:
    U64 data;
};