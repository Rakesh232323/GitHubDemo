#include "AssetInfo.h"
#include "windows.h"

string AssetInfo::LastMessage = "";

AssetInfo::AssetInfo()
{

	delimiter = "$";
	devicemsg = "";
	path = "D:\\AssetInfo.txt";
	InputData = "DeviceId:103$DeviceType:SNMP$Msg:H/wVersion:2.8$S/wVersion:2.8$x:2$y:4$z:5$a:6$b:3.10";
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
//It Parse and filter the device id and device id value in a member variable
void AssetInfo::ParseMessage()
{

	int pos = 0;
	int firstpos = 0;
	string InputMsg = InputData;
	while ((pos = InputMsg.find(delimiter)) != std::string::npos)
	{
		if (firstpos == 0)
		{
			firstpos = pos;
			string token;
			token = InputMsg.substr(0, pos);
			//again parse to extract substring (example MsId and it's value)
			int pos = token.find(":");
			string deviceid = token.substr(0, pos); // verifying the msgid string  // I got the device id to compare
			deviceIdValue = token.substr(pos + 1);
			InputMsg.erase(0, firstpos + delimiter.length());
			//cout << "\t" << InputData;
			continue;
		}

		else
		{
			string token;
			InputMsg.erase(0, pos + delimiter.length());
			devicemsg = InputMsg;  // I got the message to compare
			break;
		}

	}


}

void AssetInfo::WriteAssetInformation(string FinalMsg,bool update)
{	
	string CurrentTimeStamp;

	if (update)
	{		
		assetinfoFile << FinalMsg + "\n";

	}
	else  // It means file is empty and it is going to be the first entry in the file
	{
		assetinfoFile.open(path, ios::out | ios::ate|ios::app);
		CurrentTimeStamp = GetCurrentTimeStamp();
		FinalMsg.insert(0, CurrentTimeStamp + delimiter);
		assetinfoFile << FinalMsg + "\n";

	}

	assetinfoFile.close();

	if (!ReleaseMutex(ghMutex))
	{
		// Handle error.
	}
	
}



bool AssetInfo::GetFileContent(vector<std::string> & vecOfStrs)
{
	int emptyfile = 0;
	DWORD  dwWaitResult;
	//Create Mutex
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		(LPCWSTR)mutexname);             // named mutex

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return false;  //Rakesh Please check this
	}

	dwWaitResult = WaitForSingleObject(
		ghMutex,    // handle to mutex
		INFINITE);  // no time-out interval


	if (dwWaitResult != 0) // Not handling the errors.
		return false;   // Rakesh Please check this 


	string pathe = "D:\\AssetMange.txt";
	// Open the File
	//ifstream in(pathe.c_str());
	fstream in;
	int empty;
	in.open(path, ios::in|ios::out);

	// Check if object is valid
	if (!in)  // File does not exist
	{
		WriteAssetInformation(InputData, false);
		in.close();
		return false;
	}

	string str;
	
	// Read the next line from File untill it reaches the end.
	int count = 0;
	while (getline(in, str))
	{
		count++;
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
		{
			vecOfStrs.push_back(str);
		}
	}
	if (count == 0)  // This means file is emtpy
	{
		WriteAssetInformation(InputData, false);
		return false;

	}

	//Close The File
	in.close();
	return true;
}


string AssetInfo:: GetCurrentTimeStamp()
{
	int pos;
	time_t currenttime = time(0);
	//Converting currenttime to string form
	char dt[50] = {};
	string CurrentStamp = "TimeStamp:";
	ctime_s(dt, 50, &currenttime);
	CurrentStamp += (string)dt;
	//dt is coming with new line so removing the new line from the string
	pos = CurrentStamp.find("\n");
	CurrentStamp.erase(pos, pos + 1);
	return CurrentStamp;
}


void main()
{
	AssetInfo asset, asset2;
	bool update = false;
	int flag = 0;
	ofstream of;
	string CurrentStamp = "";
	string temp = "";  // for storing the one line actual message from the vector
	string newentry = asset.InputData;

	asset.ParseMessage();
	
	//Declare a vector to store the content of the file
	vector<std::string> vecOfStr;
	// Get the contents of file in a vector
	if(asset.GetFileContent(vecOfStr))
	{
		for (string & line : vecOfStr)
		{
			int pos = 0;			
			int count = 0;
			int firstpos = 0;
			string token;
			string deviceIdValue = "";
			temp = line;
			while ((pos = line.find(asset.delimiter)) != std::string::npos)
			{
				if (count < 2)  
				{
					if (firstpos == 0)  // Removing the TimeStamp attribute from the actual message structure
					{
						line.erase(0, pos + asset.delimiter.length());
						firstpos = pos;
						count++;
						continue;

					}
					else  // Getting the DeviceID Value and Then Removing the DeviceID Attribute from the actual message structure
					{
						string token;
						token = line.substr(0, pos);
						//again parse to extract substring (example MsId and it's value)
						int pose = token.find(":");
						string deviceid = token.substr(0, pose); // verifying the msgid string  // I got the device id to compare
					    deviceIdValue = token.substr(pose + 1);
						line.erase(0, pos + asset.delimiter.length());
						count++;
						continue;
					}
				}
				else
				{
					string token;
					line.erase(0, pos + asset.delimiter.length());  // Removing the Device Type attribute from the message structure and storing the actual device related information(Ex - Msg:H/w:1.2$...) to a local variable
					string devicemsg = line;  // I got the message to compare
					//Comparing with the actual message , messag has difference
					if (deviceIdValue == asset.deviceIdValue  && devicemsg != asset.devicemsg)
					{
						CurrentStamp = asset.GetCurrentTimeStamp();
						line = asset.InputData;  // appending the recieved messgae						
						line.insert(0, CurrentStamp + asset.delimiter);
						update = true;   // To identify we need to update the file contents
						flag = 0;
						break;			
						
					}
					else if (deviceIdValue == asset.deviceIdValue && devicemsg == asset.devicemsg)
					{
						flag = 2;  // It means entry is matching in the file no need to write or read the file 
						break;
					}
					else  // IT means the strng msg is same that means no update it may be a new entry so wee need to fill the vector with input string coming 
					{
						flag = 1;
						line = temp;  // as the strings are removed so we may loose the entire text so copying it from temp which intially only stores the original text

					}
					break;

				}
				
			}
			if (update)
				break;
			
		}
		
			if (flag == 1)  // for a new entry after some entry
			{					
				asset.WriteAssetInformation(newentry,false);
			}
			if(flag == 0)  // updae the existing record
			{
				asset.assetinfoFile.open(asset.path, ios::out | ios::trunc);
				for (std::string & line : vecOfStr)
				{
					asset.WriteAssetInformation(line, true);
				}
			}
			if (flag == 2)   // No updation required
			{
				 //Check twice do you have any file opened if yes please close the file
				if (!ReleaseMutex(ghMutex))
				{
					// Handle error.
				}

			}
		

		return;

	}

}

