#include "expression.hpp"

#include <stdexcept>

namespace {
    std::string operation_to_string(const operation& operation_value) {
        if (std::holds_alternative<arithmetic_operator>(operation_value)) {
            switch (std::get<arithmetic_operator>(operation_value)) {
                case ADD:
                    return "+";
                case SUBTRACT:
                    return "-";
                case MULTIPLY:
                    return "*";
                case DIVIDE:
                    return "/";
            }
        }

        if (std::holds_alternative<comparison_operator>(operation_value)) {
            switch (std::get<comparison_operator>(operation_value)) {
                case EQUAL:
                    return "==";
                case NOT_EQUAL:
                    return "!=";
                case LESS:
                    return "<";
                case GREATER_EQUAL:
                    return ">=";
                case GREATER:
                    return ">";
                case LESS_EQUAL:
                    return "<=";
            }
        }

        switch (std::get<logical_operator>(operation_value)) {
            case AND:
                return "&&";
            case OR:
                return "||";
            case NOT:
                return "!";
        }

        throw std::runtime_error("Unknown operation");
    }
}

expression::expression(operand operand_value) : value(operand_value) {
}

expression::expression(const expression& left_operand, operation operation_value, const expression& right_operand)
    : value(binary_expression {
        std::make_shared<expression>(left_operand),
        operation_value,
        std::make_shared<expression>(right_operand)
    }) {
}

expression::expression(logical_operator operation_value, const expression& operand_value)
    : value(unary_expression {
        operation_value,
        std::make_shared<expression>(operand_value)
    }) {
    if (operation_value != NOT) {
        throw std::runtime_error("Only ! is a valid unary logical operator");
    }
}

expression::expression(arithmetic_operator operation_value, const expression& operand_value)
    : value(unary_expression {
        operation_value,
        std::make_shared<expression>(operand_value)
    }) {
    if (operation_value != SUBTRACT) {
        throw std::runtime_error("Only - is a valid unary arithmetic operator");
    }
}

bool expression::is_operand() const {
    return std::holds_alternative<operand>(value);
}

bool expression::is_unary() const {
    return std::holds_alternative<unary_expression>(value);
}

bool expression::is_binary() const {
    return std::holds_alternative<binary_expression>(value);
}

const operand& expression::get_operand() const {
    return std::get<operand>(value);
}

const expression& expression::get_left_operand() const {
    return *std::get<binary_expression>(value).left_operand;
}

const expression& expression::get_right_operand() const {
    return *std::get<binary_expression>(value).right_operand;
}

const expression& expression::get_unary_operand() const {
    return *std::get<unary_expression>(value).operand_value;
}

const operation& expression::get_operation() const {
    if (is_unary()) {
        return std::get<unary_expression>(value).operation_value;
    }

    return std::get<binary_expression>(value).operation_value;
}

std::string expression::to_string() const {
    if (is_operand()) {
        const operand& operand_value = get_operand();

        if (std::holds_alternative<literal>(operand_value)) {
            return std::get<literal>(operand_value).to_string();
        }

        return std::get<identifier>(operand_value).to_string();
    }

    if (is_unary()) {
        return "(" + operation_to_string(get_operation()) + get_unary_operand().to_string() + ")";
    }

    return "("
        + get_left_operand().to_string()
        + " "
        + operation_to_string(get_operation())
        + " "
        + get_right_operand().to_string()
        + ")";
}
