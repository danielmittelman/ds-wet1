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

	/*
	void printTreeDebugInfo1(AppsByIdTree* tree) {
		// Enumerate the tree's data into an array
		int treeSize = tree->getTreeSize();
		AppsListIterator* appDataItersArray = new AppsListIterator[treeSize];
		tree->enumerateData(appDataItersArray);
		for (int i=0; i<treeSize; i++) {
			std::cout << "\t[" << appDataItersArray[i]->appId << ":" << appDataItersArray[i]->downloadCount << "]" << std::endl;
		}
		delete[] appDataItersArray;
	}
	void printTreeDebugInfo2(AppsByDownloadCountTree* tree) {
		// Enumerate the tree's data into an array
		int treeSize = tree->getTreeSize();
		AppsListIterator* appDataItersArray = new AppsListIterator[treeSize];
		tree->enumerateData(appDataItersArray);
		for (int i=0; i<treeSize; i++) {
			std::cout << "\t[" << appDataItersArray[i]->appId << ":" << appDataItersArray[i]->downloadCount << "]" << std::endl;
		}
		delete[] appDataItersArray;
	}
	void printDebugInfo() {
		std::cout << "************************************" << std::endl;
		std::cout << "*** mAppsList size: " << mAppsList.getSize() << std::endl;
		for (AppsList::Iterator it = mAppsList.begin();
				it != mAppsList.end(); it++) {
			std::cout << "\t***** " << it.mNode << " " << it.mNode->next << std::endl;
			std::cout << "\t[" << it->appId << ":" << it->downloadCount << "]" << std::endl;
		}
		std::cout << "*** mOSVersionsList size: " << mOSVersionsList.getSize() << std::endl;
		for (OSVersionsList::Iterator it = mOSVersionsList.begin();
				it != mOSVersionsList.end(); it++) {
			std::cout << "  Version " << it->versionCode << ":" << std::endl;
			printTreeDebugInfo2(&(it->versionAppsByDownloadCount));
		}
		std::cout << "*** mAppsById (size " << mAppsById.getTreeSize() << "):" << std::endl;
		printTreeDebugInfo1(&mAppsById);
		std::cout << "*** mAppsByDownloadCount (size " << mAppsByDownloadCount.getTreeSize() << "):" << std::endl;
		printTreeDebugInfo2(&mAppsByDownloadCount);
		std::cout << "************************************" << std::endl;
	}
	*/

private:

	enum {
		INVALID_TOP_APP_ID = -1
	};

	AppsList mAppsList;
	OSVersionsList mOSVersionsList;
	AppsByIdTree mAppsById;
	AppsByDownloadCountTree mAppsByDownloadCount;

	// globalTopAppId will be INVALID_TOP_APP_ID if there are no apps
	int mTopAppId;
	int mTopAppDownloadCount;

	// Helper function for updating downloads in an AppsByDownloadCountTree.
	void doUpdateDownloadsInTree(AppsByDownloadCountTree* tree, int groupBase,
			int multiplyFactor, bool shouldUpdateValues);

	// Helper function for updating the top app cache after inserting, removing
	// or otherwise updating the download count of apps.
	void updateTopAppData();
};

#endif    /* _234218_WET1_STATISTICS_H_ */
