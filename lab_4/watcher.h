#ifndef WATCHER_H
#define WATCHER_H

#include "util.h"
#include "tdsqueue.h"

#include <string>
#include <filesystem>

class Watcher {
private:
    std::string path;
    bool yes = false;
public:
    Watcher(std::string p) : path(p) { DEBUG("watcher constructor"); }

    void
    watch(TdSQueue<std::filesystem::directory_entry> *q);

    void
    change_path(const std::string& path);

    inline bool
    watching()
    {
        return yes;
    }

    inline void
    stop()
    {
        yes = false;
    }
};

#endif
