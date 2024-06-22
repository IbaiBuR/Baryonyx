#pragma once

#include <array>
#include <string>

#include "bitboard/bitboard.hpp"
#include "../moves/move.hpp"

namespace board {

class CastlingRights {
    public:
        enum class Flags : u8 {
            NONE,
            WK  = 1,
            WQ  = 2,
            BK  = 4,
            BQ  = 8,
            ALL = 15
        };

        constexpr CastlingRights() :
            m_castlingFlags(Flags::NONE) {}

        constexpr explicit CastlingRights(const Flags flag) :
            m_castlingFlags(flag) {}

        constexpr explicit CastlingRights(const std::string &flags) :
            m_castlingFlags(Flags::NONE) {
            for (const char c : flags) {
                switch (c) {
                case 'K':
                    *this |= CastlingRights(Flags::WK);
                    break;
                case 'Q':
                    *this |= CastlingRights(Flags::WQ);
                    break;
                case 'k':
                    *this |= CastlingRights(Flags::BK);
                    break;
                case 'q':
                    *this |= CastlingRights(Flags::BQ);
                    break;
                default:
                    break;
                }
            }
        }

        constexpr bool operator==(const CastlingRights &other) const {
            return std::to_underlying(m_castlingFlags) == std::to_underlying(other.m_castlingFlags);
        }

        constexpr bool operator!=(const CastlingRights &other) const {
            return std::to_underlying(m_castlingFlags) != std::to_underlying(other.m_castlingFlags);
        }

        constexpr CastlingRights operator|(const CastlingRights &other) const {
            return CastlingRights(static_cast<Flags>(std::to_underlying(m_castlingFlags)
                                                     | std::to_underlying(other.m_castlingFlags)));
        }

        constexpr CastlingRights &operator|=(const CastlingRights &other) {
            m_castlingFlags = static_cast<Flags>(std::to_underlying(m_castlingFlags)
                                                 | std::to_underlying(other.m_castlingFlags));
            return *this;
        }

        constexpr CastlingRights operator&(const int update) const {
            return CastlingRights(static_cast<Flags>(std::to_underlying(m_castlingFlags) & update));
        }

        constexpr CastlingRights &operator&=(const int update) {
            m_castlingFlags = static_cast<Flags>(std::to_underlying(m_castlingFlags) & update);
            return *this;
        }

        [[nodiscard]] constexpr u8 as_u8() const { return static_cast<u8>(m_castlingFlags); }

        template <Color c>
        [[nodiscard]] bool king_side_available() const {
            if constexpr (c == Color::WHITE)
                return std::to_underlying(m_castlingFlags) & std::to_underlying(Flags::WK);
            else
                return std::to_underlying(m_castlingFlags) & std::to_underlying(Flags::BK);
        }

        template <Color c>
        [[nodiscard]] bool queen_side_available() const {
            if constexpr (c == Color::WHITE)
                return std::to_underlying(m_castlingFlags) & std::to_underlying(Flags::WQ);
            else
                return std::to_underlying(m_castlingFlags) & std::to_underlying(Flags::BQ);
        }

        [[nodiscard]] std::string to_string() const {
            std::string result;

            if (king_side_available<Color::WHITE>())
                result += 'K';
            if (queen_side_available<Color::WHITE>())
                result += 'Q';
            if (king_side_available<Color::BLACK>())
                result += 'k';
            if (queen_side_available<Color::BLACK>())
                result += 'q';

            if (result.empty())
                result = '-';

            return result;
        }

    private:
        Flags m_castlingFlags;
};

class Position {
    public:
        Position() :
            m_pieces(),
            m_key(0ULL),
            m_fullMoveNumber(1),
            m_stm(Color::WHITE),
            m_epSq(Square::NONE),
            m_halfMoveClock(0) {
            m_pieces.fill(Piece::NONE);
        }

        explicit Position(const std::string &fen);

        [[nodiscard]] bitboards::Bitboard checkers() const { return m_checkersBB; }
        [[nodiscard]] Color               side_to_move() const { return m_stm; }
        [[nodiscard]] Square              ep_square() const { return m_epSq; }
        [[nodiscard]] CastlingRights      castling_rights() const { return m_castling; }
        [[nodiscard]] u8                  fifty_move_rule() const { return m_halfMoveClock; }
        [[nodiscard]] u16                 full_moves() const { return m_fullMoveNumber; }
        [[nodiscard]] ZobristKey          key() const { return m_key; }

        [[nodiscard]] Piece piece_on(const Square sq) const {
            return m_pieces[std::to_underlying(sq)];
        }

        [[nodiscard]] bitboards::Bitboard occupancies(const Color c) const {
            return m_occupiedBB[std::to_underlying(c)];
        }

        [[nodiscard]] bitboards::Bitboard piece_type_bb(const PieceType pt) const {
            return m_pieceBB[std::to_underlying(pt)];
        }

        template <Color c>
        [[nodiscard]] bool can_castle_king_side() const;

        template <Color c>
        [[nodiscard]] bool can_castle_queen_side() const;

        template <Color c>
        [[nodiscard]] int piece_count(PieceType pt) const;

        [[nodiscard]] bitboards::Bitboard attacks_to_king(Square kingSquare, Color c) const;

        [[nodiscard]] Square king_square(Color c) const;

        void set_piece(Piece p, Square sq);

        void remove_piece(Piece p, Square sq);

        void move_piece(Piece p, Square from, Square to);

        void make_move(moves::Move move);

        void reset_to_start_pos();

        [[nodiscard]] bool is_square_attacked_by(Square sq, Color c) const;

        [[nodiscard]] bool is_valid() const;

        [[nodiscard]] bool was_legal() const;

        [[nodiscard]] std::string to_fen() const;

    private:
        std::array<Piece, std::to_underlying(Square::SQUARE_NB)>                     m_pieces;
        std::array<bitboards::Bitboard, std::to_underlying(PieceType::PIECETYPE_NB)> m_pieceBB;
        std::array<bitboards::Bitboard, std::to_underlying(Color::COLOR_NB)>         m_occupiedBB;
        bitboards::Bitboard                                                          m_checkersBB;
        ZobristKey                                                                   m_key;
        u16            m_fullMoveNumber;
        Color          m_stm;
        Square         m_epSq;
        CastlingRights m_castling;
        u8             m_halfMoveClock;
};

namespace util {

constexpr auto startPosFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// clang-format off
constexpr std::array<std::string_view, std::to_underlying(Square::SQUARE_NB)> sqToCoords = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

constexpr std::array castlingRightsUpdate = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11
};
// clang-format on

} // namespace Util

void print_board(const Position &pos);

} // namespace Board
