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

std::map<int, std::string> returnCode = { { 10001,"Request is on the way." },
{ 10002, "Request accepted." },
{ 10003, "Request processed." },
{ 10004, "Requote in response to the request." },
{ 10005, "Prices in response to the request." },
{ 10006, "Request rejected." },
{ 10007, "Request canceled." },
{ 10008, "An order placed as a result of the request." },
{ 10009, "Request fulfilled." },
{ 10010, "Request partially fulfilled." },
{ 10011, "Common error of request." },
{ 10012, "Request timed out." },
{ 10013, "Invalid request." },
{ 10014, "Invalid volume." },
{ 10015, "Invalid price." },
{ 10016, "Wrong stop levels or price." },
{ 10017, "Trade is disabled." },
{ 10018, "Market is closed." },
{ 10019, "Not enough money." },
{ 10020, "Price has changed." },
{ 10021, "No price." },
{ 10022, "Invalid order expiration." },
{ 10023, "Order has been changed." },
{ 10024, "Too many trade requests." },
{ 10025, "Request does not contain changes." },
{ 10026, "Autotrading disabled on the server." },
{ 10027, "Autotrading disabled on the client side." },
{ 10028, "Request blocked by the dealer." },
{ 10029, "Modification failed due to order or position being close to market." },
{ 10030, "Fill mode is not supported." },
{ 10031, "No connection." },
{ 10032, "Allowed only for real accounts." },
{ 10033, "Reached the limit on the number of orders." },
{ 10034, "Reached the volume limit." },
{ 10035, "Invalid or prohibited order type." },
{ 10036, "Position is already closed." },
{ 10037, "Used for internal purposes." },
{ 10038, "Volume to be closed exceeds the current volume of the position." },
{ 10039, "Order to close the position already exists." },
{ 10040, "The number of open positions simultaneously present on an account for a particular group." },
{ 10041, "Request rejected, order canceled." },
{ 10042, "Only long positions are allowed." },
{ 10043, "Only short positions are allowed." },
{ 10044, "Only position closing is allowed." } };

