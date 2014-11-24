/****************************************************************************/
/*                                                                          */
/* DoubleLinkedList class implementation                                    */
/*                                                                          */
/****************************************************************************/

#include "DoubeLinkedList.h"


template<typename ValueType>
DoubleLinkedList::DoubleLinkedList() :
    mHead(NULL), mSize(0) {}

template<typename ValueType>
DoubleLinkedList::~DoubleLinkedList() {
    // Free all nodes
    while (mHead) {
        mHead = current->next;
        // Free data
        delete current->data;
        // Free the node struct itself
        delete current;
        current = mhead;
    }
}

template<typename ValueType>
void DoubleLinkedList::insert(const ValueType& elem) {
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
}

template<typename ValueType>
template<typename Condition>
ValueType* DoubleLinkedList::getDataByPredicate(const Condition& condition,
        void* extra) const {

    // Iterate over all nodes until one matches the condition
    Node* p = head;
    while (p) {
        if (condition(p->data, extra)) {
            // Found it !
            return p->data;
        }
        p = p->next;
    }

    throw NoSuchNodeException();
}

template<typename ValueType>
ValueType* DoubleLinkedList::getFront() const {
    if (isEmpty()) {
        throw NoSuchNodeException();
    }
    return mHead->data;
}

template<typename ValueType>
unsigned int DoubleLinkedList::getSize() const {
    return mSize;
}

template<typename ValueType>
bool DoubleLinkedList::isEmpty() const {
    return (mSize == 0);
}
