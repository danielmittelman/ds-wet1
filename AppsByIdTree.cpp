/****************************************************************************/
/*                                                                          */
/* AppsByIdTree class implementation                                        */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "AppsByIdTree.h"
#include "AVLTree.h"
#include "common.h"


void AppsByIdTree::addApp(const AppsListIterator& appData) {
    // This function is basically an alias to AVLTree::insert()

    // Use the appId as the search key
    insert(appData->appId, appData);
}

void AppsByIdTree::removeApp(int appId) {
    // This function is basically an alias to AVLTree::remove()

    remove(appId);
}

AppsListIterator* AppsByIdTree::getAppById(int appId) const {
    // This function is basically an alias to AVLTree::findBySearchKey()

    return findBySearchKey(appId);
}


int AppsByIdTree::predKeyData(const int& key,
        const AppsListIterator& data) const {

    // Compare an appId with an AppData pointer:
    // Simply compare the given appId with the appId in the structure
    // pointed by data.

    if (key < data->appId) {
        return -1;
    } else if (key > data->appId) {
        return 1;
    }

    return 0;
}

int AppsByIdTree::predDataData(const AppsListIterator& data,
        const AppsListIterator& other) const {

    if (data->appId < other->appId) {
        return -1;
    } else if (data->appId > other->appId) {
        return 1;
    }

    return 0;
}