string ConvertStringToLPWSTR(LPCWSTR inParam)
{
	wstring inParamWS(inParam);
	string res(inParamWS.begin(), inParamWS.end());
	return res;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


Application::Application(string m_server, string m_login, string m_password, FIX::SessionSettings& setting)
{
	sett = setting;

	mserver = s2ws(m_server);
	mpassword = s2ws(m_password);
	mlogin = stoi(m_login);
	managerConnect();

}
Application::~Application()
{
	//--- disconnect
	manager->Disconnect();
	//--- shutdown
	manager->Release(); manager = NULL;
	m_factory.Shutdown();
}

void Application::managerConnect()
{
	if ((m_factory.Initialize(L"include\\API\\Manager\\")) != MT_RET_OK)
	{
		wprintf_s(L"Loading manager API failed (%u)\n", res);
	}
	//--- check Manager API version
	if ((m_factory.Version(version)) != MT_RET_OK)
	{
		wprintf_s(L"Getting API version failed (%u)\n", res);
	}
	wprintf_s(L"Manager API version %u has been loaded\n", version);
	if (version<MTManagerAPIVersion)
	{
		wprintf_s(L"Wrong Manager API version %u, version %u required\n", version, MTManagerAPIVersion);
	}
	//--- creating manager interface
	if ((res = m_factory.CreateManager(MTManagerAPIVersion, &manager)) != MT_RET_OK)
	{
		wprintf_s(L"Creating manager interface failed (%u)\n", res);
	}
	//res = manager->DealSubscribe(this);


	//--- subscribe for notifications
	if (manager->Subscribe(this) != MT_RET_OK)
	{
	}
	//--- subscribe for notifications
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
	res = manager->Connect(mserver.c_str(), mlogin, mpassword.c_str(), L"",
		IMTManagerAPI::PUMP_MODE_SYMBOLS |
		IMTManagerAPI::PUMP_MODE_GROUPS |
		IMTManagerAPI::PUMP_MODE_ORDERS |
		IMTManagerAPI::PUMP_MODE_USERS, MT5_TIMEOUT_CONNECT);
	if (res != MT_RET_OK)
	{
		wprintf_s(L"Connection failed (%u)\n", res);
		serverReconnect();
	}
	// save server name. mserverName
	MTAPIRES retcode;
	retcode = manager->NetworkServer(mserverName);
	if (retcode != MT_RET_OK)
	{
		managerConnect();
		managerStart();
	}
}

void Application::serverReconnect()
{
	//--- get server name and start network rescan
	MTAPISTR server_name;
	if (manager->NetworkServer(server_name) == MT_RET_OK && manager->NetworkRescan(0, 10000) == MT_RET_OK)
	{
		wprintf_s(L"Reconnecting due better access point is available\n");
		//--- disconnect
		manager->Disconnect();
		//--- connect again to server by name
		res = manager->Connect(server_name, mlogin, mpassword.c_str(), L"", 0, MT5_TIMEOUT_CONNECT);
		if (res != MT_RET_OK)
		{
			wprintf_s(L"Connection failed (%u)\n", res);
		}
	}
}

void Application::managerStart()
{
	res = manager->SelectedAddAll();
}

void Application::onCreate( const FIX::SessionID& sessionID ) 
{
	appSessionID = sessionID;
}
void Application::onLogon( const FIX::SessionID& sessionID ) 
{
	managerStart();
	manager->SelectedAddAll();
	sym_sub.clear(); // Clear all the subscribe symbol. Assume no symbol is subscribed.
}
void Application::onLogout( const FIX::SessionID& sessionID ) 
{	
		sym_sub.clear(); // Clear all the symbol as unsubscription.
		std::map<std::string, std::string>::iterator it;
		int size = sym_MDReqID.size();
		for (it = sym_MDReqID.begin(); size > 0; size--)
			sym_MDReqID.erase(it++);

		std::cout << "FIX Users Logout Successful" << std::endl;
		FIXUserStatus = '0';
}

void Application::toAdmin( FIX::Message& message,
                           const FIX::SessionID& sessionID ) 
{
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

	if (message.getHeader().getField(35) == FIX::MsgType_Logon)
	{
		bool userCheck = false;
		FIX::Username username;
		FIX::Password password;
		FIX::SenderCompID sendercompid;
		message.getField(username);
		message.getField(password);
		message.getHeader().getField(sendercompid);

		FIX::Username _username;
		FIX::Password _password;
		if(sett.get(appSessionID).has("Username"))
		{ 
			_username = sett.get(appSessionID).getString("Username");
		}
		if (sett.get(appSessionID).has("Password"))
		{
			_password = sett.get(appSessionID).getString("Password");
		}

		if (boost::iequals(username.getValue(), _username.getString()))
		{
			// Username match with fix username.
			if (boost::iequals(password.getValue(), _password.getString()))
			{
				// Password match with fix password.
				userCheck = true;
			}
		}
		
		if (userCheck)
			std::cout << "FIX Users Login Successful" << std::endl;
		else
		{
			std::cout << "FIX Users Login Unsuccessful" << std::endl;
			throw FIX::RejectLogon("Login failed! login creadential do not match with our records.");
		}

	}
}

void Application::fromApp( const FIX::Message& message,
                           const FIX::SessionID& sessionID )
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{ 
	crack( message, sessionID ); 	
}

void Application::onMessage(const FIX44::MarketDataRequest &message, const FIX::SessionID &sessionID)
{
	// Make an entry into the map with Symbol and MDReqID(262).
	// Subscribe symbol for book.
	// Check tag 263 SubscriptionRequestType is 1 then subscribe, if 2 then unsubscribe.

	FIX::Symbol symbol;
	
	
	FIX44::MarketDataRequest::NoRelatedSym symgroup;
	int noReqSym = 0;
	
	bool sendQuote = true;
	bool isUnsub = false;

	IMTBookSink sink;// = { 0 };	

	//--- get Subscription Request Type tag = 263.
	FIX::SubscriptionRequestType subReqType;
	message.get(subReqType);
	//--- get MD request ID tag = 262.
	FIX::MDReqID mdreqid;
	message.get(mdreqid);
	//--- get market depth tag = 264.
	FIX::MarketDepth marketdepth;
	message.get(marketdepth);
	//--- get No Related Sym tag = 146. It use to be 1 always.
	FIX::NoRelatedSym norelatedsym;
	message.get(norelatedsym);
	noReqSym = norelatedsym.getValue();
	
	
	//--- get the symbol from MD request.
	int _symCount = 1;
	while (_symCount <= noReqSym)
	{		
		message.getGroup(_symCount, symgroup);
		symgroup.get(symbol);
		std::string sym = symbol.getString();
		std::wstring sym1 = s2ws(sym);
		std::shared_ptr<IMTConSymbol*> sym_sett(new IMTConSymbol*());

		*sym_sett = manager->SymbolCreate();

		MTAPIRES ret1;
		bool isSymbolExist = false;
		if (MT_RET_OK == manager->SymbolGet(sym1.c_str(), *sym_sett))
			isSymbolExist = true;
		
		if (FIX::SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES == subReqType.getValue())
		{
			sym_MDReqID[sym] = mdreqid.getValue(); // map of symbol to MDReqID.
			//--- symbol request for subscription.
			std::map<std::string, std::string>::iterator it;
			std::map<std::string, bool>::iterator itsub1;
			if (isSymbolExist == true) // If symbol is eligible then subscription.
			{
				//--- requested symbol is eligible for price feed.
				//--- now check symbol is already subscribed or not.
				itsub1 = sym_sub.find(sym); // map of symbol to subscription status.
				if (itsub1 != sym_sub.end()) // Subscribe Symbol when it present in map.
				{
					if (itsub1->second)
					{
						//--- symbol already subscribed. send MD request reject. (MsgType = Y)
						sendMarketDataRequestReject(mdreqid.getValue(), "Symbol already subscribed.");
						sendQuote = false;
						break;
					} // else subscribe the symbol. Not required now.
				}
				else //--- Send Market Data Request Reject when Symbol is not found in the map.
					sym_sub[sym] = TRUE; // Symbol Subscribe True.
			}
			else if (isSymbolExist == false)
			{
				//--- symbol is not eligible.
				try
				{
					//--- send MD request reject. (MsgType = Y)
					sendMarketDataRequestReject(mdreqid.getValue(), "Not eligible for Price Feed."); // Need to write sendQuote.
					sendQuote = false;
					break;
				}
				catch (...)
				{
					return;
				}
				return;
			}			
		}
		else if (FIX::SubscriptionRequestType_DISABLE_PREVIOUS_SNAPSHOT_PLUS_UPDATE_REQUEST == subReqType.getValue())
		{
			//sym_MDReqID[sym] = mdreqid.getValue(); // map of symbol to MDReqID.
			auto md_it = sym_MDReqID.find(sym);
			if (md_it != sym_MDReqID.end())
			{
				sym_MDReqID.erase(md_it);
			}
			//--- symbol request for unsubscription.
			std::map<std::string,bool>::iterator itsub;
			
			if (isSymbolExist == true)
			{
				//--- requested symbol is eligible according to MT5.
				//--- check symbol is subscribed before or not.
				itsub = sym_sub.find(sym);
				if (itsub != sym_sub.end()) 
				{
					//--- symbol is subscribed before and will unsubscribed here.
					sym_sub.erase(itsub);
				}
				else // Send Market Data Request Reject when Symbol is not found in the map.
				{
					//--- symbol is not subscribed before. Send MD request reject (MsgType = Y)
					sendMarketDataRequestReject(mdreqid.getValue(), "Symbol not subscribed yet.");
					sendQuote = false;
					break; // Break the loop because noReqSym will be always 1.
				}
			}
			else if (isSymbolExist == false)
			{
				//--- requested symbol is not eligible according to MT5 settings.
				try
				{
					//--- send MD request reject. (MsgType = Y)
					sendMarketDataRequestReject(mdreqid.getValue(), "Not eligible for Price Feed.");
					sendQuote = false;
					break; // Break the loop because noReqSym will be always 1.
				}
				catch (...)
				{
					return;
				}
				return;
			}
			sendQuote = false;
		}
		_symCount++;
	}
}

void Application::sendMarketDataRequestReject(std::string MDReqID, std::string Text)
{
	std::shared_ptr<FIX44::MarketDataRequestReject> message(new FIX44::MarketDataRequestReject());

	//--- Send Rejection with the MD request ID.
	message->set(FIX::MDReqID(MDReqID));
	//--- Reject reason will be send as Text (tag 58).
	message->set(FIX::Text(Text));

	//--- Sending Market Data Request Reject (MsgType = Y).
	FIX::Session::sendToTarget(*message, appSessionID);

	//--- Clear the message.
	message->clear();
}




void Application::OnTick(LPCWSTR symbol, const MTTickShort & tick)
{
	//--- Subscribed symbol map is empty then ignore.
	if (sym_MDReqID.size() > 0)
	{
		//--- search available symbol is in the map.
		auto it = sym_MDReqID.find(boost::nowide::narrow(symbol));
		if (it != sym_MDReqID.end())
		{
			//--- Symbol found in the map.
			std::string mktReqID = it->second;
			//--- MD snapshot message.
			std::shared_ptr<FIX44::MarketDataSnapshotFullRefresh> message(new FIX44::MarketDataSnapshotFullRefresh());
			//--- set MDReqID from symbol map.
			message->set(FIX::MDReqID(it->second));
			//--- set available symbol in fix message.
			message->set(FIX::Symbol(boost::nowide::narrow(symbol)));
			//--- create MDEntries group.
			FIX44::MarketDataSnapshotFullRefresh::NoMDEntries mdentries;
			//--- set MD Entry Type as BID.
			mdentries.set(FIX::MDEntryType(FIX::MDEntryType_BID));
			//--- set MD bid price from tick.
			mdentries.set(FIX::MDEntryPx(tick.bid));
			//--- add group to fix message.
			message->addGroup(mdentries);
			mdentries.clear();
			//--- set MD Entry Type as Offer.
			mdentries.set(FIX::MDEntryType(FIX::MDEntryType_OFFER));
			//--- set MD offer price from available tick.
			mdentries.set(FIX::MDEntryPx(tick.ask));
			//--- add group to fix message.
			message->addGroup(mdentries);
			//--- send fix message to target.
			FIX::Session::sendToTarget(*message, appSessionID);
			message->clear();
		}
	}
	// If found get the market request id and pass sym and mkreqid to send ticks.
	// construct FIX message and send it to target.
}




