#include <iostream>
#include <variant>

#include "expression.hpp"

using operand = std::variant<literal, identifier>;

expression::expression(operand operand) {
    std::cout << "test1" << std::endl;
}

expression::expression(const expression& left_operand, operation operation, const expression& right_operand) {
    std::cout << "test2" << std::endl;
}

expression::expression(logical_operator operation, const expression& operand) {
    if (operation == NOT) {
        std::cout << "test3" << std::endl;
    } else {
        throw "Error: Cannot start expression with any logical operator other than NOT";
    }
}