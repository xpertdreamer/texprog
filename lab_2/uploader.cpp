#include "uploader.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <string>

std::string
Uploader::upload_file(const std::string& path)
{
    START(upload_file);
    DEBUG("Call upload_file");
    DEBUG("PATH: %s", path.c_str());
    std::ifstream input;
    input.open(path);
    if (!input) {
        ERROR("Cannot open input gile!");
        throw std::invalid_argument("upload_file: Given input is invalid!\n");
    }
    DEBUG("File %s successfuly opened", path.c_str());
    std::stringstream ss;
    ss << input.rdbuf();
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer",
          path.c_str());
    #if TEST
    std::cout << ss.str();
    ss.clear();
    ss.seekg(0, std::ios::beg);
    #endif
    END(upload_file);
    return ss.str();
}

std::wstring
Uploader::upload_stdin()
{
    START(upload_stdin);
    DEBUG("Call upload_stdin");
    std::wstringstream ss;
    ss << std::wcin.rdbuf();
    #if TEST
    std::wcout << ss.str();
    ss.clear();
    ss.seek(0, std::ios::beg);
    #endif
    DEBUG("Input successfully readen");
    END(upload_stdin);
    return ss.str();
}
