#include "parser.h"
#include "util.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include <string>

constexpr int HEADER_SCORE = 10;
constexpr int HTMLTAG_SCORE = 5;
constexpr int UNORDERED_SCORE = 2;
constexpr int ORDERED_SCORE = 2;

static size_t
count(const std::string& text, const std::regex& regex)
{
    size_t n = 0;
    for (auto it = std::sregex_iterator(text.begin(), text.end(), regex); it != std::sregex_iterator(); ++it) ++n;
    return n;
}

static const char*
to_string(Format f) {
    switch (f) {
        case Format::AsciiDoc: return "AsciiDoc";
        case Format::Markdown: return "Markdown";
        case Format::Html:     return "HTML";
        case Format::Unknown:  break;
    }
    return "Unknown";
}

Format
Parser::detect(const std::string& text)
{
    // if (is_asciidoc(text)) return Format::AsciiDoc;
    // if (is_markdown(text)) return Format::Markdown;
    // if (is_html(text))     return Format::Html;
    // return Format::Unknown;
    const int md_score = is_markdown(text);
    const int html_score = is_html(text);
    const int adoc_score = is_asciidoc(text);
    const int best = std::max({md_score, html_score, adoc_score});
    if (best == 0) return Format::Unknown;
    if (((md_score == best) + (html_score == best) + (adoc_score == best)) > 1) {
        ERROR("Ambigioues format, return AsciiDoc");
        return Format::AsciiDoc;
    }
    if (md_score == best) return Format::Markdown;
    if (html_score == best) return Format::Html;
    return Format::AsciiDoc;
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

int
Parser::is_html(const std::string& text)
{
    static const std::regex html_header(HTML_HEADER_SIGNS, std::regex::multiline);
    static const std::regex html_paragraph(HTML_PARAGRAPH_SIGNS, std::regex::multiline);
    static const std::regex html_list(HTML_LIST_SIGNS, std::regex::multiline);
    return HTMLTAG_SCORE * count(text, html_header) + HTMLTAG_SCORE * count(text, html_paragraph) + HTMLTAG_SCORE * count(text, html_list);
}

int
Parser::is_markdown(const std::string& text)
{
    static const std::regex md_header(MD_HEADER_SIGNS, std::regex::multiline);
    static const std::regex md_unordered(MD_UNORDERED_SIGNS, std::regex::multiline);
    static const std::regex md_ordered(MD_ORDERED_SIGNS, std::regex::multiline);
    return HEADER_SCORE * count(text, md_header) + UNORDERED_SCORE * count(text, md_unordered) + ORDERED_SCORE * count(text, md_ordered);
}

int
Parser::is_asciidoc(const std::string& text)
{
    static const std::regex doc_header(DOC_HEADER_SIGNS, std::regex::multiline);
    static const std::regex doc_list(DOC_LIST_SIGNS, std::regex::multiline);
    return HEADER_SCORE * count(text, doc_header) + UNORDERED_SCORE * count(text, doc_list);
}

std::vector<Element>
Parser::find(const std::string& text, Type type)
{
    std::vector<Element> result;
    static const std::regex doc_header(DOC_HEADER_SIGNS, std::regex::multiline);
    static const std::regex doc_list(DOC_LIST_SIGNS, std::regex::multiline);
    const std::regex* rx;
    switch (type) {
        case Type::Header: rx = &doc_header; break;
        case Type::List: rx = &doc_list; break;
    }
    std::istringstream iss(text);
    std::string line;
    size_t num = 0;
    while(std::getline(iss, line)) {
        ++num;
        std::smatch match;
        if (!std::regex_match(line, match, *rx)) continue;
        Element element = {
           .type = type,
           .text = "",
           .line = num,
           .level = 0,
        };
        if (type == Type::Header) {
            uint8_t lvl = 0;
            for (char c : line) {
                if (c == '=') ++lvl;
                else if (c == ' ' || c == '\t') break;
            }
            element.level = lvl;
        }
        for (size_t g = match.size(); g-- > 1; ) {
            if (match[g].matched && !match[g].str().empty()) {
                element.text = match[g].str();
                break;
            }
        }

        result.push_back(std::move(element));
    }
    return result;
}
