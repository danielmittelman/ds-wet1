/****************************************************************************/
/*                                                                          */
/* OSVersionsList class implementation                                      */
/*                                                                          */
/****************************************************************************/

#include <cstdlib>                          // NULL definition
#include "OSVersionsList.h"


void OSVersionsList::addVersion(int versionCode) {
	if (versionCode <= 0) {
		throw InvalidVersionCodeException();
	}

	// First make sure that the versionCode is indeed larger than the largest
	// versionCode we currently have
	if ( (!isEmpty()) && (begin()->versionCode >= versionCode) ) {
		throw VersionCodeNotLargerThanCurrentException();
	}

	// The new versionCode is good, allocate a new struct with it and
	// insert it into the list
	OSVersionData data(versionCode);
	insertFront(data);
}

int OSVersionsList::getTopAppId(int versionCode) const {
	// Search for the OSVersionData node with the relevant versionCode
	// (an exception will be thrown if it is not found)
	Iterator iter = getOSVersionDataByVersionCode(versionCode);

	// Found our node, return the top app if there is one
	if (iter->versionTopAppId == INVALID_VERSION_TOP_APP_ID) {
		throw NoSuchAppException();
	}

	return iter->versionTopAppId;
}

void OSVersionsList::addApp(const AppsListIterator& appDataPtr) {
	// Search for the OSVersionData node with the relevant versionCode
	// (an exception will be thrown if it is not found)
	Iterator iter = getOSVersionDataByVersionCode(appDataPtr->versionCode);

	// Found our node, add the app to the versionAppsByDownloadCount tree in it
	try {
		iter->versionAppsByDownloadCount.addApp(appDataPtr);
	} catch (const DuplicateNodeException& e) {
		throw AppAlreadyExistsException();
	}

	// App was successfully added, don't forget to update versionTopAppId and
	// versionTopAppDownloadCount if needed
	updateVersionTopApp(iter);
}

void OSVersionsList::removeApp(int versionCode, int downloadCount, int appId) {
	// Search for the OSVersionData node with the relevant versionCode
	// (an exception will be thrown if it is not found)
	Iterator iter = getOSVersionDataByVersionCode(versionCode);

	// Found our node, remove the app from the versionAppsByDownloadCount tree
	// in it
	try {
		iter->versionAppsByDownloadCount.removeApp(downloadCount, appId);
	} catch (const ElementNotFoundException& e) {
		throw NoSuchAppException();
	}

	// App was successfully removed, don't forget to update
	// versionTopAppId and versionTopAppDownloadCount if needed
	updateVersionTopApp(iter);
}

// Throws NoSuchVersionCodeException if is no such versionCode in the list,
// or there is no versionCode in the list which is larger than the given one
int OSVersionsList::getFollowingVersion(int versionCode) const {
	if (versionCode <= 0) {
		throw InvalidVersionCodeException();
	}

	// Iterate over all nodes until one matches the version code
	FilterByVersionCodePredicate predicate;

	Node* p = mHead;

	while (p) {
		if (predicate(p->data, (void*) versionCode)) {
			// Found it - Return the previous node's versionCode
			if (p->prev) {
				return p->prev->data->versionCode;
			}

			// The versionCode given is the largest one, we cannot return
			// the following version
			throw VersionCodeIsLastException();
		}

		p = p->next;
	}

	// Given versionCode is not in the list
	throw NoSuchVersionCodeException();
}

AppsByDownloadCountTree* OSVersionsList::getAppsByDownloadCountTree(int versionCode) const {
	// Search for the OSVersionData node with the relevant versionCode
			// the following version
	Iterator iter = getOSVersionDataByVersionCode(versionCode);
	return &(iter->versionAppsByDownloadCount);
}

void OSVersionsList::updateVersionTopApp(Iterator& iter) {
	try {
		AppsListIterator* newTopAppPtr = iter->versionAppsByDownloadCount.getMax();
		AppsListIterator newTopApp = *newTopAppPtr;

		iter->versionTopAppDownloadCount = newTopApp->downloadCount;
		iter->versionTopAppId = newTopApp->appId;

	} catch (const ElementNotFoundException& e) {
		// This happens when there are no elements in versionAppsByDownloadCount
		// In this case update the versionTopApp to an invalid value
		iter->versionTopAppDownloadCount = -1;
		iter->versionTopAppId = INVALID_VERSION_TOP_APP_ID;
	}
}


// Private:

OSVersionsList::Iterator OSVersionsList::getOSVersionDataByVersionCode(int versionCode) const {
	if (versionCode <= 0) {
		throw InvalidVersionCodeException();
	}

	Iterator iter;
	try {

		iter = getIteratorByPredicate(FilterByVersionCodePredicate(),
				(void*) versionCode);

	} catch (const NoSuchNodeException& e) {
		throw NoSuchVersionCodeException();
	}

	return iter;
}

bool OSVersionsList::FilterByVersionCodePredicate::operator()
		(const OSVersionData* dataPtr, void* versionCode) const {

	// NOTE: Using "long int" here because on 64-bit machines it is
	// not possible to convert void* into int. If we try this we get:
	// error: cast from ‘void*’ to ‘int’ loses precision

	long int versionCodeInt = (long int) versionCode;
	return (dataPtr->versionCode == versionCodeInt);
}
