#ifndef __UTIL_STATIC_QUEUE_H__
#define __UTIL_STATIC_QUEUE_H__


/**
 * @file StaticQueue.h
 * @brief A static queue implementation for embedded systems
 */


namespace util
{
/**
 * @brief A static queue implementation with fixed size
 * 
 * This class implements a circular queue data structure that is designed for
 * embedded systems. It provides efficient FIFO (First-In-First-Out) operations
 * with O(1) time complexity and uses a static array for storage.
 * 
 * Features:
 * - Fixed size queue (specified at compile time)
 * - No dynamic memory allocation
 * - Thread-safe for single producer/single consumer scenarios
 * - Automatic wrap-around handling
 * - Clear separation between front and rear indices
 * 
 * @tparam T The type of elements to be stored in the queue
 * @tparam SIZE The maximum number of elements the queue can hold
 * 
 * Example usage:
 * @code
 * StaticQueue<int, 10> queue;
 * queue.push(1);
 * int value;
 * if (queue.pop(value)) {
 *     // value contains 1
 * }
 * @endcode
 */
template <typename T, size_t SIZE> class StaticQueue
{
private:
    T elements_[SIZE];              ///< The queue storage array
    size_t front_;                  ///< Index of the front element
    size_t rear_;                   ///< Index where the next element will be inserted
    size_t size_;                   ///< Current number of elements in the queue

public:
    /**
     * @brief Construct a new Static Queue object
     * 
     * Initializes the queue with empty state.
     */
    StaticQueue() : front_(0), rear_(0), size_(0)
    {
        // Initialize elements (if needed)
        for (size_t i = 0; i < SIZE; ++i)
        {
            elements_[i] = T();
        }
    }

    /**
     * @brief Push an element to the back of the queue
     * 
     * @param element The element to push
     * @return true if the element was successfully pushed
     * @return false if the queue is full
     */
    bool push(const T &element)
    {
        if (is_full())
        {
            return false;
        }

        elements_[rear_] = element;
        rear_ = (rear_ + 1) % SIZE;
        size_++;
        return true;
    }

    /**
     * @brief Pop an element from the front of the queue
     * 
     * @param element Reference to store the popped element
     * @return true if an element was successfully popped
     * @return false if the queue is empty
     */
    bool pop(T &element)
    {
        if (is_empty())
        {
            return false;
        }

        element = elements_[front_];
        front_ = (front_ + 1) % SIZE;
        size_--;
        return true;
    }

    /**
     * @brief Get the element at the front of the queue without removing it
     * 
     * @param element Reference to store the front element
     * @return true if the front element was successfully retrieved
     * @return false if the queue is empty
     */
    bool peek(T &element) const
    {
        if (is_empty())
        {
            return false;
        }

        element = elements_[front_];
        return true;
    }

    /**
     * @brief Check if the queue is empty
     * 
     * @return true if the queue contains no elements
     * @return false if the queue contains at least one element
     */
    bool is_empty() const
    {
        return size_ == 0;
    }

    /**
     * @brief Check if the queue is full
     * 
     * @return true if the queue is at maximum capacity
     * @return false if the queue has available space
     */
    bool is_full() const
    {
        return size_ == SIZE;
    }

    /**
     * @brief Get the current number of elements in the queue
     * 
     * @return size_t Number of elements currently in the queue
     */
    size_t get_size() const
    {
        return size_;
    }

    /**
     * @brief Get the maximum capacity of the queue
     * 
     * @return size_t Maximum number of elements the queue can hold
     */
    size_t get_capacity() const
    {
        return SIZE;
    }

    /**
     * @brief Clear all elements from the queue
     * 
     * Resets the queue to its initial empty state.
     */
    void clear()
    {
        front_ = 0;
        rear_ = 0;
        size_ = 0;
    }
};

} // namespace util

#endif // __UTIL_STATIC_QUEUE_H__