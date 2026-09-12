#include "counter.h"

int main(void) {
    try { Counter counter("lab_1/testcases/war.txt"); counter.tokenize(); counter.print_tokens(); }
    catch(std::exception & e) {
      ERROR("Error occured: %s", e.what());
      return 1;
    }
    return 0;
}
