/****************************************************************************/
/*                                                                          */
/* DoubleLinkedList class header                                            */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_DOUBLE_LINKED_LIST_H_
#define _234218_WET1_DOUBLE_LINKED_LIST_H_

#include "library1.h"


template<typename ValueType>
class DoubleLinkedList {
public:
    // Constructor - Creates a new empty list
    // Time complexity: O(1)
    DoubleLinkedList();

    // Destructor
    // Time complexity: O(k) where k is the list's length
    ~DoubleLinkedList();

    // Insert an element as the new first element
    // Time complexity: O(1)
    // Throws bad_alloc on allocation error
    void insertFront(const ValueType& elem);

    // Get the first element
    // Time complexity: O(1)
    // Throws NoSuchNodeException
    ValueType* getFront() const;

    // Get the first element
    // Time complexity: O(1)
    unsigned int getSize() const;

    // Get the first element
    // Time complexity: O(1)
    bool isEmpty() const;

    // Not needed for this exercise
    //void remove(const ValueType& elem);

    // Get the value of the first node matching condition. The extra parameter
    // is sent to condition on execution.
    // (Condition should receive ValueType* and return true of false)
    // Time complexity: O(k) where k is the list's length
    // Throws NoSuchNodeException if no node matching the condition is found
    template<typename Condition>
    ValueType* getDataByPredicate(const Condition& condition, void* extra) const;


    // Exception classes
    class NoSuchNodeException {};

protected:
    struct Node {
        ValueType* data;
        Node* next;
        Node* prev;
    };

    Node* mHead;
    unsigned int mSize;
};

#endif    /* _234218_WET1_DOUBLE_LINKED_LIST_H_ */
