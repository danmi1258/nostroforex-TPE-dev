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

#include <stdlib.h>
#include <wchar.h>
#include <sstream>
#include <cstring>

#include "Application.h"
#include "quickfix/Session.h"



#include "quickfix/fix44/ExecutionReport.h"
#include "quickfix/fix44/MarketDataRequest.h"
#include "quickfix/fix44/MarketDataRequestReject.h"
#include "quickfix/fix44/MassQuoteAcknowledgement.h"
#include "quickfix/fix44/MassQuote.h"




Application::Application(FIX::SessionSettings& setting)
{
	sett = setting;
}
Application::~Application()
{
}


void Application::onCreate( const FIX::SessionID& sessionID ) 
{
	//appSessionID = sessionID;
	_appSessionID.push_back(sessionID);
}
void Application::onLogon( const FIX::SessionID& sessionID ) 
{
}
void Application::onLogout( const FIX::SessionID& sessionID ) 
{	
}

void Application::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) 
{
	if (message.getHeader().getField(35) == FIX::MsgType_Logon)
	{
		FIX::Username username;
		FIX::Password password;
		FIX::SenderSubID sendersubid; // tag 50
		FIX::TargetSubID targetsubid; // tag 57
		FIX::ResetSeqNumFlag resetseqnumflag;

		if (sett.get(sessionID).has("Username"))
		{
			message.setField(FIX::Username(sett.get(sessionID).getString("Username")));
		}
		if (sett.get(sessionID).has("Password"))
		{
			message.setField(FIX::Password(sett.get(sessionID).getString("Password")));
		}
		if (sett.get(sessionID).has("SenderSubID"))
		{
			message.setField(FIX::SenderSubID(sett.get(sessionID).getString("SenderSubID")));
		}
		if (sett.get(sessionID).has("TargetSubID"))
		{
			message.setField(FIX::TargetSubID(sett.get(sessionID).getString("TargetSubID")));
		}
		if (sett.get(sessionID).has("ResetSeqNumFlag"))
		{
			if (sett.get(sessionID).getString("ResetSeqNumFlag") == "Y")
			message.setField(FIX::ResetSeqNumFlag(FIX::ResetSeqNumFlag_YES));
		}
	}
}
void Application::toApp( FIX::Message& message,
                         const FIX::SessionID& sessionID )
throw( FIX::DoNotSend ) 
{
}

void Application::fromAdmin( const FIX::Message& message,
                             const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) 
{
	// Call MessageCracker.crack method to handle the message by one of our 
	// overloaded onMessage methods below
	crack(message, sessionID);	

}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{ 
	crack( message, sessionID ); 	
}

void Application::onMessage(const FIX44::MarketDataSnapshotFullRefresh &message, const FIX::SessionID &sessionID)
{
	px price;
	FIX::Symbol sym;
	message.get(sym);
	price._symbol = sym.getValue();
	FIX::NoMDEntries mdentries;
	message.get(mdentries);
	FIX44::MarketDataSnapshotFullRefresh::NoMDEntries mdgroup;
	int _mdGrCount = 1;
	while (_mdGrCount <= mdentries.getValue())
	{
		message.getGroup(_mdGrCount, mdgroup);
		FIX::MDEntryType mdentrytype;
		mdgroup.get(mdentrytype);
		FIX::MDEntryPx mdentrypx;
		mdgroup.get(mdentrypx);
		if (mdentrytype.getValue() == FIX::MDEntryType_BID)
			price._bidpx = mdentrypx.getValue();
		else if (mdentrytype.getValue() == FIX::MDEntryType_OFFER)
			price._offerpx = mdentrypx.getValue();
		_mdGrCount++;
	}
	std::cout << "\t" << price._symbol << "\t" << price._bidpx << "\t" << price._offerpx << "\t" << (price._offerpx - price._bidpx)*100000 << "\t" << sessionID.getSenderCompID().getValue() << std::endl;

}

void Application::sendMarketDataRequest(std::string MDReqID, std::string Symbol, bool isSubscribe)
{
	std::shared_ptr<FIX44::MarketDataRequest> message(new FIX44::MarketDataRequest());

	//--- Send Rejection with the MD request ID.
	message->set(FIX::MDReqID(MDReqID));
	//--- Reject reason will be send as Text (tag 58).
	if (isSubscribe == true)
		message->set(FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES)); //FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST
	else if (isSubscribe == false)
		message->set(FIX::SubscriptionRequestType(FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST));
	message->set(FIX::MarketDepth(1));
	message->set(FIX::MDUpdateType(FIX::MDUpdateType_INCREMENTAL_REFRESH));
	
	//--- repeating group contains mdentrytypes
	FIX44::MarketDataRequest::NoMDEntryTypes nomdentrytypes;
	nomdentrytypes.set(FIX::MDEntryType(FIX::MDEntryType_BID));
	nomdentrytypes.set(FIX::MDEntryType(FIX::MDEntryType_OFFER));
	message->addGroup(nomdentrytypes);
	
	//--- repeating group contains the symbol list.
	FIX44::MarketDataRequest::NoRelatedSym norelatedsym;
	norelatedsym.set(FIX::Symbol(Symbol));
	message->addGroup(norelatedsym);
	

	//--- Sending Market Data Request Reject (MsgType = Y).
	for (FIX::SessionID i : _appSessionID)
	FIX::Session::sendToTarget(*message, i);

	//--- Clear the message.
	message->clear();
}





