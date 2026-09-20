#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

#define DATE_PATTERN R"((\d{4})-(\d{2})-(\d{2}))"

#define DATE_REPLACE "$3.$2.$1"

class Validator {
    static std::string
    valid_date(const std::string& text);

    public:
        static std::string
        validate(const std::string& text);
};

#endif
