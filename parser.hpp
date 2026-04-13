#pragma once

#include "statements/expression.hpp"

#include <string>
#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, arithmetic_operator> arithmetic_operator_key;

extern std::unordered_map<std::string, comparison_operator> comparison_operator_key;

extern std::unordered_map<std::string, logical_operator> logical_operator_key;

expression string_to_expression(std::string expr);