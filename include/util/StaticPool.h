#ifndef __SL_STATIC_POOL_H___
#define __SL_STATIC_POOL_H___

namespace util
{
template <typename T, size_t POOL_SIZE> class StaticPool
{
public:
    StaticPool() : available_elements_(POOL_SIZE)
    {
        /* Initialize all flags to indicate objects are not in use */
        memset(used_flags_, 0, POOL_SIZE * sizeof(bool));
        next_index_ = 0;
    }

    /* Allocate an object from the pool */
    T *allocate()
    {
        if (available_elements_ == 0) return NULL; /* No available objects in the pool */

        /* Check if the next object indicated by next_index_ is available */
        if (next_index_ < POOL_SIZE && !used_flags_[next_index_])
        {
            used_flags_[next_index_] = true; /* Mark object as in use */
            available_elements_--;           /* Decrement available objects count */
            return &pool_[next_index_++];
        }

        /* Find the next available object */
        for (size_t i = 0; i < POOL_SIZE; ++i)
        {
            if (!used_flags_[i])
            {
                used_flags_[i] = true; /* Mark object as in use */
                available_elements_--; /* Decrement available objects count */
                next_index_ = i + 1;   /* Update next index */
                return &pool_[i];
            }
        }
        return NULL; /* No available objects in the pool */
    }

    /* Deallocate an object from the pool */
    bool deallocate(T *ptr)
    {
        if (ptr >= &pool_[0] && ptr < &pool_[POOL_SIZE])
        {
            size_t index = ((char *)ptr - (char *)&pool_[0]) / sizeof(T);
            if (used_flags_[index] && &pool_[index] == ptr)
            {
                used_flags_[index] = false; /* Mark object as not in use */
                available_elements_++;      /* Increment available objects count */
                if (index < next_index_)
                {
                    next_index_ = index; /* Update next index if deallocated object was before it */
                }
                return true; // Deallocation successful
            }
        }
        return false; // Deallocation failed
    }

private:
    T pool_[POOL_SIZE];          /* Pool of objects */
    bool used_flags_[POOL_SIZE]; /* Flags indicating whether an object is in use */
    size_t next_index_;          /* Index of the next available slot in the pool */
    size_t available_elements_;  /* Number of available objects in the pool */
};
} // namespace util

#endif
