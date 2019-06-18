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
#include "quickfix/ThreadedSocketInitiator.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include "AppStart.h"

void wait()
{
	//std::cout << "Type Ctrl-C to quit" << std::endl;
	while (true)
	{
		FIX::process_sleep(1);
	}
}



int main( int argc, char** argv )
{
  try
  {
	std::shared_ptr<FIXR::AppStart> App(new FIXR::AppStart());
	std::shared_ptr<FIX::SessionSettings> ptr_settings(new FIX::SessionSettings(App->FixSetting()));
	std::shared_ptr<FIX::FileStoreFactory> ptr_storeFactory(new FIX::FileStoreFactory(*ptr_settings));
	std::shared_ptr<FIX::FileLogFactory> ptr_logFactory(new FIX::FileLogFactory(*ptr_settings));
	std::shared_ptr<Application> ptr_application(new Application(*ptr_settings));
	if (App->FileLogRequired() == "Y")
	{
		std::shared_ptr<FIX::ThreadedSocketInitiator> initiator(new FIX::ThreadedSocketInitiator(*ptr_application, *ptr_storeFactory, *ptr_settings, *ptr_logFactory));
		initiator->start();
		::Sleep(1000);
		ptr_application->sendMarketDataRequest("ES123", "1",true);
		wait();
		initiator->stop();
	}
	else if (App->FileLogRequired() == "N")
	{
		std::shared_ptr<FIX::ThreadedSocketInitiator> initiator(new FIX::ThreadedSocketInitiator(*ptr_application, *ptr_storeFactory, *ptr_settings));
		initiator->start();
		::Sleep(1000);
		ptr_application->sendMarketDataRequest("ES123", "1",true);
		wait();
		initiator->stop();
	}	
	
    return 0;
  }
  catch ( std::exception & e )
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
}
