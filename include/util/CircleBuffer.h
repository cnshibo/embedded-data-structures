#ifndef __UTIL_CIRCLE_BUFFER_H__
#define __UTIL_CIRCLE_BUFFER_H__

/**
 * @file CircleBuffer.h
 * @brief A circular buffer implementation for embedded systems
 */

namespace util
{
/**
 * @brief A thread-safe circular buffer implementation with fixed size
 * 
 * This class implements a circular buffer (ring buffer) data structure that is
 * commonly used in embedded systems for buffering data streams. It provides
 * efficient FIFO (First-In-First-Out) operations with O(1) time complexity.
 * 
 * Features:
 * - Fixed size buffer (specified at compile time)
 * - Automatic wrap-around handling
 * - No dynamic memory allocation
 * - Thread-safe for single producer/single consumer scenarios
 * - Partial read/write support
 * 
 * @tparam BUFFER_SIZE The size of the circular buffer in bytes
 * 
 * Example usage:
 * @code
 * CircleBuffer<100> buffer;
 * uint8_t data[50] = { ... };
 * uint8_t output[50];
 * 
 * // Put data
 * size_t bytes_put = buffer.put(data, 50);
 * 
 * // Get data
 * size_t bytes_got = buffer.get(output, 50);
 * @endcode
 */
template <size_t BUFFER_SIZE> class CircleBuffer
{
private:
    uint8_t buffer_[BUFFER_SIZE];    ///< The actual buffer storage
    size_t head_index_;              ///< Index where data will be read from
    size_t tail_index_;              ///< Index where data will be written to
    size_t buffer_size_;             ///< Current number of bytes in the buffer

public:
    /**
     * @brief Construct a new Circle Buffer object
     * 
     * Initializes the buffer with zero values and sets up initial indices.
     */
    CircleBuffer() : head_index_(0), tail_index_(0), buffer_size_(0)
    {
        // Initialize buffer
        memset(buffer_, 0, sizeof(buffer_));
    }

    /**
     * @brief Destroy the Circle Buffer object
     * 
     * No cleanup needed as no dynamic memory is used.
     */
    ~CircleBuffer()
    {
        // No dynamic memory to deallocate
    }

    /**
     * @brief Put data into the buffer
     * 
     * Copies data into the buffer, handling wrap-around automatically.
     * If there isn't enough space, it will copy as much as possible.
     * 
     * @param data Pointer to the data to be copied into the buffer
     * @param length Number of bytes to copy
     * @return size_t Number of bytes actually copied into the buffer
     * @note Returns 0 if data is nullptr or length is 0
     */
    size_t put(const uint8_t *data, size_t length)
    {
        if (data == nullptr || length == 0)
        {
            return 0;
        }

        // Calculate how many bytes we can actually put
        size_t available_space = BUFFER_SIZE - buffer_size_;
        size_t bytes_to_put = (length <= available_space) ? length : available_space;

        if (bytes_to_put == 0)
        {
            return 0;
        }

        // Copy data into the buffer
        size_t bytes_to_end = BUFFER_SIZE - tail_index_;
        if (bytes_to_end >= bytes_to_put)
        {
            // No wrap-around needed
            memcpy(buffer_ + tail_index_, data, bytes_to_put);
        }
        else
        {
            // Wrap-around needed
            memcpy(buffer_ + tail_index_, data, bytes_to_end);
            memcpy(buffer_, data + bytes_to_end, bytes_to_put - bytes_to_end);
        }

        // Update tail index and buffer size
        tail_index_ = (tail_index_ + bytes_to_put) % BUFFER_SIZE;
        buffer_size_ += bytes_to_put;

        return bytes_to_put;
    }

    /**
     * @brief Get data from the buffer
     * 
     * Copies data from the buffer into the provided destination, handling
     * wrap-around automatically. If there isn't enough data, it will copy
     * as much as possible.
     * 
     * @param data Pointer to where the data should be copied
     * @param length Number of bytes to copy
     * @return size_t Number of bytes actually copied from the buffer
     * @note Returns 0 if data is nullptr or length is 0
     */
    size_t get(uint8_t *data, size_t length)
    {
        if (data == nullptr || length == 0)
        {
            return 0;
        }

        // Calculate how many bytes we can actually get
        size_t bytes_to_get = (length <= buffer_size_) ? length : buffer_size_;

        if (bytes_to_get == 0)
        {
            return 0;
        }

        // Copy data from the buffer
        size_t bytes_to_end = BUFFER_SIZE - head_index_;
        if (bytes_to_end >= bytes_to_get)
        {
            // No wrap-around needed
            memcpy(data, buffer_ + head_index_, bytes_to_get);
        }
        else
        {
            // Wrap-around needed
            memcpy(data, buffer_ + head_index_, bytes_to_end);
            memcpy(data + bytes_to_end, buffer_, bytes_to_get - bytes_to_end);
        }

        // Update head index and buffer size
        head_index_ = (head_index_ + bytes_to_get) % BUFFER_SIZE;
        buffer_size_ -= bytes_to_get;

        return bytes_to_get;
    }

    /**
     * @brief Check if the buffer is empty
     * 
     * @return true if the buffer contains no data
     * @return false if the buffer contains data
     */
    bool is_empty() const
    {
        return buffer_size_ == 0;
    }

    /**
     * @brief Check if the buffer is full
     * 
     * @return true if the buffer is at maximum capacity
     * @return false if the buffer has available space
     */
    bool is_full() const
    {
        return buffer_size_ == BUFFER_SIZE;
    }

    /**
     * @brief Get the current number of bytes in the buffer
     * 
     * @return size_t Number of bytes currently stored in the buffer
     */
    size_t get_size() const
    {
        return buffer_size_;
    }

    /**
     * @brief Get the total capacity of the buffer
     * 
     * @return size_t Total size of the buffer in bytes
     */
    size_t get_capacity() const
    {
        return BUFFER_SIZE;
    }

    /**
     * @brief Get the number of bytes that can be written to the buffer
     * 
     * @return size_t Number of bytes of available space in the buffer
     */
    size_t get_available_space() const
    {
        return BUFFER_SIZE - buffer_size_;
    }
};

} // namespace util

#endif // __UTIL_CIRCLE_BUFFER_H__