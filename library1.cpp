/****************************************************************************/
/*                                                                          */
/* This file contains adapter functions so that our code in the             */
/* Statistics class matches the interface functions                         */
/* for Data Structures 1 Assignment Wet1                                    */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.cpp                                                 */
/*                                                                          */
/****************************************************************************/

#include "library1.h"
#include "Statistics.h"


#define RETURN_INVALID_INPUT_IF_NULL(x)     \
    if ((x) == NULL) {                        \
        return INVALID_INPUT;               \
    };


/*
 * Adapters for the Data Structures Wet1 interface functions
 */

void* Init() {
    Statistics* DS = new Statistics();
    return (void*)DS;
}

StatusType AddVersion(void* DS, int versionCode) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->AddVersion(versionCode);
}

StatusType AddApplication(void *DS, int appID, int versionCode, int downloadCount) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->AddApplication(appID, versionCode, downloadCount);
}

StatusType RemoveApplication(void *DS, int appID) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->RemoveApplication(appID);
}

StatusType IncreaseDownloads(void *DS, int appID, int downloadIncrease) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->IncreaseDownloads(appID, downloadIncrease);
}

StatusType UpgradeApplication(void *DS, int appID) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->UpgradeApplication(appID);
}

StatusType GetTopApp(void *DS, int versionCode, int *appID) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->GetTopApp(versionCode, appID);
}

StatusType GetAllAppsByDownloads(void *DS, int versionCode, int **apps, int *numOfApps) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->GetAllAppsByDownloads(versionCode, apps, numOfApps);
}

StatusType UpdateDownloads(void *DS, int groupBase, int multiplyFactor) {
    RETURN_INVALID_INPUT_IF_NULL(DS);
    return ((Statistics*)DS)->UpdateDownloads(groupBase, multiplyFactor);
}

void Quit(void** DS) {
    if (DS == NULL || *DS == NULL) {
        return;
    }

    delete (Statistics*) *DS;
    *DS = NULL;
}
