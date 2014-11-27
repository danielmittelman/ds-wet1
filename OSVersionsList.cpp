/****************************************************************************/
/*                                                                          */
/* OSVersionsList class implementation                                      */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "OSVersionsList.h"


void OSVersionList::addVersion(int versionCode) {
    if (versionCode <= 0) {
        throw InvalidVersionCodeException();
    }

    // First make sure that the versionCode is indeed larger than the largest
    // versionCode we currently have
    if ( (!isEmpty()) && (getFront()->versionCode >= versionCode) ) {
        throw VersionCodeNotLargerThanCurrentException();
    }

    // The new versionCode is good, allocate a new struct with it and
    // insert it into the list
    OSVersionsData data(versionCode);
    insertFront(data);
}

int OSVersionList::getTopAppId(int versionCode) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getOSVersionDataByVersionCode(versionCode);

    // Found our node, return the top app if there is one
    if (data->versionTopAppId == INVALID_VERSION_TOP_APP) {
        throw NoSuchAppException();
    }

    return data->versionTopAppId;
}

void OSVersionList::addApp(const AppData* appDataPtr) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getOSVersionDataByVersionCode(versionCode);

    // Found our node, add the app to the versionAppsByDownloadCount tree in it
    try {
        data->versionAppsByDownloadCount.insert(appDataPtr);
    } catch (const AppsByDownloadCountTree.KeyAlreadyExistsException& e) {
        throw AppAlreadyExistsException();
    }

    // App was successfully added, don't forget to update versionTopAppId and
    // versionTopAppDownloadCount if needed
    // NOTE: We could have recalculated the app maximum as in removeApp, but
    // this is faster and simpler
    if ( (data->versionTopAppDownloadCount < appDataPtr->downloadCount) ||
         (  data->versionTopAppDownloadCount == appDataPtr->downloadCount &&
            data->versionTopAppId > appDataPtr->appId  ) ) {
        data->versionTopAppDownloadCount = appDataPtr->downloadCount;
        data->versionTopAppId = appDataPtr->appId;
    }
}

void OSVersionList::removeApp(int versionCode, int appId) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getOSVersionDataByVersionCode(versionCode);

    // Found our node, remove the app from the versionAppsByDownloadCount tree
    // in it
    try {
        // TODO: The remove function currently receives a key, if the way we
        // represent it doesn't change we need to use a key that contains
        // both the downloadCount and the appId (for collisions)
        data->versionAppsByDownloadCount.remove(appId);
    } catch (const AppsByDownloadCountTree.NoSuchKeyException& e) {
        throw NoSuchAppException();
    }

    // App was successfully removed, don't forget to update
    // versionTopAppId and versionTopAppDownloadCount if needed
    OSVersionsData* newTopApp = data->getMax();
    if (data->versionTopAppDownloadCount < newTopApp->downloadCount) {
        data->versionTopAppDownloadCount = newTopApp->downloadCount;
        data->versionTopAppId = newTopApp->appId;
    }
}

// Throws NoSuchVersionCodeException if is no such versionCode in the list,
// or there is no versionCode in the list which is larger than the given one
int OSVersionList::getFollowingVersion(int versionCode) const {
    if (versionCode <= 0) {
        throw InvalidVersionCodeException();
    }

    // Iterate over all nodes until one matches the version code
    FilterByVersionCodePredicate predicate;

    Node* p = mHead;
    while (p) {
        if (predicate(p->data, versionCode)) {
            // Found it - Return the previous node's versionCode
            if (p->prev) {
                return p->prev->data->versionCode;
            }

            // The versionCode given is the largest one
            throw NoSuchVersionCodeException();
        }

        p = p->next;
    }

    // Given versionCode is not in the list
    throw NoSuchVersionCodeException()
}

AppsByDownloadCountTree* getAppsByDownloadCountTree(int versionCode) const {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getOSVersionDataByVersionCode(versionCode);
    return &(data->versionAppsByDownloadCount);
}


OSVersionData* OSVersionList::getOSVersionDataByVersionCode(int versionCode) {
    if (versionCode <= 0) {
        throw InvalidVersionCodeException();
    }

    // Search for the node with the relevant versionCode
    OSVersionsData* data = NULL;
    try {
        data = getDataByPredicate(FilterByVersionCodePredicate(), versionCode);
    } catch (const NoSuchNodeException& e) {
        throw NoSuchVersionCodeException();
    }

    return data;
}

bool OSVersionList::FilterByVersionCodePredicate::operator()
        (const OSVersionsData& data, void* versionCode) const {

    int versionCodeInt = (int) versionCode;
    return (data->versionCode == versionCodeInt);
}
