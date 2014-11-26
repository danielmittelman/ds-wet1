/****************************************************************************/
/*                                                                          */
/* OSVersionsList class implementation                                      */
/*                                                                          */
/****************************************************************************/

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

    // OK the new versionCode is good, allocate a new struct with it and
    // insert it into the list
    OSVersionsData data;
    data.versionCode = versionCode;
    data.versionTopAppId = -1;
    // data.versionAppsByDownloadCount is left empty for now

    insertFront(data);
}

int OSVersionList::getTopAppId(int versionCode) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getAppDataByVersionCode(versionCode);

    // Found our node, return the top app if there is one
    if (data->versionTopAppId == INVALID_VERSION_TOP_APP) {
        throw NoSuchAppException();
    }

    return data->versionTopAppId;
}

void OSVersionList::addApp(int appId, int versionCode, int downloadCount) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getAppDataByVersionCode(versionCode);

    // Found our node, add the app to the versionAppsByDownloadCount tree in it
    AppData appData(appId, versionCode, downloadCount);
    try {
        // TODO: The insert function will probably change. In any case, we
        // cannot just send downloadCount as the key because it needs to also
        // consider the appId in case of collision
        data->versionAppsByDownloadCount.insert(downloadCount, appData);
    } catch (const AppsByDownloadCountTree.KeyAlreadyExistsException& e) {
        throw AppAlreadyExistsException();
    }

    // App was successfully added, don't forget to update versionTopAppId and
    // versionTopAppDownloadCount if needed
    if (data->versionTopAppDownloadCount < downloadCount) {
        data->versionTopAppDownloadCount = downloadCount;
        data->versionTopAppId = appId;
    }
}

void OSVersionList::removeApp(int appId, int versionCode) {
    // Search for the OSVersionData node with the relevant versionCode
    // (an exception will be thrown if it is not found)
    OSVersionsData* data = getAppDataByVersionCode(versionCode);

    // Found our node, remove the app from the versionAppsByDownloadCount tree
    // in it
    try {
        // TODO: The remove function currently receives a key, if the way we
        // represent it doesn't change we need to use a key that contains
        // both the downloadCount and the appId (for collisions)
        data->versionAppsByDownloadCount.remove(downloadCount);
    } catch (const AppsByDownloadCountTree.NoSuchKeyException& e) {
        throw NoSuchAppException();
    }

    // App was successfully removed, don't forget to update
    // versionTopAppId and versionTopAppDownloadCount if needed
    updateTopApp(data);
}

OSVersionData* OSVersionList::getAppDataByVersionCode(int versionCode) {
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
