#ifndef __UTIL_CIRCLE_BUFFER_H__
#define __UTIL_CIRCLE_BUFFER_H__

namespace util
{
template <size_t BUFFER_SIZE> class CircleBuffer
{
private:
    uint8_t _buffer[BUFFER_SIZE];
    size_t _head_index;
    size_t _tail_index;
    size_t _buffer_size;

public:
    CircleBuffer() : _head_index(0), _tail_index(0), _buffer_size(0)
    {
        // Initialize buffer
        memset(_buffer, 0, sizeof(_buffer));
    }

    ~CircleBuffer()
    {
        // No dynamic memory to deallocate
    }

    bool put(const uint8_t *data, size_t length)
    {
        if (length > BUFFER_SIZE - _buffer_size)
        {
            // Insufficient space in buffer
            return false;
        }

        // Copy data into the buffer
        size_t bytes_to_end = BUFFER_SIZE - _tail_index;
        if (bytes_to_end >= length)
        {
            // No wrap-around needed
            memcpy(_buffer + _tail_index, data, length);
        }
        else
        {
            // Wrap-around needed
            memcpy(_buffer + _tail_index, data, bytes_to_end);
            memcpy(_buffer, data + bytes_to_end, length - bytes_to_end);
        }

        // Update tail index and buffer size
        _tail_index = (_tail_index + length) % BUFFER_SIZE;
        _buffer_size += length;

        return true;
    }

    bool get(uint8_t *data, size_t length)
    {
        if (length > _buffer_size)
        {
            // Insufficient data in buffer
            return false;
        }

        // Copy data from the buffer
        size_t bytes_to_end = BUFFER_SIZE - _head_index;
        if (bytes_to_end >= length)
        {
            // No wrap-around needed
            memcpy(data, _buffer + _head_index, length);
        }
        else
        {
            // Wrap-around needed
            memcpy(data, _buffer + _head_index, bytes_to_end);
            memcpy(data + bytes_to_end, _buffer, length - bytes_to_end);
        }

        // Update head index and buffer size
        _head_index = (_head_index + length) % BUFFER_SIZE;
        _buffer_size -= length;
        return true;
    }

    bool is_empty() const
    {
        return _buffer_size == 0;
    }

    bool is_full() const
    {
        return _buffer_size == BUFFER_SIZE;
    }

    size_t get_size() const
    {
        return _buffer_size;
    }

    size_t get_capacity() const
    {
        return BUFFER_SIZE;
    }
};

} // namespace util

#endif // __UTIL_CIRCLE_BUFFER_H__