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

    virtual ~AVLTree();

	/* Returns true if the data and the search key are considered equal, false otherwise */
	virtual bool predKeyData(SearchType& key, DataType& data) const = 0;

	/* Returns 1 if data > other, -1 if data < other and 0 if both are equal */
	virtual int predDataData(DataType& data, DataType& other) const = 0;

    int enumerateData(DataType array[]) {
    	NULL_CHECK(array);
    	int beginIndex = 0;

    	doInOrderEnumeration(root, array, &beginIndex);
    	return beginIndex;
    };

    const int getTreeSize() {
    	return calculateTreeSize(0, root);
    }

    DataType* getMax() const {};

    int getTreeHeight() const {
    	return calculateHeight(root);
    }

    DataType& findBySearchKey(SearchType& key) {
    	NULL_CHECK(key);

    	return binarySearch(root, key)->data;
    }

    void insert(DataType& data) {
    	//NULL_CHECK(key);
    	//NULL_CHECK(data);

    	root = recursiveInsert(root, data);
    }

    void remove(SearchType& key) {
    	//NULL_CHECK(key);
    }


private:
    AVLNode root;


    /* Internal helper functions */

    int getNodeBalanceFactor(AVLNode node) {
    	//NULL_CHECK(node);

    	int rightHeight = calculateHeight(node->right);
    	int leftHeight = calculateHeight(node->left);

    	return leftHeight - rightHeight;
    }

    /* Rotation functions */
    AVLNode rrRotate(AVLNode parent) {
    	//NULL_CHECK(parent);

    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = tempNode->left;
    	tempNode->left = parent;

    	return tempNode;
    }

    AVLNode llRotate(AVLNode parent) {
    	//NULL_CHECK(parent);

    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = tempNode->right;
    	tempNode->right = parent;

    	return tempNode;
    }

    AVLNode rlRotate(AVLNode parent) {
    	//NULL_CHECK(parent);

    	AVLNode tempNode;
    	tempNode = parent->right;
    	parent->right = llRotate(tempNode);

    	return rrRotate(parent);
    }

    AVLNode lrRotate(AVLNode parent) {
    	//NULL_CHECK(parent);

    	AVLNode tempNode;
    	tempNode = parent->left;
    	parent->left = rrRotate(tempNode);

    	return llRotate(parent);
    }


    AVLNode balanceNode(AVLNode node) {
    	//NULL_CHECK(node);
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

    AVLNode recursiveInsert(AVLNode node, DataType& data) {
    	//NULL_CHECK(key);
    	//NULL_CHECK(data);

    	if(IS_NULL(node)) {
    		AVLNode newNode = new AVLNodeStruct<SearchType, DataType>();
    		newNode->data = data;
    		newNode->root = root;
    		return newNode;
    	} else if(predDataData(data, node->data)) {
    		node->left = recursiveInsert(node->left, data);
    		return balanceNode(node);
    	} else {
    		node->right = recursiveInsert(node->right, data);
    		return balanceNode(node);
    	}
    }

    AVLNode binarySearch(AVLNode node, SearchType& searchKey) {
    	//NULL_CHECK(node);
    	//NULL_CHECK(key);

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
