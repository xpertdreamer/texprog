#include "counter.h"

int main(void) {
    try { Counter counter("lab_1/testcases/test.txt"); }
    catch(std::exception & e) {
      ERROR("Error occured: %s", e.what());
      return 1;
    }
    return 0;
}
