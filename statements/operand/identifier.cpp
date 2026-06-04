#include "identifier.hpp"

identifier::identifier(std::string name) : name(name) {
}

const std::string& identifier::get_name() const {
    return name;
}

std::string identifier::to_string() const {
    return name;
}
