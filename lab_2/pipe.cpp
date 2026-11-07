#include "pipe.h"
#include "parser.h"
#include "uploader.h"
#include <algorithm>
#include <ios>

std::pair<const std::vector<Element>, int>
process(stdfs::path path)
{
    Uploader up(path, BATCH);
    std::vector<Element> res;
    std::size_t line_ofs = 1;

    while(true) {
        std::streamsize read = up.upload_batch();
        if (read <= 0) break;
        std::string text(up.get_buf().data(), read);
        std::vector temp = Parser::find(text, Type::Header, line_ofs);
        res.insert(res.end(), temp.begin(), temp.end());
        line_ofs += static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
    }

    if (up.stream_bad()) {
        ERROR("io error while reading");
        return {{},-1};
    }

    return {res, 2};
}
