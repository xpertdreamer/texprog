#ifndef WATCHER_H
#define WATCHER_H

#include "task.h"
#include "tdsqueue.h"

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

        std::map<stdfs::path, task_status> cache;

        std::atomic<bool> run = {false};

        std::vector<std::thread> workers;

        stdfs::path watch_path;
        // mutex
        std::mutex mutex;
        // new file callback

        TdSQueue<Task> queue;

        void scan();

        void file_handler(const stdfs::path& path);

    public:
        void start(const stdfs::path& wpath);

        void stop();

        bool change_directory(const stdfs::path& npath);

        void loop();

        TdSQueue<Task>& get_queue();

        std::map<stdfs::path, task_status>& get_cache();

        void worker();

        ~Watcher() {stop();}
};

#endif
