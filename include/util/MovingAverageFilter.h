#ifndef __UTIL_MOVING_AVERAGE_FILTER_HPP__
#define __UTIL_MOVING_AVERAGE_FILTER_HPP__

#include <stddef.h>  // for size_t

namespace util
{

/**
 * @brief A fixed-size moving average filter for embedded systems.
 * 
 * Features:
 * - No dynamic memory allocation
 * - Template-based buffer size
 * - Efficient running average using circular buffer
 * 
 * @tparam SIZE Number of samples used for averaging
 */
template <size_t SIZE>
class MovingAverageFilter
{
private:
    float buffer_[SIZE];  ///< Circular buffer for samples
    size_t index_;        ///< Current write index
    float sum_;           ///< Sum of current samples
    float average_;       ///< Current average
    bool inited_;         ///< Whether buffer has been filled once

public:
    /**
     * @brief Construct a new MovingAverageFilter object
     */
    MovingAverageFilter()
        : index_(0), sum_(0.0f), average_(0.0f), inited_(false)
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            buffer_[i] = 0.0f;
        }
    }

    /**
     * @brief Add a new sample to the filter
     * 
     * @param sample The new sample to insert
     * @return true Always returns true (placeholder for error handling)
     */
    bool put(float sample)
    {
        if (inited_) {
            sum_ -= buffer_[index_];
        }
        buffer_[index_] = sample;
        sum_ += sample;

        if (inited_) {
            average_ = sum_ / SIZE;
        } else {
            average_ = sum_ / (index_ + 1);  // Use actual sample count
            if (index_ == SIZE - 1) {
                inited_ = true;
                average_ = sum_ / SIZE;  // Recalculate with full buffer
            }
        }

        index_ = (index_ + 1) % SIZE;
        return true;
    }

    /**
     * @brief Get the current average
     * 
     * @return float The filtered average value
     */
    float get() const
    {
        return average_;
    }

    /**
     * @brief Reset the filter
     */
    void clear()
    {
        for (size_t i = 0; i < SIZE; ++i)
        {
            buffer_[i] = 0.0f;
        }
        index_ = 0;
        sum_ = 0.0f;
        average_ = 0.0f;
        inited_ = false;
    }

    /**
     * @brief Check whether the filter has received SIZE samples
     * 
     * @return true if buffer is fully initialized
     * @return false otherwise
     */
    bool is_ready() const
    {
        return inited_;
    }

    /**
     * @brief Get current sample count
     * 
     * @return size_t Number of samples currently in the buffer
     */
    size_t sample_count() const {
        return inited_ ? SIZE : index_;
    }

    /**
     * @brief Get buffer size
     * 
     * @return size_t The template parameter SIZE
     */
    static size_t buffer_size() {
        return SIZE;
    }
};

} // namespace util

#endif // __UTIL_MOVING_AVERAGE_FILTER_HPP__
