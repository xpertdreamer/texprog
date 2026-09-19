#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <regex>

/**
 * @def HTML_HEADER_SIGNS
 * @brief Regular expression for detecting HTML header tags.
 * Matches opening and closing header tags from h1 to h6.
 * @details Regex breakdown:
 *
 * - '\s*' - zero or more whitspaces
 * - '<h[1-6]' - literal <h followed by 1-6
 * - '[^>]*' - zero or more characters (not >)
 * - '>' - closing bracket literal
 * - '|' - OR
 * - '<\/h[1-6]>' - literal closing tag
 */
#define HTML_HEADER_SIGNS    R"(\s*<h[1-6][^>]*>|<\/h[1-6]>)"

/**
 * @def HTML_PARAGRAPH_SIGNS
 * @brief Regular expression for detecting HTML paragraph tags.
 * Matches opening and closing 'p' tags.
 * @details Regex breakdown:
 *
 * - '\s*' - zero or more whitspaces
 * - '<p' - literal itself
 * - '[^>]*' - zero or more characters (not >)
 * - '>' - closing bracket literal
 * - '|' - OR
 * - '<\/p>' - literal closing tag
 */
#define HTML_PARAGRAPH_SIGNS R"(\s*<p[^>]*>|<\/p>)"

/**
 * @def HTML_LIST_SIGNS
 * @brief Regular expression for detecting HTML list tags.
 * Matches opening and closing tags of unordered and ordered lists.
 * @details Regex breakdown:
 *
 * - '\s*' - zero or more whitspaces
 * - '<' - literal itself
 * - '(ul|ol|li)' - literal group (tag itself)
 * - '[^>]*' - zero or more characters (not >)
 * - '>' - closing bracket literal
 * - '|' - OR
 * - '<\/(il|ol|li)>' - literal closing tag
 */
#define HTML_LIST_SIGNS      R"(\s*<(ul|ol|li)[^>]*>|<\/(ul|ol|li)>)"

/**
 * @def MD_HEADER_SIGNS
 * @brief Regular expression for detecting Markdown headers.
 * Matches Markdown headers with levels 1-6
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '(#{1,6})' - literal '#' from 1 to 6 times in a row
 * - '\s+' - one or more whitespaces
 * - '(.+)' - any header text (at least one character)
 * - '$' - end of the line
 */
#define MD_HEADER_SIGNS      R"(^(#{1,6})\s+(.+)$)"

/**
 * @def MD_UNORDERED_SIGNS
 * @brief Regular expression for detecting Markdown unordered lists.
 * Matches Markdown lines starting with '-', '+' or '*'.
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '\s*' - zero or more whitspaces
 * - '[-+*]' - any bullet marker
 * - '(.+)' - any text (at least one character)
 * - '$' - end of the line
 */
#define MD_UNORDERED_SIGNS   R"(^\s*[-+*]\s+(.+)$)"

/**
 * @def MD_ORDERED_SIGNS
 * @brief Regular expression for detecting Markdown ordered lists.
 * Matches Markdown lines starting with number following by dot and whitespace
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '\s*' - zero or more whitspaces
 * - '\d+' - one or more digits
 * - '\.' - literal '.'
 * - '(.+)' - any text (at least one character)
 * - '$' - end of the line
 */
#define MD_ORDERED_SIGNS     R"(^\s*\d+\.\s+(.+)$)"

/**
 * @def DOC_HEADER_SIGNS
 * @brief Regular expression for detecting AsciiDoc headers.
 * Matches AsciiDoc lines starting with literal '=' repeated between 1-6 times.
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '={1,6}' - literal '=' repeated 1-6 times
 * - '\s+' - one or more whitespaces
 * - '(.+)' - any text (at least one character)
 * - '$' - end of the line
 */
#define DOC_HEADER_SIGNS     R"(^={1,6}\s+(.+)$)"

/**
 * @def DOC_LIST_SIGNS
 * @brief Regular expression for detecting AsciiDoc lists.
 * Matches AsciiDoc lines starting with literals '*', '-' or '.' following by text.
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '\s*' - zero or more whitespaces
 * - '([*\-]|\.)' - literals '*', '-' or '.'
 * - '\s+' - one or more whitespaces
 * - '(.+)' - any text (at least one character)
 * - '$' - end of the line
 */
#define DOC_LIST_SIGNS       R"(^\s*([*\-]|\.)\s+(.+)$)"

class Parser {
    static bool
    is_html(const std::string& text);

    static bool
    is_markdown(const std::string& text);

    static bool
    is_asciidoc(const std::string& text);

    public:
        static bool
        validate(const std::string& text);

        // TODO: parse
};

#endif
