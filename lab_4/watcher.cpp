#include "watcher.h"

#include "tdsqueue.h"
#include "util.h"

// #include <chrono>
#include <filesystem>
// #include <thread>

void
Watcher::watch(TdSQueue<std::filesystem::directory_entry> *q)
{
    yes = true;
    while(yes) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_regular_file(entry.status()) && !q->contains(entry)) {
                DEBUG("%s\n", entry.path().c_str());
                q->push(entry);
            }
        }
    }
}

void
Watcher::change_path(const std::string& _path)
{
    if (!std::filesystem::exists(_path) || !std::filesystem::is_directory(_path)) {
        ERROR("cannot change path: it is not a directory");
        return;
    }

    path = _path;
}
