#pragma once

#ifndef APPSTART_H
#define APPSTART_H

#include "stdafx.h"
#include <string>
#include <stdlib.h>
#include <wchar.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include "boost\algorithm\string.hpp"


class AppStart
	{
	private:
		std::string		exePath;

		std::string		mLogin;
		std::string		mPassword;
		std::string		mIP;
		std::string		mPort;
	public:
		AppStart();
		static std::string getExeLocation();
		void readConfigFile();

		std::string ManagerLogin() { return mLogin; };
		std::string ManagerPassword() { return mPassword; };
		std::string ManagerIP() { return mIP; };
		std::string Port() { return mPort; };
	};

#endif
