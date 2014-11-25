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
struct AVLNodeStruct {
	KeyType key;
	DataType data;
	struct AVLNodeStruct<KeyType, DataType>* right;
	struct AVLNodeStruct<KeyType, DataType>* left;
	struct AVLNodeStruct<KeyType, DataType>* root;

	AVLNodeStruct() {
		right = NULL;
		left = NULL;
		root = NULL;
	}

	/*AVLNodeStruct(KeyType key, DataType data, AVLNodeStruct<KeyType, DataType>* root) :
			key(key), data(data), root(root) {
		right = NULL;
		left = NULL;
	}*/
};


template<typename KeyType, typename DataType>
class AVLTree {
public:

	typedef AVLTree<KeyType, DataType> ThisType;
	typedef AVLNodeStruct<KeyType, DataType> *AVLNode;

    AVLTree() {
    	root = NULL;
    }

    AVLTree(AVLNode root) {
    	this->root = root;
    }

    //~AVLTree();

    int enumerateData(DataType array[]) {
    	int beginIndex = 0;

    	doInOrderEnumeration(root, array, &beginIndex);
    	return beginIndex;
    };

    /* Because we work with comparators, this function will only work when an ephemeral data type, containing
     * only the search key in the correct field, is passed */

    /*DataType* findDataByKey(DataType& keyData) {

    }*/

    const int getTreeSize() {
    	return calculateTreeSize(0, root);
    }

    int getTreeHeight() const {
    	return calculateHeight(root);
    }

    void insert(KeyType key, DataType data) {
    	root = recursiveInsert(root, key, data);
    	// Use recursiveInsert to handle the insert and rotations
    }

    void remove(KeyType key);


private:
    AVLNode root;


    /* Internal helper functions */
    /*ThisType* getNodeParent() {
    	return findNodeParent(key, root);
    }*/

    int getNodeBalanceFactor(AVLNode node) {
    	int rightHeight = calculateHeight(node->right);
    	int leftHeight = calculateHeight(node->left);

    	return leftHeight - rightHeight;
    }

    /* Rotation functions */
    AVLNode rrRotate(AVLNode parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = tempNode->left;
    	tempNode->left = parent;

    	return tempNode;
    }

    AVLNode llRotate(AVLNode parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = tempNode->right;
    	tempNode->right = parent;

    	return tempNode;
    }

    AVLNode rlRotate(AVLNode parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = llRotate(tempNode);

    	return rrRotate(parent);
    }

    AVLNode lrRotate(AVLNode parent) {
    	if(IS_NULL(parent)) {
    		// Throw exception MoFo
    	}

    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = rrRotate(tempNode);

    	return llRotate(parent);
    }


    AVLNode balanceNode(AVLNode node) {
    	int balanceFactor = getNodeBalanceFactor(node);

    	/* Evaluate the balance factor and determine whether, and which,
    	 * rotation is required to balance the tree */

    	AVLNode balancedNode = node;
    	if(balanceFactor > 1) {
    		if(getNodeBalanceFactor(node->left) > 0) {
    			balancedNode = llRotate(node);
    		} else {
    			balancedNode = lrRotate(node);
    		}
    	} else if(balanceFactor < -1) {
    		if(getNodeBalanceFactor(node->right) > 0) {
    			balancedNode = rlRotate(node);
    		} else {
    			balancedNode = rrRotate(node);
    		}
    	}

    	return balancedNode;
    }


    /* Internal helper recursive functions */

    AVLNode recursiveInsert(AVLNode node, KeyType key, DataType data) {
    	if(IS_NULL(node)) {
    		AVLNode newNode = new AVLNodeStruct<KeyType, DataType>();
    		newNode->data = data;
    		newNode->key = key;
    		newNode->root = root;
    		return newNode;
    	} else if(node->key > key) {
    		node->left = recursiveInsert(node->left, key, data);
    		return balanceNode(node);
    	} else {
    		node->right = recursiveInsert(node->right, key, data);
    		return balanceNode(node);
    	}
    }

    // lookForParent means that if the value was not found, return a pointer to the node that would have
    // been its parent had it existed. Use for insertion or whatever
    /*ThisType* searchByKey(KeyType key, ThisType* startTree) {
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
    }*/

    /*ThisType* findNodeParent(KeyType, ThisType* currentNode) {
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
    }*/

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
};

#endif    /* _234218_WET1_AVLTREE_H_ */
