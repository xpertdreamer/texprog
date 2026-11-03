#ifndef WATCHER_H
#define WATCHER_H

#include "tdsqueue.h"
#include <filesystem>
#include <unordered_set>

namespace stdfs = std::filesystem;

// TODO: results struct

class Watcher {
    private:
        stdfs::path watch_path;
        // mutex
        std::unordered_set<stdfs::path> completed;
        // new file callback

    public:
        void start(stdfs::path wpath);

        void stop();

        bool change_directory();

        bool loop();
};

#endif
