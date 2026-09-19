#include "uploader.h"
#include "util.h"
#include <fstream>
#include <sstream>

std::string
Uploader::upload_file(const std::string& path)
{
    DEBUG("Call upload_file");
    DEBUG("PATH: %s", path.c_str());
    std::ifstream input;
    input.open(path);
    if (!input) {
        ERROR("Cannot open input gile!");
        throw std::invalid_argument("upload_file: Given input is invalid!\n");
    }
    DEBUG("File %s successfuly opened\n", path.c_str());
    std::stringstream ss;
    ss << input.rdbuf();
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer\n",
          path.c_str());
    #if TEST
    std::cout << ss.str();
    ss.clear();
    ss.seekg(0, std::ios::beg);
    #endif
    return ss.str();
}

std::string
Uploader::upload_stdin()
{
    DEBUG("Call upload_stdin");
    std::stringstream ss;
    ss << std::cin.rdbuf();
    #if TEST
    std::cout << ss.str();
    ss.clear();
    ss.seekg(0, std::ios::beg);
    #endif
    DEBUG("Input successfully readen");
    return ss.str();
}
