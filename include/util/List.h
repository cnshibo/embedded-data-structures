#ifndef __UTIL_LIST_H__
#define __UTIL_LIST_H__

#include <cstddef> // For size_t

namespace util
{

// Linux kernel-style list structure
typedef struct list_head
{
    struct list_head *next, *prev;
} list_node_t;

// Embedded list class template
template <typename T> class List
{
public:
    // Constructor
    List() : head_(), size_(0)
    {
        head_.next = &head_;
        head_.prev = &head_;
    }

    // Insert a new node after the specified position
    void insert(list_node_t *pos, T *newNode)
    {
        newNode->_list.next = pos->next;
        newNode->_list.prev = pos;
        pos->next->prev = &newNode->_list;
        pos->next = &newNode->_list;
        ++size_;
    }

    // Remove the specified node from the list
    void remove(T *entry)
    {
        entry->_list.next->prev = entry->_list.prev;
        entry->_list.prev->next = entry->_list.next;
        --size_;
    }

    // Push an element to the back of the list
    void push_back(T *newNode)
    {
        insert(&head_, newNode);
    }

    // Pop the element at the back of the list
    T *pop_back()
    {
        if (empty())
        {
            return nullptr;
        }
        T *back = reinterpret_cast<T *>(head_.prev);
        remove(back);
        return back;
    }

    // Pop the element at the front of the list
    T *pop_front()
    {
        if (empty())
        {
            return nullptr;
        }
        T *front = reinterpret_cast<T *>(head_.next);
        remove(front);
        return front;
    }

    // Get the size of the list
    size_t size() const
    {
        return size_;
    }

    // Check if the list is empty
    bool empty() const
    {
        return size_ == 0;
    }

    // Get the pointer to the first element
    list_node_t *begin()
    {
        return head_.next;
    }

    // Get the pointer to the last element
    list_node_t *end()
    {
        return &head_;
    }

private:
    list_node_t head_; // Head of the list
    size_t size_;      // Size of the list
};

} // namespace util

#endif // __UTIL_LIST_H__
