#ifndef WATCHER_H
#define WATCHER_H

#include <string>
#include "util.h"

class Watcher {
private:
    std::string path;
public:
    Watcher(std::string p) : path(p) { DEBUG("watcher constructor"); }

    void
    watch();
};

#endif
