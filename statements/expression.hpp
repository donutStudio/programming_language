#pragma once

#include "operand/identifier.hpp"
#include "operand/literal.hpp"

#include <variant>

enum arithmetic_operator {ADD, SUBTRACT, MULTIPLY, DIVIDE};
enum comparison_operator {EQUAL, NOT_EQUAL, LESS, GREATER_EQUAL, GREATER, LESS_EQUAL};
enum logical_operator {AND, OR, NOT};

using operand = std::variant<literal, identifier>;
using operation = std::variant<arithmetic_operator, comparison_operator, logical_operator>;

class expression {
    public:
        expression(operand operand);
        expression(const expression& left_operand, operation operation, const expression& right_operand);
        expression(logical_operator operation, const expression& operand);
};