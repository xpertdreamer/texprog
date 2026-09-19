#include "parser.h"
#include <regex>

bool
Parser::validate(const std::string& text)
{

}

bool
Parser::is_html(const std::string& text)
{
    const std::regex html_tags(HTML_SIGNS);
    return std::regex_search(text, html_tags);
}
