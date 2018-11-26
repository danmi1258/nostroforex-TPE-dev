/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#else
#include "config.h"
#endif

#include "quickfix/FileStore.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/FileLog.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include "AppStart.h"
#include "CService.h"

// 
// Settings of the service
// 

// Internal name of the service
#define SERVICE_NAME             L"TestWindowsService"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"TestWindowsService Sample Service"

// Service start options.
#define SERVICE_START_TYPE       SERVICE_DEMAND_START

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L""

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\LocalService"

// The password to the service account name
#define SERVICE_PASSWORD         NULL







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


/*
int main( int argc, char** argv )
{
  try
  {
	std::shared_ptr<FIXR::AppStart> App(new FIXR::AppStart());
	std::shared_ptr<FIX::SessionSettings> ptr_settings(new FIX::SessionSettings(App->FixSetting()));
	std::shared_ptr<FIX::FileStoreFactory> ptr_storeFactory(new FIX::FileStoreFactory(*ptr_settings));
	std::shared_ptr<FIX::FileLogFactory> ptr_logFactory(new FIX::FileLogFactory(*ptr_settings));
	std::shared_ptr<Application> ptr_application(new Application(App->ManagerIP(), App->ManagerLogin(), App->ManagerPassword(), *ptr_settings));
	if (App->FileLogRequired() == "Y")
	{
		std::shared_ptr<FIX::ThreadedSocketAcceptor> acceptor(new FIX::ThreadedSocketAcceptor(*ptr_application, *ptr_storeFactory, *ptr_settings, *ptr_logFactory));
		acceptor->start();
		wait();
		acceptor->stop();
	}
	else if (App->FileLogRequired() == "N")
	{
		std::shared_ptr<FIX::ThreadedSocketAcceptor> acceptor(new FIX::ThreadedSocketAcceptor(*ptr_application, *ptr_storeFactory, *ptr_settings));
		acceptor->start();
		wait();
		acceptor->stop();
	}	
	
    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
}*/
