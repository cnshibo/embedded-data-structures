#ifndef __UTIL_STATIC_QUEUE_H__
#define __UTIL_STATIC_QUEUE_H__

#include <cstddef> // For size_t

namespace util
{

template <typename T, size_t MAX_SIZE> class StaticQueue
{
private:
    T *elements_[MAX_SIZE];
    size_t front_;
    size_t rear_;
    size_t size_;

public:
    StaticQueue() : front_(0), rear_(0), size_(0)
    {
    }

    bool empty() const
    {
        return size_ == 0;
    }

    bool full() const
    {
        return size_ == MAX_SIZE;
    }

    size_t size() const
    {
        return size_;
    }

    bool push(T *item)
    {
        if (full())
        {
            return false; // Queue is full, cannot push
        }
        elements_[rear_] = item;
        rear_ = (rear_ + 1) % MAX_SIZE;
        ++size_;
        return true;
    }

    void pop()
    {
        if (empty())
        {
            return; // Queue is empty, cannot pop
        }
        front_ = (front_ + 1) % MAX_SIZE;
        --size_;
    }

    const T *front() const
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return elements_[front_];
    }

    T *front()
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return elements_[front_];
    }

    const T *back() const
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return elements_[(rear_ + MAX_SIZE - 1) % MAX_SIZE];
    }

    T *back()
    {
        if (empty())
        {
            return NULL; // Queue is empty
        }
        return elements_[(rear_ + MAX_SIZE - 1) % MAX_SIZE];
    }
};

} // namespace util

#endif // __UTIL_STATIC_QUEUE_H__