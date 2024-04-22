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

#if 0

#include <iostream>
#include "CircleBuffer.h"

int main() {
    const size_t BufferSize = 10;
    util::CircleBuffer<BufferSize> buffer;

    // Example data to be put into the buffer
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    size_t data_length = sizeof(data);

    // Put data into the buffer
    if (buffer.put(data, data_length)) {
        std::cout << "Data put into buffer successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to put data into buffer. Buffer full." << std::endl;
    }

    // Get data from the buffer
    uint8_t received_data[BufferSize];
    if (buffer.get(received_data, data_length)) {
        std::cout << "Data retrieved from buffer: ";
        for (size_t i = 0; i < data_length; ++i) {
            std::cout << std::hex << static_cast<int>(received_data[i]) << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cerr << "Failed to retrieve data from buffer. Buffer empty." << std::endl;
    }

    // Check if buffer is empty or full
    if (buffer.is_empty()) {
        std::cout << "Buffer is empty." << std::endl;
    }

    if (buffer.is_full()) {
        std::cout << "Buffer is full." << std::endl;
    }

    // Get current size and capacity of the buffer
    size_t current_size = buffer.get_size();
    size_t buffer_capacity = buffer.get_capacity();
    std::cout << "Current size of buffer: " << current_size << std::endl;
    std::cout << "Buffer capacity: " << buffer_capacity << std::endl;

    return 0;
}

#endif