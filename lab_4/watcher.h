#ifndef WATCHER_H
#define WATCHER_H

#include "task.h"
#include "tdsqueue.h"
#include "parser.h"

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <atomic>

namespace stdfs = std::filesystem;

constexpr unsigned short THREADS = 2;

class Watcher {
    private:
        std::unique_ptr<std::thread> loop_thread;

        std::map<stdfs::path, std::pair<std::vector<Element>, task_status>> cache;

        std::atomic<bool> run = {false};

        std::vector<std::thread> workers;

        stdfs::path watch_path;
        // mutex
        std::mutex mutex;
        // new file callback

    public:

        ~Watcher() { stop(); }
};

#endif
