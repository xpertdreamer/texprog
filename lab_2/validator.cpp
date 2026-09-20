#include "validator.h"

#include <regex>

std::string
Validator::valid_date(const std::string& text)
{
    std::regex pattern(DATE_PATTERN);
    return std::regex_replace(text, pattern, DATE_REPLACE);
}
