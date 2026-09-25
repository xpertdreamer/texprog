#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <string>
#include <regex>

/**
 * @enum Format
 * @brief Supported document formats.
 */
enum class Format {
    Html,      /**< HTML format */
    AsciiDoc,  /**< AsciiDoc format */
    Markdown,  /**< Markdown format */
    Unknown    /**< Unknown or undetected format */
};

/**
 * @enum Type
 * @brief Types of parsed document elements.
 */
enum class Type {
    Header,     /**< Header element (h1-h6, #, =) */
    Paragraph,  /**< Paragraph element */
    List        /**< List element (ordered or unordered) */
};

/**
 * @struct Element
 * @brief Represents a single parsed document element.
 */
struct Element {
    Type type;          /**< Type of the element */
    std::string text;   /**< Text content of the element */
    size_t line;        /**< Line number where the element starts */
    uint8_t level;      /**< Nesting/header level (0 if not applicable) */
};

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

/**
 * @def DOC_PARAGRAPH_SIGNS
 * @brief Regular expression for detecting AsciiDoc paragraphs.
 * Matches a line of text surrounded by blank lines, starting with an
 * alphanumeric character (Latin or Cyrillic).
 * @details Regex breakdown:
 *
 * - '^' - start of the line
 * - '\s*' - zero or more whitespaces
 * - '\n' - newline character
 * - '[а-яА-ЯёЁa-zA-Z0-9]' - first character (Cyrillic, Latin or digit)
 * - '[^\n]*' - zero or more characters except newline
 * - '\n' - newline character
 * - '\s*' - zero or more whitespaces
 * - '$' - end of the line
 */
#define DOC_PARAGRAPH_SIGNS R"(^\s*\n[а-яА-ЯёЁa-zA-Z0-9][^\n]*\n\s*$)"

/**
 * @class Parser
 * @brief Provides format detection and element extraction for markup documents.
 */
class Parser {
    /**
     * @brief Checks whether the given text is in HTML format.
     * @param text Input document text
     * @return Confidence score (non-zero if HTML)
     */
    static int
    is_html(const std::string& text);

    /**
     * @brief Checks whether the given text is in Markdown format.
     * @param text Input document text
     * @return Confidence score (non-zero if Markdown)
     */
    static int
    is_markdown(const std::string& text);

    /**
     * @brief Checks whether the given text is in AsciiDoc format.
     * @param text Input document text
     * @return Confidence score (non-zero if AsciiDoc)
     */
    static int
    is_asciidoc(const std::string& text);

    public:
        /**
         * @brief Takes enum format and return its name in c_str format
         * @param f Format returned from detect or other
         * @return The name of given format
         */
        static const char*
        to_string(Format f);
        /**
         * @brief Validates that the text conforms to the given format.
         * @param text Input document text
         * @param goal Expected format to validate against
         * @return true if the text matches the target format, false otherwise
         */
        static bool
        validate(const std::string& text, Format goal);

        /**
         * @brief Detects the format of the given document.
         * @param text Input document text
         * @return Detected Format (Format::Unknown if detection fails)
         */
        static Format
        detect(const std::string& text);

        /**
         * @brief Finds all elements of the specified type in the document.
         * @param text Input document text
         * @param type Type of elements to search for
         * @return Vector of found Element objects
         */
        static std::vector<Element>
        find(const std::string& text, Type type);
};

#endif
