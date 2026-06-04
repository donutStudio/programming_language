#include "parser.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    struct parser_test_case {
        std::string source;
        std::string expected_tree;
    };

    bool run_parser_tests() {
        const std::vector<parser_test_case> tests = {
            {"1 + 2 * 3", "(1 + (2 * 3))"},
            {"(1 + 2) * 3", "((1 + 2) * 3)"},
            {"!ready || count >= 10", "((!ready) || (count >= 10))"},
            {"value == 3 && other != 4", "((value == 3) && (other != 4))"},
            {"-10 + score", "((-10) + score)"},
            {"true && false || enabled", "((true && false) || enabled)"}
        };

        bool all_passed = true;

        for (const parser_test_case& test : tests) {
            try {
                std::string actual_tree = string_to_expression(test.source).to_string();

                if (actual_tree != test.expected_tree) {
                    all_passed = false;
                    std::cerr
                        << "[FAIL] " << test.source << "\n"
                        << "  expected: " << test.expected_tree << "\n"
                        << "  actual:   " << actual_tree << "\n";
                } else {
                    std::cout << "[PASS] " << test.source << " -> " << actual_tree << "\n";
                }
            } catch (const std::exception& exception) {
                all_passed = false;
                std::cerr << "[FAIL] " << test.source << " threw: " << exception.what() << "\n";
            }
        }

        const std::vector<std::string> invalid_tests = {
            "1 +",
            "&& value",
            "(1 + 2",
            "value )"
        };

        for (const std::string& source : invalid_tests) {
            try {
                string_to_expression(source);
                all_passed = false;
                std::cerr << "[FAIL] " << source << " should have been rejected\n";
            } catch (const std::exception&) {
                std::cout << "[PASS] " << source << " rejected\n";
            }
        }

        return all_passed;
    }
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--test") {
        return run_parser_tests() ? 0 : 1;
    }

    std::string input;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, input)) {
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            std::cout << string_to_expression(input).to_string() << "\n";
        } catch (const std::exception& exception) {
            std::cerr << exception.what() << "\n";
        }
    }

    return 0;
}
