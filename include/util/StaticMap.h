#ifndef __UTIL_STATIC_MAP_H__
#define __UTIL_STATIC_MAP_H__

namespace util
{

/*
// Custom hash function for uint16_t keys
uint64_t _custom_hash_function_uint16(const uint16_t key)
{
   // Simple hash function for uint16_t keys
   // Example: Just return the key itself
   return static_cast<uint64_t>(key);
}
*/

template <typename Key, typename Value, size_t TABLE_SIZE> class StaticMap
{
private:
    struct KeyValuePair
    {
        Key key_;
        Value *value_ptr_; // Pointer to Value object
        bool used_;        // Indicates if the slot is used or not
    };

    KeyValuePair table_[TABLE_SIZE];
    size_t (*hash_function_)(const Key &); // Function pointer for hash function

public:
    // Constructor that accepts a hash function
    StaticMap(size_t (*hf)(const Key &)) : hash_function_(hf)
    {
        // Initialize all slots as unused
        for (size_t i = 0; i < TABLE_SIZE; ++i)
        {
            table_[i].used_ = false;
            table_[i].value_ptr_ = NULL;
        }
    }

    ~StaticMap()
    {
        // Cleanup
        for (size_t i = 0; i < TABLE_SIZE; ++i)
        {
            table_[i].value_ptr_ = NULL; // Set value pointer to NULL
        }
    }

    bool insert(const Key &key, Value *value_ptr)
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        while (table_[index].used_)
        {
            // Linear probing for collision resolution
            if (table_[index].key_ == key)
            {
                // Key already exists, update value pointer
                table_[index].value_ptr_ = value_ptr; // Update value pointer
                return true;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == start_index)
            {
                // Table is full
                return false;
            }
        }

        // Insert new key-value pair
        table_[index].key_ = key;
        table_[index].value_ptr_ = value_ptr;
        table_[index].used_ = true;
        return true;
    }

    Value *get(const Key &key) const
    {
        size_t index = hash_function_(key);
        size_t start_index = index;
        Value *value_ptr = NULL;

        while (table_[index].used_)
        {
            if (table_[index].key_ == key)
            {
                // Key found, return corresponding value pointer
                value_ptr = table_[index].value_ptr_;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == start_index)
            {
                // Key not found
                value_ptr = NULL;
            }
        }

        return value_ptr;
    }

    bool contains(const Key &key) const
    {
        Value *temp = get(key, temp);
        
        if (temp == NULL)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool remove(const Key &key)
    {
        size_t index = hash_function_(key);
        size_t start_index = index;

        while (table_[index].used_)
        {
            if (table_[index].key_ == key)
            {
                // Key found, mark slot as unused and set value pointer to NULL
                table_[index].used_ = false;
                table_[index].value_ptr_ = NULL;
                return true;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == start_index)
            {
                // Key not found
                return false;
            }
        }

        // Key not found
        return false;
    }
};

} // namespace util

#endif // __UTIL_STATIC_MAP_H__
