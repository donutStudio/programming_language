#pragma once

#include <string>

class literal {
    public:
        literal(std::string value);

        std::string to_string() const;

    private:
        std::string value;
};
