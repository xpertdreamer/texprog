#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>

#include "util.h"

#define TEST 1 // 0/1

class Counter {
private:
  std::ifstream input;
  std::stringstream buf_stream;
  std::vector<std::string> tokens;
  std::unordered_map<std::string, std::size_t> pairs;

public:
  inline Counter() { ERROR("No input file providen!\n"); }

  inline Counter(std::string path) {
    DEBUG("PATH: %s\n", path.c_str());
    input.open(path);
    if (!input) {
      ERROR("Cannot open input file!\n");
      throw std::invalid_argument("Counter constructor, Given input is invalid!\n");
    }
    DEBUG("File %s successfuly opened\n", path.c_str());
    // jump to the end of file
    input.seekg(0, std::ios::end);
    std::streampos len = input.tellg();
    // jump to the end of file
    input.seekg(0, std::ios::beg);
    std::vector<char> buffer(len);
    // read file to temporary buffer (faster than just .rdbuf() method according to StackOverflow)
    input.read(&buffer[0], len);
    // rdbuf returns pointer to file buffer
    buf_stream.rdbuf()->pubsetbuf(&buffer[0], len);
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer\n",
          path.c_str());
    #if TEST
    std::cout << buf_stream.str();
    #endif
  }

  inline ~Counter() {
    DEBUG("Counter: destructor closed\n");
  }

  // TODO: getters and setters

  inline void print_counted() const {
    for (auto pair : pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
  }

  // TODO: tokenization method
  bool tokenize();
};
