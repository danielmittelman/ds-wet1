/****************************************************************************/
/*                                                                          */
/* AppsByDownloadCountTree class header                                     */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_APPS_BY_DOWNLOAD_COUNT_TREE_H_
#define _234218_WET1_APPS_BY_DOWNLOAD_COUNT_TREE_H_

#include "AVLTree.h"
#include "common.h"


// Helper struct that will be used as the KeyType for the AVLTree
struct DownloadCountAndId {
    int appId;
    int downloadCount;

    // Constructor for this struct, for simpler initialization
    DownloadCountAndId(int appId, int downloadCount) :
            appId(appId),
            downloadCount(downloadCount) {}
};


class AppsByDownloadCountTree: public AVLTree< DownloadCountAndId, AppsListIterator > {
public:
    // Adds an app to the data structue
    // Time complexity: O(log(n)) where n is the number of apps in the tree
    // Throws ElementNotFoundException if an element with the provided key was not found
    // Throws DuplicateNodeException if a node with the provided key already exists
    void addApp(const AppsListIterator& appData);

    // Removes an app from the data structue
    // Time complexity: O(log(n)) where n is the number of apps in the tree
    // Throws ElementNotFoundException if an app with the provided id was not
    // found
    void removeApp(int downloadCount, int appId);

private:
    /**
     * Abstract predicate function for comparing a search key and a data instance.
     * Must be overriden to enable searching, insertion and removal from the tree.
     * @return 1 if key > data, -1 if key < data and 0 if both are considered equal
     */
    virtual int predKeyData(const DownloadCountAndId& key,
            const AppsListIterator& data) const;

    /**
     * Abstract predicate function for comparing two data instance.
     * Must be overriden to enable insertion and removal from the tree.
     * @return 1 if data > other, -1 if data < other and 0 if both are equal
     */
    virtual int predDataData(const AppsListIterator& data,
            const AppsListIterator& other) const;
};

#endif    /* _234218_WET1_APPS_BY_DOWNLOAD_COUNT_TREE_H_ */
