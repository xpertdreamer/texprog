#include "watcher.h"
#include "task.h"
#include <filesystem>

// need to be locked only if class field requested
void
Watcher::scan()
{
    stdfs::path path;
    mutex.lock();
        std::lock_guard l(mutex);
        path = path;
    mutex.unlock();

    if (!stdfs::exists(path) || !stdfs::is_directory(path)) {
        ERROR("cannot scan current directory");
        return;
    }

    for (const auto& e : stdfs::directory_iterator(path)) {
        if (e.is_regular_file()) handle_file(e.path());
    }
}

void
Watcher::handle_file(const stdfs::path& fpath)
{
    std::lock_guard l(mutex);

    stdfs::directory_entry e(fpath);
    if (!e.exists() || !e.is_regular_file()) {
        cache.erase(fpath);
        ERROR("cannot process file: {}", fpath.relative_path().string());
        return;
    }

    auto it = cache.find(fpath);
    if (it == cache.end()) {
        cache[fpath].second = task_status::waiting;
        queue.push(Task{fpath, task_status::waiting});
        DEBUG("new file queued {}", fpath.relative_path().string());
    } else if (it->second.second == task_status::failed) {
        it->second.second = task_status::waiting;
        queue.push(Task{fpath, task_status::waiting});
    }
}

void loop();

void worker();

void start(const stdfs::path& wpath);

void stop();

bool change_directory(const stdfs::path& npath);

void dump_state();

std::vector<Element> get_res(const stdfs::path& rpath);

void drop_queue();
