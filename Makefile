all:
	# Must compile using this command:
	#g++ -o Statistics -DNDEBUG -Wall *.cpp
    # For now use this command:
	g++ -g -o Statistics -DNDEBUG -Wall AppsByDownloadCountTree.cpp AppsByIdTree.cpp library1.cpp main1.cpp OSVersionsList.cpp Statistics.cpp > BuildLog.txt 2>&1
