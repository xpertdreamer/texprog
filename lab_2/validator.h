#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

/**
 * @def DATE_PATTERN
 * @brief Regular expression for detecting dates.
 * Matches dates of the form YYYY-MM-DD with three capture groups.
 * @details Regex breakdown:
 *
 * - '\d{4}' - four digits (year)
 * - '-' - literal '-'
 * - '\d{2}' - two digits (month)
 * - '-' - literal '-'
 * - '\d{2}' - two digits (day)
 */
#define DATE_PATTERN LR"((\d{4})-(\d{2})-(\d{2}))"

/**
 * @def DATE_REPLACE
 * @brief Replacement template for reformatting dates to DD.MM.YYYY.
 * @details Reverse the capture group order:
 *
 * - '$3' - day
 * - '.' - literal '.'
 * - '$2' - month
 * - '.' - literal '.'
 * - '$1' - year
 */
#define DATE_REPLACE L"$3.$2.$1"

/**
 * @def PHONE_PATTERN
 * @brief Regular expression for detecting phone numbers.
 * Matches numbers starting with '+' and containing digits.
 * @details Regex breakdown:
 *
 * - '\+' - literal '+'
 * - '\d' - a single digit
 * - '[\d-]*' - zero or more digits or hyphens
 * - '\d' - a single digit
 */
#define PHONE_PATTERN LR"((\+\d[\d-]*\d))"

/**
 * @def NAME_PATTERN
 * @brief Regular expression for detecting full names.
 * Matches three whitespace-separated words.
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '(\S+)' - one or more non-whitespace characters (first word)
 * - '\s+' - one or more whitespaces
 * - '(\S+)' - one or more non-whitespace characters (second word)
 * - '\s+' - one or more whitespaces
 * - '(\S+)' - one or more non-whitespace characters (third word)
 */
#define NAME_PATTERN LR"((^\S+)\s+(\S+)\s+(\S+))"

/**
 * @def EMAIL_PATTERN
 * @brief Regular expression for detecting email addresses.
 * Matches typical email addresses.
 * @details Regex breakdown:
 *
 * - '[A-Za-z0-9._%+-]+' - one or more allowed local-part characters
 * - '@' - literal '@'
 * - '[A-Za-z0-9.-]+' - one or more domain characters
 * - '\.' - literal '.'
 * - '[A-Za-z]{2,}' - two or more letters (TLD)
 */
#define EMAIL_PATTERN LR"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})"

/**
 * @class Validator
 * @brief Provides text validation and normalization for personal data fields.
 */
class Validator {
    /**
     * @brief Validates and normalizes a date from ISO format to DD.MM.YYYY.
     * @param text Input text containing a date
     * @return Normalized date string
     */
    static std::wstring
    valid_date(const std::wstring& text);

    /**
     * @brief Validates and normalizes a phone number.
     * @param text Input text containing a phone number
     * @return Normalized phone number string
     */
    static std::wstring
    valid_phone(const std::wstring& text);

    /**
     * @brief Validates a full name.
     * @param text Input text containing a full name
     * @return Validated name string
     */
    static std::wstring
    valid_name(const std::wstring& text);

    /**
     * @brief Validates an email address.
     * @param text Input text containing an email address
     * @return Validated email string
     */
    static std::wstring
    valid_email(const std::wstring& text);

    public:
        /**
         * @brief Applies all validators to the given text.
         * @param text Input text to validate
         * @return Text with all recognized fields validated and normalized
         */
        static std::wstring
        validate(const std::wstring& text);
};

#endif
