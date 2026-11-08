#ifndef PIPE_H
#define PIPE_H

#include "parser.h"

#include <cstddef>
#include <vector>
#include <filesystem>
#include <atomic>

namespace stdfs = std::filesystem;

constexpr std::size_t BATCH = 512 * 1024 * 1024;

// elems + code
std::pair<std::vector<Element>, int>
process(const stdfs::path& path, const std::atomic<bool>& run);

#endif
