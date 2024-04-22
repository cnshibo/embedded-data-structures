#ifndef __UTIL_STATIC_QUEUE_H__
#define __UTIL_STATIC_QUEUE_H__

#include <cstddef> // For size_t

namespace util
{

template <typename T, size_t MAX_SIZE> class StaticQueue
{
private:
    T *_elements[MAX_SIZE];
    size_t _front;
    size_t _rear;
    size_t _size;

public:
    StaticQueue() : _front(0), _rear(0), _size(0)
    {
    }

    bool empty() const
    {
        return _size == 0;
    }

    bool full() const
    {
        return _size == MAX_SIZE;
    }

    size_t size() const
    {
        return _size;
    }

    bool push(T *item)
    {
        if (full())
        {
            return false; // Queue is full, cannot push
        }
        _elements[_rear] = item;
        _rear = (_rear + 1) % MAX_SIZE;
        ++_size;
        return true;
    }

    void pop()
    {
        if (empty())
        {
            return; // Queue is empty, cannot pop
        }
        _front = (_front + 1) % MAX_SIZE;
        --_size;
    }

    const T *front() const
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return _elements[_front];
    }

    T *front()
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return _elements[_front];
    }

    const T *back() const
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return _elements[(_rear + MAX_SIZE - 1) % MAX_SIZE];
    }

    T *back()
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return _elements[(_rear + MAX_SIZE - 1) % MAX_SIZE];
    }
};

} // namespace util

#endif // __UTIL_STATIC_QUEUE_H__

/************************************************************************/
/*                                                                      */
/************************************************************************/
#if 0

int main() {
    // Define the type of elements you want to store in the queue
    typedef int ElementType;

    // Define the maximum size of the queue
    const size_t MAX_SIZE = 5;

    // Create an instance of StaticQueue
    util::StaticQueue<ElementType, MAX_SIZE> queue;

    // Check if the queue is empty
    if (queue.empty()) {
        std::cout << "Queue is empty" << std::endl;
    } else {
        std::cout << "Queue is not empty" << std::endl;
    }

    // Push elements into the queue
    for (int i = 0; i < MAX_SIZE; ++i) {
        ElementType* element = new ElementType(i); // Create new element
        queue.push(element); // Push element into the queue
    }

    // Check if the queue is full
    if (queue.full()) {
        std::cout << "Queue is full" << std::endl;
    } else {
        std::cout << "Queue is not full" << std::endl;
    }

    // Pop elements from the queue
    while (!queue.empty()) {
        ElementType* front = queue.front(); // Get the front element
        std::cout << "Front element: " << *front << std::endl;
        queue.pop(); // Pop the front element
        delete front; // Don't forget to delete the element if you allocated it dynamically
    }

    return 0;
}

#endif