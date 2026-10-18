#include "watcher.h"

#include <filesystem>
#include "util.h"

void
Watcher::watch()
{
    for(;;) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            DEBUG("%s\n", entry.path().c_str());
        }
    }
}
