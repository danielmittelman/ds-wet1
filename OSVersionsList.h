/****************************************************************************/
/*                                                                          */
/* OSVersionsList class header                                              */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_OSVERSIONS_LIST_H_
#define _234218_WET1_OSVERSIONS_LIST_H_

#include <exception>
#include "DoubleLinkedList.h"
#include "AppsByDownloadCountTree.h"
#include "common.h"

using std::exception;


const int INVALID_VERSION_TOP_APP_ID = -1;


// Exception classes
class InvalidVersionCodeException : public exception {};
class VersionCodeNotLargerThanCurrentException : public exception {};
class NoSuchVersionCodeException : public exception {};
class NoSuchAppException : public exception {};
class AppAlreadyExistsException : public exception {};


struct OSVersionData {
	int versionCode;
	AppsByDownloadCountTree versionAppsByDownloadCount;

	// versionTopAppId will be INVALID_VERSION_TOP_APP_ID if there are no apps
	int versionTopAppId;
	int versionTopAppDownloadCount;

	// Constructor for this struct, to make sure that the top app fields
	// are initialized correctly
	OSVersionData(int versionCode) :
			versionCode(versionCode),
			versionAppsByDownloadCount(),
			versionTopAppId(INVALID_VERSION_TOP_APP_ID),
			// Initialize the top app download count to -1 so that every new
			// app's download count will be larger than it
			versionTopAppDownloadCount(-1) {}

};

class OSVersionsList: public DoubleLinkedList< OSVersionData > {
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
	void addApp(const AppsListIterator& appDataPtr);

	// Removes an app from the data structue. Must receive the app's
	// versionCode because apps are indexed by versionCode
	// Time complexity: O(k + log(n)) where k is the list's length and n is
	// the number of apps in the version's versionAppsByDownloadCount tree
	// Throws InvalidVersionCodeException if the given versionCode is <= 0
	// Throws NoSuchVersionCodeException if is no such versionCode in the list
	// Throws NoSuchAppException if there are no apps with the given id in the
	// version's versionAppsByDownloadCount tree
	void removeApp(int versionCode, int appId);

	// Finds a node with the given versionCode and returns the versionCode of
	// the previous node in the list (the previous node since the list is
	// sorted from highest to lowest versionCodes)
	// Time complexity: O(k) where k is the list's length
	// Throws InvalidVersionCodeException if the given versionCode is <= 0
	// Throws NoSuchVersionCodeException if is no such versionCode in the list,
	// or there is no versionCode in the list which is larger than the given one
	int getFollowingVersion(int versionCode) const;

	// Finds a node with the given versionCode and returns the tree of apps
	// with this versionCode, sorted by downloads count
	// Time complexity: O(k) where k is the list's length
	// Throws InvalidVersionCodeException if the given versionCode is <= 0
	// Throws NoSuchVersionCodeException if is no such versionCode in the list
	AppsByDownloadCountTree* getAppsByDownloadCountTree(int versionCode) const;

private:
	// Helper function to get the OSVersionData of a specific versionCode
	// Throws InvalidVersionCodeException if the given versionCode is <= 0
	// Throws NoSuchVersionCodeException if is no such versionCode in the list
	OSVersionData* getOSVersionDataByVersionCode(int versionCode) const;


	// Predicate to be used with getDataByPredicate
	class FilterByVersionCodePredicate {
	public:
		bool operator() (const OSVersionData* dataPtr, void* versionCode) const;
	};
};

#endif    /* _234218_WET1_OSVERSIONS_LIST_H_ */
