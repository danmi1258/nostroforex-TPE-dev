//+------------------------------------------------------------------+
//|                                                   Simple Manager |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Manager.h"
//#include "AppStart.h"
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
	Manager *manager;

	manager = new Manager(argc,argv);
	if (manager->createManagerInterface() != MT_RET_OK)
		return(0);
	manager->StartWS();
   wprintf_s(L"Press any key to stop...");
   _getwch();
   manager->StopWS();
   wprintf_s(L"\n");
//--- return
   return(0);
  }
//+------------------------------------------------------------------+
