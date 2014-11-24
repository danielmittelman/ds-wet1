/****************************************************************************/
/*                                                                          */
/* OSVersionsList class header                                              */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_OSVERSIONS_LIST_H_
#define _234218_WET1_OSVERSIONS_LIST_H_

#include "library1.h"
#include "DoubleLinkedList.h"


struct OSVersionsData {
    int versionCode;
    // versionTopAppId will be INVALID_VERSION_TOP_APP if there are no apps
    int versionTopAppId;
    AppsByDownloadCountTree versionAppsByDownloadCount;
};

class OSVersionsList: public DoubleLinkedList< OSVersionsData > {
public:
    // Using default constructor

    // Using default destructor

    // Insert an element as the new first element
    // Time complexity: O(1)
    // Throws bad_alloc on allocation error
    // Throws InvalidVersionCodeException if the versionCode is <= 0
    // Throws VersionCodeNotLargerThanCurrentException if the versionCode
    // is <= than the current largest versionCode
    void addVersion(int versionCode);

    // Returns the ID of the top app with the given versionCode
    // Time complexity: O(k) where k is the list's length
    // Throws InvalidVersionCodeException if the given versionCode is <= 0
    // Throws NoSuchVersionCodeException if is no such versionCode in the list
    // Throws NoSuchAppException if there are no apps with the given
    // versionCode
    int getTopAppId(int versionCode) const;

    // Adds an app to the data structue
    // Time complexity: O(k + log(n)) where k is the list's length and n is
    // the number of apps in the relevant versionAppsByDownloadCount tree
    // Throws InvalidVersionCodeException if the given versionCode is <= 0
    // Throws NoSuchVersionCodeException if is no such versionCode in the list
    // Throws AppAlreadyExistsException if the app already exists in the
    // version's versionAppsByDownloadCount tree
    void addApp(int appId, int versionCode, int downloadCount);

    // Removes an app from the data structue. Must receive the app's
    // versionCode because apps are indexed by versionCode
    // Time complexity: O(k + log(n)) where k is the list's length and n is
    // the number of apps in the version's versionAppsByDownloadCount tree
    // Throws InvalidVersionCodeException if the given versionCode is <= 0
    // Throws NoSuchVersionCodeException if is no such versionCode in the list
    // Throws NoSuchAppException if there are no apps with the given id in the
    // version's versionAppsByDownloadCount tree
    void removeApp(int appId, int versionCode);

    // Exception classes
    class InvalidVersionCodeException {};
    class VersionCodeNotLargerThanCurrentException {};
    class NoSuchVersionCodeException {};
    class NoSuchAppException {};
    class AppAlreadyExistsException {};

private:
    enum {
        INVALID_VERSION_TOP_APP = -1;
    };

    // Predicate to be used with getDataByPredicate
    class FilterByVersionCodePredicate {
        bool operator() (const OSVersionsData& data, void* versionCode) const;
    };
};

#endif    /* _234218_WET1_OSVERSIONS_LIST_H_ */
