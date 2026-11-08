#ifndef WATCHER_H
#define WATCHER_H

#include "task.h"
#include "tdsqueue.h"
#include "parser.h"

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_set>
#include <atomic>
#include <future>

enum class Mode { threads, async };

using result = std::pair<std::vector<Element>, int>;

namespace stdfs = std::filesystem;

constexpr unsigned short THREADS = 2;
constexpr unsigned short MXASYNC = 2;

class Watcher {
    private:
        Mode mode = Mode::threads;
        std::atomic<bool> run = {false};
        std::mutex mutex;

        stdfs::path watch_path;
        std::map<stdfs::path, std::pair<std::vector<Element>, task_status>> cache;
        std::map<stdfs::path, std::shared_ptr<std::atomic<bool>>> cancels;
        TdSQueue<Task> queue;

        std::unique_ptr<std::thread> loop_thread;
        std::vector<std::thread> workers;

        void scan();

        void handle_file(const stdfs::path& fpath);

        void loop();

        void worker();

        void worker_threads();

        void worker_async();

        struct pending_result {
            std::future<result> future;
            stdfs::path path;
        };

        std::vector<pending_result> pending;

    public:
        void start(const stdfs::path& wpath, Mode m);

        void stop();

        void exit();

        bool change_directory(const stdfs::path& npath);

        void dump_state();

        std::vector<Element> get_res(const stdfs::path& rpath);

        void drop_queue();

        std::optional<task_status> get_stat(const stdfs::path& rpath);

        void cancel(const stdfs::path& path);
};

#endif
