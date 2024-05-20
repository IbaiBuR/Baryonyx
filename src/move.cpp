#include "move.hpp"

#include <format>

std::string Move::toString() const {
    std::string result;

    if (this->isPromotion()) {
        result = std::format("");
    }
    else {
        result = std::format("");
    }

    return result;
}
