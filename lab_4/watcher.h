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
        std::atomic<bool> run = {false};
        std::mutex mutex;

        stdfs::path watch_path;
        std::map<stdfs::path, std::pair<std::vector<Element>, task_status>> cache;
        TdSQueue<Task> queue;

        std::unique_ptr<std::thread> loop_thread;
        std::vector<std::thread> workers;

        void scan();

        void handle_file(const stdfs::path& fpath);

        void loop();

        void worker();

    public:
        void start(const stdfs::path& wpath);

        void stop();

        bool change_directory(const stdfs::path& npath);

        void dump_state();

        std::vector<Element> get_res(const stdfs::path& rpath);

        void drop_queue();
};

#endif
