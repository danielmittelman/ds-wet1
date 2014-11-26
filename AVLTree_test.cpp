#include "AVLTree.h"

#include <iostream>

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


static bool testTreeCreation() {
	AVLTree<int, int> tree = AVLTree<int, int>();
	int dump[10];


	cout << "\nTree size: " << tree.getTreeSize() << endl;
	int arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}
	tree.insert(9,9);

	cout << "\nTree size: " << tree.getTreeSize() << endl;
	arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}
	tree.insert(2,2);

	cout << "\nTree size: " << tree.getTreeSize() << endl;
	arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}
	tree.insert(1,1);

	cout << "\nTree size: " << tree.getTreeSize() << endl;
	arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}
	tree.insert(0,0);

	cout << "\nTree size: " << tree.getTreeSize() << endl;
	arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}
	tree.insert(5,5);

	cout << "\nTree size: " << tree.getTreeSize() << endl;
	arrSize = tree.enumerateData(dump);
	for(int i = 0 ; i < arrSize ; i++) {
		cout << dump[i] << ", ";
	}

	return true;

}


int main() {
		RUN_TEST(testTreeCreation);
		return 0;
}
