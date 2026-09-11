#include "expression.hpp"

expression::expression(std::string value)
    : type(OPERAND_EXPRESSION), text(value), left(nullptr), right(nullptr) {
}

expression::expression(std::string operation, const expression& right)
    : type(UNARY_EXPRESSION), text(operation), left(nullptr), right(std::make_shared<expression>(right)) {
}

expression::expression(const expression& left, std::string operation, const expression& right)
    : type(BINARY_EXPRESSION),
      text(operation),
      left(std::make_shared<expression>(left)),
      right(std::make_shared<expression>(right)) {
}

expression_type expression::get_type() const {
    return type;
}

std::string expression::get_text() const {
    return text;
}

bool expression::is_assignable() const {
    return type == OPERAND_EXPRESSION && !text.empty() && !std::isdigit(text[0]) && text != "true" && text != "false";
}

std::string expression::to_string() const {
    if (type == OPERAND_EXPRESSION) {
        return text;
    }

    if (type == UNARY_EXPRESSION) {
        return "(" + text + right->to_string() + ")";
    }

    return "(" + left->to_string() + " " + text + " " + right->to_string() + ")";
}
