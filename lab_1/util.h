#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <iostream>

#ifndef _WIN32
#define ERROR_COLOR "\e[1;31m"
#define DEBUG_COLOR "\e[1;34m"
#define RESET_COLOR "\e[0m"
#else
#define ERROR_COLOR ""
#define DEBUG_COLOR ""
#define RESET_COLOR ""
#endif

#define DEBUG(fmt, ...)                                             \
    do {                                                            \
        fprintf(stdout, "%sDEBUG:%s ", DEBUG_COLOR, RESET_COLOR);   \
        fprintf(stdout, fmt, ##__VA_ARGS__);                        \
    } while (0)

#define ERROR(fmt, ...)                                                 \
    do {                                                                \
        fprintf(stderr, "%sERROR:%s ", ERROR_COLOR, RESET_COLOR);       \
        fprintf(stderr, fmt, ##__VA_ARGS__);                            \
    } while (0)

#define START(name)                                                            \
  auto start_##name = std::chrono::high_resolution_clock::now();
#define END(name) \
    auto end_##name = std::chrono::high_resolution_clock::now();        \
    auto duration_##name = std::chrono::duration_cast<std::chrono::microseconds>(end_##name - start_##name).count(); \
    std::cout << "Timer [" << #name << "] took: " << duration_##name << " microseconds\n";

#endif
