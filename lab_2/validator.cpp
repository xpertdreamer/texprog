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

std::wstring
Validator::valid_name(const std::wstring& text)
{
    DEBUG("Call valid_name");
    static const std::wregex pattern(NAME_PATTERN);
    std::wsmatch match;
    if (!std::regex_search(text, match, pattern)) return L"";
    std::wstring raw = match.str();
    static const std::wregex junk(LR"([^А-Яа-яЁё])");
    std::wstring result;
    for (int i = 1; i <= 3; ++i) {
        std::wstring w = std::regex_replace(match.str(i), junk, L"");
        if (w.empty()) continue;
        w[0] = std::towupper(w[0]);
        for (std::size_t j = 1; j < w.size(); ++j)
            w[j] = std::towlower(w[j]);
        if (!result.empty()) result += L' ';
        result += w;
    }
    return result;
}

std::wstring
Validator::valid_email(const std::wstring& text)
{
    DEBUG("Call valid_email");
    static const std::wregex pattern(EMAIL_PATTERN);
    std::wsmatch match;
    if (!std::regex_search(text, match, pattern)) return L"";
    std::wstring lower = match.str();
    for (auto& c : lower) c = std::towlower(c);
    return lower;
}

std::wstring
Validator::validate(const std::wstring &text)
{
    DEBUG("Call validate");
    std::wstring name = valid_name(text);
    std::wstring email = valid_email(text);
    std::wstring phone = valid_phone(text);
    std::wstring date = valid_date(text);
    return name + L" " + email + L" "  + phone + L" " + date;
}
