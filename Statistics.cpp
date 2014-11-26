/****************************************************************************/
/*                                                                          */
/* Statistics class implementation                                          */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include <new>                              // bad_alloc definition
#include <exception>
#include "Statistics.h"


using std::bad_alloc;
using std::exception;


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

StatusType Statistics::AddApplication(int appId, int versionCode, int downloadCount) {
    try {
        // 1. Create a new AppData record with the app's params, and insert it
        // into the apps list
        AppData appData(appId, versionCode, downloadCount);
        AppData* appDataPtr = NULL;
        appDataPtr = mAppsList.insertFront(appData);

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

StatusType Statistics::RemoveApplication(int appId) {
    // TODO
}

StatusType Statistics::IncreaseDownloads(int appId, int downloadIncrease) {
    // Find the app's data using mAppsById
    AppData* appData = mAppsById.getAppById(appId);

    // Extract the app's current download count and calculate the new one
    int oldDownloadCount = appData->downloadCount;
    int newDownloadCount = oldDownloadCount += downloadIncrease;

    // Update the mOSVersions data structure
    mOSVersions.remove(appData->versionCode, appId);
    mOSVersions.insert(appData);

    // Update the mAppsByDownloadCount tree
    mAppsByDownloadCount.remove(oldDownloadCount, appId);
    mAppsByDownloadCount.insert(appData);

    // Update the app's AppData structure
    appData->downloadCount = newDownloadCount;
}

StatusType Statistics::UpgradeApplication(int appId) {
    // Find the app's data using mAppsById
    AppData* appData = mAppsById.getAppById(appId);

    // Extract the app's current versionCode
    int oldVersionCode = appData->downloadCount;

    // Get the following versionCode in the mOSVersions
    int newVersionCode = mOSVersions.getFollowingVersion(oldVersionCode);

    // Remove old appData pointer from the old place in the mOSVersions tree
    mOSVersions.remove(appData->oldVersionCode, appId);
    // Update the AppData structure with the new version
    appData->versionCode = newVersionCode;
    // Re-insert the appData pointer to the new place in the mOSVersions tree
    mOSVersions.insert(appData);
}

StatusType Statistics::GetTopApp(int versionCode, int *appId) {
    // TODO
}

StatusType Statistics::GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps) {
    // TODO
}

StatusType Statistics::UpdateDownloads(int groupBase, int multiplyFactor) {
    // TODO
}
