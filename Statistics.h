/****************************************************************************/
/*                                                                          */
/* Statistics class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_STATISTICS_H_
#define _234218_WET1_STATISTICS_H_

#include "library1.h"
#include "common.h"
#include "OSVersionsList.h"


// Define AppsList - A list of AppData
typedef DoubleLinkedList<AppData> AppsList;


class Statistics {
public:
    Statistics();

    ~Statistics();

    StatusType AddVersion(int versionCode);

    StatusType AddApplication(int appID, int versionCode, int downloadCount);

    StatusType RemoveApplication(int appID);

    StatusType IncreaseDownloads(int appID, int downloadIncrease);

    StatusType UpgradeApplication(int appID);

    StatusType GetTopApp(int versionCode, int *appID);

    StatusType GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps);

    StatusType UpdateDownloads(int groupBase, int multiplyFactor);

private:
    enum {
        INVALID_TOP_APP_ID = -1;
    };

    AppsList mAppsList;
    OSVersionsList mOSVersions;
    AppsByIdTree mAppsById;
    AppsByDownloadCountTree mAppsByDownloadCount;

    // globalTopAppId will be INVALID_TOP_APP_ID if there are no apps
    int mTopAppId;
    int mTopAppDownloadCount;
};

#endif    /* _234218_WET1_STATISTICS_H_ */
