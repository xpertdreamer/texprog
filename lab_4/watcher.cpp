#include "watcher.h"
#include "task.h"
#include "util.h"
#include "../lab_2/pipe.h"

#include <cstring>
#include <filesystem>
#include <memory>
#include <thread>

void
Watcher::scan()
{
    mutex.lock();
    stdfs::path path = watch_path;
    mutex.unlock();
    if (!stdfs::exists(path) || !stdfs::is_directory(path)) {
        ERROR("cannot scan current directory: do not exists or regular file");
        return;
    }

    for (const auto& e : stdfs::directory_iterator(path)) {
        if (e.is_regular_file())
            file_handler(e.path());
    }
}

void
Watcher::file_handler(const stdfs::path& path)
{
    mutex.lock();
    stdfs::directory_entry e(path);
    if (!e.exists() || !e.is_regular_file()) {
        cache.erase(path);
        ERROR("cannot process path {} - do not exists, or not a regular file", path.relative_path().string());
        mutex.unlock();
        return;
    }

    auto iterator = cache.find(path);
    if (iterator == cache.end()) {
        cache[path].second = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("new file queued {}", path.relative_path().string());
    } else if (// iterator->second == task_status::completed ||
               iterator->second.second == task_status::failed) {
        iterator->second.second = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("file reuploaded {}", path.relative_path().string());
    }
    mutex.unlock();
}

void
Watcher::start(const stdfs::path& wpath)
{
    if (run) stop();

    mutex.lock();
    run = true;
    watch_path = wpath;
    mutex.unlock();

    workers.clear();
    workers.reserve(THREADS);
    for (size_t i = 0; i < THREADS; ++i) {
        workers.emplace_back([this](){worker();});
    }
    // TODO: notifier

    loop_thread = std::make_unique<std::thread>(&Watcher::loop, this);
}

void
Watcher::stop()
{
    // TODO: end notifier
    if (!run) return;
    run = false;

    if (loop_thread) {
        loop_thread->detach();
        loop_thread.reset();
    }

    for (auto& t : workers) {
        t.detach();
    }
    workers.clear();

    mutex.lock();
    cache.clear();
    watch_path.clear();
    queue.drop();
    mutex.unlock();
}

bool
Watcher::change_directory(const stdfs::path& npath)
{
    if (!stdfs::exists(npath) || !stdfs::is_directory(npath))
        return false;
    // TODO: change notifier

    mutex.lock();
    watch_path = npath;
    mutex.unlock();

    scan();

    return true;
}

// NOTE: this is just a placeholder for now
void
Watcher::loop()
{
    while (run) {
        scan();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    // TODO: notifier
}

TdSQueue<Task>&
Watcher::get_queue()
{
    return queue;
}

std::map<stdfs::path, std::pair<std::vector<Element>, task_status>>&
Watcher::get_cache()
{
    return cache;
}

void
Watcher::worker()
{
    while (run) {
        Task task;
        bool do_process = false;

        mutex.lock();
        if (!queue.empty()) {
            task = queue.pop();

            auto it = cache.find(task.path);
            if (it != cache.end() && it->second.second == task_status::waiting) {
                it->second.second = task_status::running;
                do_process = true;
            }
        }
        mutex.unlock();

        if (do_process) {
            auto result = process(task.path);

            mutex.lock();
            auto it = cache.find(task.path);
            if (it != cache.end()) {
                it->second.second = (result.second == (int)task_status::failed)
                           ? task_status::failed
                           : task_status::completed;
                it->second.first = result.first;
            }
            mutex.unlock();
        }

        if (!do_process) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
