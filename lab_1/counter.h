#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>
#include "util.h"

class Counter {
private:
  std::ifstream input;
  std::vector<std::pair<std::string, size_t>> pairs;

public:
  inline Counter() { ERROR("No input file providen!\n"); }

  inline Counter(std::string path) {
    input.open(path);
    if (!input.is_open()) {
      ERROR("Cannot open input file!\n");
      throw std::invalid_argument("Counter constructor, Given input is invalid!\n");
    }

    DEBUG("File %s successfuly opened\n", path.c_str());
  }

  inline ~Counter() {
    input.close();
    DEBUG("Counter: input closed\n");
  }

  inline void print_counted() {
    for (auto pair : pairs) {
        std::cout << pair.first << " - " << pair.second << std::endl;
    }
  }


};
