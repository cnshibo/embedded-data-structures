#ifndef __UTIL_STATIC_POOL_H__
#define __UTIL_STATIC_POOL_H__


/**
 * @file StaticPool.h
 * @brief A static memory pool implementation for embedded systems
 */


namespace util
{
/**
 * @brief A static memory pool implementation with fixed size
 * 
 * This class implements a memory pool that pre-allocates a fixed number of
 * objects of the same type. It provides efficient memory allocation and
 * deallocation with O(1) time complexity and no fragmentation.
 * 
 * Features:
 * - Fixed size pool (specified at compile time)
 * - No dynamic memory allocation during runtime
 * - Thread-safe for single allocator/single deallocator scenarios
 * - Automatic object initialization and cleanup
 * - Memory reuse without fragmentation
 * 
 * @tparam T The type of objects to be stored in the pool
 * @tparam SIZE The maximum number of objects the pool can hold
 * 
 * Example usage:
 * @code
 * StaticPool<MyClass, 10> pool;
 * MyClass* obj = pool.allocate();
 * if (obj) {
 *     // Use the object
 *     pool.deallocate(obj);
 * }
 * @endcode
 */
template <typename T, size_t SIZE> class StaticPool
{
private:
    union PoolElement
    {
        T object_;                   ///< The actual object storage
        PoolElement *next_;          ///< Pointer to next free element (when free)
    };

    PoolElement pool_[SIZE];         ///< The pool storage array
    bool used_flags_[SIZE];          ///< Flags indicating which elements are in use
    PoolElement *next_index_;        ///< Pointer to next available element
    size_t available_elements_;      ///< Number of elements currently available

public:
    /**
     * @brief Construct a new Static Pool object
     * 
     * Initializes the pool and sets up the free list.
     */
    StaticPool() : next_index_(nullptr), available_elements_(SIZE)
    {
        // Initialize all elements as unused
        for (size_t i = 0; i < SIZE; ++i)
        {
            used_flags_[i] = false;
            if (i < SIZE - 1)
            {
                pool_[i].next_ = &pool_[i + 1];
            }
            else
            {
                pool_[i].next_ = nullptr;
            }
        }
        next_index_ = &pool_[0];
    }

    /**
     * @brief Destroy the Static Pool object
     * 
     * Calls destructors for all allocated objects.
     */
    ~StaticPool()
    {
        // Call destructors for all allocated objects
        for (size_t i = 0; i < SIZE; ++i)
        {
            if (used_flags_[i])
            {
                (&pool_[i].object_)->~T();
            }
        }
    }

    /**
     * @brief Allocate an object from the pool
     * 
     * Returns a pointer to an uninitialized object from the pool. The caller
     * is responsible for constructing the object using placement new.
     * 
     * @return T* Pointer to allocated memory, or nullptr if pool is full
     */
    T *allocate()
    {
        if (next_index_ == nullptr || available_elements_ == 0)
        {
            return nullptr;
        }

        PoolElement *element = next_index_;
        next_index_ = element->next_;
        available_elements_--;

        // Find the index of the allocated element
        size_t index = element - pool_;
        used_flags_[index] = true;

        return &element->object_;
    }

    /**
     * @brief Deallocate an object back to the pool
     * 
     * Returns an object to the pool. The caller is responsible for calling
     * the object's destructor before deallocating.
     * 
     * @param ptr Pointer to the object to deallocate
     * @return true if the object was successfully deallocated
     * @return false if the pointer is invalid or already deallocated
     */
    bool deallocate(T *ptr)
    {
        if (ptr == nullptr)
        {
            return false;
        }

        // Find the index of the element
        PoolElement *element = reinterpret_cast<PoolElement *>(ptr);
        size_t index = element - pool_;

        // Check if the pointer is valid and the element is in use
        if (index >= SIZE || !used_flags_[index])
        {
            return false;
        }

        // Add the element back to the free list
        element->next_ = next_index_;
        next_index_ = element;
        used_flags_[index] = false;
        available_elements_++;

        return true;
    }

    /**
     * @brief Check if the pool is empty
     * 
     * @return true if all elements are allocated
     * @return false if there are available elements
     */
    bool is_full() const
    {
        return available_elements_ == 0;
    }

    /**
     * @brief Get the number of available elements in the pool
     * 
     * @return size_t Number of elements that can still be allocated
     */
    size_t get_available_elements() const
    {
        return available_elements_;
    }

    /**
     * @brief Get the total capacity of the pool
     * 
     * @return size_t Maximum number of elements the pool can hold
     */
    size_t get_capacity() const
    {
        return SIZE;
    }
};

} // namespace util

#endif // __UTIL_STATIC_POOL_H__
