#include <iostream>
#include <string>
#include "parser.hpp"

int main() {
    std::string input;
    
    while (true) {
        std::cin >> input;
        string_to_expression(input);
    }

    return 0;
}