/****************************************************************************/
/*                                                                          */
/* AppsByIdTree class implementation                                        */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "AppsByIdTree.h"
#include "AVLTree.h"
#include "common.h"


void AppsByIdTree::addApp(const AppData* appData) {
    // This function is basically an alias to AVLTree::insert()

    if (appData == NULL) {
        throw InvalidAppDataException();
    }

    // Use the appId as the search key
    insert(appData->appId, appData);
}

AppData** AppsByIdTree::getAppById(int appId) const {
    // This function is basically an alias to AVLTree::findBySearchKey()

    return findBySearchKey(appId);
}


virtual int AppsByIdTree::predKeyData(int key, const AppData* data) const {
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

virtual int AppsByIdTree::predDataData(const AppData* data,
        const AppData* other) const {

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

