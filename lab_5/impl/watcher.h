/**
 * @file watcher.h
 * @brief This file contains the declaration of the Watcher class, which monitors
 * a directory, queues files for processing and collects results.
 */


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

/**
 * @enum Mode
 * @brief Execution strategy for processing queued files.
 */
enum class Mode { threads, async };

using result = std::pair<std::vector<Element>, int>;

namespace stdfs = std::filesystem;

constexpr unsigned short THREADS = 2;
constexpr unsigned short MXASYNC = 2;

/**
 * @class Watcher
 * @brief Watches a directory, queues new/failed files and processes them
 * using either a thread pool or std::async.
 */
class Watcher {
    private:
        Mode mode = Mode::threads; ///< Current execution mode
        std::atomic<bool> run = {false}; ///< Main run flag for all loops
        std::mutex mutex;  ///< Guards access

        stdfs::path watch_path; ///< Directory currently being watched
        std::map<stdfs::path, std::pair<std::vector<Element>, task_status>> cache; ///< Per-file results and status
        std::map<stdfs::path, std::shared_ptr<std::atomic<bool>>> cancels; ///< Cancel flags for running tasks
        TdSQueue<Task> queue; ///< Pending tasks

        std::unique_ptr<std::thread> loop_thread; ///< Directory scanning thread
        std::vector<std::thread> workers; ///< Worker thread

        /**
         * @brief Scan the watched directory and enqueue regular files.
         */
        void scan();

        /**
         * @brief Validate a file, enqueue it if new or previously failed.
         * @param fpath Path of the file to handle
         */
        void handle_file(const stdfs::path& fpath) noexcept(false);

         /**
         * @brief Main scanning loop; calls scan() periodically until run is false.
         */
        void loop();

        /**
         * @brief Entry point of a worker thread; dispatches by mode.
         */
        void worker();

        /**
         * @brief Worker loop for Mode::threads (uses shared task queue).
         */
        void worker_threads();

        /**
         * @brief Worker loop for Mode::async (uses per-task futures).
         */
        void worker_async();

        /**
         * @struct pending_result
         * @brief A future representing an async task and its path.
         */
        struct pending_result {
            std::future<result> future;
            stdfs::path path;
        };

        std::vector<pending_result> pending; ///< Async tasks

    public:
        /**
         * @brief Start watching a directory.
         * @param wpath Directory to watch
         * @param m Execution mode
         * @note Calls stop() before starting.
         */
        void start(const stdfs::path& wpath, Mode m);

        /**
         * @brief Stop all threads and clear internal state.
         * @note Idempotent; safe to call multiple times.
         */
        void stop();

        /**
         * @brief Switch the watched directory.
         * @param npath New directory
         * @return @p true on success, @p false if path is not an existing directory
         * @note Clears cache and queue, then performs an immediate scan.
         */
        bool change_directory(const stdfs::path& npath);

        /**
         * @brief Print current cache and queue contents to stdout.
         */
        void dump_state();

        /**
         * @brief Get cached processing results for a path.
         * @param rpath File path
         * @return Cached elements, or an empty vector if the path is not tracked
         */
        std::vector<Element> get_res(const stdfs::path& rpath);

        /**
         * @brief Drop all pending tasks from the queue.
         */
        void drop_queue();

        /**
         * @brief Get the status of a tracked path.
         * @param rpath File path
         * @return Status if the path is tracked, std::nullopt otherwise
         */
        std::optional<task_status> get_stat(const stdfs::path& rpath);

        /**
         * @brief Request cancellation of a running task.
         * @param path File path
         * @note Sets the task's cancel flag; only affects tasks in running state.
         */
        void cancel(const stdfs::path& path);
};

#endif
