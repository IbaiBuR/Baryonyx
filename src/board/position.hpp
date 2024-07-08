#pragma once

#include <array>
#include <string>
#include <vector>

#include "bitboard/bitboard.hpp"
#include "../moves/move.hpp"

namespace board {

class castling_rights {
    public:
        enum class castling_flag : u8 {
            none,
            wk  = 1,
            wq  = 2,
            bk  = 4,
            bq  = 8,
            all = 15
        };

        constexpr castling_rights() :
            m_flags(castling_flag::none) {}

        constexpr explicit castling_rights(const castling_flag flag) :
            m_flags(flag) {}

        constexpr explicit castling_rights(const std::string& flags) :
            m_flags(castling_flag::none) {
            for (const char c : flags) {
                switch (c) {
                case 'K':
                    *this |= castling_rights(castling_flag::wk);
                    break;
                case 'Q':
                    *this |= castling_rights(castling_flag::wq);
                    break;
                case 'k':
                    *this |= castling_rights(castling_flag::bk);
                    break;
                case 'q':
                    *this |= castling_rights(castling_flag::bq);
                    break;
                default:
                    break;
                }
            }
        }

        constexpr bool operator==(const castling_rights& other) const {
            return std::to_underlying(m_flags) == std::to_underlying(other.m_flags);
        }

        constexpr bool operator!=(const castling_rights& other) const {
            return std::to_underlying(m_flags) != std::to_underlying(other.m_flags);
        }

        constexpr castling_rights operator|(const castling_rights& other) const {
            return castling_rights(static_cast<castling_flag>(std::to_underlying(m_flags)
                                                              | std::to_underlying(other.m_flags)));
        }

        constexpr castling_rights& operator|=(const castling_rights& other) {
            m_flags = static_cast<castling_flag>(std::to_underlying(m_flags)
                                                 | std::to_underlying(other.m_flags));
            return *this;
        }

        constexpr castling_rights operator&(const int update) const {
            return castling_rights(
                static_cast<castling_flag>(std::to_underlying(m_flags) & update));
        }

        constexpr castling_rights& operator&=(const int update) {
            m_flags = static_cast<castling_flag>(std::to_underlying(m_flags) & update);
            return *this;
        }

        [[nodiscard]] constexpr u8 as_u8() const { return static_cast<u8>(m_flags); }

        template <color C>
        [[nodiscard]] bool king_side_available() const {
            if constexpr (C == color::white)
                return std::to_underlying(m_flags) & std::to_underlying(castling_flag::wk);
            else
                return std::to_underlying(m_flags) & std::to_underlying(castling_flag::bk);
        }

        template <color C>
        [[nodiscard]] bool queen_side_available() const {
            if constexpr (C == color::white)
                return std::to_underlying(m_flags) & std::to_underlying(castling_flag::wq);
            else
                return std::to_underlying(m_flags) & std::to_underlying(castling_flag::bq);
        }

        [[nodiscard]] std::string to_string() const {
            std::string result;

            if (king_side_available<color::white>())
                result += 'K';
            if (queen_side_available<color::white>())
                result += 'Q';
            if (king_side_available<color::black>())
                result += 'k';
            if (queen_side_available<color::black>())
                result += 'q';

            if (result.empty())
                result = '-';

            return result;
        }

    private:
        castling_flag m_flags;
};

class position {
    public:
        position() :
            m_pieces(),
            m_key(0ULL),
            m_full_move_number(1),
            m_stm(color::white),
            m_ep_sq(square::none),
            m_half_move_clock(0) {
            m_hash_history.reserve(constants::max_game_ply);
            m_pieces.fill(piece::none);
        }

        explicit position(const std::string& fen);

        /// @note Only used during perft to avoid copying the hash history vector recursively
        static position copy_without_hash_history(const position& other) {
            position copy = other;
            copy.m_hash_history.clear();
            return copy;
        }

        [[nodiscard]] bitboards::bitboard checkers() const { return m_checkers_bb; }
        [[nodiscard]] color               side_to_move() const { return m_stm; }
        [[nodiscard]] square              ep_square() const { return m_ep_sq; }
        [[nodiscard]] castling_rights     castling() const { return m_castling; }
        [[nodiscard]] u8                  fifty_move_rule() const { return m_half_move_clock; }
        [[nodiscard]] u16                 full_moves() const { return m_full_move_number; }
        [[nodiscard]] zobrist_key         key() const { return m_key; }
        [[nodiscard]] bool last_move_was_null() const { return m_last_move_was_null; }

        [[nodiscard]] piece piece_on(const square sq) const {
            return m_pieces[std::to_underlying(sq)];
        }

        [[nodiscard]] bitboards::bitboard occupancies(const color c) const {
            return m_occupied_bb[std::to_underlying(c)];
        }

        [[nodiscard]] bitboards::bitboard piece_type_bb(const piece_type pt) const {
            return m_piece_bb[std::to_underlying(pt)];
        }

        template <color C>
        [[nodiscard]] bool can_castle_king_side() const;

        template <color C>
        [[nodiscard]] bool can_castle_queen_side() const;

        template <color C>
        [[nodiscard]] int piece_count(piece_type pt) const;

        [[nodiscard]] bitboards::bitboard attacks_to_king(square kingSquare, color c) const;

        [[nodiscard]] square king_square(color c) const;

        void set_piece(piece p, square sq);

        void remove_piece(piece p, square sq);

        void move_piece(piece p, square from, square to);

        template <bool SaveHashHistory>
        void make_move(moves::move move);

        void make_null_move();

        void reset_to_start_pos();

        [[nodiscard]] bool has_no_pawns(color c) const;

        [[nodiscard]] bool is_square_attacked_by(square sq, color c) const;

        [[nodiscard]] bool is_valid() const;

        [[nodiscard]] bool was_legal() const;

        [[nodiscard]] bool has_repeated() const;

        [[nodiscard]] std::string to_fen() const;

    private:
        // clang-format off
        static constexpr std::array castling_rights_update = {
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

        template <color C>
        [[nodiscard]] bool has_no_pawns() const;

        std::vector<zobrist_key>                                    m_hash_history;
        std::array<piece, constants::num_squares>                   m_pieces;
        std::array<bitboards::bitboard, constants::num_piece_types> m_piece_bb;
        std::array<bitboards::bitboard, constants::num_colors>      m_occupied_bb;
        bitboards::bitboard                                         m_checkers_bb;
        zobrist_key                                                 m_key;
        u16                                                         m_full_move_number;
        color                                                       m_stm;
        square                                                      m_ep_sq;
        castling_rights                                             m_castling;
        u8                                                          m_half_move_clock;
        bool                                                        m_last_move_was_null{false};
};

namespace util {

inline constexpr auto start_pos_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// clang-format off
inline constexpr std::array<std::string_view, constants::num_squares> sq_to_coords = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};
// clang-format on

} // namespace util

void print_board(const position& pos);

} // namespace board
