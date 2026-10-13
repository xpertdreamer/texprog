#include "parser.h"
#include "uploader.h"

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

int
main(int argc, char* argv[])
{
    constexpr std::size_t BATCH = 1024 * 1024 * 100;
    std::string path;
    if (argc > 1) {
        path = std::string(argv[1]);
    }
    try {
        Uploader up(path, BATCH);
        std::vector<Element> res;
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
