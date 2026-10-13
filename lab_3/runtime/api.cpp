#include "parser.h"
#include "uploader.h"
#include "util.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

extern "C" int
lab2(const char* path)
{
    constexpr std::size_t BATCH = 512 * 1024 * 1024;
    try {
        Uploader up(path, BATCH);
        DEBUG("batch size=%ld", up.get_size());
        std::vector<Element> res;
        DEBUG("Vector res allocated");
        std::size_t line_ofs = 1;
        while (true) {
            std::streamsize read = up.upload_batch();
            if (read <= 0) {
                break;
            }
            std::string text(up.get_buf().data(), read);
            std::vector b = Parser::find(text, Type::Header, line_ofs);
            res.insert(res.end(), b.begin(), b.end());
            line_ofs += static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
        }
        if (up.stream_bad()) {
            ERROR("io error while reading");
            return 1;
        }

        std::cout << "Found:" << std::endl;
        for (const auto &it : res) {
            std::cout << std::left << std::setw(100) << it.text << std::left <<std::setw(10) << "\t" << "line=" << it.line;
            if (it.type == Type::Header) {
                std::cout << std::right << std::setw(10) << "\tlevel=" << static_cast<int>(it.level);
            }
            std::cout << "\n";
        }
        std::cout << "Batch size: " << BATCH << std::endl;
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
