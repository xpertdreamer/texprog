#include "util.h"

template<typename T>
void
TdSQueue<T>::push(T value)
{
    mutex.lock();
    queue.push(value);
    mutex.unlock();
}

template<typename T>
T
TdSQueue<T>::pop()
{
    mutex.lock();
    if (queue.empty()) {
        ERROR("queue is empty. unable to pop");
        mutex.unlock();
        return T();
    }
    T result = std::move(queue.front());
    queue.pop();
    mutex.unlock();
    return result;
}

template<typename T>
bool
TdSQueue<T>::empty()
{
    mutex.lock();
    bool result = queue.empty();
    mutex.unlock();
    DEBUG("empty=%b", result);
    return result;
}

template<typename T>
std::size_t
TdSQueue<T>::size()
{
    mutex.lock();
    std::size_t result = queue.size();
    mutex.unlock();
    DEBUG("size=%ld", result);
    return result;
}
