#include "watcher.h"
#include "pipe.h"
#include "../exceptions.h"

#include "task.h"
#include <chrono>
#include <filesystem>
#include <future>
#include <memory>
#include <thread>

// need to be locked only if class field requested
void
Watcher::scan()
{
    stdfs::path path;
    {
        std::lock_guard l(mutex);
        path = watch_path;
    }

    if (!stdfs::exists(path) || !stdfs::is_directory(path)) {
        ERROR("cannot scan current directory");
        return;
    }

    for (const auto& e : stdfs::directory_iterator(path)) {
        if (e.is_regular_file()) handle_file(e.path());
    }
}

static constexpr std::size_t MAX_FILE_SIZE = 24 * 1024 * 1024;

void
Watcher::handle_file(const stdfs::path& fpath) noexcept(false)
{
    std::lock_guard l(mutex);

    stdfs::directory_entry e(fpath);
    if (!e.exists() || !e.is_regular_file()) {
        cache.erase(fpath);
        ERROR("cannot process file: {}", fpath.relative_path().string());
        return;
    }

    auto it = cache.find(fpath);

    if (e.file_size() > MAX_FILE_SIZE && !(it->second.second == task_status::cancelled)) {
        cache[fpath].second = task_status::cancelled;
        throw SizeException();
    }

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
        try {
            scan();
        } catch (const SizeException& se) {
            ERROR("{}", se.what());
        }

        for (int i = 0; i < 10 && run; ++i)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void
Watcher::worker_threads()
{
    while (run) {
        Task task;
        std::shared_ptr<std::atomic<bool>> ctrl;
        bool taken = false;
        {
            std::lock_guard l(mutex);
            if (!queue.empty()) {
                task = queue.pop();
                auto it = cache.find(task.path);
                if (it != cache.end() && it->second.second == task_status::waiting) {
                    ctrl = std::make_shared<std::atomic<bool>>(false);
                    it->second.second = task_status::running;
                    taken = true;
                }
            }
        }

        if (!taken) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        auto [element, status] = process(task.path, run, *ctrl);
        if (!run) return;

        std::lock_guard l(mutex);
        cancels.erase(task.path);
        auto it = cache.find(task.path);
        if (it != cache.end()) {
            it->second.second = (status == -1) ? task_status::failed
                : task_status::completed;
            it->second.first = std::move(element);
        }
    }
}

void
Watcher::worker()
{
    switch (mode) {
        case Mode::threads: worker_threads(); break;
        case Mode::async: worker_async(); break;
    }
}

void
Watcher::worker_async()
{
    while (run) {
        bool did = false;
        {
            std::lock_guard l(mutex);
            for (auto it = pending.begin(); it != pending.end();) {
                if (it->future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                    auto [element, status] = it->future.get();
                    cancels.erase(it->path);
                    auto c = cache.find(it->path);
                    if (c != cache.end()) {
                        c->second.second = map_status(status);
                        c->second.first = std::move(element);
                    }
                    it = pending.erase(it);
                    did = true;
                } else ++it;
            }
        }

        {
            std::lock_guard l(mutex);
            while (pending.size() < MXASYNC && !queue.empty()) {
                Task t = queue.pop();
                auto it = cache.find(t.path);
                if (it == cache.end() || it->second.second != task_status::waiting) continue;
                it->second.second = task_status::running;
                auto ctrl = std::make_shared<std::atomic<bool>>(false);
                cancels[t.path] = ctrl;

                pending.push_back({
                    std::async(std::launch::async, [p = t.path, ctrl, this]{
                        return process(p, run, *ctrl);
                    }),
                    t.path
                });
                did = true;
            }
        }

        if (!did) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void
Watcher::start(const stdfs::path& wpath, Mode m)
{
    stop();

    mutex.lock();
    watch_path = wpath;
    mutex.unlock();

    mode = m;
    run = true;
    workers.reserve(THREADS);
    for (std::size_t i = 0; i < THREADS; ++i) {
        workers.emplace_back([this](){worker();});
    }
    loop_thread = std::make_unique<std::thread>(&Watcher::loop, this);
}

void
Watcher::stop()
{
    if (!run.exchange(false)) return;

    if (loop_thread && loop_thread->joinable()) loop_thread->join();
    loop_thread.reset();

    for (auto& w : workers) {
        if (w.joinable()) w.join();
    }
    workers.clear();

    for (auto& p : pending) {
        if (p.future.valid()) p.future.wait();
    }
    pending.clear();

    std::lock_guard l(mutex);
    cache.clear();
    cancels.clear();
    watch_path.clear();
    queue.drop();
}

bool
Watcher::change_directory(const stdfs::path& npath)
{
    if (!stdfs::exists(npath) || !stdfs::is_directory(npath))
        return false;

    {
        std::lock_guard l(mutex);
        watch_path = npath;
        cache.clear();
        queue.drop();
    }

    scan();
    return true;
}

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

void
Watcher::cancel(const stdfs::path& rpath) {
    std::lock_guard l(mutex);
    auto it = cache.find(rpath);
    if (it == cache.end()) {
        ERROR("not tracked");
        return;
    }

    if (it->second.second == task_status::running) {
        auto c = cancels.find(rpath);
        if (c != cancels.end() && c->second) *c->second = true;
        return;
    }

    ERROR("nothing to cancel here");
}

std::optional<task_status>
Watcher::get_stat(const stdfs::path& p)
{
    std::lock_guard l(mutex);
    auto it = cache.find(p);
    return it == cache.end() ? std::nullopt : std::optional<task_status>(it->second.second);
}
