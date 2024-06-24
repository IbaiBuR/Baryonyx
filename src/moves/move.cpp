#include "move.hpp"

#include <format>

#include "../board/position.hpp"

namespace moves {

constexpr char promo_flag_to_char(const move::move_flag m) {
    switch (m) {
    case move::move_flag::knight_promo:
    case move::move_flag::knight_capture_promo:
        return 'n';
    case move::move_flag::bishop_promo:
    case move::move_flag::bishop_cature_promo:
        return 'b';
    case move::move_flag::rook_promo:
    case move::move_flag::rook_capture_promo:
        return 'r';
    case move::move_flag::queen_promo:
    case move::move_flag::queen_capture_promo:
        return 'q';
    default:
        throw std::invalid_argument("Invalid move flag provided.\n");
    }
}

constexpr piece_type promotion_to_piece_type(const move::move_flag m) {
    switch (m) {
    case move::move_flag::knight_promo:
    case move::move_flag::knight_capture_promo:
        return piece_type::knight;
    case move::move_flag::bishop_promo:
    case move::move_flag::bishop_cature_promo:
        return piece_type::bishop;
    case move::move_flag::rook_promo:
    case move::move_flag::rook_capture_promo:
        return piece_type::rook;
    case move::move_flag::queen_promo:
    case move::move_flag::queen_capture_promo:
        return piece_type::queen;
    default:
        throw std::invalid_argument("Invalid move flag provided.\n");
    }
};

std::string move::to_string() const {
    std::string result;

    const auto from = board::util::sq_to_coords[std::to_underlying(this->from())];
    const auto to   = board::util::sq_to_coords[std::to_underlying(this->to())];

    if (this->is_promotion()) {
        result = std::format("{}{}{}", from, to, promo_flag_to_char(flag()));
    }
    else {
        result = std::format("{}{}", from, to);
    }

    return result;
}

piece move::get_promoted_piece(const color stm) const {
    return static_cast<piece>(std::to_underlying(promotion_to_piece_type(flag()))
                              + std::to_underlying(stm) * 6);
}

} // namespace moves
