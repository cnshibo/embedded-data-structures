#ifndef __SL_STATIC_POOL_H___
#define __SL_STATIC_POOL_H___

namespace util
{
template <typename T, size_t POOL_SIZE> class StaticPool
{
public:
    StaticPool() : _available_elements(POOL_SIZE)
    {
        /* Initialize all flags to indicate objects are not in use */
        memset(_used_flags, 0, POOL_SIZE * sizeof(bool));
        _next_index = 0;
    }

    /* Allocate an object from the pool */
    T *allocate()
    {
        if (_available_elements == 0) return NULL; /* No available objects in the pool */

        /* Check if the next object indicated by _next_index is available */
        if (_next_index < POOL_SIZE && !_used_flags[_next_index])
        {
            _used_flags[_next_index] = true; /* Mark object as in use */
            _available_elements--;           /* Decrement available objects count */
            return &_pool[_next_index++];
        }

        /* Find the next available object */
        for (size_t i = 0; i < POOL_SIZE; ++i)
        {
            if (!_used_flags[i])
            {
                _used_flags[i] = true; /* Mark object as in use */
                _available_elements--; /* Decrement available objects count */
                _next_index = i + 1;   /* Update next index */
                return &_pool[i];
            }
        }
        return NULL; /* No available objects in the pool */
    }

    /* Deallocate an object from the pool */
    bool deallocate(T *ptr)
    {
        if (ptr >= &_pool[0] && ptr < &_pool[POOL_SIZE])
        {
            size_t index = ((char *)ptr - (char *)&_pool[0]) / sizeof(T);
            if (_used_flags[index] && &_pool[index] == ptr)
            {
                _used_flags[index] = false; /* Mark object as not in use */
                _available_elements++;      /* Increment available objects count */
                if (index < _next_index)
                {
                    _next_index = index; /* Update next index if deallocated object was before it */
                }
                return true; // Deallocation successful
            }
        }
        return false; // Deallocation failed
    }

private:
    T _pool[POOL_SIZE];          /* Pool of objects */
    bool _used_flags[POOL_SIZE]; /* Flags indicating whether an object is in use */
    size_t _next_index;          /* Index of the next available slot in the pool */
    size_t _available_elements;  /* Number of available objects in the pool */
};
} // namespace util

#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/
#if 0

#include <iostream>
#include "StaticPool.h" // Include the header file for StaticPool

int main() {
    // Define a StaticPool of integers with a size of 10
    util::StaticPool<int, 10> intPool;

    // Allocate integers from the pool
    int* num1 = intPool.allocate();
    int* num2 = intPool.allocate();

    // Check if allocation was successful
    if (num1 && num2) {
        *num1 = 10;
        *num2 = 20;

        std::cout << "Allocated numbers: " << *num1 << ", " << *num2 << std::endl;

        // Deallocate the integers
        if (intPool.deallocate(num1)) {
            std::cout << "Deallocation successful for num1" << std::endl;
        } else {
            std::cout << "Deallocation failed for num1" << std::endl;
        }

        if (intPool.deallocate(num2)) {
            std::cout << "Deallocation successful for num2" << std::endl;
        } else {
            std::cout << "Deallocation failed for num2" << std::endl;
        }
    } else {
        std::cout << "Failed to allocate numbers from the pool" << std::endl;
    }

    return 0;
}

#endif
