#include "AssetInfo.h"
#include "windows.h"



string AssetInfo::LastMessage = "";

AssetInfo::AssetInfo()
{

	delimiter = "$";
	devicemsg = "";
	path = "D:\\AssetMange.txt";
//	InputData = "MsgId:501$DeviceId:101$DeviceType:SNMP$H/wVersion:1.1$S/wVersion:2.2$x:2$y:3$z:1$a:1$2";
	InputData = "DeviceId:102$DeviceType:SNMP$Msg:H/wVersion:1.1$S/wVersion:2.5$x:2$y:3$z:1$a:1";
	//InputData = "MsgId:501$DeviceId:101$DeviceType:SNMP$Msg:H/wVersion:1.1$S/wVersion:2.2$x:2$y:3$z:1$a:1"; - i used this initally

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

void AssetInfo::WriteAssetInformation(string FinalMsg)
{
	DWORD  dwWaitResult;

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
	//remove(path);
	//rename("D:\\temp.txt", path);	

	LastMessage = FinalMsg;

	//Releae the handle at the end of the operation
	if (!ReleaseMutex(ghMutex))
	{
		// Handle error.
	}
	
}



bool AssetInfo::GetFileContent(vector<std::string> & vecOfStrs)
{

	string pathe = "D:\\AssetMange.txt";
	// Open the File
	std::ifstream in(pathe.c_str());
	// Check if object is valid
	if (!in)
	{
		std::cerr << "Cannot open the File : " << pathe << std::endl;
		return false;
	}

	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
			vecOfStrs.push_back(str);
	}
	//Close The File
	in.close();
	return true;
}



void main()
{
	AssetInfo asset, asset2;
	bool update = false;
	ofstream of;

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
						line = asset.InputData;  // appending the recieved messgae
						time_t currenttime = time(0);
						//Converting currenttime to string form
						char dt[50] = {};
						string CurrentStamp = "TimeStamp:";
						ctime_s(dt, 50, &currenttime);
						CurrentStamp += (string)dt;
						//dt is coming with new line so removing the new line from the string
						int posi = CurrentStamp.find("\n");
						CurrentStamp.erase(posi, posi + 1);
						line.insert(0, CurrentStamp + asset.delimiter);
						update = true;
						break;

						// set some thing true to know					
						
					}
					break;

				}
				   // here it was continue
				
			}
			if (update)
				break;
			
		}

		if (update)   // if true then only truncate the file
		{
			of.open(asset.path, ios::out | ios::trunc);
			for (std::string & line : vecOfStr)
			{
				of << line +"\n";
			}
			of.close();
		}
		else
		{
			return;
			//Release the handle
			//no need to open the file to write also
		}

	}
}

