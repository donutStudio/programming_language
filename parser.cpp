#include "parser.hpp"

#include <cctype>
#include <stdexcept>
#include <vector>

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

namespace {
    enum token_type {
        INTEGER,
        BOOLEAN,
        IDENTIFIER,
        SYMBOL
    };

    struct token {
        token_type type;
        std::string text;
    };

    class parser_state {
        public:
            parser_state(const std::vector<token>& tokens) : tokens(tokens), position(0) {
            }

            expression parse_expression() {
                expression result = parse_logical_or_expression();

                if (!is_at_end()) {
                    throw std::runtime_error("Unexpected token: " + peek().text);
                }

                return result;
            }

        private:
            const std::vector<token>& tokens;
            std::size_t position;

            expression parse_logical_or_expression() {
                expression left_expression = parse_logical_and_expression();

                while (match_symbol("||")) {
                    left_expression = expression(left_expression, OR, parse_logical_and_expression());
                }

                return left_expression;
            }

            expression parse_logical_and_expression() {
                expression left_expression = parse_equality_expression();

                while (match_symbol("&&")) {
                    left_expression = expression(left_expression, AND, parse_equality_expression());
                }

                return left_expression;
            }

            expression parse_equality_expression() {
                expression left_expression = parse_comparison_expression();

                while (true) {
                    if (match_symbol("==")) {
                        left_expression = expression(left_expression, EQUAL, parse_comparison_expression());
                    } else if (match_symbol("!=")) {
                        left_expression = expression(left_expression, NOT_EQUAL, parse_comparison_expression());
                    } else {
                        return left_expression;
                    }
                }
            }

            expression parse_comparison_expression() {
                expression left_expression = parse_additive_expression();

                while (true) {
                    if (match_symbol("<")) {
                        left_expression = expression(left_expression, LESS, parse_additive_expression());
                    } else if (match_symbol("<=")) {
                        left_expression = expression(left_expression, LESS_EQUAL, parse_additive_expression());
                    } else if (match_symbol(">")) {
                        left_expression = expression(left_expression, GREATER, parse_additive_expression());
                    } else if (match_symbol(">=")) {
                        left_expression = expression(left_expression, GREATER_EQUAL, parse_additive_expression());
                    } else {
                        return left_expression;
                    }
                }
            }

            expression parse_additive_expression() {
                expression left_expression = parse_multiplicative_expression();

                while (true) {
                    if (match_symbol("+")) {
                        left_expression = expression(left_expression, ADD, parse_multiplicative_expression());
                    } else if (match_symbol("-")) {
                        left_expression = expression(left_expression, SUBTRACT, parse_multiplicative_expression());
                    } else {
                        return left_expression;
                    }
                }
            }

            expression parse_multiplicative_expression() {
                expression left_expression = parse_unary_expression();

                while (true) {
                    if (match_symbol("*")) {
                        left_expression = expression(left_expression, MULTIPLY, parse_unary_expression());
                    } else if (match_symbol("/")) {
                        left_expression = expression(left_expression, DIVIDE, parse_unary_expression());
                    } else {
                        return left_expression;
                    }
                }
            }

            expression parse_unary_expression() {
                if (match_symbol("!")) {
                    return expression(NOT, parse_unary_expression());
                }

                if (match_symbol("-")) {
                    return expression(SUBTRACT, parse_unary_expression());
                }

                return parse_primary_expression();
            }

            expression parse_primary_expression() {
                if (match_symbol("(")) {
                    expression grouped_expression = parse_logical_or_expression();
                    consume_symbol(")");
                    return grouped_expression;
                }

                if (is_at_end()) {
                    throw std::runtime_error("Unexpected end of expression");
                }

                token current_token = advance();

                if (current_token.type == INTEGER) {
                    return expression(literal(std::stoi(current_token.text)));
                }

                if (current_token.type == BOOLEAN) {
                    return expression(literal(current_token.text == "true"));
                }

                if (current_token.type == IDENTIFIER) {
                    return expression(identifier(current_token.text));
                }

                throw std::runtime_error("Unexpected token: " + current_token.text);
            }

            bool is_at_end() const {
                return position >= tokens.size();
            }

            const token& peek() const {
                return tokens.at(position);
            }

            token advance() {
                return tokens.at(position++);
            }

            bool match_symbol(const std::string& symbol) {
                if (!is_at_end() && peek().type == SYMBOL && peek().text == symbol) {
                    position++;
                    return true;
                }

                return false;
            }

            void consume_symbol(const std::string& symbol) {
                if (!match_symbol(symbol)) {
                    throw std::runtime_error("Expected token: " + symbol);
                }
            }
    };

    bool is_identifier_start(char character) {
        return std::isalpha(static_cast<unsigned char>(character)) || character == '_';
    }

    bool is_identifier_character(char character) {
        return std::isalnum(static_cast<unsigned char>(character)) || character == '_';
    }

    std::vector<token> tokenize(const std::string& source) {
        std::vector<token> tokens;
        std::size_t position = 0;

        while (position < source.length()) {
            char character = source[position];

            if (std::isspace(static_cast<unsigned char>(character))) {
                position++;
                continue;
            }

            if (std::isdigit(static_cast<unsigned char>(character))) {
                std::size_t start = position;

                while (position < source.length() && std::isdigit(static_cast<unsigned char>(source[position]))) {
                    position++;
                }

                tokens.push_back({INTEGER, source.substr(start, position - start)});
                continue;
            }

            if (is_identifier_start(character)) {
                std::size_t start = position;

                while (position < source.length() && is_identifier_character(source[position])) {
                    position++;
                }

                std::string text = source.substr(start, position - start);

                if (text == "true" || text == "false") {
                    tokens.push_back({BOOLEAN, text});
                } else {
                    tokens.push_back({IDENTIFIER, text});
                }

                continue;
            }

            if (position + 1 < source.length()) {
                std::string pair = source.substr(position, 2);

                if (pair == "&&" || pair == "||" || pair == "==" || pair == "!=" || pair == "<=" || pair == ">=") {
                    tokens.push_back({SYMBOL, pair});
                    position += 2;
                    continue;
                }
            }

            if (character == '(' || character == ')' || character == '+' || character == '-' || character == '*' || character == '/' || character == '<' || character == '>' || character == '!') {
                tokens.push_back({SYMBOL, std::string(1, character)});
                position++;
                continue;
            }

            throw std::runtime_error("Unexpected character: " + std::string(1, character));
        }

        return tokens;
    }
}

expression string_to_expression(std::string expr) {
    return parser_state(tokenize(expr)).parse_expression();
}
