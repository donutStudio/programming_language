#pragma once

#include <string>

class identifier {
    public:
        identifier(std::string name);

        const std::string& get_name() const;
        std::string to_string() const;

    private:
        std::string name;
};
