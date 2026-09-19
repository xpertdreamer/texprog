#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <regex>

#define HTML_SIGNS R"(<\s*\/?\s*(html|body|div|p|span|a)>)"

class Parser {
    static bool
    is_html(const std::string& text);

    static bool
    is_markdown(const std::string& text);

    static bool
    has_asciidoc(const std::string& text);

    public:
        static bool
        validate(const std::string& text);
};

#endif
