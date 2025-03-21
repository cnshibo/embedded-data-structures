#ifndef __UTIL_CIRCLE_BUFFER_H__
#define __UTIL_CIRCLE_BUFFER_H__

namespace util
{
template <size_t BUFFER_SIZE> class CircleBuffer
{
private:
    uint8_t buffer_[BUFFER_SIZE];
    size_t head_index_;
    size_t tail_index_;
    size_t buffer_size_;

public:
    CircleBuffer() : head_index_(0), tail_index_(0), buffer_size_(0)
    {
        // Initialize buffer
        memset(buffer_, 0, sizeof(buffer_));
    }

    ~CircleBuffer()
    {
        // No dynamic memory to deallocate
    }

    bool put(const uint8_t *data, size_t length)
    {
        if (length > BUFFER_SIZE - buffer_size_)
        {
            // Insufficient space in buffer
            return false;
        }

        // Copy data into the buffer
        size_t bytes_to_end = BUFFER_SIZE - tail_index_;
        if (bytes_to_end >= length)
        {
            // No wrap-around needed
            memcpy(buffer_ + tail_index_, data, length);
        }
        else
        {
            // Wrap-around needed
            memcpy(buffer_ + tail_index_, data, bytes_to_end);
            memcpy(buffer_, data + bytes_to_end, length - bytes_to_end);
        }

        // Update tail index and buffer size
        tail_index_ = (tail_index_ + length) % BUFFER_SIZE;
        buffer_size_ += length;

        return true;
    }

    bool get(uint8_t *data, size_t length)
    {
        if (length > buffer_size_)
        {
            // Insufficient data in buffer
            return false;
        }

        // Copy data from the buffer
        size_t bytes_to_end = BUFFER_SIZE - head_index_;
        if (bytes_to_end >= length)
        {
            // No wrap-around needed
            memcpy(data, buffer_ + head_index_, length);
        }
        else
        {
            // Wrap-around needed
            memcpy(data, buffer_ + head_index_, bytes_to_end);
            memcpy(data + bytes_to_end, buffer_, length - bytes_to_end);
        }

        // Update head index and buffer size
        head_index_ = (head_index_ + length) % BUFFER_SIZE;
        buffer_size_ -= length;
        return true;
    }

    bool is_empty() const
    {
        return buffer_size_ == 0;
    }

    bool is_full() const
    {
        return buffer_size_ == BUFFER_SIZE;
    }

    size_t get_size() const
    {
        return buffer_size_;
    }

    size_t get_capacity() const
    {
        return BUFFER_SIZE;
    }
};

} // namespace util

#endif // __UTIL_CIRCLE_BUFFER_H__