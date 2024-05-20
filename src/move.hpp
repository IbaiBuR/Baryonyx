#pragma once

#include "types.hpp"

#include <string>
#include <utility>

class Move {
    public:
        enum class MoveFlag : U8 {
            NOMOVE,
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

        constexpr Move(const Square &from, const Square &to, const MoveFlag &flag) {
            data = std::to_underlying(from) | std::to_underlying(to) << 6
                 | std::to_underlying(flag) << 12;
        }

        [[nodiscard]] constexpr Square from() const { return static_cast<Square>(data & 0X3F); }

        [[nodiscard]] constexpr Square to() const { return static_cast<Square>(data >> 6 & 0X3F); }

        [[nodiscard]] constexpr MoveFlag flag() const {
            return static_cast<MoveFlag>(data >> 12 & 0x0F);
        }

        [[nodiscard]] constexpr bool isPromotion() const {
            return this->flag() >= MoveFlag::KNIGHT_PROMO;
        }

        [[nodiscard]] std::string toString() const;

    private:
        U16 data;
};