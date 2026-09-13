#include "stl.h"

void primes(std::vector<int>& container) {
    for (auto& i : container) {
        if (i <= 1) continue;
        bool is_prime = true;
        for (int j = 2; j < i; ++j ) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) i *= i;
    }
}
