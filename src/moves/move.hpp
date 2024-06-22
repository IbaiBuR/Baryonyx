#pragma once

#include <string>
#include <utility>

#include "../chess.hpp"

namespace moves {

class Move {
    public:
        enum class MoveFlag : u8 {
            NOMOVE,
            NULLMOVE,
            QUIET,
            CAPTURE,
            ENPASSANT,
            CASTLE,
            DOUBLEPUSH,
            KNIGHT_PROMO,
            BISHOP_PROMO,
            ROOK_PROMO,
            QUEEN_PROMO,
            KNIGHT_CAPTURE_PROMO,
            BISHOP_CATURE_PROMO,
            ROOK_CAPTURE_PROMO,
            QUEEN_CAPTURE_PROMO
        };

        constexpr Move() = default;

        constexpr Move(const Square from, const Square to, const MoveFlag flag) {
            m_data = std::to_underlying(from) | std::to_underlying(to) << 6
                   | std::to_underlying(flag) << 12;
        }

        static constexpr Move none() { return {Square::A8, Square::A8, MoveFlag::NOMOVE}; }
        static constexpr Move null() { return {Square::A8, Square::A8, MoveFlag::NULLMOVE}; }

        [[nodiscard]] constexpr Square from() const { return static_cast<Square>(m_data & 0X3F); }

        [[nodiscard]] constexpr Square to() const {
            return static_cast<Square>(m_data >> 6 & 0X3F);
        }

        [[nodiscard]] constexpr MoveFlag flag() const {
            return static_cast<MoveFlag>(m_data >> 12 & 0x0F);
        }

        [[nodiscard]] constexpr bool is_promotion() const {
            return this->flag() >= MoveFlag::KNIGHT_PROMO;
        }

        [[nodiscard]] constexpr bool is_capture() const {
            const MoveFlag flag = this->flag();

            return flag == MoveFlag::CAPTURE || flag >= MoveFlag::KNIGHT_CAPTURE_PROMO
                || flag == MoveFlag::ENPASSANT;
        }

        [[nodiscard]] constexpr bool is_en_passant() const {
            return this->flag() == MoveFlag::ENPASSANT;
        }

        [[nodiscard]] constexpr bool is_double_push() const {
            return this->flag() == MoveFlag::DOUBLEPUSH;
        }

        [[nodiscard]] constexpr bool is_castling() const {
            return this->flag() == MoveFlag::CASTLE;
        }

        [[nodiscard]] constexpr bool is_quiet() const {
            return !this->is_capture() && !this->is_promotion();
        }

        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] Piece get_promoted_piece(Color stm) const;

        constexpr bool operator==(const Move &other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const Move &other) const { return m_data != other.m_data; }

    private:
        u16 m_data;
};

} // namespace Moves