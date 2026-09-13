/**
 * @file stl.h
 * @brief STL-based algorithms for integer container manipulation.
 */

#ifndef STL_H
#define STL_H

#include <utility>
#include <vector>
#include <set>

/**
 * @brief Replaces each prime number in the container with its square.
 * @param container Vector of integers to process, modified in place.
 */
void primes(std::vector<int>& container);

/**
 * @brief Sorts the container in ascending order.
 * @param container Vector of integers to sort, modified in place.
 */
void sort(std::vector<int>& container);

/**
 * @brief Returns all elements of the container within the given range.
 * @param container Vector of integers to search.
 * @param range Pair representing the inclusive range [first, second].
 * @return Vector containing elements that fall within the range.
 */
std::vector<int> find(std::vector<int>& container, std::pair<int, int> range);

#endif
