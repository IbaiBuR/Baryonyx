#include "move.hpp"

#include <format>

#include "../board/position.hpp"

namespace Moves {

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

std::string Move::toString() const {
    std::string result;

    const auto from = Board::Util::sqToCoords[std::to_underlying(this->from())];
    const auto to   = Board::Util::sqToCoords[std::to_underlying(this->to())];

    if (this->isPromotion()) {
        result = std::format("{}{}{}", from, to, promoFlagToChar(this->flag()));
    }
    else {
        result = std::format("{}{}", from, to);
    }

    return result;
}

} // namespace Moves
