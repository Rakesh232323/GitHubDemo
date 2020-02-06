#include "AssetInfo.h"
#include "windows.h"



string AssetInfo::LastMessage = "";

AssetInfo::AssetInfo()
{
	path = "D:\\AssetMange.txt";
//	InputData = "MsgId:501$DeviceId:101$DeviceType:SNMP$H/wVersion:1.1$S/wVersion:2.2$x:2$y:3$z:1$a:1$2";
	//InputData = "DeviceId:101$DeviceType:SNMP$Msg:H/wVersion:1.1$S/wVersion:2.2$x:2$y:3$z:1$a:1";
	InputData = "MsgId:501$DeviceId:101$DeviceType:SNMP$Msg:H/wVersion:1.1$S/wVersion:2.2$x:2$y:3$z:1$a:1";

}

void AssetInfo::GetAssetInformation()
{

}
void AssetInfo::ReadAssetInfromation()
{
	int line = 0;
	int counter = 0;
	ifassetInfo.open(path, ios::in);

	while (!ifassetInfo.eof())
	{
		//getline(ifassetInfo, line);
		counter++;
	}

}

/*
1. Open a file to write
2. Lock the file 
3. Write the Data
4. Release the lock

*/
void AssetInfo::ParseMessage()
{
	string delimiter = "$";
	string FinalMsg = "";
	size_t pos = 0;
	size_t firstpos = 0;
	std::string token;

	// // Think to remove if not feasible
	//if (LastMessage == InputData)
	//	return;
	while ((pos = InputData.find(delimiter)) != std::string::npos) 
	{
		if (firstpos == 0)
		{
			string token;
			firstpos = pos;
			token = InputData.substr(0, pos);
			//again parse to extract substring (example MsId and it's value)
			int pos = token.find(":");
			string msgid = token.substr(0, pos); // verifying the msgid string
			string msgidvalue = token.substr(pos + 1);
			if (msgidvalue != "501")
			break;
			InputData.erase(0, firstpos + delimiter.length());
			//cout << "\t" << InputData;
			continue;
		}		
	//token = InputData.substr(0,pos+1);		
	//std::cout << token << std::endl;   // This is to print in the console	
	FinalMsg += InputData.substr(0, pos + 1);
	InputData.erase(0, pos + delimiter.length());
	}

	// Create a Separeate function to writeinfile
	cout << "\n" << "After Parsing writing the below message directly to the file\n";
	cout << FinalMsg;

	 //Appending the TimeStamp

	int count = 0;
	string temp = "";
	time_t currenttime = time(0);
	//Converting currenttime to string form
	char dt[50] = {};
	string CurrentStamp = "TimeStamp:";
	ctime_s(dt, 50, &currenttime);
	CurrentStamp += (string)dt;
	//dt is coming with new line so removing the new line from the string
	int posi = CurrentStamp.find("\n");
	CurrentStamp.erase(posi, posi + 1);	
		
	while ((pos = FinalMsg.find(delimiter)) != std::string::npos)
	{	
		//as message format has DeviceType after the second delimeter 
		if (count <= 1)
		{
			temp += FinalMsg.substr(0, pos + 1);
			FinalMsg.erase(0, pos + delimiter.length());
			count++;
			continue;
		}
		//adding the delimeter to the string
		temp += CurrentStamp + delimiter + FinalMsg;
		
			break;		
	}

	//Critical Section
	WriteAssetInformation(temp);
}

void AssetInfo::WriteAssetInformation(string FinalMsg)
{
	DWORD  dwWaitResult;
	//Create Mutex
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		(LPCWSTR)mutexname);             // named mutex

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return;
	}

	dwWaitResult = WaitForSingleObject(
		ghMutex,    // handle to mutex
		INFINITE);  // no time-out interval


	if (dwWaitResult != 0) // Not handling the errors.
		return;
	//Open the file for both readin and writing purpose and apply lock mechanism 

	//Call Read from file to avoid duplicacy 
	ofstream assetmanageinfoFile;

	assetmanageinfoFile.open(path, ios::out | ios::app | ios::ate);

	//size of the Final message
	int size = FinalMsg.size();	
	assetmanageinfoFile << FinalMsg + "\n";
	assetmanageinfoFile.close();

	if (!ReleaseMutex(ghMutex))
	{
		// Handle error.
	}

	LastMessage = FinalMsg;
	
}

void main()
{
	AssetInfo asset,asset2;
	
	asset.ParseMessage();
	asset.ParseMessage();

	//getchar();
	//assetmanagement.ReadAssetInfromation();
}