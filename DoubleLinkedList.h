/****************************************************************************/
/*                                                                          */
/* DoubleLinkedList template class implementation                           */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_DOUBLE_LINKED_LIST_H_
#define _234218_WET1_DOUBLE_LINKED_LIST_H_

#include <cstdlib>                          // NULL definition
#include <new>                              // bad_alloc definition
#include <exception>

using std::bad_alloc;
using std::exception;


// Exception classes
class NoSuchNodeException : public exception {};
class InvalidIteratorException : public exception {};


template<typename ValueType>
class DoubleLinkedList {
public:

    // Define the internal Node structure
    struct Node {
        ValueType* data;
        Node* next;
        Node* prev;
    };

    // Define an iterator class, to be returned on insert and which can
    // be provided to remove (among other things).
    class Iterator {
    public:
        friend class DoubleLinkedList<ValueType>;

        // Default constructor - Creates an invalid Iterator
        Iterator() : mNode(NULL) {};

        // Constructor from node pointer
        Iterator(Node* node) : mNode(node) {};

        Iterator operator++() {
            if (mNode == NULL) {
                return *this;
            }

            Node* oldPtr = mNode;
            mNode = mNode->next;
            return Iterator(oldPtr);
        }

        // Getters for the data
        ValueType* operator*() {
            if (mNode == NULL) {
                throw InvalidIteratorException();
            }
            return mNode->data;
        }
        ValueType* operator->() {
            if (mNode == NULL) {
                throw InvalidIteratorException();
            }
            return mNode->data;
        }

        // Const getters for the data
        const ValueType* operator*() const {
            return mNode->data;
        }
        const ValueType* operator->() const {
            return mNode->data;
        }

    private:
        // Pointer to node. If mNode is NULL the iterator is considered invalid
        Node* mNode;
    };


    // Constructor - Creates a new empty list
    // Time complexity: O(1)
    DoubleLinkedList() : mHead(NULL), mSize(0) {}

    // Destructor
    // Time complexity: O(k) where k is the list's length
    virtual ~DoubleLinkedList() {
        // Free all nodes
        while (mHead) {
            // Free data
            delete mHead->data;
            // Free the node struct itself
            delete mHead;
            mHead = mHead->next;
        }
    }

    // Insert an element as the new first element. The given element is
    // copied (using copy constructor) and the copy is placed in the list.
    // Time complexity: O(1)
    // Throws bad_alloc on allocation error
    virtual Iterator insertFront(const ValueType& elem) {
        // Allocate a new node
        Node* node = new Node;

        // Allocate a copy of elem, to keep in our node
        try {
            node->data = new ValueType(elem);
        } catch (const bad_alloc& e) {
            delete node;
            throw;
        }

        node->next = mHead;
        node->prev = NULL;

        // Insert the new node into the list and update mSize accordingly
        mHead = node;
        mSize++;

        return Iterator(node);
    }

    // Remove the head of the list.
    // Time complexity: O(1)
    virtual void removeFront() {
        if (isEmpty()) {
            throw NoSuchNodeException();
        }

        Node* nextNode = mHead->next;
        // Free the front node:
        delete mHead->data;
        delete mHead;

        // Update mHead
        mHead = nextNode;
    }

    // Remove the element pointed by the given iterator
    // Time complexity: O(1)
    virtual void remove(const Iterator& iter) {
        if (iter.mNode == NULL) {
            throw InvalidIteratorException();
        }

        Node* prevNode = iter.mNode->prev;
        if (prevNode == NULL) {
            prevNode = mHead;
        }

        Node* nextNode = iter.mNode->next;

        // Rewire prevNode's next pointer to nextNode
        prevNode->next = nextNode;
        // Rewire nextNode's prev pointer to prevNode (unless our node is
        // actually last in the list)
        if (nextNode != NULL) {
            nextNode->prev = prevNode;
        }

        // Finally, free our node
        delete mHead->data;
        delete mHead;
    }

    // Get the first element
    // Time complexity: O(1)
    // Throws InvalidIteratorException
    virtual Iterator begin() const {
        if (isEmpty()) {
            throw InvalidIteratorException();
        }
        return Iterator(mHead);
    }

    // Iterator end() function for iterating over the list
    // Return the value that an iterator gets after being incremented past
    // the last element of the list
    // Time complexity: O(1)
    virtual Iterator end() const {
        return Iterator(NULL);
    }

    // Get the first element
    // Time complexity: O(1)
    virtual unsigned int getSize() const {
        return mSize;
    }

    // Get the first element
    // Time complexity: O(1)
    virtual bool isEmpty() const {
        return (mSize == 0);
    }

    // Get the value of the first node matching condition. The extra parameter
    // is sent to condition on execution.
    // (Condition should receive ValueType* and return true of false)
    // Time complexity: O(k) where k is the list's length
    // Throws NoSuchNodeException if no node matching the condition is found
    template<typename Condition>
    ValueType* getDataByPredicate(const Condition& condition,
            void* extra) const {

        // Iterate over all nodes until one matches the condition
        Node* p = mHead;
        while (p) {
            if (condition(p->data, extra)) {
                // Found it !
                return p->data;
            }
            p = p->next;
        }

        throw NoSuchNodeException();
    }


protected:
    Node* mHead;
    unsigned int mSize;
};

#endif    /* _234218_WET1_DOUBLE_LINKED_LIST_H_ */
