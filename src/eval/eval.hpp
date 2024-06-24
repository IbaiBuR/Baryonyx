#pragma once

#include "../board/position.hpp"

namespace eval {

/// @brief Packed Evaluation: https://minuskelvin.net/chesswiki/content/packed-eval.html
class packed_score {
    public:
        constexpr packed_score() :
            m_score(0) {}

        constexpr explicit packed_score(const int score) :
            m_score(score) {}

        constexpr packed_score(const i16 mgScore, const i16 egScore) {
            m_score = static_cast<score>((static_cast<u32>(egScore) << 16) + mgScore);
        }

        constexpr packed_score operator*(const int mul) const {
            return packed_score(m_score * mul);
        }

        constexpr packed_score operator+(const packed_score& other) const {
            return packed_score(m_score + other.m_score);
        }

        constexpr packed_score operator-(const packed_score& other) const {
            return packed_score(m_score - other.m_score);
        }

        constexpr packed_score& operator+=(const packed_score& other) {
            m_score += other.m_score;
            return *this;
        }

        constexpr packed_score& operator-=(const packed_score& other) {
            m_score -= other.m_score;
            return *this;
        }

        [[nodiscard]] constexpr score midgame() const { return static_cast<i16>(m_score); }

        [[nodiscard]] constexpr score endgame() const {
            return static_cast<i16>(static_cast<u32>(m_score + 0X8000) >> 16);
        }

    private:
        score m_score;
};

score evaluate(const board::position& pos);

} // namespace eval