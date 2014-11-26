/****************************************************************************/
/*                                                                          */
/* Common definitions for Data Structures 1 Wet1 assignment                 */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_COMMON_H_
#define _234218_WET1_COMMON_H_


// Define AppData - Data structure to hold the information for each app
struct AppData {
    int appId;
    int versionCode;
    int downloadCount;

    // Constructor for this struct, for simpler initialization
    AppData(int appId, int versionCode, downloadCount) :
            appId(appId),
            versionCode(versionCode),
            downloadCount(downloadCount) {}

};

#endif    /* _234218_WET1_COMMON_H_ */
