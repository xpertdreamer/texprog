#include "parser.h"
#include "util.h"

#include <regex>

static const char*
to_string(Format f) {
    switch (f) {
        case Format::Html:     return "HTML";
        case Format::AsciiDoc: return "AsciiDoc";
        case Format::Markdown: return "Markdown";
        case Format::Unknown:  break;
    }
    return "Unknown";
}

Format
Parser::detect(const std::string& text)
{
    if (is_html(text))     return Format::Html;
    if (is_markdown(text)) return Format::Markdown;
    if (is_asciidoc(text)) return Format::AsciiDoc;
    return Format::Unknown;
}

bool
Parser::validate(const std::string& text, Format goal)
{
    const Format have = detect(text);
    if (have == Format::Unknown) {
        ERROR("Unknown file format provided");
        return false;
    }
    if (have != goal) {
        ERROR("Format mismatch\texpected=%s\tgot=%s", to_string(goal), to_string(have));
        return false;
    }
    return true;
}

bool
Parser::is_html(const std::string& text)
{
    static const std::regex html_header(HTML_HEADER_SIGNS, std::regex::multiline);
    static const std::regex html_paragraph(HTML_PARAGRAPH_SIGNS, std::regex::multiline);
    static const std::regex html_list(HTML_LIST_SIGNS, std::regex::multiline);
    return std::regex_search(text, html_header) || std::regex_search(text, html_paragraph) || std::regex_search(text, html_list);
}

bool
Parser::is_markdown(const std::string& text)
{
    static const std::regex md_header(MD_HEADER_SIGNS, std::regex::multiline);
    static const std::regex md_unordered(MD_UNORDERED_SIGNS, std::regex::multiline);
    static const std::regex md_ordered(MD_ORDERED_SIGNS, std::regex::multiline);
    return std::regex_search(text, md_header) || std::regex_search(text, md_unordered) || std::regex_search(text, md_ordered);
}

bool
Parser::is_asciidoc(const std::string& text)
{
    static const std::regex doc_header(DOC_HEADER_SIGNS, std::regex::multiline);
    static const std::regex doc_list(DOC_LIST_SIGNS, std::regex::multiline);
    return std::regex_search(text, doc_header) || std::regex_search(text, doc_list);
}
