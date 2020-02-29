#pragma once
#ifndef BITSTAMP_H
#define BITSTAMP_H

//--- QuickFIX Library include
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"

#include "quickfix/FileStore.h"
#include "quickfix/SocketInitiator.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/FileLog.h"
#include "quickfix/fix44/MarketDataSnapshotFullRefresh.h"
#include "quickfix/fix44/MarketDataRequest.h"

#include "Constant.h"


class Bitstamp :public FIX::Application, public FIX::MessageCracker
{
public:
	Bitstamp() {}
	Bitstamp(std::string file,bool log) :_fix_settings_file_path(file), _logRequired(log)
	{}
	virtual ~Bitstamp() {};
	virtual void onTick(Tick & t) {};
	void sendMDRequest();
	void init(std::string file, bool log);
	void start();
	void stop();
private:

	//--- Private data members.
	std::string										_fix_settings_file_path;
	bool											_logRequired;
	FIX::SessionSettings						*_settings;
	FIX::FileStoreFactory					*_storeFactory;
	FIX::FileLogFactory						*_logFactory;
	FIX::SocketInitiator						*_connection;
	FIX::SessionID							_sessionID;

	// Application overloads
	void onCreate(const FIX::SessionID& sesseionID) {
		_sessionID = sesseionID;
	};
	void onLogon(const FIX::SessionID& sessionID) {};
	void onLogout(const FIX::SessionID& sessionID) {};
	void toAdmin(FIX::Message&, const FIX::SessionID&);
	void toApp(FIX::Message&, const FIX::SessionID&)
		throw(FIX::DoNotSend) {};
	void fromAdmin(const FIX::Message&, const FIX::SessionID&)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) {};
	void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);

	void processMDSnapshotFullRefresh(const FIX44::MarketDataSnapshotFullRefresh & message, const FIX::SessionID & sessionID);
};






/*****************************************************/
/*	Function: init.							         */
/*  Parameter: void.                                 */
/*  Description: initilize fix settings.             */
/*****************************************************/
void Bitstamp::init(std::string file, bool log)
{
	_fix_settings_file_path = file;
	_logRequired = log;
	try
	{
		_settings = new FIX::SessionSettings("settings.cfg");
		_storeFactory = new FIX::FileStoreFactory(*_settings);
		_logFactory = new FIX::FileLogFactory(*_settings);
		if (_logRequired == true)
			_connection = new FIX::SocketInitiator(*this, *_storeFactory, *_settings, *_logFactory);
		else
			_connection = new FIX::SocketInitiator(*this, *_storeFactory, *_settings);
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

}

/*****************************************************/
/*	Function: start.						         */
/*  Parameter: void.                                 */
/*  Description: start fix session.                  */
/*****************************************************/
void Bitstamp::start()
{
	try
	{
		_connection->start();
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}

/*****************************************************/
/*	Function: start.						         */
/*  Parameter: void.                                 */
/*  Description: start fix session.                  */
/*****************************************************/

void Bitstamp::stop()
{
	try
	{
		_connection->stop();
	}
	catch (std::exception e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}


void Bitstamp::toAdmin(FIX::Message & message, const FIX::SessionID & sessionID)
{
	if (message.getHeader().getField(35) == FIX::MsgType_Logon)
	{
		if (_settings->get(sessionID).has("Username"))
		{
			FIX::Username username;
			username = _settings->get(sessionID).getString("Username");
			message.setField(username);
		}
		if (_settings->get(sessionID).has("Password"))
		{
			FIX::Password password;
			password = _settings->get(sessionID).getString("Password");
			message.setField(password);
		}
		if (_settings->get(sessionID).has("TargetSubID"))
		{
			FIX::TargetSubID targetsubid;
			targetsubid = _settings->get(sessionID).getString("TargetSubID");
			message.setField(targetsubid);
		}
		if (_settings->get(sessionID).has("SenderSubID"))
		{
			FIX::SenderSubID sendersubid;
			sendersubid = _settings->get(sessionID).getString("SenderSubID");
			message.setField(sendersubid);
		}
		message.setField(FIX::ResetSeqNumFlag(FIX::ResetSeqNumFlag_YES));
	}
}

inline void Bitstamp::fromApp(const FIX::Message & message, const FIX::SessionID & sessionID) throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType)
{
	if (message.getHeader().getField(35) == FIX::MsgType_MarketDataSnapshotFullRefresh)
	{
		FIX44::MarketDataSnapshotFullRefresh msg(message);
		processMDSnapshotFullRefresh(msg, sessionID);
	}
}

void Bitstamp::processMDSnapshotFullRefresh(const FIX44::MarketDataSnapshotFullRefresh & message, const FIX::SessionID & sessionID)
{

	double askpx = 0.0;
	double bidpx = 0.0;
	double asksize = 0.0;
	double bidsize = 0.0;

	FIX::MDReqID mdreqid; // tag = 262
	FIX::NoMDEntries nomdentries; // tag = 268
	FIX::Symbol symbol; // tag = 55
	FIX::MDEntryType mdentrytype; // tag = 269
	FIX::MDEntryPx mdentrypx; // tag = 270
	FIX::MDEntrySize mdentrysize; // tag 271
	FIX::Group group = FIX44::MarketDataSnapshotFullRefresh::NoMDEntries(); 

	message.getFieldIfSet(mdreqid);
	message.getFieldIfSet(symbol);
	message.getFieldIfSet(nomdentries);
	int list = nomdentries.getValue();
	for (int i = 1; i <= list; i++)
	{
		message.getGroup(i, group);
		group.getFieldIfSet(mdentrytype);
		group.getFieldIfSet(mdentrypx);
		//group.getFieldIfSet(mdentrysize);
		if (mdentrytype.getValue() == FIX::MDEntryType_BID)
		{
			bidpx = mdentrypx.getValue();
			//bidsize = mdentrysize.getValue();
		}
		else if (mdentrytype.getValue() == FIX::MDEntryType_OFFER)
		{
			askpx = mdentrypx.getValue();
			//asksize = mdentrysize.getValue();
		}
		group.clear();
	}
	Tick t(symbol.getValue(), bidpx, askpx,bidsize,asksize, "bitstamp");
	onTick(t);
}

void Bitstamp::sendMDRequest()
{
	FIX::MDReqID mdReqID("123");																			//--- MDReqID tag 262.
	FIX::SubscriptionRequestType subType(FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES);				//--- SubscriptionRequestType tag 263.
	FIX::MarketDepth marketDepth(FIX::MDBookType_TOP_OF_BOOK);												//--- MarketDepth tag 264.
	FIX44::MarketDataRequest message(mdReqID, subType, marketDepth);

	//--- Market Data Entry Types Bid and Offer.
	FIX44::MarketDataRequest::NoMDEntryTypes marketDataEntryGroup;					//--- NoMDEntryTypes tag 267.

	FIX::MDEntryType mdEntryType_B(FIX::MDEntryType_BID);								//--- MDEntryType tag 269.
	marketDataEntryGroup.set(mdEntryType_B);
	message.addGroup(marketDataEntryGroup);

	FIX::MDEntryType mdEntryType_O(FIX::MDEntryType_OFFER);							//--- MDEntryType tag 269.
	marketDataEntryGroup.set(mdEntryType_O);
	message.addGroup(marketDataEntryGroup);

	//--- Market Data Symbol Add.
	FIX44::MarketDataRequest::NoRelatedSym symbolGroup;								//--- NoRelatedSym tag 146.
	symbolGroup.set(FIX::Symbol("1"));
	message.addGroup(symbolGroup);
	FIX::Session::sendToTarget(message,_sessionID);
}



#endif