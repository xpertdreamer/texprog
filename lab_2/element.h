#include "util.h"

#include <cstdint>
#include <string>
#include <vector>

class Element {
    public:
        virtual ~Element() = default;
};

class Header : public Element {
    private:
        uint8_t level = 1;
        std::string text = "";
    public:
        inline Header(uint8_t l, const std::string& t) : level(l), text(t) { DEBUG("Header initialized\tlevel=%u\ttext=%s", l, t.c_str()); }
};

class Paragraph : public Element {
    private:
        std::string text = "";
    public:
        inline Paragraph(const std::string& t) : text(t) { DEBUG("Paragraph initialized\ttext=%s", text.c_str()); }
};

struct ListElement {
    std::string text = "";
    uint8_t level = 1;
};

class List : public Element {
    private:
        std::vector<ListElement> elements;
        bool ordered = false;
    public:
};
