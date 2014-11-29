/****************************************************************************/
/*                                                                          */
/* AppsByDownloadCountTree class implementation                             */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "AppsByDownloadCountTree.h"
#include "AVLTree.h"
#include "common.h"


void AppsByDownloadCountTree::addApp(const AppData* appData) {
    // This function is basically an alias to AVLTree::insert()

    if (appData == NULL) {
        throw InvalidAppDataException();
    }

    // Create a DownloadCountAndId to be used as key
    key = DownloadCountAndId(appData->appId, appData->downloadCount);
    insert(key, appData);
}

void AppsByDownloadCountTree::removeApp(int downloadCount, int appId) {
    // This function is basically an alias to AVLTree::remove()

    // Create a DownloadCountAndId to be used as key
    key = DownloadCountAndId(appId, downloadCount);
    remove(key);
}


virtual int AppsByDownloadCountTree::predKeyData(const DownloadCountAndId& key,
        const AppData* data) const {

    // Compare an appId with an AppData pointer:
    // Simply compare the given appId with the appId in the structure
    // pointed by data.

    if (data == NULL) {
        // Tree integrity error, technically this cannot happen if there
        // are no bugs in the Tree implementation
        throw NullArgumentException();
    }

    if (key.downloadCount < data->downloadCount) {
        return -1;
    } else if (key.downloadCount > data->downloadCount) {
        return 1;
    }

    // Equal downloadCount, sort using appId
    // NOTE: Lower appId is considered larger value here, to match instructions
    // in FAQ and PDF about finding the top app
    if (data->appId < key.appId) {
        return -1;
    } else if (data->appId > key.appId) {
        return 1;
    }

    // Equal downloadCount and appId
    // NOTE: This should not be possible as appId is unique
    return 0;
}

virtual int AppsByDownloadCountTree::predDataData(const AppData* data,
        const AppData* other) const {

    // Compare 2 appdata pointers by download count and appId
    if (data == NULL || other == NULL) {
        // Tree integrity error, technically this cannot happen if there
        // are no bugs in the Tree implementation
        throw NullArgumentException();
    }

    if (data->downloadCount < other->downloadCount) {
        return -1;
    } else if (data->downloadCount > other->downloadCount) {
        return 1;
    }

    // Equal downloadCount, sort using appId
    // NOTE: Lower appId is considered larger value here, to match instructions
    // in FAQ and PDF about finding the top app
    if (other->appId < data->appId) {
        return -1;
    } else if (other->appId > data->appId) {
        return 1;
    }

    // Equal downloadCount and appId
    // NOTE: This should not be possible as appId is unique
    return 0;
}

