#include "stdafx.h"
#include "Manager.h"

using namespace std;


//Manager::Manager(std::string server, std::string login, std::string password, std::string port)
Manager::Manager(int arg,wchar_t* argv[])//const wchar_t* argv[]
{
	initialization(arg, argv);

	if ((res = factory.Initialize(L"API\\")) != MT_RET_OK)
	{
		wprintf_s(L"Loading manager API failed (%u)\n", res);
	}
	//--- check Manager API version
	if ((res = factory.Version(version)) != MT_RET_OK)
	{
		wprintf_s(L"Getting API version failed (%u)\n", res);
	}
	wprintf_s(L"Manager API version %u has been loaded\n", version);

	if (version<MTManagerAPIVersion)
	{
		wprintf_s(L"Wrong Manager API version %u, version %u required\n", version, MTManagerAPIVersion); 
	}
	//createManagerInterface();
	//logMessage("Manager Constructor : End");

}

Manager::~Manager()
{
	managerDisconnect();
}

void Manager::initialization(int argc,wchar_t * argv[])
{
	//--- show the starting banner
/*#ifdef _WIN64
	wprintf_s(L"HistoryManager 64 bit\nCopyright 2001-2017, MetaQuotes Software Corp.\n\n");
#else
	wprintf_s(L"HistoryManager 32 bit\nCopyright 2001-2017, MetaQuotes Software Corp.\n\n");
#endif*/
	//--- get parameters
	for (int i = 1; i<argc; i++)
	{
		if (_wcsnicmp(argv[i], L"/server:", _countof(L"/server:") - 1) == 0)
		{
			wcscpy_s(server, argv[i] + _countof(L"/server:") - 1);
			continue;
		}
		if (_wcsnicmp(argv[i], L"/login:", _countof(L"/login:") - 1) == 0)
		{
			login = _wcstoui64(argv[i] + _countof(L"/login:") - 1, NULL, 10);
			continue;
		}
		if (_wcsnicmp(argv[i], L"/password:", _countof(L"/password:") - 1) == 0)
		{
			wcscpy_s(password, argv[i] + _countof(L"/password:") - 1);
			continue;
		}
		if (_wcsnicmp(argv[i], L"/port:", _countof(L"/port:") - 1) == 0)
		{
			wcscpy_s(port1, argv[i] + _countof(L"/port:") - 1);
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, port1, -1, port, 6, &DefChar, NULL);
			continue;
		}
	}
	//--- check and request parameters
	if (server[0] == L'\0')
	{
		wprintf_s(L"Server: ");
		wscanf_s(L"%255s", server, (int)_countof(server));
	}
	if (login == 0)
	{
		wprintf_s(L"Login: ");
		wscanf_s(L"%255s", tmp, (int)_countof(tmp));
		login = _wcstoui64(tmp, NULL, 10);
	}
	if (password[0] == L'\0')
	{
		wprintf_s(L"Password: ");
		wscanf_s(L"%63s", password, (int)_countof(password));
	}
	if (port1[0] == L'\0')
	{
		wprintf_s(L"Port: ");
		wscanf_s(L"%63s", port1, (int)_countof(port));
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, port1, -1, port, 6, &DefChar, NULL);
	}

	//--- check parameters
	if (server[0] == L'\0' || login == 0 || password[0] == L'\0' || port[0] == L'\0')
	{
		Usage();
	}
}

MTAPIRES Manager::createManagerInterface()
{
	//--- creating manager interface
	if ((res = factory.CreateManager(MTManagerAPIVersion, &manager)) != MT_RET_OK)
	{
		wprintf_s(L"Creating manager interface failed (%u)\n", res);
	}
	if (manager->Subscribe(this) != MT_RET_OK)
	{
	}
	if (manager->TickSubscribe(this) != MT_RET_OK)
	{
	}
	//--- subscribe for users
	if (manager->UserSubscribe(this) != MT_RET_OK)
	{
	}
	//--- subscribe for users
	if (manager->GroupSubscribe(this) != MT_RET_OK)
	{
	}
	if(connectManager() != MT_RET_OK)
	{
		return MT_RET_ERROR;
	}
	
	::Sleep(500);
	//ws_server.run(std::stoi(boost::nowide::narrow(port1)),manager);
	//manager->SelectedAddAll();
	//ws_server.run(std::stoi(m_port));
	return MT_RET_OK;
}

MTAPIRES Manager::connectManager()
{
	res = manager->Connect(server, login, password, L"",
		IMTManagerAPI::PUMP_MODE_SYMBOLS |
		IMTManagerAPI::PUMP_MODE_GROUPS |
		IMTManagerAPI::PUMP_MODE_ORDERS |
		IMTManagerAPI::PUMP_MODE_USERS, MT5_TIMEOUT_CONNECT);
	if (res != MT_RET_OK)
	{
		wprintf_s(L"Connection failed (%u)\n", res);
		return MT_RET_ERROR;
		//serverReconnect();
	}
	return MT_RET_OK;
}

void Manager::serverReconnect()
{
	//--- get server name and start network rescan
	MTAPISTR server_name;
	if (manager->NetworkServer(server_name) == MT_RET_OK && manager->NetworkRescan(0, 10000) == MT_RET_OK)
	{
		wprintf_s(L"Reconnecting due better access point is available\n");
		//--- disconnect
		manager->Disconnect();
		//--- connect again to server by name
		res = manager->Connect(server_name, login, password, L"", 0, MT5_TIMEOUT_CONNECT);
		if (res != MT_RET_OK)
		{
			wprintf_s(L"Connection failed (%u)\n", res);
		}
	}
	//logMessage("serverReconnect : End");
}

void Manager::managerDisconnect()
{
	//logMessage("managerDisconnect : Start");
	ws_server.stop();
	//--- disconnect
	manager->Disconnect();
	//--- shutdown
	manager->Release(); manager = NULL;
	factory.Shutdown();
	//logMessage("managerDisconnect : End");
}


std::vector<std::string> Manager::split(std::string strToSplit, char delimeter)
{
	//logMessage("split : Start");
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	//logMessage("split : End");
	return splittedStrings;
}




void Manager::Usage()
{
	wchar_t name[_MAX_PATH] = L"", *ptr = NULL;
	//--- get module file name
	GetModuleFileName(NULL, name, _countof(name));
	ptr = wcsrchr(name, L'\\');
	if (ptr != NULL) ptr++;
	else          ptr = name;
	//--- show usage
	wprintf(L"\nUsage:\n"
		L"  %s /server:address:port /login:login /password:password /port:port \n", ptr);
}

WCHAR * Manager::convertStringToWCHAR(std::string inStr)
{
	WCHAR *outStr;
	//logMessage("convertStringToWCHAR : Start");

	const size_t sizeLogin = strlen(inStr.c_str()) + 1;
	outStr = new WCHAR[sizeLogin];

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, outStr, sizeLogin, inStr.c_str(), sizeLogin - 1);
	//logMessage("convertStringToWCHAR : End");
	return outStr;
}

INT64 Manager::convertStingToINT64(std::string inStr)
{
	return INT64();
}

void Manager::OnTick(LPCWSTR symbol, const MTTickShort & tick)
{
	ws_server.sendJSONTick(boost::nowide::narrow(symbol), tick);
}
