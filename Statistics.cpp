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
        if ( (mTopAppDownloadCount < downloadCount) ||
             (mTopAppDownloadCount == downloadCount && mTopAppId > appId) ) {
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
    // Find the app's data using mAppsById
    try {
        AppData** appData = mAppsById.getAppById(appId);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }


    // Remove the app from the mAppsById tree
    try {
        mAppsById.remove(appId);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }

    // Remove the app from the mAppsByDownloadCount tree
    try {
        mAppsByDownloadCount.remove(/* TODO */);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }

    // Remove the app from the mOSVersions data structure
    // TODO

    // Finally, remove the AppData structure from mAppsList
    // TODO
}

StatusType Statistics::IncreaseDownloads(int appId, int downloadIncrease) {
    // TODO: Add try..catch and return statements in this function

    // Find the app's data using mAppsById
    AppData** appData = mAppsById.getAppById(appId);

    // Extract the app's current download count and calculate the new one
    int oldDownloadCount = (*appData)->downloadCount;
    int newDownloadCount = oldDownloadCount += downloadIncrease;

    // Update the mOSVersions data structure
    mOSVersions.remove((*appData)->versionCode, appId);
    mOSVersions.insert(*appData);

    // Update the mAppsByDownloadCount tree
    mAppsByDownloadCount.remove(oldDownloadCount, appId);
    mAppsByDownloadCount.insert(*appData);

    // Update the app's AppData structure
    appData->downloadCount = newDownloadCount;
}

StatusType Statistics::UpgradeApplication(int appId) {
    // TODO: Add try..catch and return statements in this function

    // Find the app's data using mAppsById
    AppData** appData = mAppsById.getAppById(appId);

    // Extract the app's current versionCode
    int oldVersionCode = (*appData)->downloadCount;

    // Get the following versionCode in the mOSVersions
    int newVersionCode = mOSVersions.getFollowingVersion(oldVersionCode);

    // Remove old appData pointer from the old place in tthe mOSVersions tree
    mOSVersions.remove((*appData)->oldVersionCode, appId);
    // Update the AppData structure with the new version
    (*appData)->versionCode = newVersionCode;
    // Re-insert the appData pointer to the new place in the mOSVersions tree
    mOSVersions.insert(*appData);
}

StatusType Statistics::GetTopApp(int versionCode, int *appId) {
    if (appId == NULL) {
        return INVALID_INPUT;
    }

    *appId = -1;    // Default value for appId in case of an error

    if (versionCode < 0) {
        // User wants the global top app in the whole system
        if (mTopAppId == INVALID_TOP_APP_ID) {
            // There are no apps in the system
            return FAILURE;
        }

        *appId = mTopAppId;
        return SUCCESS;
    }

    // User wants the top app of the specified versionCode
    try {
        *appId = mOSVersions.getTopAppId(versionCode);
    } catch (InvalidVersionCodeException) {
        return INVALID_INPUT;
    } catch (NoSuchVersionCodeException) {
        // There is no OSVersion with the specified versionCode
        return FAILURE;
    } catch (NoSuchAppException) {
        // There are no apps with the specified versionCode
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps) {
    if (apps == NULL || numOfApps == NULL) {
        return INVALID_INPUT;
    }

    // Default values in case of error
    *numOfApps = 0;
    *app = NULL;

    AppsByDownloadCountTree* tree;

    if (versionCode < 0) {
        // User wants all apps in the system by downloads - Traverse
        // mAppsByDownloadCount
        tree = &mAppsByDownloadCount;

    } else {
        // User wants all apps with a specific versionCode by downloads - Traverse
        // the tree inside the relevant node in mOSVersions
        try {
            tree = mOSVersions.getAppsByDownloadCountTree(versionCode);
        } catch (const InvalidVersionCodeException& e) {
            return INVALID_INPUT;
        } catch (const NoSuchVersionCodeException& e) {
            // Return defaults for apps and numOfApps
            return SUCCESS;
        }
    }

    // TODO: Export the rest of the code of this function into a function
    // of AppsByDownloadCountTree

    // Now that we got our tree:

    // Get the tree's size and check if empty
    int treeSize = tree->getTreeSize();
    if (treeSize == 0) {
        // There are no apps with the specified versionCode
        // Return defaults for apps and numOfApps
        return SUCCESS;
    }

    // Enumerate the tree's data into an array
    // Allocate array using malloc() because the user uses free() later
    AppData** appDataArray = (AppData**) malloc(sizeof(AppData**) * treeSize);
    if (appDataArray == NULL) {
        return ALLOCATION_ERROR;
    }
    tree.enumerateData(*appDataArray);


    // We need to extract the data we want from the AppData pointers array
    // to an array of AppIds
    // Allocate array using malloc() because the user uses free() later
    int* appIdsArray = (int*) malloc(sizeof(int) * treeSize);
    if (appIdsArray == NULL) {
        // Cleanup previously allocated array
        free(appDataArray);
        return ALLOCATION_ERROR;
    }

    for (int i=0; i<treeSize; i++) {
        // NOTE: We trust enumerateData() here in that all the elements in
        // the array it returned are valid pointers
        appIdsArray[i] = appDataArray[i]->appId;
    }

    free(appDataArray);

    // Got everything we need, return it to the user
    *numOfApps = treeSize;
    *apps = appIdsArray;

    return SUCCESS;
}

StatusType Statistics::UpdateDownloads(int groupBase, int multiplyFactor) {
    // TODO
}
