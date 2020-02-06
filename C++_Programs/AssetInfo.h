#include <stdio.h>
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include<ctime>
#include<vector>


HANDLE ghMutex;
const char * const mutexname = "AssetInfo";

class AssetInfo
{

public:
	string InputData;
	char *path;
	string delimiter;
	ifstream ifassetInfo;
	string deviceIdValue;
	string devicemsg;
	static string LastMessage;
	AssetInfo();
	void GetAssetInformation();	
	void ReadAssetInfromation();
	void WriteAssetInformation(string msg);
	void ParseMessage();
	bool GetFileContent(vector<std::string> & vecOfStrs);
};