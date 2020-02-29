//+------------------------------------------------------------------+
//|                                                   Simple Manager |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
//#include "AppStart.h"
#include "WebSocket.h"
#include "ManagerInterface.h"
#include <functional>

//#include "CService.h"

#include <iostream>
//---
//#define MT5_TIMEOUT_CONNECT   30000
//#define MT5_TIMEOUT_DEALER    10000


/*
int _tmain(int argc, TCHAR* argv[])
{
	if (argc > 1 && lstrcmpi(argv[1], TEXT("install")) == 0)
	{
		InstallService();
	}
	else if (argc > 1 && lstrcmpi(argv[1], TEXT("uninstall")) == 0)
	{
		UninstallService();
	}
	else
	{
		RunService();
	}

	return 0;
}
*/
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int wmain(int argc,wchar_t* argv[])
  {
	//Manager *manager;
	WebSocketS *ws_server;
	ManagerInterface *manager;

	//manager = new Manager(argc,argv);
	manager = new ManagerInterface(L"188.72.227.210:443", 1005, L"trt487598");
	ws_server = new WebSocketS(argc,argv);
	ws_server->setAddSubscriptionCmd(std::bind(&ManagerInterface::addSubscription, std::ref(manager), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	manager->init();
	manager->start();
	ws_server->run(1245);
   wprintf_s(L"Press any key to stop...");
   _getwch();
   ws_server->stop();
   manager->stop();
   wprintf_s(L"\n");
//--- return
   return(0);
  }
//+------------------------------------------------------------------+
