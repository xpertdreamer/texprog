#ifndef TASK_H
#define TASK_H

#include <filesystem>
#include <ostream>


// TODO: check if this is correct
enum class task_status {
    failed = -1,
    waiting = 0,
    running = 1,
    completed = 2,
};

inline static const char*
status_to_string(task_status ts)
{
#define CASE_STATUS(member) case task_status::member: return #member

    switch(ts) {
        CASE_STATUS(failed);
        CASE_STATUS(waiting);
        CASE_STATUS(running);
        CASE_STATUS(completed);
        default: return "unknown";
    }

#undef CASE_STATUS
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
