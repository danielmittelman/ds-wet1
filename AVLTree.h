/****************************************************************************/
/*                                                                          */
/* AVLTree class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_AVLTREE_H_
#define _234218_WET1_AVLTREE_H_

#define IS_NULL(a) (a == NULL)
#define IS_NULLPTR(a) (*a == NULL)
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define ABS(a) (((a) < 0) ? (-1*(a)) : (a))

#include <cstdlib>


template<typename KeyType, typename ValueType>
class AVLTree {
public:

	typedef AVLTree<KeyType, ValueType> ThisType;

    AVLTree();

    AVLTree(KeyType& key, ValueType& data, ThisType root) : key(key), data(data), root(root) {
    	right = NULL;
    	left = NULL;
    }

    ~AVLTree();

    ValueType* enumerateData() {
    	ValueType treeArray[getTreeSize()];
    	int beginIndex = 0;

    	doInOrderEnumeration(this, treeArray, beginIndex);
    	return treeArray;
    };

    ValueType* findDataByKey(KeyType& key) {
    	AVLTree* keyNode = searchByKey(key, this, false);
    	return keyNode->data;
    }

    int getTreeSize() const {
    	return calculateTreeSize(0, this);
    }

    void insert(KeyType& key, ValueType& data) {
    	// Find the prospected parent node, if the key was actually found an exception will be thrown
    	AVLTree* parent = searchByKey(this, key, true);

    	// Insert the new node where it should be located
    	if(parent->key < key) {
    		parent->right = ThisType(key, data, root);
    	} else {
    		parent->left = ThisType(key, data, root);
    	}

    	// Do gilgulim

    }

    void remove(KeyType& key);


private:
    KeyType key;
    ValueType data;
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

    // lookForParent means that if the value was not found, return a pointer to the node that would have
    // been its parent had it existed. Use for insertion or whatever
    ThisType* searchByKey(KeyType& key, ThisType* startTree, bool lookForParent) {
    	if(IS_NULL(key)) {
    		// Throw exception that the value is null
    	}
    	if(IS_NULL(startTree)) {
    		if(lookForParent) {
    			return startTree;
    		} else {
    			// Throw exception that the value was not found
    		}
    	}

    	// Yay!
    	if(startTree->key == key) {
    		if(lookForParent) {
    			// Oh no! We're trying to insert an item that was found! Throw an exception, you scallywag!
    		} else {
        		return startTree;
    		}
    	}

    	// If this node's key is not the key we're looking for, search recursively
    	if(startTree->key > key) {
    		return searchByKey(key, startTree->left, false);
    	} else {
    		return searchByKey(key, startTree->right, false);
    	}
    }

    ThisType* findNodeParent(KeyType& nodeKey, ThisType* currentNode) {
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

    void doInOrderEnumeration(ThisType* currentNode, ValueType* array, int* currentIndex) {
    	if(IS_NULL(currentNode) || IS_NULLPTR(currentNode)) {
    		return;
    	}

    	doInOrderEnumeration(currentNode->left, array, currentIndex);
    	array[currentIndex++] = currentNode->data;
    	doInOrderEnumeration(currentNode->right, array, currentIndex);
    }
};


#endif    /* _234218_WET1_AVLTREE_H_ */
