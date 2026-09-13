#include "stl.h"
#include "util.h"

void primes(std::vector<int>& container) {
    START(primes);
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
    END(primes);
}

void sort(std::vector<int>& container) {
    START(sort);
    long long len = static_cast<long long>(container.size());
    long long i, j, gap;
    for (gap = len / 2; gap > 0; gap = gap / 2)
        for (i = gap; i < len; i++)
            for (j = i - gap; j >= 0 && container[j] > container[j+gap]; j = j - gap)
                std::swap(container[j], container[j+gap]);
    END(sort);
}

std::vector<int> find(std::vector<int>& container, std::pair<int, int> range) {
    START(find);
    if (range.first == range.second || range.first > range.second) {
        ERROR("Range is invalid: %d < x < %d\n", range.first, range.second);
        return {};
    }
    std::set<int> res;
    for (const auto& elem : container) {
        if (elem >= range.first && elem <= range.second) res.insert(elem);
    }
    END(find);
    return std::vector<int>(res.begin(), res.end());
}
