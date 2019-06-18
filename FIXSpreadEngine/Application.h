/* -*- C++ -*- */

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

#ifndef EXECUTOR_APPLICATION_H
#define EXECUTOR_APPLICATION_H

//#include "Manager.h"

#include <string>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <fstream>
#include <cstdarg>
#include <time.h>
#include <vector>



#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"

#include "boost\algorithm\string.hpp"
#include <boost\nowide\convert.hpp>


#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix\fix44\MarketDataSnapshotFullRefresh.h"


#include "Constant.h"


using namespace std;

using UserData = std::vector<std::string>;
using UserDataRecords = std::vector<UserData>;

static char		FIXUserStatus;

struct px
{
	std::string		_symbol;
	double			_bidpx;
	double			_offerpx;
};


class Application : public FIX::Application, public FIX::MessageCracker
{
public:
	FIX::SessionSettings sett;

private:
	std::vector<FIX::SessionID>				_appSessionID;
	
private:

	UINT									version = 0;
	wstring									mserver;
	UINT64									mlogin;
	wstring									mpassword;

public: 
	
  Application(FIX::SessionSettings&);
  ~Application();

  // Application overloads
  void onCreate( const FIX::SessionID& );
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& );
  void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon );
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );
  // MessageCracker overloads

  void onMessage(const FIX44::MarketDataSnapshotFullRefresh &message, const FIX::SessionID&);
  void sendMarketDataRequest(std::string,std::string,bool);

};

#endif
