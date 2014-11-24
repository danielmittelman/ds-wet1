/****************************************************************************/
/*                                                                          */
/* Statistics class header                                                  */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_STATISTICS_H_
#define _234218_WET1_STATISTICS_H_

#include "library1.h"


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
    DoubleLinkedList mOSVersions;
    AVLTree mAppsById;
    AVLTree mAppsByDownloadCount;
};

#endif    /* _234218_WET1_STATISTICS_H_ */
