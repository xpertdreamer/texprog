#ifndef WATCHER_H
#define WATCHER_H

#include "task.h"
#include "tdsqueue.h"
#include <filesystem>
#include <map>
#include <mutex>
#include <unordered_set>

namespace stdfs = std::filesystem;

// TODO: results struct


class Watcher {
    private:
        std::map<stdfs::path, task_status> cache;

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

        bool loop();

        void dump_queue();
};

#endif
