/****************************************************************************/
/*                                                                          */
/* AVLTree class header                                                     */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_AVLTREE_H_
#define _234218_WET1_AVLTREE_H_

#define IS_NULL(a) (a == 0)
#define NULL_CHECK(a) if(IS_NULL(a)) throw NullArgumentException()
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

#include <cstdlib>
#include <exception>


using std::exception;

/* Exception classes */
class NullArgumentException : public exception {};
class ElementNotFoundException : public exception {};
class DuplicateNodeException : public exception {};


template<typename SearchType, typename DataType>
struct AVLNodeStruct {
	DataType data;
	int height;
	struct AVLNodeStruct<SearchType, DataType>* right;
	struct AVLNodeStruct<SearchType, DataType>* left;

	AVLNodeStruct() {
		right = NULL;
		left = NULL;
		height = 0;
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

	virtual ~AVLTree() {
		if (root) {
			recursiveFree(root);
		}
	};

	void recursiveFree(AVLNode node) {
		if (!node) {
			return;
		}
		recursiveFree(node->left);
		recursiveFree(node->right);
		delete node;
	}

	/**
	 * Adds a new data element to the tree. A search key must also be provided to enable throwing an
	 * exception if a node with a duplicate key is identified
	 * @throws DuplicateNodeException if a node with the provided key already exists
	 */
	void insert(const SearchType& key, const DataType& data) {
		root = recursiveInsert(root, key, data);
	}

	/**
	 * Removes a data element from the tree.
	 * @throws ElementNotFoundException if an element with the provided key was not found
	 */
	void remove(const SearchType& key) {
		root = recursiveRemove(root, key);
	}

	/**
	 * Dumps the tree into a sorted array.
	 * @return The size of the array that was used
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
	DataType* findBySearchKey(const SearchType& key) const {
		NULL_CHECK(key);
		return &(binarySearch(root, key)->data);
	}

	/**
	 * Returns the max data element in the tree, being the right-lower-most element.
	 * @throws ElementNotFoundException when the tree is completely empty
	 */
	DataType* getMax() const {
		if(IS_NULL(root)) {
			throw ElementNotFoundException();
		}

		return &(findMax(root)->data);
	}

	/**
	 * Returns the total number of nodes in the tree
	 */
	bool isEmpty() const {
		return IS_NULL(root);
	}

	/**
	 * Returns the total number of nodes in the tree
	 */
	int getTreeSize() const {
		return calculateTreeSize(0, root);
	}

	/**
	 * Returns the tree's height, being the length of the longest route from the root to a leaf
	 */
	int getTreeHeight() const {
		int leftHeight = (IS_NULL(root->left)) ? -1 : root->left->height;
		int rightHeight = (IS_NULL(root->right)) ? -1 : root->right->height;

		return MAX(leftHeight, rightHeight) + 1;
	}


private:
	AVLNode root;

	/**
	 * Abstract predicate function for comparing a search key and a data instance.
	 * Must be overriden to enable searching, insertion and removal from the tree.
	 * @return 1 if key > data, -1 if key < data and 0 if both are considered equal
	 */
	virtual int predKeyData(const SearchType& key, const DataType& data) const = 0;

	/**
	 * Abstract predicate function for comparing two data instance.
	 * Must be overriden to enable insertion and removal from the tree.
	 * @return 1 if data > other, -1 if data < other and 0 if both are equal
	 */
	virtual int predDataData(const DataType& data, const DataType& other) const = 0;


	/* Internal helper functions */

	void adjustNodeHeight(AVLNode node) {
		if(IS_NULL(node)) {
			return;
		}

		int leftHeight = (IS_NULL(node->left)) ? -1 : node->left->height;
		int rightHeight = (IS_NULL(node->right)) ? -1 : node->right->height;
		node->height = MAX(leftHeight, rightHeight) + 1;
	}

	int getNodeBalanceFactor(AVLNode node) {
		int rightHeight = (IS_NULL(node->right)) ? -1 : node->right->height;
		int leftHeight = (IS_NULL(node->left)) ? -1 : node->left->height;

		return leftHeight - rightHeight;
	}

	/* Rotation functions */
	AVLNode rrRotate(AVLNode parent) {
		AVLNode tempNode;
		tempNode = parent->right;
		parent->right = tempNode->left;
		tempNode->left = parent;

		adjustNodeHeight(tempNode->left);
		adjustNodeHeight(tempNode);

		return tempNode;
	}

	AVLNode llRotate(AVLNode parent) {
		AVLNode tempNode;
		tempNode = parent->left;
		parent->left = tempNode->right;
		tempNode->right = parent;

		adjustNodeHeight(tempNode->right);
		adjustNodeHeight(tempNode);

		return tempNode;
	}

	AVLNode rlRotate(AVLNode parent) {
		AVLNode tempNode;
		tempNode = parent->right;
		parent->right = llRotate(tempNode);
		adjustNodeHeight(tempNode);

		return rrRotate(parent);
	}

	AVLNode lrRotate(AVLNode parent) {
		AVLNode tempNode;
		tempNode = parent->left;
		parent->left = rrRotate(tempNode);
		adjustNodeHeight(tempNode);

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

	AVLNode recursiveInsert(AVLNode node, const SearchType& key, const DataType& data) {
		// If node is null, the algorithm has reached the insertion location. Create and return
		if(IS_NULL(node)) {
			AVLNode newNode = new AVLNodeStruct<SearchType, DataType>();
			newNode->data = data;
			return newNode;
		}

		// If there is another node with the same key, halt and throw an exception
		if(predKeyData(key, node->data) == 0) {
			throw DuplicateNodeException();
		}

		// If the current node is considered greater than the data, continue on the left subtree
		if(predDataData(data, node->data) < 0) {
			node->left = recursiveInsert(node->left, key, data);
		} else {
			// If the current node is considered lower than the data, continue on the right subtree
			node->right = recursiveInsert(node->right, key, data);
		}

		// Update this node's height. If either one of its children does not exist,
		// consider its height to be -1
		adjustNodeHeight(node);

		// Finally, balance the current node
		return balanceNode(node);
	}

	AVLNode recursiveRemove(AVLNode node, const SearchType& searchKey) {
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
				// The successor node will have either 0 or 1 (right) children.
				node->right = recursiveRemove(node->right, searchKey);
			}
		}

		// Finally, fix the node's height and balance the node
		adjustNodeHeight(node);
		return balanceNode(node);
	}

	AVLNode binarySearch(const AVLNode node, const SearchType& searchKey) const {
		if(IS_NULL(node)) {
			throw ElementNotFoundException();
		}

		if(predKeyData(searchKey, node->data) == 0) {
			return node;
		}

		if(predKeyData(searchKey, node->data) > 0) {
			return binarySearch(node->right, searchKey);
		} else {
			return binarySearch(node->left, searchKey);
		}
	}

	AVLNode findMax(const AVLNode node) const {
		if(!IS_NULL(node->right)) {
			return findMax(node->right);
		}

		return node;
	}

	int calculateTreeSize(int currentSize, AVLNode node) const {
		if(currentSize < 0 || IS_NULL(node)) {
			return 0;
		}

		// The size of the tree consists of the size of the right tree, left tree and the current node
		return 1 + calculateTreeSize(0, node->right) + calculateTreeSize(0, node->left);
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
