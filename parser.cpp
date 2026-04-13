#include "parser.hpp"

#include <iostream>

std::unordered_map<std::string, arithmetic_operator> arithmetic_operator_key = {
    {"+", ADD},
    {"-", SUBTRACT},
    {"*", MULTIPLY},
    {"/", DIVIDE}
};

std::unordered_map<std::string, comparison_operator> comparison_operator_key = {
    {"==", EQUAL},
    {"!=", NOT_EQUAL},
    {"<", LESS},
    {">=", GREATER_EQUAL},
    {">", GREATER},
    {"<=", LESS_EQUAL}
};

std::unordered_map<std::string, logical_operator> logical_operator_key = {
    {"&&", AND},
    {"||", OR},
    {"!", NOT}
};

expression string_to_expression(std::string expr) {
    for (const auto& pair : logical_operator_key) { 
        auto operator_pos = expr.find(pair.first);
        if (operator_pos == 0) {
            return expression(pair.second, string_to_expression(expr.substr(pair.first.length())));
        } else if (operator_pos != std::string::npos) {
            return expression(string_to_expression(expr.substr(0, operator_pos)), pair.second, string_to_expression(expr.substr(operator_pos + pair.first.length())));
        }
    }
    for (const auto& pair : comparison_operator_key) {
        auto operator_pos = expr.find(pair.first);
        if (operator_pos == 0) {
            throw "Error: Cannot start expression with comparison operator";
        } else if (operator_pos != std::string::npos) {
            return expression(string_to_expression(expr.substr(0, operator_pos)), pair.second, string_to_expression(expr.substr(operator_pos + pair.first.length())));
        }
    }
    for (const auto& pair : arithmetic_operator_key) {
        auto operator_pos = expr.find(pair.first);
        if (operator_pos == 0) {
            throw "Error: Cannot start expression with non-incrementing arithmetic operator";
        } else if (operator_pos != std::string::npos) {
            return expression(string_to_expression(expr.substr(0, operator_pos)), pair.second, string_to_expression(expr.substr(operator_pos + pair.first.length())));
        }
    }
    return expression(literal());
}