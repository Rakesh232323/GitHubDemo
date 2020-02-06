#include <stdio.h>
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include<ctime>


HANDLE ghMutex;
const char * const mutexname = "AssetInfo";

class AssetInfo
{

public:
	string InputData;
	char *path;
	ifstream ifassetInfo;
	static string LastMessage;
	AssetInfo();
	void GetAssetInformation();	
	void ReadAssetInfromation();
	void WriteAssetInformation(string msg);
	void ParseMessage();
};