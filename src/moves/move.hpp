#pragma once

#include <string>
#include <utility>

#include "../chess.hpp"

namespace moves {

class move {
    public:
        enum class move_flag : u8 {
            no_move,
            null_move,
            quiet,
            capture,
            en_passant,
            castle,
            double_push,
            knight_promo,
            bishop_promo,
            rook_promo,
            queen_promo,
            knight_capture_promo,
            bishop_cature_promo,
            rook_capture_promo,
            queen_capture_promo
        };

        constexpr move() = default;

        constexpr move(const square from, const square to, const move_flag flag) {
            m_data = std::to_underlying(from) | std::to_underlying(to) << 6
                   | std::to_underlying(flag) << 12;
        }

        static constexpr move none() { return {square::a8, square::a8, move_flag::no_move}; }
        static constexpr move null() { return {square::a8, square::a8, move_flag::null_move}; }

        [[nodiscard]] constexpr square from() const { return static_cast<square>(m_data & 0X3F); }

        [[nodiscard]] constexpr square to() const {
            return static_cast<square>(m_data >> 6 & 0X3F);
        }

        [[nodiscard]] constexpr move_flag flag() const {
            return static_cast<move_flag>(m_data >> 12 & 0x0F);
        }

        [[nodiscard]] constexpr bool is_promotion() const {
            return this->flag() >= move_flag::knight_promo;
        }

        [[nodiscard]] constexpr bool is_capture() const {
            const move_flag flag = this->flag();

            return flag == move_flag::capture || flag >= move_flag::knight_capture_promo
                || flag == move_flag::en_passant;
        }

        [[nodiscard]] constexpr bool is_en_passant() const {
            return this->flag() == move_flag::en_passant;
        }

        [[nodiscard]] constexpr bool is_double_push() const {
            return this->flag() == move_flag::double_push;
        }

        [[nodiscard]] constexpr bool is_castling() const {
            return this->flag() == move_flag::castle;
        }

        [[nodiscard]] constexpr bool is_quiet() const {
            return !this->is_capture() && !this->is_promotion();
        }

        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] piece get_promoted_piece(color stm) const;

        constexpr bool operator==(const move& other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const move& other) const { return m_data != other.m_data; }

    private:
        u16 m_data;
};

} // namespace moves