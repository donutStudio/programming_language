#pragma once

#include <string>
#include <variant>

using literal_value = std::variant<int, bool>;

class literal {
    public:
        literal(int value);
        literal(bool value);

        const literal_value& get_value() const;
        std::string to_string() const;

    private:
        literal_value value;
};
