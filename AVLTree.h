/****************************************************************************/
/*                                                                          */
/* AVLTree class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_AVLTREE_H_
#define _234218_WET1_AVLTREE_H_

#include "library1.h"

template<typename KeyType, typename ValueType>
class AVLTree {
public:
    AVLTree();

    ~AVLTree();

private:
    struct AVLNode {
        KeyType key;
        ValueType* data;
        AVLNode* left;
        AVLNode* right;
    };

    AVLNode* root;
};


// struct <typename T> AVLTree::AVLNode {
//     data;
//     AVLNode* left;
//     AVLNode* right;
// };

#endif    /* _234218_WET1_AVLTREE_H_ */
