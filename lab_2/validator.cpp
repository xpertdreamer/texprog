#include "validator.h"
#include "util.h"

#include <regex>
#include <string>

std::wstring
Validator::valid_date(const std::wstring& text)
{
    DEBUG("Call valid_date");
    static const std::wregex pattern(DATE_PATTERN);
    std::wsmatch match;
    if (!std::regex_search(text, match, pattern)) return L"";
    return match.str(3) + L"." + match.str(2) + L"." + match.str(1);
}

std::wstring
Validator::valid_phone(const std::wstring& text)
{
    DEBUG("Call valid_phone");
    static const std::wregex pattern(PHONE_PATTERN);
    std::wsmatch match;
    if (!std::regex_search(text, match, pattern)) return L"";
    return std::regex_replace(match.str(), std::wregex(L"-"), L"");
}

}
