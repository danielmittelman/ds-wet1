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

/**
 * Interfaces for the tree comparators
 */

/**
 * Use this interface to implement the search comparator (will be used to search for a node by a single value)
 */
template<typename SearchType, typename DataType>
class AVLTreeSearchComparator {
public:
	/**
	 * @return true if the search type argument is considered equal to the data type
	 */
	virtual bool equals (const SearchType&, const DataType&) const = 0;
};

/**
 * Use this interface to implement the data comparator (will be used to sort the data in the tree)
 */
template<typename DataType>
class AVLTreeDataComparator {
public:
	/**
	 * @return 1 if A>B, 0 if A==b, -1 if A<B
	 */
	virtual int operator() (const DataType&, const DataType&) const = 0;
};

template<typename SearchType, typename DataType>
class AVLTree {
public:

	typedef AVLTree<SearchType, DataType> ThisType;

    AVLTree(DataType data, AVLTreeDataComparator* sortPred, AVLTreeSearchComparator* searchPred) :
    		sortPred(sortPred), searchPred(searchPred), data(data) {
    	root = this;
    	right = NULL;
    	left = NULL;
    }

    AVLTree(DataType data, AVLTreeDataComparator* sortPred, AVLTreeSearchComparator* searchPred) :
    		sortPred(sortPred), searchPred(searchPred), data(data), root(root) {
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

    void insert(DataType data) {
    	// Use recursiveInsert to handle the insert and rotations

    }

    void remove(DataType data);


private:
    DataType data;
    AVLTreeDataComparator* sortPred;
    AVLTreeSearchComparator* searchPred;
    ThisType* left;
    ThisType* right;
    ThisType* root;


    /* Internal helper functions */
    ThisType* getNodeParent() {
    	return findNodeParent(key, root);
    }

    int getNodeHeight() const {
    	return calculateNodeHeightDownwards(0, root);
    }

    int getNodeBalanceFactor() const {
    	int rightHeight = (IS_NULL(right)) ? getNodeHeight() : right->getNodeHeight();
    	int leftHeight = (IS_NULL(left)) ? getNodeHeight() : left->getNodeHeight();

    	return leftHeight - rightHeight;
    }


    /* Internal helper recursive functions */

    void recursiveInsert(KeyType key, DataType data, ThisType* currentNode) {

    }

    // lookForParent means that if the value was not found, return a pointer to the node that would have
    // been its parent had it existed. Use for insertion or whatever
    ThisType* searchByKey(SearchType key, ThisType* startTree) {
    	if(IS_NULL(startTree)) {
    			// Throw exception that the value was not found
    	}

    	// Yay!
    	if(searchPred->equals(key, startTree->data)) {
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

    int calculateNodeHeightDownwards(int height, ThisType* currentNode) {
    	if(IS_NULL(key) || IS_NULL(currentNode)) {
    		// Something's wrong here, throw exception
    	}

    	if(key == currentNode->key) {
    		return 0;
    	}

    	if(currentNode->right->key < key) {
    		return calculateNodeHeightDownwards(height + 1, currentNode->right);
    	} else {
    		return calculateNodeHeightDownwards(height + 1, currentNode->left);
    	}
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
