#pragma once

#include <memory>
#include <string>

enum expression_type {
    OPERAND_EXPRESSION,
    UNARY_EXPRESSION,
    BINARY_EXPRESSION
};

class expression {
    public:
        expression(std::string value);
        expression(std::string operation, const expression& right);
        expression(const expression& left, std::string operation, const expression& right);

        expression_type get_type() const;
        std::string get_text() const;
        bool is_assignable() const;
        std::string to_string() const;

    private:
        expression_type type;
        std::string text;
        std::shared_ptr<expression> left;
        std::shared_ptr<expression> right;
};
