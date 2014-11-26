/****************************************************************************/
/*                                                                          */
/* Statistics class implementation                                          */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include <new>                              // bad_alloc definition
#include "Statistics.h"


using std::bad_alloc;


Statistics::Statistics() :
    mAppsList(), mOSVersions(), mAppsById(), mAppsByDownloadCount(),
    mTopAppId(INVALID_TOP_APP_ID), mTopAppDownloadCount(-1) {}


Statistics::~Statistics() {
    // Nothing important to destruct (all private members are automatically
    // destructed anyway)
}

StatusType Statistics::AddVersion(int versionCode) {
    try {
        mOSVersions.addVersion(versionCode);
    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (const InvalidVersionCodeException& e) {
        return INVALID_INPUT;
    } catch (const VersionCodeNotLargerThanCurrentException& e) {
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::AddApplication(int appID, int versionCode, int downloadCount) {
    // 1. Create a new AppData record with the app's params, and insert it
    // into the apps list
    AppData appData(appId, versionCode, downloadCount);
    AppData* appDataPtr = NULL;

    try {
        appDataPtr = mAppsList.insertFront(appData);
    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    }

    try {
        // 2. Insert a pointer to the new app to mOSVersions
        try {
            mOSVersions.addApp(appDataPtr);
        } catch (const exception& e) {
            mAppsList.remove(appData); // TODO
            throw;
        }

        // TODO: More try and catch for each of these, to remove previously
        // added data / pointers

        // 3. Insert new app to mAppsById and mAppsByDownloadCount
        mAppsById.addApp(appDataPtr);
        mAppsByDownloadCount.addApp(appDataPtr);

        // 4. Update mTopAppId and mTopAppDownloadCount if needed
        if (mTopAppDownloadCount < downloadCount) {
            mTopAppDownloadCount = downloadCount;
            mTopAppId = appId;
        }

    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (const InvalidVersionCodeException& e) {
        return INVALID_INPUT;
    } catch (const NoSuchVersionCodeException& e) {
        return FAILURE;
    } catch (const AppAlreadyExistsException& e) {
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::RemoveApplication(int appID) {
    // TODO
}

StatusType Statistics::IncreaseDownloads(int appID, int downloadIncrease) {
    // TODO
}

StatusType Statistics::UpgradeApplication(int appID) {
    // TODO
}

StatusType Statistics::GetTopApp(int versionCode, int *appID) {
    // TODO
}

StatusType Statistics::GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps) {
    // TODO
}

StatusType Statistics::UpdateDownloads(int groupBase, int multiplyFactor) {
    // TODO
}
