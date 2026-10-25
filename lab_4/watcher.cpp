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
