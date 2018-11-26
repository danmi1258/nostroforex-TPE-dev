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



#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"


#include "mt5managerapi\Manager\API\MT5APIManager.h"

#include "boost\algorithm\string.hpp"
#include <boost\nowide\convert.hpp>

#include "quickfix/fix44/NewOrderSingle.h"
#include "quickfix\fix44\MarketDataSnapshotFullRefresh.h"


#include "Constant.h"


using namespace std;

using UserData = std::vector<std::string>;
using UserDataRecords = std::vector<UserData>;

static char		FIXUserStatus;

// FIX Application run as server or acceptor.

class Application : public FIX::Application, public FIX::MessageCracker, 
					public IMTTickSink,public IMTManagerSink,
					public IMTUserSink, public IMTBookSink,public IMTConGroupSink
{
public:
	FIX::SessionSettings sett;
	// OnTick Event handler for new quote arrival.

private:
	std::map<std::string, std::string>						sym_MDReqID; // symbol to MDReqID. 
	std::map<std::string, bool>								sym_sub; // Symbol to subscription status.
	FIX::SessionID											appSessionID;

private:

	CMTManagerAPIFactory					m_factory;
	IMTManagerAPI							*manager = NULL;
	UINT									version = 0;
	MTAPIRES								res = MT_RET_OK_NONE;
	wstring									mserver;
	UINT64									mlogin;
	wstring									mpassword;
	MTAPISTR								mserverName;

public: 
	
  Application(string m_server,string m_login, string m_password, FIX::SessionSettings&);
  ~Application();
  void managerConnect();
  void serverReconnect();
  void managerStart();

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

  void onMessage(const FIX44::MarketDataRequest&, const FIX::SessionID&);
  void sendMarketDataRequestReject(std::string,std::string);

  // OnTick callback used to intercept the new quote arrival in the system.
  void  OnTick(LPCWSTR symbol, const MTTickShort&  tick);

};

#endif
