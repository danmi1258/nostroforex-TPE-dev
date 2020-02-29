#include "AppStart.h"
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>


AppStart::AppStart()
{
	readConfigFile();
}

std::string AppStart::getExeLocation()
{
	TCHAR path[2048] = { 0 };
	std::string logFilename;
	GetModuleFileName(NULL, path, 2048);
	std::wstring test(&path[0]);
	const std::string exe_path(test.begin(), test.end());
	logFilename = exe_path.substr(0, exe_path.find_last_of("/\\"));
	return logFilename;
}

void AppStart::readConfigFile()
{
	std::string KeyID, KeyValue;
	std::fstream infile;
	infile.open(getExeLocation() + "\\AppConfig\\AppSetting.txt");
	while (infile.good()) // To get you all the lines.
	{
		std::getline(infile, KeyID, '=');		
		if (KeyID == std::string("ManagerLogin"))
			std::getline(infile, mLogin, '\n');
		else if (KeyID == std::string("ManagerPassword"))
			std::getline(infile, mPassword, '\n');
		else if (KeyID == std::string("ManagerIP"))
			std::getline(infile, mIP, '\n');
		else if (KeyID == std::string("Port"))
			std::getline(infile, mPort, '\n');
	}
	infile.close();
}


