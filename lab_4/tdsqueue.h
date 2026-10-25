/**
 * @file tdsqueue.h
 * @brief This file contains definitions of the thread-safe queue class.
 */

#ifndef TDSQUEUE_H
#define TDSQUEUE_H

#include <mutex>
#include <queue>

template<typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& d) : data(d), next(nullptr) {}
    ~Node() = default;
};

/**
 * @class TdSQueue
 * @brief A thread-safe wrapper around the standard library's `queue` class using mutex
 * @tparam T Type of elements
 */
template<typename T>
class TdSQueue {
private:
    size_t size;
    Node<T>* head;
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
    get_size();

    void
    peek();

    bool
    contains(T t);

    void
    drop();
};

#include "tdsqueue.tpp"

#endif
