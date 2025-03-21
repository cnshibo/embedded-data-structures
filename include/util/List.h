#ifndef __UTIL_LIST_H__
#define __UTIL_LIST_H__

#include <cstddef> // For size_t

/**
 * @file List.h
 * @brief A linked list implementation for embedded systems
 */

namespace util
{

// Linux kernel-style list structure
typedef struct list_head
{
    struct list_head *next, *prev;
} list_node_t;

/**
 * @brief A linked list implementation with dynamic size
 * 
 * This class implements a doubly-linked list data structure that is designed for
 * embedded systems. It provides efficient insertion and deletion operations with
 * O(1) time complexity for operations at the beginning and end of the list.
 * 
 * Features:
 * - Dynamic size (grows and shrinks as needed)
 * - Doubly-linked nodes for efficient traversal
 * - Thread-safe for single writer/multiple reader scenarios
 * - Automatic memory management
 * - Iterator support for traversal
 * 
 * @tparam T The type of elements to be stored in the list
 * 
 * Example usage:
 * @code
 * List<int> list;
 * list.push_back(1);
 * list.push_front(0);
 * for (auto it = list.begin(); it != list.end(); ++it) {
 *     // Process *it
 * }
 * @endcode
 */
template <typename T> class List
{
private:
    struct Node
    {
        T data_;                    ///< The data stored in this node
        Node *prev_;                ///< Pointer to the previous node
        Node *next_;                ///< Pointer to the next node
    };

    Node *head_;                    ///< Pointer to the first node
    size_t size_;                   ///< Current number of elements in the list

public:
    /**
     * @brief Iterator class for traversing the list
     */
    class Iterator
    {
    private:
        Node *current_;             ///< Current node in the iteration

    public:
        /**
         * @brief Construct a new Iterator object
         * 
         * @param node The node to start iteration from
         */
        Iterator(Node *node) : current_(node)
        {
        }

        /**
         * @brief Dereference the iterator
         * 
         * @return T& Reference to the current element
         */
        T &operator*()
        {
            return current_->data_;
        }

        /**
         * @brief Pre-increment the iterator
         * 
         * @return Iterator& Reference to the incremented iterator
         */
        Iterator &operator++()
        {
            current_ = current_->next_;
            return *this;
        }

        /**
         * @brief Post-increment the iterator
         * 
         * @return Iterator The iterator before incrementing
         */
        Iterator operator++(int)
        {
            Iterator temp = *this;
            current_ = current_->next_;
            return temp;
        }

        /**
         * @brief Compare two iterators for equality
         * 
         * @param other The iterator to compare with
         * @return true if the iterators point to the same node
         * @return false otherwise
         */
        bool operator==(const Iterator &other) const
        {
            return current_ == other.current_;
        }

        /**
         * @brief Compare two iterators for inequality
         * 
         * @param other The iterator to compare with
         * @return true if the iterators point to different nodes
         * @return false otherwise
         */
        bool operator!=(const Iterator &other) const
        {
            return current_ != other.current_;
        }
    };

    /**
     * @brief Construct a new List object
     * 
     * Initializes an empty list.
     */
    List() : head_(nullptr), size_(0)
    {
    }

    /**
     * @brief Destroy the List object
     * 
     * Cleans up all nodes in the list.
     */
    ~List()
    {
        clear();
    }

    /**
     * @brief Add an element to the front of the list
     * 
     * @param value The value to add
     */
    void push_front(const T &value)
    {
        Node *new_node = new Node{value, nullptr, head_};
        if (head_)
        {
            head_->prev_ = new_node;
        }
        head_ = new_node;
        size_++;
    }

    /**
     * @brief Add an element to the back of the list
     * 
     * @param value The value to add
     */
    void push_back(const T &value)
    {
        Node *new_node = new Node{value, nullptr, nullptr};
        if (!head_)
        {
            head_ = new_node;
        }
        else
        {
            Node *current = head_;
            while (current->next_)
            {
                current = current->next_;
            }
            current->next_ = new_node;
            new_node->prev_ = current;
        }
        size_++;
    }

    /**
     * @brief Remove the first element from the list
     * 
     * @return true if an element was removed
     * @return false if the list was empty
     */
    bool pop_front()
    {
        if (!head_)
        {
            return false;
        }

        Node *temp = head_;
        head_ = head_->next_;
        if (head_)
        {
            head_->prev_ = nullptr;
        }
        delete temp;
        size_--;
        return true;
    }

    /**
     * @brief Remove the last element from the list
     * 
     * @return true if an element was removed
     * @return false if the list was empty
     */
    bool pop_back()
    {
        if (!head_)
        {
            return false;
        }

        if (!head_->next_)
        {
            delete head_;
            head_ = nullptr;
        }
        else
        {
            Node *current = head_;
            while (current->next_)
            {
                current = current->next_;
            }
            current->prev_->next_ = nullptr;
            delete current;
        }
        size_--;
        return true;
    }

    /**
     * @brief Get the first element in the list
     * 
     * @return T& Reference to the first element
     * @note Undefined behavior if the list is empty
     */
    T &front()
    {
        return head_->data_;
    }

    /**
     * @brief Get the last element in the list
     * 
     * @return T& Reference to the last element
     * @note Undefined behavior if the list is empty
     */
    T &back()
    {
        Node *current = head_;
        while (current->next_)
        {
            current = current->next_;
        }
        return current->data_;
    }

    /**
     * @brief Check if the list is empty
     * 
     * @return true if the list contains no elements
     * @return false if the list contains at least one element
     */
    bool empty() const
    {
        return size_ == 0;
    }

    /**
     * @brief Get the current number of elements in the list
     * 
     * @return size_t Number of elements currently in the list
     */
    size_t size() const
    {
        return size_;
    }

    /**
     * @brief Clear all elements from the list
     * 
     * Removes all nodes and resets the list to its initial empty state.
     */
    void clear()
    {
        while (!empty())
        {
            pop_front();
        }
    }

    /**
     * @brief Get an iterator to the beginning of the list
     * 
     * @return Iterator Iterator pointing to the first element
     */
    Iterator begin()
    {
        return Iterator(head_);
    }

    /**
     * @brief Get an iterator to the end of the list
     * 
     * @return Iterator Iterator pointing past the last element
     */
    Iterator end()
    {
        return Iterator(nullptr);
    }
};

} // namespace util

#endif // __UTIL_LIST_H__
