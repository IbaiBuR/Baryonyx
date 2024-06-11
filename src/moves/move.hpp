#pragma once

#include <string>
#include <utility>

#include "../chess.hpp"

namespace Moves {

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

        [[nodiscard]] constexpr bool isPromotion() const {
            return this->flag() >= MoveFlag::KNIGHT_PROMO;
        }

        [[nodiscard]] constexpr bool isCapture() const {
            const MoveFlag flag = this->flag();

            return flag == MoveFlag::CAPTURE || flag >= MoveFlag::KNIGHT_CAPTURE_PROMO
                || flag == MoveFlag::ENPASSANT;
        }

        [[nodiscard]] constexpr bool isEnPassant() const {
            return this->flag() == MoveFlag::ENPASSANT;
        }

        [[nodiscard]] constexpr bool isDoublePush() const {
            return this->flag() == MoveFlag::DOUBLEPUSH;
        }

        [[nodiscard]] constexpr bool isCastling() const { return this->flag() == MoveFlag::CASTLE; }

        [[nodiscard]] constexpr bool isQuiet() const {
            return !this->isCapture() && !this->isPromotion();
        }

        constexpr bool operator==(const Move &other) const { return m_data == other.m_data; }
        constexpr bool operator!=(const Move &other) const { return m_data != other.m_data; }

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] Piece getPromotedPiece(Color stm) const;

    private:
        u16 m_data;
};

} // namespace Moves