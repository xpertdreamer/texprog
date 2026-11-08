#ifndef TASK_H
#define TASK_H

#include <filesystem>
#include <ostream>

enum class task_status {
    failed = -1,
    waiting = 0,
    running = 1,
    completed = 2,
    cancelled,
};

inline const char*
status_to_string(task_status ts)
{
#define CASE_STATUS(member) case task_status::member: return #member

    switch(ts) {
        CASE_STATUS(failed);
        CASE_STATUS(waiting);
        CASE_STATUS(running);
        CASE_STATUS(completed);
        CASE_STATUS(cancelled);
        default: return "unknown";
    }

#undef CASE_STATUS
}

inline task_status
map_status(int s)
{
    switch (s) {
        case 2: return task_status::completed;
        case -1: return task_status::failed;
        case 3: return task_status::cancelled;
        case 0: return task_status::waiting;
        default: return task_status::failed;
    }
}

struct Task {
    std::filesystem::path path;
    task_status status;
    // ...

    inline task_status
    get_status()
    {
        return status;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        os << "[Path=" << t.path << " Status=" << status_to_string(t.status) << "]";
        return os;
    }
};

#endif
