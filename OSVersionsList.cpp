/****************************************************************************/
/*                                                                          */
/* OSVersionsList class implementation                                      */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "OSVersionsList.h"


void OSVersionsList::addVersion(int versionCode) {
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
    OSVersionData data(versionCode);
    insertFront(data);
}

int OSVersionsList::getTopAppId(int versionCode) const {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionData* data = getOSVersionDataByVersionCode(versionCode);

    // Found our node, return the top app if there is one
    if (data->versionTopAppId == INVALID_VERSION_TOP_APP_ID) {
        throw NoSuchAppException();
    }

    return data->versionTopAppId;
}

void OSVersionsList::addApp(const AppsListIterator& appDataPtr) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionData* data = getOSVersionDataByVersionCode(appDataPtr->versionCode);

    // Found our node, add the app to the versionAppsByDownloadCount tree in it
    try {
        data->versionAppsByDownloadCount.addApp(appDataPtr);
    } catch (const DuplicateNodeException& e) {
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

void OSVersionsList::removeApp(int versionCode, int appId) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionData* data = getOSVersionDataByVersionCode(versionCode);

    // Found our node, remove the app from the versionAppsByDownloadCount tree
    // in it
    try {
        // TODO: The remove function currently receives a key, if the way we
        // represent it doesn't change we need to use a key that contains
        // both the downloadCount and the appId (for collisions)
        data->versionAppsByDownloadCount.removeApp(versionCode, appId);
    } catch (const ElementNotFoundException& e) {
        throw NoSuchAppException();
    }

    // App was successfully removed, don't forget to update
    // versionTopAppId and versionTopAppDownloadCount if needed
    AppsListIterator* newTopAppPtr = data->versionAppsByDownloadCount.getMax();
    AppsListIterator newTopApp = *newTopAppPtr;
    if (data->versionTopAppDownloadCount < newTopApp->downloadCount) {
        data->versionTopAppDownloadCount = newTopApp->downloadCount;
        data->versionTopAppId = newTopApp->appId;
    }
}

// Throws NoSuchVersionCodeException if is no such versionCode in the list,
// or there is no versionCode in the list which is larger than the given one
int OSVersionsList::getFollowingVersion(int versionCode) const {
    if (versionCode <= 0) {
        throw InvalidVersionCodeException();
    }

    // Iterate over all nodes until one matches the version code
    FilterByVersionCodePredicate predicate;

    Node* p = mHead;
    while (p) {
        if (predicate(p->data, (void*) versionCode)) {
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
    throw NoSuchVersionCodeException();
}

AppsByDownloadCountTree* OSVersionsList::getAppsByDownloadCountTree(int versionCode) const {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionData* data = getOSVersionDataByVersionCode(versionCode);
    return &(data->versionAppsByDownloadCount);
}


// Private:

OSVersionData* OSVersionsList::getOSVersionDataByVersionCode(int versionCode) const {
    if (versionCode <= 0) {
        throw InvalidVersionCodeException();
    }

    // Search for the node with the relevant versionCode
    OSVersionData* data = NULL;
    try {
        data = getDataByPredicate(FilterByVersionCodePredicate(), (void*) versionCode);
    } catch (const NoSuchNodeException& e) {
        throw NoSuchVersionCodeException();
    }

    return data;
}

bool OSVersionsList::FilterByVersionCodePredicate::operator()
        (const OSVersionData* dataPtr, void* versionCode) const {

    int versionCodeInt = (int) versionCode;
    return (dataPtr->versionCode == versionCodeInt);
}
