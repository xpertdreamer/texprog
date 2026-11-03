#ifndef TASK_H
#define TASK_H

#include <filesystem>

// TODO: check if this is correct
enum class task_status {
    failed = -1,
    waiting = 0,
    running = 1,
    completed = 2,
};

struct Task {
    std::filesystem::path path;
    task_status status;
    // ...

    inline task_status
    get_status()
    {
        return status;
    }
};

#endif
