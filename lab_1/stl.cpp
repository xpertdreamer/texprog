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

void sort(std::vector<int>& container) {
    size_t len = container.size();
    int i, j, gap;
    for (gap = len / 2; gap > 0; gap = gap / 2)
        for (i = gap; i < len; i++)
            for (j = i - gap; j >= 0 && container[j] > container[j+gap]; j = j - gap)
                std::swap(container[j], container[j+gap]);
}
