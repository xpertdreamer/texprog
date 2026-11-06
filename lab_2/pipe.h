#ifndef PIPE_H
#define PIPE_H

#include "parser.h"

#include <cstddef>
#include <vector>
#include <filesystem>

namespace stdfs = std::filesystem;

constexpr std::size_t BATCH = 512 * 1024 * 1024;

// elems + code
std::pair<const std::vector<Element>&, int>
process(stdfs::path);

#endif
