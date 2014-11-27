/****************************************************************************/
/*                                                                          */
/* AVLTree class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_AVLTREE_H_
#define _234218_WET1_AVLTREE_H_

#define IS_NULL(a) (a == 0)
#define NULL_CHECK(a) if(IS_NULL(a)) throw NullArgumentException()
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

#include <cstdlib>

// TODO remove
#include <iostream>


/* Exception classes */
class AVLTreeException {};
class NullArgumentException : public AVLTreeException {};
class ElementNotFoundException : public AVLTreeException {};
class DuplicateNodeException : public AVLTreeException {};


template<typename SearchType, typename DataType>
struct AVLNodeStruct {
	DataType data;
	struct AVLNodeStruct<SearchType, DataType>* right;
	struct AVLNodeStruct<SearchType, DataType>* left;
	struct AVLNodeStruct<SearchType, DataType>* root;

	AVLNodeStruct() {
		right = NULL;
		left = NULL;
		root = NULL;
	}
};


template<typename SearchType, typename DataType>
class AVLTree {
public:

	typedef AVLTree<SearchType, DataType> ThisType;
	typedef AVLNodeStruct<SearchType, DataType> *AVLNode;

    AVLTree() {
    	root = NULL;
    }

    AVLTree(AVLNode root) {
    	this->root = root;
    }

    virtual ~AVLTree() {};

	/**
	 * Abstract predicate function for comparing a search key and a data instance.
	 * Must be overriden to enable searching, insertion and removal from the tree.
	 * @return 1 if key > data, -1 if key < data and 0 if both are considered equal
	 */
	virtual int predKeyData(SearchType& key, DataType& data) const = 0;

	/**
	 * Abstract predicate function for comparing two data instance.
	 * Must be overriden to enable insertion and removal from the tree.
	 * @return 1 if data > other, -1 if data < other and 0 if both are equal
	 */
	virtual int predDataData(DataType& data, DataType& other) const = 0;

	/**
	 * Adds a new data element to the tree. A search key must also be provided to enable throwing an
	 * exception if a node with a duplicate key is identified
	 * @throws DuplicateNodeException if a node with the provided key already exists
	 */
    void insert(SearchType& key, DataType& data) {
    	root = recursiveInsert(root, key, data);
    }

    /**
     * Removes a data element from the tree.
     * @throws ElementNotFoundException if an element with the provided key was not found
     */
    void remove(SearchType& key) {
    	root = recursiveRemove(root, key);
    }

    /**
     * Dumps the tree into a sorted array.
     * @return The size of the array
     */
    int enumerateData(DataType array[]) {
    	NULL_CHECK(array);
    	int beginIndex = 0;

    	doInOrderEnumeration(root, array, &beginIndex);
    	return beginIndex;
    };

    /**
     * Fills the tree with elements from a sorted array with identical size to the tree.
     * Any other array (unsorted or being of a different size) will result in a failure
     * and no exception will be thrown.
     */
    void arrayFillTree(DataType array[]) {
    	NULL_CHECK(array);
    	int beginIndex = 0;

    	doInOrderFill(root, array, &beginIndex);
    };

    /**
     * Returns the data of an element with the provided key.
     * @throws ElementNotFoundException when no element with the provided key was found
     */
    DataType* findBySearchKey(SearchType& key) {
    	NULL_CHECK(key);
    	return &(binarySearch(root, key)->data);
    }

    /**
     * Returns the max data element in the tree, being the right-lower-most element.
     * @throws ElementNotFoundException when the tree is completely empty
     */
    DataType* getMax() {
    	if(IS_NULL(root)) {
    		throw ElementNotFoundException();
    	}

    	return &(findMax(root)->data);
    }

    /**
     * Returns the total number of nodes in the tree
     */
    const int getTreeSize() {
    	return calculateTreeSize(0, root);
    }

    /**
     * Returns the tree's height, being the length of the longest route from the root to a leaf
     */
    int getTreeHeight() const {
    	return calculateHeight(root);
    }

private:
    AVLNode root;

    /* Internal helper functions */

    int getNodeBalanceFactor(AVLNode node) {
    	int rightHeight = calculateHeight(node->right);
    	int leftHeight = calculateHeight(node->left);

    	return leftHeight - rightHeight;
    }

    /* Rotation functions */
    AVLNode rrRotate(AVLNode parent) {
    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = tempNode->left;
    	tempNode->left = parent;

    	return tempNode;
    }

    AVLNode llRotate(AVLNode parent) {
    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = tempNode->right;
    	tempNode->right = parent;

    	return tempNode;
    }

    AVLNode rlRotate(AVLNode parent) {
    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = llRotate(tempNode);

    	return rrRotate(parent);
    }

    AVLNode lrRotate(AVLNode parent) {
    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = rrRotate(tempNode);

    	return llRotate(parent);
    }


    AVLNode balanceNode(AVLNode node) {
    	if(IS_NULL(node)) {
    		return node;
    	}

    	int balanceFactor = getNodeBalanceFactor(node);

    	/* Evaluate the balance factor and determine whether, and which,
    	 * rotation is required to balance the tree */
    	AVLNode balancedNode = node;
    	if(balanceFactor >= 2) {
    		if(getNodeBalanceFactor(node->left) > 0) {
    			balancedNode = llRotate(node);
    		} else {
    			balancedNode = lrRotate(node);
    		}
    	} else if(balanceFactor <= -2) {
    		if(getNodeBalanceFactor(node->right) > 0) {
    			balancedNode = rlRotate(node);
    		} else {
    			balancedNode = rrRotate(node);
    		}
    	}

    	return balancedNode;
    }


