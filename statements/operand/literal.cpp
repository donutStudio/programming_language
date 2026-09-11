#include "literal.hpp"

literal::literal(std::string value) : value(value) {
}

std::string literal::to_string() const {
    return value;
}
