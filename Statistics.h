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
};

#endif    /* _234218_WET1_STATISTICS_H_ */
