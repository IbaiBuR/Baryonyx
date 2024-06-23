#pragma once

#include "../board/position.hpp"

namespace eval {

/// @brief Packed Evaluation: https://minuskelvin.net/chesswiki/content/packed-eval.html
class PackedScore {
    public:
        constexpr PackedScore() :
            m_score(0) {}

        constexpr explicit PackedScore(const int score) :
            m_score(score) {}

        constexpr PackedScore(const i16 mgScore, const i16 egScore) {
            m_score = static_cast<Score>((static_cast<u32>(egScore) << 16) + mgScore);
        }

        constexpr PackedScore operator*(const int mul) const { return PackedScore(m_score * mul); }

        constexpr PackedScore operator+(const PackedScore& other) const {
            return PackedScore(m_score + other.m_score);
        }

        constexpr PackedScore operator-(const PackedScore& other) const {
            return PackedScore(m_score - other.m_score);
        }

        constexpr PackedScore& operator+=(const PackedScore& other) {
            m_score += other.m_score;
            return *this;
        }

        constexpr PackedScore& operator-=(const PackedScore& other) {
            m_score -= other.m_score;
            return *this;
        }

        [[nodiscard]] constexpr Score midgame() const { return static_cast<i16>(m_score); }

        [[nodiscard]] constexpr Score endgame() const {
            return static_cast<i16>(static_cast<u32>(m_score + 0X8000) >> 16);
        }

    private:
        Score m_score;
};

Score evaluate(const board::Position& pos);

} // namespace eval