/**
 * @file tdsqueue.h
 * @brief This file contains definitions of the thread-safe queue class.
 */

#ifndef TDSQUEUE_H
#define TDSQUEUE_H

#include <mutex>
#include <queue>

/**
 * @class TdSQueue
 * @brief A thread-safe wrapper around the standard library's `queue` class using mutex
 * @tparam T Type of elements
 */
template<typename T>
class TdSQueue {
private:
    std::queue<T> queue; ///< Underlying standard queue.
    std::mutex mutex; ///< Mutex to guard access
public:
    TdSQueue() = default;
    ~TdSQueue() = default;

    /**
    * @brief Pushes value with generic type T into queue
    * @param value Value of generic type to push
    */
    void
    push(T value);

    /**
    * @brief Pop front element
    * @return Front element of the wrapped queue
    */
    T
    pop();

    /**
    * @brief Check whether the queue is empty
    * @return @p true if the queue is empty
    */
    bool
    empty();

    /**
    * @brief Returns the number of elements currently stored in the queue
    * @return Number of elements in the queue
    */
    std::size_t
    size();
};

#include "tdsqueue.tpp"

#endif
