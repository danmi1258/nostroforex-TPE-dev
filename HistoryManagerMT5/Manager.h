#ifndef Manager_h
#define Manager_h

#pragma once
#include "stdafx.h"
#include "Constant.h"
#include "WebSocketServer.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <ctime>
#include <map>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#pragma comment (lib, "Ws2_32.lib")

//#define FILE_DIRECTORY "C:\\Config\\historyFile.txt"
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "33333"
//#define MAX_DEFAULT_BARS 200
//
//#define MT5_TIMEOUT_CONNECT   30000
//#define MT5_TIMEOUT_DEALER    10000


//+------------------------------------------------------------------+
//| Example usage                                                    |
//+------------------------------------------------------------------+
/*void Usage(void)
{
	wchar_t name[_MAX_PATH] = L"", *ptr = NULL;
	//--- get module file name
	GetModuleFileName(NULL, name, _countof(name));
	ptr = wcsrchr(name, L'\\');
	if (ptr != NULL) ptr++;
	else          ptr = name;
	//--- show usage
	wprintf(L"\nUsage:\n"
		L"  %s /server:address:port /login:login /password:password\n", ptr);
}*/

namespace pt = boost::property_tree;
using namespace pt;

class Manager: public IMTTickSink, public IMTManagerSink,
				public IMTUserSink, public IMTBookSink,
				public IMTConGroupSink
{
private:
	CMTManagerAPIFactory factory;
	//IMTAdminAPI          *manager = NULL;
	IMTManagerAPI          *manager = NULL;
	UINT                 version = 0;
	MTAPIRES             res = MT_RET_OK_NONE;
	wchar_t              server[256] = L"";
	UINT64               login = 0;
	wchar_t              password[64] = L"";
	wchar_t              tmp[256] = L"";
	wchar_t              port1[6] = L"";
	char			     port[6];
	MTAPISTR             strapi;
	MTChartBar			 *bars, *processChartBars;
	UINT				 bars_total;
	INT					 sizeChartBar;
	WebSocketServer		 ws_server;
	std::string			 m_server;
	std::string			 m_password;
	std::string			 m_login;
	std::string			 m_port = "25500";
	
public:

	enum messageType { getHistory };

	void Usage();

	Manager(int arg, wchar_t* argv[]);//const wchar_t* argv[]
	//Manager(std::string server, std::string login, std::string password, std::string port);
	~Manager();

	/* Manager API related. */
	void initialization(int argument,wchar_t* argv[]);
	MTAPIRES createManagerInterface();
	MTAPIRES connectManager();
	void serverReconnect();
	void managerDisconnect();
	void StartWS()
	{
		ws_server.run(std::stoi(boost::nowide::narrow(port1)), manager);
	}
	void StopWS()
	{
		ws_server.stop();
	}
	
	/* Request Message validation and parse related. */
	std::vector<std::string> split(std::string strToSplit, char delimeter);

	/* Utilities function. */
	WCHAR *convertStringToWCHAR(std::string inStr);
	INT64 convertStingToINT64(std::string inStr);

	virtual void  OnTick(LPCWSTR symbol, const MTTickShort&  tick);
};

#endif // !Manager_h