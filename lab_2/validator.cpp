#include "validator.h"

#include <regex>

std::string
Validator::valid_date(const std::string& text)
{
    std::regex pattern(DATE_PATTERN);
    return std::regex_replace(text, pattern, DATE_REPLACE);
}

std::string
Validator::valid_phone(const std::string& text)
{
    std::regex pattern(PHONE_VALIDATOR);
    std::smatch match;
    if (!std::regex_search(text, match, pattern)) return text;
    std::string res = match.str();
    res.erase(std::remove(res.begin(), res.end(), '-'), res.end());
    return std::regex_replace(text, pattern, res);
}
