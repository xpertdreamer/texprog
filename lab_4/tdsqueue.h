/**
 * @file tdsqueue.h
 * @brief This file contains definitions of the thread-safe queue class.
 */

#ifndef TDSQUEUE_H
#define TDSQUEUE_H

#include <mutex>
#include <queue>

template<typename T>
class TdSQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
public:

};

#endif
