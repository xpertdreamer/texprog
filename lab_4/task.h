/**
 * @file task.h
 * @brief This file contains task status definitions, status conversion helpers
 * and the Task structure used by the Watcher.
 */

#ifndef TASK_H
#define TASK_H

#include <filesystem>
#include <ostream>

/**
 * @enum task_status
 * @brief Lifecycle status of a single file processing task.
 */
enum class task_status {
    failed = -1,
    waiting = 0,
    running = 1,
    completed = 2,
    cancelled,
};

/**
 * @brief Convert a task_status value to its string representation.
 * @param ts Status to convert
 * @return Constant string literal with the status name
 */
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

/**
 * @brief Map an integer status returned by process() to task_status.
 * @param s Integer status (2 - completed, -1 - failed, 3 - cancelled, 0 - waiting)
 * @return Corresponding task_status; task_status::failed for unknown values
 */
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

/**
 * @struct Task
 * @brief A unit of work describing a single file to be processed.
 */
struct Task {
    std::filesystem::path path;
    task_status status;
    // ...

    /**
     * @brief Get the current status of the task.
     * @return Status stored in the task
     */
    inline task_status
    get_status()
    {
        return status;
    }

    /**
     * @brief Stream output operator for debugging.
     * @param os Output stream
     * @param t Task to print
     * @return Reference to the output stream
     */
    inline friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        os << "[Path=" << t.path << " Status=" << status_to_string(t.status) << "]";
        return os;
    }
};

#endif
