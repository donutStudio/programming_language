#include "literal.hpp"

literal::literal(int value) : value(value) {
}

literal::literal(bool value) : value(value) {
}

const literal_value& literal::get_value() const {
    return value;
}

std::string literal::to_string() const {
    if (std::holds_alternative<int>(value)) {
        return std::to_string(std::get<int>(value));
    }

    return std::get<bool>(value) ? "true" : "false";
}
