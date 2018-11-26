#pragma once

#ifndef APPSTART_H
#define APPSTART_H

#include <string>
#include <stdlib.h>
#include <wchar.h>
#include <sstream>
#include <cstring>
#include "boost\algorithm\string.hpp"


namespace FIXR
{

	class AppStart
	{
	private:
		std::string		exePath;

		std::string		fSetting;
		std::string		mLogin;
		std::string		mPassword;
		std::string		mIP;
		std::string		isLogRequired = "N";
	public:
		AppStart();
		static std::string getExeLocation();
		void readConfigFile();

		std::string FixSetting() { return fSetting; };
		std::string ManagerLogin() { return mLogin; };
		std::string ManagerPassword() { return mPassword; };
		std::string ManagerIP() { return mIP; };
		std::string FileLogRequired() { return isLogRequired; };
	};
}

#endif
