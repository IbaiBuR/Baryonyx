#include "move.hpp"

#include <format>

#include "../board/position.hpp"

namespace moves {

constexpr char promoFlagToChar(const Move::MoveFlag m) {
    switch (m) {
    case Move::MoveFlag::KNIGHT_PROMO:
    case Move::MoveFlag::KNIGHT_CAPTURE_PROMO:
        return 'n';
    case Move::MoveFlag::BISHOP_PROMO:
    case Move::MoveFlag::BISHOP_CATURE_PROMO:
        return 'b';
    case Move::MoveFlag::ROOK_PROMO:
    case Move::MoveFlag::ROOK_CAPTURE_PROMO:
        return 'r';
    case Move::MoveFlag::QUEEN_PROMO:
    case Move::MoveFlag::QUEEN_CAPTURE_PROMO:
        return 'q';
    default:
        throw std::invalid_argument("Invalid MoveFlag provided.\n");
    }
}

constexpr PieceType promotionToPieceType(const Move::MoveFlag m) {
    switch (m) {
    case Move::MoveFlag::KNIGHT_PROMO:
    case Move::MoveFlag::KNIGHT_CAPTURE_PROMO:
        return PieceType::KNIGHT;
    case Move::MoveFlag::BISHOP_PROMO:
    case Move::MoveFlag::BISHOP_CATURE_PROMO:
        return PieceType::BISHOP;
    case Move::MoveFlag::ROOK_PROMO:
    case Move::MoveFlag::ROOK_CAPTURE_PROMO:
        return PieceType::ROOK;
    case Move::MoveFlag::QUEEN_PROMO:
    case Move::MoveFlag::QUEEN_CAPTURE_PROMO:
        return PieceType::QUEEN;
    default:
        throw std::invalid_argument("Invalid MoveFlag provided.\n");
    }
};

std::string Move::to_string() const {
    std::string result;

    const auto from = board::util::sqToCoords[std::to_underlying(this->from())];
    const auto to   = board::util::sqToCoords[std::to_underlying(this->to())];

    if (this->is_promotion()) {
        result = std::format("{}{}{}", from, to, promoFlagToChar(flag()));
    }
    else {
        result = std::format("{}{}", from, to);
    }

    return result;
}

Piece Move::get_promoted_piece(const Color stm) const {
    return static_cast<Piece>(std::to_underlying(promotionToPieceType(flag()))
                              + std::to_underlying(stm) * 6);
}

} // namespace moves
