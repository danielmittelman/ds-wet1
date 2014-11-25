/****************************************************************************/
/*                                                                          */
/* AVLTree class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_AVLTREE_H_
#define _234218_WET1_AVLTREE_H_

#define IS_NULL(a) (a == NULL)
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define ABS(a) (((a) < 0) ? (-1*(a)) : (a))

#include <cstdlib>

// TODO remove
#include <iostream>


/*
 * KeyType and DataType both must override the =, <, > operators
 * TODO Maybe create corresponding interfaces?
 */

template<typename KeyType, typename DataType>
class AVLTree {
public:

	typedef AVLTree<KeyType, DataType> ThisType;

    AVLTree(KeyType key, DataType data) : key(key), data(data) {
    	root = this;
    	right = NULL;
    	left = NULL;
    }

    AVLTree(KeyType key, DataType data, ThisType* root) : key(key), data(data), root(root) {
    	right = NULL;
    	left = NULL;
    }

    //~AVLTree();

    DataType* enumerateData() {
    	DataType treeArray[getTreeSize()];
    	int beginIndex = 0;

    	doInOrderEnumeration(this, treeArray, beginIndex);
    	return treeArray;
    };

    /* Because we work with comparators, this function will only work when an ephemeral data type, containing
     * only the search key in the correct field, is passed */

    /*DataType* findDataByKey(DataType& keyData) {

    }*/

    const int getTreeSize() {
    	return calculateTreeSize(0, this);
    }

    void insert(KeyType key, DataType data) {
    	recursiveInsert(key, data, root);
    	// Use recursiveInsert to handle the insert and rotations

    }

    void remove(KeyType key);


private:
    KeyType key;
    DataType data;
    ThisType* left;
    ThisType* right;
    ThisType* root;


    /* Internal helper functions */
    ThisType* getNodeParent() {
    	return findNodeParent(key, root);
    }

    int getTreeHeight() const {
    	return calculateHeight(root);
    }

    int getNodeHeight() const {
    	return calculateHeight(this);
    }

    int getNodeBalanceFactor() const {
    	int rightHeight = (IS_NULL(right)) ? getNodeHeight() : right->getNodeHeight();
    	int leftHeight = (IS_NULL(left)) ? getNodeHeight() : left->getNodeHeight();

    	return leftHeight - rightHeight;
    }


    /* Rotation functions */
    ThisType* rrRotate(ThisType* parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	ThisType* tempNode;
    	tempNode = parent->right;
    	parent->right = tempNode->left;
    	tempNode->left = parent;

    	return tempNode;
    }

    ThisType* llRotate(ThisType* parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	ThisType* tempNode;
    	tempNode = parent->left;
    	parent->left = tempNode->right;
    	tempNode->right = parent;

    	return tempNode;
    }

    ThisType* rlRotate(ThisType* parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	ThisType* tempNode;
    	tempNode = parent->right;
    	parent->right = llRotate(tempNode);

    	return rrRotate(parent);
    }

    ThisType* lrRotate(ThisType* parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	ThisType* tempNode;
    	tempNode = parent->left;
    	parent->left = rrRotate(tempNode);
    	return llRotate(parent);
    }


    ThisType* balanceNode(ThisType* node) {
    	int balanceFactor = node->getNodeBalanceFactor();

    	/* Evaluate the balance factor and determine whether, and which,
    	 * rotation is required to balance the tree */

    	if(balanceFactor > 1) {
    		if(node->left->getNodeBalanceFactor() > 0) {
    			node = node->llRotate(node);
    		} else {
    			node = node->lrRotate(node);
    		}
    	} else if(balanceFactor < -1) {
    		if(node->right->getNodeBalanceFactor() > 0) {
    			node = rlRotate(node);
    		} else {
    			node = rrRotate(node);
    		}
    	}
    }


    /* Internal helper recursive functions */

    void recursiveInsert(KeyType key, DataType data, ThisType* currentNode) {
    	if(IS_NULL(currentNode)) {
    		// Something's gone wrong, throw exception
    		std::cout << "currentNode is null";
    	}

    	if(currentNode->key == key) {
    		// This is not right, it's not supposed to be here! Throw exception
    		std::cout << "Crap";
    	}


    	// Either insert the node or continue the recursion
    	ThisType newNode = ThisType(key, data, root);
    	if(currentNode->key < key) {
    		if(IS_NULL(currentNode->right)) {
    			currentNode->right = newNode;
    			return;
    		} else {
    			recursiveInsert(key, data, currentNode->right);
    		}
    	} else {
    		if(IS_NULL(currentNode->left)) {
    			currentNode->left = newNode;
    			return;
    		} else {
    			recursiveInsert(key, data, currentNode->left);
    		}
    	}
    }

    // lookForParent means that if the value was not found, return a pointer to the node that would have
    // been its parent had it existed. Use for insertion or whatever
    ThisType* searchByKey(KeyType key, ThisType* startTree) {
    	if(IS_NULL(startTree)) {
    			// Throw exception that the value was not found
    	}

    	// Yay!
    	if(key == startTree->key) {
        	return startTree;
    	}

    	// If this node's key is not the key we're looking for, search recursively

    	if(startTree->key > key) {
    		return searchByKey(key, startTree->left);
    	} else {
    		return searchByKey(key, startTree->right);
    	}
    }

    ThisType* findNodeParent(KeyType, ThisType* currentNode) {
    	if(IS_NULL(key)) {
    		// Throw exception that the value is null
    	}
    	if(IS_NULL(currentNode)) {
    		// Throw exception that the value was not found
    	}
    	if(currentNode->key == key) {
    		// The user asked to get the root's parent. Throw exception as it's impossible
    	}

    	// If either one of the node's children is the requesting node, return this node
    	if((!(IS_NULL(currentNode->right)) && currentNode->right->key == key)
    			|| (!(IS_NULL(currentNode->left)) && currentNode->left->key == key)) {
    		return currentNode;
    	}

    	// Otherwise, use binary search to continue
    	if(currentNode->key > key) {
    		return findNodeParent(key, currentNode->left);
    	} else {
    		return findNodeParent(key, currentNode->right);
    	}
    }

    int calculateTreeSize(int currentSize, ThisType* currentNode) {
    	if(currentSize < 0 || IS_NULL(currentNode)) {
    		return 0;
    	}

    	// The size of the tree consists of the size of the right tree, left tree and the current node
    	return 1 + calculateTreeSize(0, currentNode->right) + calculateTreeSize(0, currentNode->left);
    }

    int calculateHeight(ThisType* node) {
    	int height = 0;

    	if(!IS_NULL(node)) {
    		int leftHeight = calculateHeight(node->left);
    		int rightHeight = calculateHeight(node->right);
    		height = MAX(leftHeight, rightHeight) + 1;
    	}

    	return height;
    }

    void doInOrderEnumeration(ThisType* currentNode, DataType* array, int* currentIndex) {
    	if(IS_NULL(currentNode) || IS_NULLPTR(currentNode)) {
    		return;
    	}

    	doInOrderEnumeration(currentNode->left, array, currentIndex);
    	array[currentIndex++] = currentNode->data;
    	doInOrderEnumeration(currentNode->right, array, currentIndex);
    }
};

#endif    /* _234218_WET1_AVLTREE_H_ */
