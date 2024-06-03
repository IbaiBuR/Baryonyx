<div align="center">

<img src="logo/baryonyx_logo.png" alt="Baryonyx" width="400">

# Baryonyx

[![License][license-badge]][license-link]
[![Build][build-badge]][build-link]
[![Commits][commits-badge]][commits-link]

</div>

## Overview

Baryonyx is a work-in-progress UCI-compliant chess engine written in C++, initially based on my previous C
engine [Nibble][nibble].
It is a personal project with the goal of learning modern C++ and chess programming.

## Features

- Board representation & Move generation
    - [Bitboards][bitboards]
        - [Magic Bitboards][magic-bitboards]
    - [Copy-Make][copy-make]
    - [Pseudo-legal Move Generation][pseudo-legal-movegen]
- Evaluation
    - [Material][material]
    - [PesTO Piece-Square Tables][psqts]
    - [Tapered Evaluation][tapered-eval]
- Search
    - TDB
- Transposition Table
    - TDB

## Building

Since Baryonyx is written in C++23, you'll need a recent version of g++/clang++ (g++ >= 13 / clang++ >= 15) to build it.
See [compiler-support][compilers] for reference.

To create a working executable, follow this steps:

1. Clone the repository: ```git clone https://github.com/IbaiBuR/Baryonyx.git```
2. Go to the directory of the repository: ```cd Baryonyx```
3. Run make: ```make```

> [!NOTE]
> It is recommended to use clang++ instead of g++, as it produces faster binaries
> If you prefer to use with g++, you can specify it when running make -> make CXX=g++

[license-badge]: https://img.shields.io/github/license/IbaiBuR/Baryonyx?style=for-the-badge

[build-badge]: https://img.shields.io/github/actions/workflow/status/IbaiBuR/Baryonyx/build.yml?style=for-the-badge

[commits-badge]: https://img.shields.io/github/commit-activity/w/IbaiBuR/Baryonyx?style=for-the-badge

[license-link]: https://github.com/IbaiBuR/Baryonyx/blob/main/LICENSE

[build-link]: https://github.com/IbaiBuR/Baryonyx/blob/main/.github/workflows/build.yml

[commits-link]: https://github.com/IbaiBuR/Baryonyx/commits/main/

[nibble]: https://github.com/IbaiBuR/Nibble

[bitboards]: https://www.chessprogramming.org/Bitboards

[magic-bitboards]: https://analog-hors.github.io/site/magic-bitboards/

[copy-make]: https://www.chessprogramming.org/Copy-Make

[pseudo-legal-movegen]: https://www.chessprogramming.org/Move_Generation#Pseudo-legal

[material]: https://www.chessprogramming.org/Material

[psqts]: https://www.chessprogramming.org/Piece-Square_Tables

[tapered-eval]: https://www.chessprogramming.org/Tapered_Eval

[compilers]: https://en.cppreference.com/w/cpp/compiler_support/23