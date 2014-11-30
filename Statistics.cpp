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
    mAppsList(), mOSVersionsList(), mAppsById(), mAppsByDownloadCount(),
    mTopAppId(INVALID_TOP_APP_ID), mTopAppDownloadCount(-1) {}


Statistics::~Statistics() {
    // Nothing important to destruct (all private members are automatically
    // destructed anyway)
}

StatusType Statistics::AddVersion(int versionCode) {
    if (versionCode <= 0) {
        return INVALID_INPUT;
    }

    try {
        mOSVersionsList.addVersion(versionCode);
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
    if (appId <= 0 || versionCode <= 0) {
        return INVALID_INPUT;
    }

    try {
        // 1. Create a new AppData record with the app's params, and insert it
        // into the apps list
        AppData appData(appId, versionCode, downloadCount);
        AppsListIterator appDataIter = mAppsList.insertFront(appData);

        // 2. Insert a pointer to the new app to mOSVersionsList
        try {
            mOSVersionsList.addApp(appDataIter);
        } catch (const exception& e) {
            mAppsList.remove(appDataIter);
            throw;
        }

        // 3. Insert new app to mAppsById and mAppsByDownloadCount
        try {
            mAppsById.addApp(appDataIter);
        } catch (const exception& e) {
            mOSVersionsList.removeApp(versionCode, appId);
            mAppsList.remove(appDataIter);
            throw;
        }

        try {
            mAppsByDownloadCount.addApp(appDataIter);
        } catch (const exception& e) {
            mAppsById.removeApp(appId);
            mOSVersionsList.removeApp(versionCode, appId);
            mAppsList.remove(appDataIter);
            throw;
        }

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
    if (appId <= 0) {
        return INVALID_INPUT;
    }

    // Find the app's data using mAppsById
    AppsListIterator* appDataIterPtr = NULL;
    try {
        appDataIterPtr = mAppsById.getAppById(appId);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }
    AppsListIterator appDataIter = *appDataIterPtr;

    try {

        // Remove the app from the mAppsById tree
        mAppsById.removeApp(appId);

        // Remove the app from the mAppsByDownloadCount tree
        mAppsByDownloadCount.removeApp(appDataIter->downloadCount, appDataIter->appId);

        // Remove the app from the mOSVersionsList data structure
        mOSVersionsList.removeApp(appDataIter->versionCode, appDataIter->appId);

        // Finally, remove the AppData structure from mAppsList
        mAppsList.remove(appDataIter);

    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }

    // Update mTopAppId and mTopAppDownloadCount if needed
    AppsListIterator* maxAppDataIterPtr = mAppsByDownloadCount.getMax();
    AppsListIterator maxAppDataIter = *maxAppDataIterPtr;
    if (mTopAppDownloadCount < maxAppDataIter->downloadCount) {
        mTopAppDownloadCount = maxAppDataIter->downloadCount;
        mTopAppId = maxAppDataIter->appId;
    }

    return SUCCESS;
}

StatusType Statistics::IncreaseDownloads(int appId, int downloadIncrease) {
    if (appId <= 0 || downloadIncrease <= 0) {
        return INVALID_INPUT;
    }

    // Find the app's data using mAppsById
    AppsListIterator* appDataIterPtr = NULL;
    try {
        appDataIterPtr = mAppsById.getAppById(appId);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }
    AppsListIterator appDataIter = *appDataIterPtr;

    try {

        // Extract the app's current download count and calculate the new one
        int oldDownloadCount = appDataIter->downloadCount;
        int newDownloadCount = oldDownloadCount += downloadIncrease;

        // Update the mOSVersionsList data structure
        mOSVersionsList.removeApp(appDataIter->versionCode, appId);
        mOSVersionsList.addApp(appDataIter);

        // Update the mAppsByDownloadCount tree
        mAppsByDownloadCount.removeApp(oldDownloadCount, appId);
        mAppsByDownloadCount.addApp(appDataIter);

        // Update the app's AppData structure
        appDataIter->downloadCount = newDownloadCount;

    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (const InvalidVersionCodeException& e) {
        return INVALID_INPUT;
    } catch (const NoSuchVersionCodeException& e) {
        return FAILURE;
    } catch (const NoSuchAppException& e) {
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::UpgradeApplication(int appId) {
    if (appId <= 0) {
        return INVALID_INPUT;
    }

    // Find the app's data using mAppsById
    AppsListIterator* appDataIterPtr = NULL;
    try {
        appDataIterPtr = mAppsById.getAppById(appId);
    } catch (const ElementNotFoundException& e) {
        return FAILURE;
    }
    AppsListIterator appDataIter = *appDataIterPtr;

    try {

        // Extract the app's current versionCode
        int oldVersionCode = appDataIter->downloadCount;

        // Get the following versionCode in the mOSVersionsList
        int newVersionCode = mOSVersionsList.getFollowingVersion(oldVersionCode);

        // Remove old appData pointer from the old place in tthe mOSVersionsList tree
        mOSVersionsList.removeApp(oldVersionCode, appId);
        // Update the AppData structure with the new version
        appDataIter->versionCode = newVersionCode;
        // Re-insert the appData pointer to the new place in the mOSVersionsList tree
        mOSVersionsList.addApp(appDataIter);

    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (const InvalidVersionCodeException& e) {
        return INVALID_INPUT;
    } catch (const NoSuchVersionCodeException& e) {
        return FAILURE;
    } catch (const NoSuchAppException& e) {
        return FAILURE;
    } catch (const AppAlreadyExistsException& e) {
        // Shouldn't really happen
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::GetTopApp(int versionCode, int *appId) {
    if (versionCode <= 0 || appId == NULL) {
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
        *appId = mOSVersionsList.getTopAppId(versionCode);
    } catch (const InvalidVersionCodeException& e) {
        return INVALID_INPUT;
    } catch (const NoSuchVersionCodeException& e) {
        // There is no OSVersion with the specified versionCode
        return FAILURE;
    } catch (const NoSuchAppException& e) {
        // There are no apps with the specified versionCode
        return FAILURE;
    }

    return SUCCESS;
}

StatusType Statistics::GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps) {
    if (versionCode <= 0 || apps == NULL || numOfApps == NULL) {
        return INVALID_INPUT;
    }

    // Default values in case of error
    *numOfApps = 0;
    *apps = NULL;

    AppsByDownloadCountTree* tree;

    if (versionCode < 0) {
        // User wants all apps in the system by downloads - Traverse
        // mAppsByDownloadCount
        tree = &mAppsByDownloadCount;

    } else {
        // User wants all apps with a specific versionCode by downloads - Traverse
        // the tree inside the relevant node in mOSVersionsList
        try {
            tree = mOSVersionsList.getAppsByDownloadCountTree(versionCode);
        } catch (const InvalidVersionCodeException& e) {
            return INVALID_INPUT;
        } catch (const NoSuchVersionCodeException& e) {
            // Return defaults for apps and numOfApps
            return SUCCESS;
        }
    }


    // Now that we got our tree:

    // Get the tree's size and check if empty
    int treeSize = tree->getTreeSize();
    if (treeSize == 0) {
        // There are no apps with the specified versionCode
        // Return defaults for apps and numOfApps
        return SUCCESS;
    }

    // Enumerate the tree's data into an array
    AppsListIterator* appDataItersArray = NULL;
    try {
        appDataItersArray = new AppsListIterator[treeSize];
    } catch (const std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }

    tree->enumerateData(appDataItersArray);


    // We need to extract the data we want from the AppData pointers array
    // to an array of AppIds
    // Allocate array using malloc() because the user uses free() later
    int* appIdsArray = (int*) malloc(sizeof(int) * treeSize);
    if (appIdsArray == NULL) {
        // Cleanup previously allocated array
        delete[] appDataItersArray;
        return ALLOCATION_ERROR;
    }

    for (int i=0; i<treeSize; i++) {
        // NOTE: We trust enumerateData() here in that all the elements in
        // the array it returned are valid pointers
        appIdsArray[i] = appDataItersArray[i]->appId;
    }

    delete[] appDataItersArray;

    // Got everything we need, return it to the user
    *numOfApps = treeSize;
    *apps = appIdsArray;

    return SUCCESS;
}

StatusType Statistics::UpdateDownloads(int groupBase, int multiplyFactor) {
	// Check argument validity
	if(groupBase < 1 || multiplyFactor <= 0) {
		return INVALID_INPUT;
	}

	// Dump the AppsByDownloadCountTree into an array
	AppListIterator apps[mAppsByDownloadCount.getTreeSize()];
	int arraySize = mAppsByDownloadCount.enumerateData(apps);

	// Create two stacks using DoubleLinkedLists, one for holding the elements that
	// were will not be modified and the second for storing only modified elements. Both stacks
	// will already be sorted, as the original array is sorted.
	AppStack stack1 = AppStack();
	AppStack stack2 = AppStack();

	// Increase the download count for each application that is associated
	// with the provided groupBase
	for(int i = 0 ; i < arraySize ; i++) {
		if(apps[i]->appId % groupBase == 0) {
			apps[i]->downloadCount *= multiplyFactor;
			stack1.insertFront(apps[i]);
		} else {
			stack2.insertFront(apps[i]);
		}
	}

	// Reuse the allocated array to merge the stacks, creating a sorted array
	int dlCount1, dlCount2;
	for(int i = 0 ; i < arraySize ; i++) {

		try {
			dlCount1 = stack1.getFront()->downloadCount;
		} catch (exception& e) {
			dlCount = -1;
		}
	}


	// TODO Refactor with new list iterator

    throw exception();
    return SUCCESS;
}


