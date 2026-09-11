#include "parser.hpp"

#include <cctype>
#include <stdexcept>
#include <vector>

namespace {
    struct token {
        std::string text;
    };

    bool is_letter(char character) {
        return std::isalpha(static_cast<unsigned char>(character)) || character == '_';
    }

    bool is_name_character(char character) {
        return std::isalnum(static_cast<unsigned char>(character)) || character == '_';
    }

    std::vector<token> tokenize(std::string source) {
        std::vector<token> tokens;
        std::size_t position = 0;

        while (position < source.length()) {
            char current = source[position];

            if (std::isspace(static_cast<unsigned char>(current))) {
                position++;
                continue;
            }

            if (std::isdigit(static_cast<unsigned char>(current))) {
                std::size_t start = position;

                while (position < source.length() && std::isdigit(static_cast<unsigned char>(source[position]))) {
                    position++;
                }

                tokens.push_back({source.substr(start, position - start)});
                continue;
            }

            if (is_letter(current)) {
                std::size_t start = position;

                while (position < source.length() && is_name_character(source[position])) {
                    position++;
                }

                tokens.push_back({source.substr(start, position - start)});
                continue;
            }

            if (position + 1 < source.length()) {
                std::string two_characters = source.substr(position, 2);

                if (two_characters == "==" || two_characters == "!=" || two_characters == "<=" ||
                    two_characters == ">=" || two_characters == "&&" || two_characters == "||") {
                    tokens.push_back({two_characters});
                    position += 2;
                    continue;
                }
            }

            std::string one_character = source.substr(position, 1);

            if (one_character == "(" || one_character == ")" || one_character == "+" || one_character == "-" ||
                one_character == "*" || one_character == "/" || one_character == "<" || one_character == ">" ||
                one_character == "!" || one_character == "=") {
                tokens.push_back({one_character});
                position++;
                continue;
            }

            throw std::runtime_error("Unexpected character: " + one_character);
        }

        return tokens;
    }

    class expression_parser {
        public:
            expression_parser(std::vector<token> tokens) : tokens(tokens), current(0) {
            }

            expression parse() {
                expression result = parse_assignment();

                if (!is_at_end()) {
                    throw std::runtime_error("Unexpected token: " + peek());
                }

                return result;
            }

        private:
            std::vector<token> tokens;
            std::size_t current;

            expression parse_assignment() {
                expression left = parse_or();

                if (!match("=")) {
                    return left;
                }

                if (!left.is_assignable()) {
                    throw std::runtime_error("Left side of assignment must be a variable name");
                }

                return expression(left, "=", parse_assignment());
            }

            expression parse_or() {
                expression left = parse_and();

                while (match("||")) {
                    left = expression(left, "||", parse_and());
                }

                return left;
            }

            expression parse_and() {
                expression left = parse_equality();

                while (match("&&")) {
                    left = expression(left, "&&", parse_equality());
                }

                return left;
            }

            expression parse_equality() {
                expression left = parse_comparison();

                while (match("==") || match("!=")) {
                    std::string operation = previous();
                    left = expression(left, operation, parse_comparison());
                }

                return left;
            }

            expression parse_comparison() {
                expression left = parse_term();

                while (match("<") || match("<=") || match(">") || match(">=")) {
                    std::string operation = previous();
                    left = expression(left, operation, parse_term());
                }

                return left;
            }

            expression parse_term() {
                expression left = parse_factor();

                while (match("+") || match("-")) {
                    std::string operation = previous();
                    left = expression(left, operation, parse_factor());
                }

                return left;
            }

            expression parse_factor() {
                expression left = parse_unary();

                while (match("*") || match("/")) {
                    std::string operation = previous();
                    left = expression(left, operation, parse_unary());
                }

                return left;
            }

            expression parse_unary() {
                if (match("!") || match("-")) {
                    std::string operation = previous();
                    return expression(operation, parse_unary());
                }

                return parse_primary();
            }

            expression parse_primary() {
                if (match("(")) {
                    expression inside_parentheses = parse_assignment();
                    consume(")", "Expected ')' after expression");
                    return inside_parentheses;
                }

                if (is_at_end()) {
                    throw std::runtime_error("Unexpected end of expression");
                }

                return expression(advance());
            }

            bool match(std::string text) {
                if (is_at_end() || peek() != text) {
                    return false;
                }

                current++;
                return true;
            }

            void consume(std::string text, std::string error_message) {
                if (!match(text)) {
                    throw std::runtime_error(error_message);
                }
            }

            bool is_at_end() const {
                return current >= tokens.size();
            }

            std::string peek() const {
                return tokens[current].text;
            }

            std::string previous() const {
                return tokens[current - 1].text;
            }

            std::string advance() {
                return tokens[current++].text;
            }
    };
}

expression string_to_expression(std::string source) {
    return expression_parser(tokenize(source)).parse();
}
