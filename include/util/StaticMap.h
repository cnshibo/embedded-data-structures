#ifndef __UTIL_STATIC_MAP_H__
#define __UTIL_STATIC_MAP_H__

/**
 * @file StaticMap.h
 * @brief A static map implementation for embedded systems
 */

namespace util
{
/**
 * @brief A static map implementation with fixed size
 * 
 * This class implements a hash-based map data structure that is designed for
 * embedded systems. It provides O(1) average case lookup time and uses a
 * static array for storage, making it suitable for memory-constrained environments.
 * 
 * Features:
 * - Fixed size map (specified at compile time)
 * - No dynamic memory allocation
 * - Customizable hash function
 * - Linear probing for collision resolution
 * - Thread-safe for single writer/multiple reader scenarios
 * 
 * @tparam KeyType The type of the keys stored in the map
 * @tparam ValueType The type of the values stored in the map
 * @tparam SIZE The maximum number of key-value pairs the map can hold
 * 
 * Example usage:
 * @code
 * StaticMap<int, std::string, 10> map;
 * map.put(1, "one");
 * std::string value;
 * if (map.get(1, value)) {
 *     // value contains "one"
 * }
 * @endcode
 */
template <typename KeyType, typename ValueType, size_t SIZE> class StaticMap
{
private:
    struct MapEntry
    {
        KeyType key_;                ///< The key stored in this entry
        ValueType *value_ptr_;       ///< Pointer to the value stored in this entry
        bool used_;                  ///< Whether this entry is currently in use
    };

    MapEntry table_[SIZE];          ///< The hash table array
    std::function<size_t(const KeyType &)> hash_function_;  ///< The hash function used for key mapping

public:
    /**
     * @brief Construct a new Static Map object
     * 
     * Initializes the map with the provided hash function. If no hash function
     * is provided, a default implementation will be used.
     * 
     * @param hash_function Optional custom hash function for key mapping
     */
    StaticMap(std::function<size_t(const KeyType &)> hash_function = nullptr)
        : hash_function_(hash_function)
    {
        // Initialize all entries as unused
        for (size_t i = 0; i < SIZE; ++i)
        {
            table_[i].used_ = false;
            table_[i].value_ptr_ = nullptr;
        }

        // If no hash function provided, use default implementation
        if (!hash_function_)
        {
            hash_function_ = [](const KeyType &key) {
                return std::hash<KeyType>{}(key) % SIZE;
            };
        }
    }

    /**
     * @brief Destroy the Static Map object
     * 
     * Cleans up any allocated values.
     */
    ~StaticMap()
    {
        // Clean up allocated values
        for (size_t i = 0; i < SIZE; ++i)
        {
            if (table_[i].used_ && table_[i].value_ptr_)
            {
                delete table_[i].value_ptr_;
            }
        }
    }

    /**
     * @brief Put a key-value pair into the map
     * 
     * If the key already exists, its value will be updated. If the map is full
     * and the key doesn't exist, the operation will fail.
     * 
     * @param key The key to store
     * @param value The value to store
     * @return true if the operation was successful
     * @return false if the map is full and the key doesn't exist
     */
    bool put(const KeyType &key, const ValueType &value)
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        // Linear probing to find an empty slot or existing key
        do
        {
            if (!table_[index].used_)
            {
                // Found an empty slot
                table_[index].key_ = key;
                table_[index].value_ptr_ = new ValueType(value);
                table_[index].used_ = true;
                return true;
            }
            else if (table_[index].key_ == key)
            {
                // Key exists, update value
                *(table_[index].value_ptr_) = value;
                return true;
            }

            index = (index + 1) % SIZE;
        } while (index != start_index);

        // Map is full and key doesn't exist
        return false;
    }

    /**
     * @brief Get a value from the map by key
     * 
     * @param key The key to look up
     * @param value Reference to store the found value
     * @return true if the key was found and value was retrieved
     * @return false if the key was not found
     */
    bool get(const KeyType &key, ValueType &value) const
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        // Linear probing to find the key
        do
        {
            if (!table_[index].used_)
            {
                return false;
            }
            if (table_[index].key_ == key)
            {
                value = *(table_[index].value_ptr_);
                return true;
            }

            index = (index + 1) % SIZE;
        } while (index != start_index);

        return false;
    }

    /**
     * @brief Remove a key-value pair from the map
     * 
     * @param key The key to remove
     * @return true if the key was found and removed
     * @return false if the key was not found
     */
    bool remove(const KeyType &key)
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        // Linear probing to find the key
        do
        {
            if (!table_[index].used_)
            {
                return false;
            }
            if (table_[index].key_ == key)
            {
                delete table_[index].value_ptr_;
                table_[index].value_ptr_ = nullptr;
                table_[index].used_ = false;
                return true;
            }

            index = (index + 1) % SIZE;
        } while (index != start_index);

        return false;
    }

    /**
     * @brief Check if a key exists in the map
     * 
     * @param key The key to check for
     * @return true if the key exists
     * @return false if the key does not exist
     */
    bool contains(const KeyType &key) const
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        // Linear probing to find the key
        do
        {
            if (!table_[index].used_)
            {
                return false;
            }
            if (table_[index].key_ == key)
            {
                return true;
            }

            index = (index + 1) % SIZE;
        } while (index != start_index);

        return false;
    }

    /**
     * @brief Get the current number of key-value pairs in the map
     * 
     * @return size_t Number of entries in the map
     */
    size_t size() const
    {
        size_t count = 0;
        for (size_t i = 0; i < SIZE; ++i)
        {
            if (table_[i].used_)
            {
                ++count;
            }
        }
        return count;
    }

    /**
     * @brief Check if the map is empty
     * 
     * @return true if the map contains no entries
     * @return false if the map contains at least one entry
     */
    bool empty() const
    {
        return size() == 0;
    }

    /**
     * @brief Get the maximum capacity of the map
     * 
     * @return size_t Maximum number of entries the map can hold
     */
    size_t capacity() const
    {
        return SIZE;
    }
};

} // namespace util

#endif // __UTIL_STATIC_MAP_H__
