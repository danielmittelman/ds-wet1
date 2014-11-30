/****************************************************************************/
/*                                                                          */
/* Statistics class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_STATISTICS_H_
#define _234218_WET1_STATISTICS_H_

#include "library1.h"
#include "common.h"
#include "DoubleLinkedList.h"
#include "OSVersionsList.h"
#include "AVLTree.h"
#include "AppsByIdTree.h"


// Define AppsList - A list of AppData
typedef DoubleLinkedList<AppData> AppsList;
typedef AppsList::Iterator AppsListIterator;
typedef DoubleLinkedList<AppsListIterator> AppStack;


class Statistics {
public:
    Statistics();

    ~Statistics();

    StatusType AddVersion(int versionCode);

    StatusType AddApplication(int appId, int versionCode, int downloadCount);

    StatusType RemoveApplication(int appId);

    StatusType IncreaseDownloads(int appId, int downloadIncrease);

    StatusType UpgradeApplication(int appId);

    StatusType GetTopApp(int versionCode, int *appId);

    StatusType GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps);

    StatusType UpdateDownloads(int groupBase, int multiplyFactor);

private:
    enum {
        INVALID_TOP_APP_ID = -1;
    };

    AppsList mAppsList;
    OSVersionsList mOSVersionsList;
    AppsByIdTree mAppsById;
    AppsByDownloadCountTree mAppsByDownloadCount;

    // globalTopAppId will be INVALID_TOP_APP_ID if there are no apps
    int mTopAppId;
    int mTopAppDownloadCount;

    void doUpdateDownloadsInTree(AppsByDownloadCountTree* tree, int groupBase, int multiplyFactor, bool isKTree) {
    	// Dump the AppsByDownloadCountTree into an array
    	AppsListIterator apps[tree->getTreeSize()];
    	int arraySize = tree->enumerateData(apps);

    	// Create two stacks using DoubleLinkedLists, one for holding the elements that
    	// were will not be modified and the second for storing only modified elements. Both stacks
    	// will already be sorted, as the original array is sorted.
    	AppStack stack1 = AppStack();
    	AppStack stack2 = AppStack();

    	// Increase the download count for each application that is associated
    	// with the provided groupBase and push into one of the stack in reverse,
    	// so that at the end the head element in both stacks will be the lowest
    	for(int i = (arraySize - 1) ; i >= 0 ; i--) {
    		if(apps[i]->appId % groupBase == 0) {
    			// If this is not one of the k OSVersion trees, actually update the value, otherwise
    			// the value was already updated and we shouldn't touch it again
    			if(!isKTree) {
    				apps[i]->downloadCount *= multiplyFactor;
    			}
    			stack1.insertFront(apps[i]);
    		} else {
    			stack2.insertFront(apps[i]);
    		}
    	}

    	// Reuse the allocated array to merge the stacks, creating a sorted array
    	int dlCount1, dlCount2;
    	for(int i = 0 ; i < arraySize ; i++) {
    		// Get the download count from the top element in both stacks
    		try {
    			dlCount1 = stack1.getFront()->downloadCount;
    		} catch (exception& e) {
    			dlCount1 = -1;
    		}

    		try {
    			dlCount2 = stack2.getFront()->downloadCount;
    		} catch (exception& e) {
    			dlCount2 = -1;
    		}

    		// Select the lower element, first by downloadCount and then by appId, and insert into the array
    		if(dlCount1 < dlCount2) {
    			apps[i] = stack1.popFront(); // TODO: Implement something like this
    		} else if(dlCount2 < dlCount1) {
    			apps[i] = stack2.popFront();
    		} else {
    			apps[i] = (stack1.getFront()->appId > stack2.getFront()->appId) ? stack1.popFront() : stack2.popFront();
    		}
    	}

    	// Restore the array into the tree
    	tree->arrayFillTree(apps);

    	// Make sure everything's deallocated
    	delete[] apps;
    	delete stack1;
    	delete stack2;
    }
};

#endif    /* _234218_WET1_STATISTICS_H_ */
