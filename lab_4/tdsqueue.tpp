#include "util.h"

template<typename T>
void
TdSQueue<T>::push(T value)
{
    mutex.lock();
    auto node = new Node<T>(value);
    if (size == 0) head = node;
    else {
        auto curr = head;
        while (curr->next != nullptr) curr = curr->next;
        curr->next = node;
    }
    ++size;
    mutex.unlock();
}

template<typename T>
T
TdSQueue<T>::pop()
{
    mutex.lock();
    if (size == 0) {
        ERROR("queue is empty. unable to pop");
        mutex.unlock();
        return T();
    }
    auto temp = head;
    T result = temp->data;
    head = head->next;
    delete temp;
    --size;
    mutex.unlock();
    return result;
}

template<typename T>
bool
TdSQueue<T>::empty()
{
    mutex.lock();
    bool result = size == 0 ? true : false;
    mutex.unlock();
    DEBUG("empty=%b", result);
    return result;
}

template<typename T>
std::size_t
TdSQueue<T>::get_size()
{
    mutex.lock();
    std::size_t result = size;
    mutex.unlock();
    DEBUG("size=%ld", result);
    return result;
}

template<typename T>
void
TdSQueue<T>::peek()
{
    mutex.lock();
    if (size == 0) {
        ERROR("queue is empty");
        mutex.unlock();
        return;
    }

    for (auto curr = head; curr != nullptr; curr = curr->next) {
        std::cout << curr->data << " ";
    }
    std::cout << std::endl;
    mutex.unlock();
}

template<typename T>
bool
TdSQueue<T>::contains(T t)
{
    mutex.lock();
    if (size == 0) {
        mutex.unlock();
        return false;
    }

    for (auto curr = head; curr != nullptr; curr = curr->next) {
        if (curr->data == t) {
            mutex.unlock();
            return true;
        }
    }
    mutex.unlock();
    return false;
}

template<typename T>
void
TdSQueue<T>::drop()
{
    mutex.lock();
    if (size == 0) { mutex.unlock(); return; }
    for (auto curr = head; curr != nullptr;) {
        auto next = curr->next;
        delete curr;
        curr = next;
    }
    head = nullptr;
    size = 0;
    mutex.unlock();
}
