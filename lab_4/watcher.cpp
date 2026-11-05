#include "watcher.h"
#include "task.h"
#include "util.h"

#include <filesystem>

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
        cache[path] = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("new file queued {}", path.relative_path().string());
    } else if (iterator->second == task_status::completed || iterator->second == task_status::failed) {
        iterator->second = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("file reuploaded {}", path.relative_path().string());
    }
    mutex.unlock();
}

void
Watcher::start(const stdfs::path& wpath)
{
    stop();
    mutex.lock();
    watch_path = wpath;
    mutex.unlock();
    scan();
    // TODO: notifier
}

void
Watcher::stop()
{
    // TODO: end notifier
    mutex.lock();
    cache.clear();
    watch_path.clear();
    mutex.unlock();
}

bool
Watcher::change_directory(const stdfs::path& npath)
{
    return false;
}

bool
Watcher::loop()
{
    return false;
}
