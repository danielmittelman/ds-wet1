/****************************************************************************/
/*                                                                          */
/* Statistics class implementation                                          */
/*                                                                          */
/****************************************************************************/

#include <new>

#include "Statistics.h"

using std::bad_alloc;


Statistics::Statistics() {
    // TODO
}

Statistics::~Statistics() {
    Quit();
}

StatusType Statistics::AddVersion(int versionCode) {
    try {
        // TODO
    } catch (const bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
}

StatusType Statistics::AddApplication(int appID, int versionCode, int downloadCount) {
    // TODO
}

StatusType Statistics::RemoveApplication(int appID) {
    // TODO
}

StatusType Statistics::IncreaseDownloads(int appID, int downloadIncrease) {
    // TODO
}

StatusType Statistics::UpgradeApplication(int appID) {
    // TODO
}

StatusType Statistics::GetTopApp(int versionCode, int *appID) {
    // TODO
}

StatusType Statistics::GetAllAppsByDownloads(int versionCode, int **apps, int *numOfApps) {
    // TODO
}

StatusType Statistics::UpdateDownloads(int groupBase, int multiplyFactor) {
    // TODO
}

void Statistics::Quit() {
    // TODO
}

