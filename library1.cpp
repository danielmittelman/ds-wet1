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


/*
 * Adapters for the Data Structures Wet1 interface functions
 */

void* Init() {
    Statistics* DS = new Statistics();
    return (void*)DS;
}

StatusType AddVersion(void* DS, int versionCode) {
    return ((Statistics*)DS)->AddVersion(versionCode);
}

/* TODO: Continue for all interface functions */

StatusType AddApplication(void *DS, int appID, int versionCode, int downloadCount);
StatusType RemoveApplication(void *DS, int appID);
StatusType IncreaseDownloads(void *DS, int appID, int downloadIncrease);
StatusType UpgradeApplication(void *DS, int appID);
StatusType GetTopApp(void *DS, int versionCode, int *appID);
StatusType GetAllAppsByDownloads(void *DS, int versionCode, int **apps, int *numOfApps);
StatusType UpdateDownloads(void *DS, int groupBase, double multiplyFactor);
void Quit(void** DS);

