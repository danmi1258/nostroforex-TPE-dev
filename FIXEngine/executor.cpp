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


int main( int argc, char** argv )
{
	Application *app = new Application("settings.cfg", true);
	app->start();
	::Sleep(5000);
	std::string str;
	int i = 0;
	/*while (true)
	{		
		if (i == 10)
			app->sendMDRequest();
		std::cout << i << std::endl;
		i++;
		//std::cin >> str;
	}*/
	app->sendMDRequest();
	::Sleep(50000);
	app->stop();
}
