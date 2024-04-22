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
    List() : _head(), _size(0)
    {
        _head.next = &_head;
        _head.prev = &_head;
    }

    // Insert a new node after the specified position
    void insert(list_node_t *pos, T *newNode)
    {
        newNode->_list.next = pos->next;
        newNode->_list.prev = pos;
        pos->next->prev = &newNode->_list;
        pos->next = &newNode->_list;
        ++_size;
    }

    // Remove the specified node from the list
    void remove(T *entry)
    {
        entry->_list.next->prev = entry->_list.prev;
        entry->_list.prev->next = entry->_list.next;
        --_size;
    }

    // Push an element to the back of the list
    void push_back(T *newNode)
    {
        insert(&_head, newNode);
    }

    // Pop the element at the back of the list
    T *pop_back()
    {
        if (empty())
        {
            return nullptr;
        }
        T *back = reinterpret_cast<T *>(_head.prev);
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
        T *front = reinterpret_cast<T *>(_head.next);
        remove(front);
        return front;
    }

    // Get the size of the list
    size_t size() const
    {
        return _size;
    }

    // Check if the list is empty
    bool empty() const
    {
        return _size == 0;
    }

    // Get the pointer to the first element
    list_node_t *begin()
    {
        return _head.next;
    }

    // Get the pointer to the last element
    list_node_t *end()
    {
        return &_head;
    }

private:
    list_node_t _head; // Head of the list
    size_t _size;      // Size of the list
};

} // namespace util

#endif // __UTIL_LIST_H__

/************************************************************************/
/* how to use                                                           */
/************************************************************************/
#if 0
#include <iostream>
#include "embedded_list.h" // Include the header file

// Example class with a list_node_t member
class Node
{
public:
    util::list_node_t _list;
    int data;

    Node(int d) : data(d)
    {
    }
};

int main()
{
    util::List<Node> myList; // Instantiate the list

    // Create objects of Node
    Node node1(10), node2(20), node3(30);

    // Insert objects into the list
    myList.push_back(&node1);
    myList.push_back(&node2);
    myList.push_back(&node3);

    std::cout << "Size of the list: " << myList.size() << std::endl;

    // Remove an object from the list
    Node* removedNode = myList.pop_front();
    if (removedNode != nullptr)
    {
        std::cout << "Removed node with data: " << removedNode->data << std::endl;
    }

    std::cout << "Size after removal: " << myList.size() << std::endl;

    // Iterate over the list
    std::cout << "Elements in the list:" << std::endl;
    for (util::list_node_t* pos = myList.begin(); pos != myList.end(); pos = pos->next)
    {
        Node* node = reinterpret_cast<Node*>(pos);
        std::cout << node->data << std::endl; // Access data member of Node
    }

    return 0;
}

#endif
