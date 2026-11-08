#include "pipe.h"
#include "parser.h"
#include "uploader.h"
#include <algorithm>
#include <ios>
#include <iterator>
#include <optional>

typedef struct Batch {
    std::string text;
    std::size_t lines = 0;
} batch;

std::optional<Batch> // rust style error handling is pretty good
read_batch(Uploader& u)
{
    std::streamsize read = u.upload_batch();
    if (read <= 0) return std::nullopt;

    Batch b;
    b.text.assign(u.get_buf().data(), (std::size_t)read);
    b.lines = (std::size_t)(std::count(b.text.begin(), b.text.end(), '\n'));
    return b;
}

static std::vector<Element>
parse_batch(const batch& b, std::size_t offset)
{
    return Parser::find(b.text, Type::Header, offset);
}

std::pair<std::vector<Element>, int>
process(const stdfs::path& path, const std::atomic<bool>& run)
{
    Uploader up(path, BATCH);
    std::vector<Element> res;
    std::size_t line_ofs = 1;

    while(run) {
        auto b = read_batch(up);
        if (!b) break;

        auto parsed = parse_batch(*b, line_ofs);
        res.insert(res.end(), std::make_move_iterator(parsed.begin()), std::make_move_iterator(parsed.end()));
        line_ofs += b->lines;
    }

    if (!run) return {{}, 0};
    if (up.stream_bad()) {
        ERROR("io error while reading");
        return {{},-1};
    }
    return {std::move(res), 2};
}
