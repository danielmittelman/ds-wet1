/****************************************************************************/
/*                                                                          */
/* AppsByDownloadCountTree class implementation                             */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "AppsByDownloadCountTree.h"
#include "AVLTree.h"
#include "common.h"


void AppsByDownloadCountTree::addApp(const AppsListIterator& appData) {
    // This function is basically an alias to AVLTree::insert()

    // Create a DownloadCountAndId to be used as key
    DownloadCountAndId key(appData->appId, appData->downloadCount);
    insert(key, appData);
}

void AppsByDownloadCountTree::removeApp(int downloadCount, int appId) {
    // This function is basically an alias to AVLTree::remove()

    // Create a DownloadCountAndId to be used as key
    DownloadCountAndId key(appId, downloadCount);
    remove(key);
}


int AppsByDownloadCountTree::predKeyData(const DownloadCountAndId& key,
        const AppsListIterator& data) const {

    // Compare an appId with an AppData pointer:
    // Simply compare the given appId with the appId in the structure
    // pointed by data.

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

int AppsByDownloadCountTree::predDataData(const AppsListIterator& data,
        const AppsListIterator& other) const {

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

