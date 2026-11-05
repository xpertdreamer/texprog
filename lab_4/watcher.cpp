#include "watcher.h"
#include "task.h"
#include "util.h"

#include <filesystem>

void
Watcher::scan()
{
    if (!stdfs::exists(watch_path) || !stdfs::is_directory(watch_path)) {
        ERROR("cannot scan current directory: do not exists or regular file");
        return;
    }

    for (const auto& e : stdfs::directory_iterator(watch_path)) {
        if (e.is_regular_file())
            file_handler(e.path());
    }
}

void
Watcher::file_handler(const stdfs::path& path)
{
    stdfs::directory_entry e(path);
    if (!e.exists() || !e.is_regular_file()) {
        cache.erase(path);
        ERROR("cannot process path {} - do not exists, or not a regular file", path.relative_path().string());
        return;
    }

    auto iterator = cache.find(path);
    if (iterator == cache.end()) {
        cache[path] = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("new file queued {}", path.relative_path().string());
        return;
    }
    if (iterator->second == task_status::completed || iterator->second == task_status::failed) {
        iterator->second = task_status::waiting;
        Task t{path, task_status::waiting};
        queue.push(t);
        DEBUG("file reuploaded {}", path.relative_path().string());
    }
}

void
Watcher::start(const stdfs::path& wpath)
{
    stop();
    watch_path = wpath;


}

void
Watcher::stop()
{

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
