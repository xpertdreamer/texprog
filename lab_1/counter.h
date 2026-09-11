#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>
#include "util.h"

class Counter {
private:
  std::ifstream input;
  std::stringstream buf;
  std::unordered_map<size_t, std::string> pairs;

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
    buf << input.rdbuf();
    input.close();
    DEBUG("File %s successfully closed, and input sent to the buffer\n", path.c_str());
  }

  inline ~Counter() {
    DEBUG("Counter: destructor closed\n");
  }

  inline void print_counted() {
    for (auto pair : pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
  }
};
