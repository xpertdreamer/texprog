#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

#define DATE_PATTERN LR"((\d{4})-(\d{2})-(\d{2}))"

#define DATE_REPLACE L"$3.$2.$1"

#define PHONE_PATTERN LR"((\+\d[\d-]*\d))"

#define NAME_PATTERN LR"((^\S+)\s+(\S+)\s+(\S+))"

#define EMAIL_PATTERN LR"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})"

class Validator {
    static std::wstring
    valid_date(const std::wstring& text);

    static std::wstring
    valid_phone(const std::wstring& text);

    static std::wstring
    valid_name(const std::wstring& text);

    static std::wstring
    valid_email(const std::wstring& text);

    public:
        static std::wstring
        validate(const std::wstring& text);
};

#endif
