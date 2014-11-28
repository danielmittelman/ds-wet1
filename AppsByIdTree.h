/****************************************************************************/
/*                                                                          */
/* AppsByIdTree class header                                                */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_APPS_BY_ID_TREE_H_
#define _234218_WET1_APPS_BY_ID_TREE_H_

#include "AVLTree.h"
#include "common.h"


class AppsByIdTree: public AVLTree< int, AppData* > {
public:
    // Adds an app to the data structue
    // Time complexity: O(log(n)) where n is the number of apps in the tree
    // Throws ElementNotFoundException if an element with the provided key was not found
    // Throws DuplicateNodeException if a node with the provided key already exists
    void addApp(const AppData* appData);

    // Finds a node with the given appId and returns it
    // Time complexity: O(log(n)) where n is the number of apps in the tree
    // Throws ElementNotFoundException if an element with the provided key was not found
    // Throws DuplicateNodeException if a node with the provided key already exists
    AppData** getAppById(int appId) const;

private:
    /**
     * Abstract predicate function for comparing a search key and a data instance.
     * Must be overriden to enable searching, insertion and removal from the tree.
     * @return 1 if key > data, -1 if key < data and 0 if both are considered equal
     */
    virtual int predKeyData(int key, const AppData* data) const {
        // Compare an appId with an AppData pointer:
        // Simply compare the given appId with the appId in the structure
        // pointed by data.

        if (data == NULL) {
            // Tree integrity error, technically this cannot happen if there
            // are no bugs in the Tree implementation
            throw NullArgumentException();
        }

        if (key < data->appId) {
            return -1;
        } else if (key == data->appId) {
            return 0;
        } else {
            return 1;
        }
    }

    /**
     * Abstract predicate function for comparing two data instance.
     * Must be overriden to enable insertion and removal from the tree.
     * @return 1 if data > other, -1 if data < other and 0 if both are equal
     */
    virtual int predDataData(const AppData* data, const AppData* other) const {
        // Compare 2 appdata pointers by appId
        if (data == NULL || other == NULL) {
            // Tree integrity error, technically this cannot happen if there
            // are no bugs in the Tree implementation
            throw NullArgumentException();
        }

        if (data->appId < other->appId) {
            return -1;
        } else if (data->appId == other->appId) {
            return 0;
        } else {
            return 1;
        }
    }
};

#endif    /* _234218_WET1_APPS_BY_ID_TREE_H_ */
