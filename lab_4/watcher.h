#ifndef WATCHER_H
#define WATCHER_H

#include "tdsqueue.h"
#include <filesystem>

typedef std::filesystem::directory_entry dirent;

class Watcher {
    private:
        TdSQueue<dirent> queue;
        // TODO: table
    public:
        // TODO: watch

        ~Watcher() {

        }
};

#endif