    /* Internal helper recursive functions */

    AVLNode recursiveInsert(AVLNode node, SearchType& key, DataType& data) {
    	// If node is null, the algorithm has reached the insertion location. Create and return
    	if(IS_NULL(node)) {
    		AVLNode newNode = new AVLNodeStruct<SearchType, DataType>();
    		newNode->data = data;
    		newNode->root = root;
    		return newNode;
    	}
    	// If there is another node with the same key, stop and throw an exception
    	else if(predKeyData(key, node->data) == 0) {
    		throw DuplicateNodeException();
    	}
    	// If the current node is considered greater than the data, continue on the left subtree
    	else if(predDataData(data, node->data) < 0) {
    		node->left = recursiveInsert(node->left, key, data);
    	}
    	// If the current node is considered lower than the data, continue on the right subtree
    	else {
    		node->right = recursiveInsert(node->right, key, data);
    	}

    	// Finally, balance the current node
    	return balanceNode(node);
    }

    AVLNode recursiveRemove(AVLNode node, SearchType& searchKey) {
    	if(IS_NULL(node)) {
    		throw ElementNotFoundException();
    	}

    	if(predKeyData(searchKey, node->data) > 0) {
    		node->right = recursiveRemove(node->right, searchKey);
    	} else if(predKeyData(searchKey, node->data) < 0) {
    		node->left = recursiveRemove(node->left, searchKey);
    	} else {
    		// Split into 3 cases:
    		if(IS_NULL(node->left) || IS_NULL(node->right)) {
    			AVLNode child = node->right ? node->right : node->left;
    			AVLNode temp;

    			// Case 1: If the node has no children, delete it and release its pointer
    			if(IS_NULL(child)) {
    				temp = node;
    				node = NULL;
    				delete(temp);
    			}
    			// Case 2: If the node has a single child, replace the node
    			// with the child and release the original child
    			else {
    				*node = *child;
    				delete(child);
    			}
    		}
    		// Case 3: If the node has two children, substitute it with the
    		// next inorder node and delete the successor node
    		else {
    			// Find the next inorder node (find the left-most node in the right subtree)
    			AVLNode nextNode = findNextInorder(node->right);

    			// Substitute the nodes' data, preserving their child pointers
    			DataType temp = nextNode->data;
    			nextNode->data = node->data;
    			node->data = temp;

    			// Continue by running the algorithm on the right subtree and delete the replaced node.
    			// The successor node will have either 0 or 1 (right) child.
    			node->right = recursiveRemove(node->right, searchKey);
    		}
    	}

    	// Finally, balance the current node
    	return balanceNode(node);
    }

    AVLNode binarySearch(AVLNode node, SearchType& searchKey) {
    	if(predKeyData(searchKey, node->data) == 0) {
    		return node;
    	}

    	if(predKeyData(searchKey, node->data) > 0) {
    		if(IS_NULL(node->right)) {
    			throw ElementNotFoundException();
    		} else {
    			return binarySearch(node->right, searchKey);
    		}
    	} else {
    		if(IS_NULL(node->left)) {
    			throw ElementNotFoundException();
    		} else {
    			return binarySearch(node->left, searchKey);
    		}
    	}
    }

    AVLNode findMax(AVLNode node) {
    	if(!IS_NULL(node->right)) {
    		return findMax(node->right);
    	}

    	return node;
    }

    int calculateTreeSize(int currentSize, AVLNode node) {
    	if(currentSize < 0 || IS_NULL(node)) {
    		return 0;
    	}

    	// The size of the tree consists of the size of the right tree, left tree and the current node
    	return 1 + calculateTreeSize(0, node->right) + calculateTreeSize(0, node->left);
    }

    int calculateHeight(AVLNode node) {
    	int height = 0;

    	if(!IS_NULL(node)) {
    		int leftHeight = calculateHeight(node->left);
    		int rightHeight = calculateHeight(node->right);
    		height = MAX(leftHeight, rightHeight) + 1;
    	}

    	return height;
    }

    void doInOrderEnumeration(AVLNode node, DataType* array, int* currentIndex) {
    	if(IS_NULL(node)) {
    		return;
    	}

    	doInOrderEnumeration(node->left, array, currentIndex);
    	int indexValue = *currentIndex;
    	array[indexValue++] = node->data;
    	*currentIndex = indexValue;
    	doInOrderEnumeration(node->right, array, currentIndex);
    }

    void doInOrderFill(AVLNode node, DataType* array, int* currentIndex) {
    	if(IS_NULL(node)) {
    		return;
    	}

    	doInOrderFill(node->left, array, currentIndex);
    	int indexValue = *currentIndex;
    	node->data = array[indexValue++];
    	*currentIndex = indexValue;
    	doInOrderFill(node->right, array, currentIndex);
    }

    AVLNode findNextInorder(AVLNode node) {
    	if(IS_NULL(node->left)) {
    		return node;
    	}

    	return findNextInorder(node->left);
    }
};

#endif    /* _234218_WET1_AVLTREE_H_ */
