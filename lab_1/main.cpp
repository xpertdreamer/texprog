#include "counter.h"

int main(void) {
    try { Counter counter("lab_1/testcases/war.txt"); counter.tokenize(); counter.count(); counter.print_counted(); }
    catch(std::exception & e) {
      ERROR("Error occured: %s", e.what());
      return 1;
    }
    return 0;
}
