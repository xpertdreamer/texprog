#include "parser.h"
#include <regex>

bool
Parser::validate(const std::string& text)
{

}

bool
Parser::is_html(const std::string& text)
{
    const std::regex html_header(HTML_HEADER_SIGNS);
    const std::regex html_paragraph(HTML_PARAGRAPH_SIGNS);
    const std::regex html_list(HTML_LIST_SIGNS);
    return std::regex_search(text, html_header) || std::regex_search(text, html_paragraph) || std::regex_search(text, html_list);
}

bool
Parser::is_markdown(const std::string& text)
{
    const std::regex md_header(MD_HEADER_SIGNS);
    const std::regex md_unordered(MD_ORDERED_SIGNS);
    const std::regex md_ordered(MD_ORDERED_SIGNS);
    return std::regex_search(text, md_header) || std::regex_search(text, md_unordered) || std::regex_search(text, md_ordered);
}

bool
Parser::is_asciidoc(const std::string& text)
{
    const std::regex doc_header(DOC_HEADER_SIGNS);
    const std::regex doc_list(DOC_LIST_SIGNS);
    return std::regex_search(text, doc_header) || std::regex_search(text, doc_list);
}
