#include "watcher.h"
#include "task.h"
#include <filesystem>
#include <thread>

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

void
Watcher::loop()
{
    while (run) {
        scan();
        for (int i = 0; i < 100 && run; ++i)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void
Watcher::worker()
{

}

void start(const stdfs::path& wpath);

void stop();

bool change_directory(const stdfs::path& npath);

void
Watcher::dump_state()
{
    std::lock_guard l(mutex);
    std::cout << "Cache:" << '\n';
    for (const auto& [k, v] : cache) {
        if (v.second != task_status::waiting)
            std::cout << k << " : " << status_to_string(v.second) << '\n';
    }
    std::cout << "\nQueue" << '\n';
    if (!queue.empty()) queue.peek();
    else std::cout << '\n';
}

std::vector<Element>
Watcher::get_res(const stdfs::path& rpath)
{
    std::lock_guard l(mutex);
    auto it = cache.find(rpath);
    return it == cache.end() ? std::vector<Element>{} : it->second.first;
}

void
Watcher::drop_queue()
{
    std::lock_guard l(mutex);
    queue.drop();
}
