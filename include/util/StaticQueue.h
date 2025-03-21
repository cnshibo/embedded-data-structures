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