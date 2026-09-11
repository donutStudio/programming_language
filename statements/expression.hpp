#pragma once

#include "operand/identifier.hpp"
#include "operand/literal.hpp"

#include <memory>
#include <string>
#include <variant>

enum arithmetic_operator {ADD, SUBTRACT, MULTIPLY, DIVIDE};
enum comparison_operator {EQUAL, NOT_EQUAL, LESS, GREATER_EQUAL, GREATER, LESS_EQUAL};
enum logical_operator {AND, OR, NOT};
enum assignment_operator {ASSIGN};

using operand = std::variant<literal, identifier>;
using operation = std::variant<arithmetic_operator, comparison_operator, logical_operator, assignment_operator>;

class expression {
    public:
        expression(operand operand_value);
        expression(const expression& left_operand, operation operation_value, const expression& right_operand);
        expression(logical_operator operation_value, const expression& operand_value);
        expression(arithmetic_operator operation_value, const expression& operand_value);

        bool is_operand() const;
        bool is_unary() const;
        bool is_binary() const;
        bool is_assignable() const;

        const operand& get_operand() const;
        const expression& get_left_operand() const;
        const expression& get_right_operand() const;
        const expression& get_unary_operand() const;
        const operation& get_operation() const;

        std::string to_string() const;

    private:
        struct unary_expression {
            operation operation_value;
            std::shared_ptr<expression> operand_value;
        };

        struct binary_expression {
            std::shared_ptr<expression> left_operand;
            operation operation_value;
            std::shared_ptr<expression> right_operand;
        };

        std::variant<operand, unary_expression, binary_expression> value;
};
