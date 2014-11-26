#include "AVLTree.h"

#include <iostream>
#include <string>

using namespace std;


#define ASSERT(expr) do { \
if (!(expr)) {\
cout << "\nAssertion failed " << #expr << "(" << __LINE__ << ").\n"; \
return false; \
}\
else {\
		cout << "."; \
} \
}while(0)

#define RUN_TEST(test) do { \
	cout << "Running " << #test; \
if (test()) {\
cout << "[OK]\n"; \
} \
} while (0)

#define ASSERT_NO_CRASH(expr) expr; ASSERT(true)
#define ASSERT_EQUALS(expr,expected) ASSERT((expr) == (expected))
#define ASSERT_NOT_EQUALS(expr,unexpected) ASSERT((expr) != (unexpected))
#define ASSERT_STRING_EQUALS(s1,s2) ASSERT(strcmp(s1, s2) == 0)

#define ASSERT_SUCCESS(expr) ASSERT_EQUALS(expr, PRIORITY_QUEUE_SUCCESS)
#define ASSERT_TRUE(expr) ASSERT_EQUALS(expr, true)
#define ASSERT_FALSE(expr) ASSERT_EQUALS(expr, false)
#define ASSERT_NULL(expr) ASSERT_EQUALS(expr, NULL)
#define ASSERT_NOT_NULL(expr) ASSERT_NOT_EQUALS(expr, NULL)


/* The data type to be stored in the tree */
struct CompoundData {
public:
	int id;
	char letter;

	CompoundData() { id = 0 ; letter = 0; };

	CompoundData(int id, char letter) : id(id), letter(letter) {};
};

/* AVLTree implementation - A new class extending AVLTree with predetermined types that
 * overrides the predicate methods. This class can then be used as a fully functional AVL tree */
class TestTree : public AVLTree<int, CompoundData> {
public:
	virtual ~TestTree() {};

private:
	// Key-data comparison function. In this example, the key is considered equal to the data
	// if the key field inside data is equal to key
	virtual int predKeyData(int& key, CompoundData& data) const {
		if(key > data.id) return 1;
		if(key < data.id) return -1;

		return 0;
	}

	// Data-data comparison function. In this example, the nodes are to be sorted
	// by id, then by letter
	virtual int predDataData(CompoundData& data, CompoundData& other) const {
		if(data.id > other.id) return 1;
		if(data.id < other.id) return -1;

		if(data.id == other.id) {
			if(data.letter > other.letter) return 1;
			if(data.letter < other.letter) return -1;
		}

		return 0;
	}
};




static bool testTreeCreation() {
	// Create a new TestTree
	TestTree tree = TestTree();
	return true;
}

static bool testTreeInsertAndEnumeration() {
	int vals[] = {9,2,5,1,7,6,3};
	char chars[] = {'e','a','z','f','n','o','n'};

	TestTree tree = TestTree();
	for(int i = 0 ; i < 7 ; i++) {
		ASSERT_EQUALS(tree.getTreeSize(), i);
		CompoundData temp = CompoundData(vals[i], chars[i]);
		tree.insert(temp);
		ASSERT_EQUALS(tree.getTreeSize(), i+1);
	}

	int arrSize = tree.getTreeSize();
	CompoundData array[arrSize];
	ASSERT_EQUALS(arrSize, tree.enumerateData(array));

	for(int i = 0 ; i < arrSize ; i++) {
		cout << array[i].letter;
	}

	return true;
}

static bool testTreeSearch() {
	int vals[] = {9,2,5,1,7,6,3};
	char chars[] = {'e','a','z','f','n','o','n'};

	TestTree tree = TestTree();
	for(int i = 0 ; i < 7 ; i++) {
		CompoundData temp = CompoundData(vals[i], chars[i]);
		tree.insert(temp);
	}

	CompoundData* ptr;
	for(int i = 0 ; i < 7 ; i++) {
		ptr = tree.findBySearchKey(vals[i]);
		ASSERT_EQUALS(chars[i], ptr->letter);
	}

	try {
		int badId = 123;
		ptr = tree.findBySearchKey(badId);
		ASSERT(false);
	} catch(ElementNotFoundException& e) {
		ASSERT(true);
	}

	return true;
}

static bool testTreeGetMax() {
	int vals[] = {9,2,5,1,7,6,3};
	char chars[] = {'e','a','z','f','n','o','n'};

	TestTree tree = TestTree();
	for(int i = 0 ; i < 7 ; i++) {
		CompoundData temp = CompoundData(vals[i], chars[i]);
		tree.insert(temp);
	}

	CompoundData* c = tree.getMax();
	ASSERT_EQUALS(c->id, 9);
	ASSERT_EQUALS(c->letter, 'e');
	return true;
}


int main() {
		RUN_TEST(testTreeCreation);
		RUN_TEST(testTreeInsertAndEnumeration);
		RUN_TEST(testTreeSearch);
		RUN_TEST(testTreeGetMax);
		return 0;
}
