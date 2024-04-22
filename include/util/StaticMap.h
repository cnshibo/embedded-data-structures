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
        Key _key;
        Value *_value_ptr; // Pointer to Value object
        bool _used;        // Indicates if the slot is used or not
    };

    KeyValuePair _table[TABLE_SIZE];
    size_t (*_hash_function)(const Key &); // Function pointer for hash function

public:
    // Constructor that accepts a hash function
    StaticMap(size_t (*hf)(const Key &)) : _hash_function(hf)
    {
        // Initialize all slots as unused
        for (size_t i = 0; i < TABLE_SIZE; ++i)
        {
            _table[i]._used = false;
            _table[i]._value_ptr = NULL;
        }
    }

    ~StaticMap()
    {
        // Cleanup
        for (size_t i = 0; i < TABLE_SIZE; ++i)
        {
            _table[i]._value_ptr = NULL; // Set value pointer to NULL
        }
    }

    bool insert(const Key &key, Value *value_ptr)
    {
        size_t index = _hash_function(key);
        size_t start_index = index;

        while (_table[index]._used)
        {
            // Linear probing for collision resolution
            if (_table[index]._key == key)
            {
                // Key already exists, update value pointer
                _table[index]._value_ptr = value_ptr; // Update value pointer
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
        _table[index]._key = key;
        _table[index]._value_ptr = value_ptr;
        _table[index]._used = true;
        return true;
    }

    Value *get(const Key &key) const
    {
        size_t index = _hash_function(key);
        size_t start_index = index;
        Value *value_ptr = NULL;

        while (_table[index]._used)
        {
            if (_table[index]._key == key)
            {
                // Key found, return corresponding value pointer
                value_ptr = _table[index]._value_ptr;
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
        size_t index = _hash_function(key);
        size_t start_index = index;

        while (_table[index]._used)
        {
            if (_table[index]._key == key)
            {
                // Key found, mark slot as unused and set value pointer to NULL
                _table[index]._used = false;
                _table[index]._value_ptr = NULL;
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

/************************************************************************/
/* usage example                                                        */
/************************************************************************/

#if 0

#include "StaticMap.h"

// Define necessary types and constants
typedef uint16_t KeyType;
typedef int ValueType;
const size_t TABLE_SIZE = 100;

// Custom hash function for uint16_t keys
size_t custom_hash_function_uint16(const KeyType key)
{
    return static_cast<size_t>(key) % TABLE_SIZE;
}

int main()
{
    // Create a StaticMap object with the custom hash function
    util::StaticMap<KeyType, ValueType, TABLE_SIZE> map(custom_hash_function_uint16);

    // Create some key-value pairs
    KeyType key1 = 123;
    ValueType value1 = 42;

    KeyType key2 = 456;
    ValueType value2 = 84;

    // Insert key-value pairs into the map
    map.insert(key1, &value1);
    map.insert(key2, &value2);

    // Retrieve values from the map
    ValueType *retrievedValue1 = NULL;
    ValueType *retrievedValue2 = NULL;

    if (map.get(key1, retrievedValue1))
    {
        // Value for key1 found
        std::cout << "Value for key1: " << *retrievedValue1 << std::endl;
    }
    else
    {
        // Key1 not found
        std::cout << "Key1 not found in the map" << std::endl;
    }

    if (map.get(key2, retrievedValue2))
    {
        // Value for key2 found
        std::cout << "Value for key2: " << *retrievedValue2 << std::endl;
    }
    else
    {
        // Key2 not found
        std::cout << "Key2 not found in the map" << std::endl;
    }

    // Remove a key-value pair from the map
    map.remove(key1);

    return 0;
}

#endif
