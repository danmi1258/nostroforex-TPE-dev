#pragma once
#include "stdafx.h"
#include "Constant.h"
#include "ClientInstrument.h"
#include <unordered_map>

class ManagerInterface : public IMTTickSink, public IMTManagerSink
{
public:
	ManagerInterface(LPCWSTR mserver, UINT64 mlogin, LPCWSTR mpassword) : _server(mserver), _login(mlogin), _password(mpassword)
	{}
	/*void assign(LPCWSTR mserver, UINT64 mlogin, LPCWSTR mpassword)
	{
		_server = mserver;
		_login = mlogin;
		_password = mpassword;
	}*/
	MTAPIRES init();
	MTAPIRES start();
	void stop();
	std::pair<bool, std::string> addSubscription(std::string clientID, std::string symbol, InstrumentDataPublisher::DataUpdateHandler mdHandler) 
	{
		return std::pair<bool, std::string>(std::make_pair(true, "BCHUSD"));
	}
	virtual void  OnTick(LPCWSTR symbol, const MTTickShort&  tick);
private:
	//--- Manager API
	CMTManagerAPIFactory		_factory;
	IMTManagerAPI				*_manager = NULL;
	UINT						_version = 0;
	MTAPIRES					_res = MT_RET_OK_NONE;

	//--- Server parameters
	LPCWSTR						_server = L"188.72.227.210:443";
	UINT64						_login = 1005;
	LPCWSTR						_password = L"trt487598";

	using InstrumentPublisherCont = std::unordered_map<std::string, std::shared_ptr<InstrumentDataPublisher>>;
	InstrumentPublisherCont _instrumentMDPubBySymbol;

};

MTAPIRES ManagerInterface::init()
{
	/***********************************************************/

	if ((_res = _factory.Initialize(L"API\\")) != MT_RET_OK)
	{
		wprintf_s(L"Loading manager API failed (%u)\n", _res);
	}
	//--- check Manager API version
	if ((_res = _factory.Version(_version)) != MT_RET_OK)
	{
		wprintf_s(L"Getting API version failed (%u)\n", _res);
	}
	wprintf_s(L"Manager API version %u has been loaded\n", _version);

	if (_version<MTManagerAPIVersion)
	{
		wprintf_s(L"Wrong Manager API version %u, version %u required\n", _version, MTManagerAPIVersion);
	}
	/***********************************************************/
	//--- creating manager interface
	if ((_res = _factory.CreateManager(MTManagerAPIVersion, &_manager)) != MT_RET_OK)
	{
		wprintf_s(L"Creating manager interface failed (%u)\n", _res);
	}
	if (_manager->Subscribe(this) != MT_RET_OK)
	{
	}
	if (_manager->TickSubscribe(this) != MT_RET_OK)
	{
	}
	/*if (connectManager() != MT_RET_OK)
	{
		return MT_RET_ERROR;
	}*/

	::Sleep(500);
	return MT_RET_OK;
}

MTAPIRES ManagerInterface::start()
{
	_res = _manager->Connect(_server, _login, _password, L"",
		IMTManagerAPI::PUMP_MODE_SYMBOLS | IMTManagerAPI::PUMP_MODE_FULL, MT5_TIMEOUT_CONNECT); //PUMP_MODE_FULL
	if (_res != MT_RET_OK)
	{
		wprintf_s(L"Connection failed (%u)\n", _res);
		return MT_RET_ERROR;
	}
	/*******Should be deleted******/
	std::shared_ptr<InstrumentDataPublisher> defaultObj(new InstrumentDataPublisher);
	std::pair<InstrumentPublisherCont::iterator, bool> retIns = _instrumentMDPubBySymbol.insert(std::make_pair("BTCUSD", defaultObj));
	
	using DataUpdateHandler = std::function<void(const MTTickShort &)>;
	DataUpdateHandler h;
	retIns.first->second->addHandler(h);
	/*******Should be deleted******/

	return MT_RET_OK;
}

void ManagerInterface::stop()
{
	_manager->Disconnect();
	_manager->Release();
	_manager = NULL;
	_factory.Shutdown();
}

void ManagerInterface::OnTick(LPCWSTR symbol, const MTTickShort&  tick)
{
	std::string targetSymbol = boost::nowide::narrow(symbol);

	auto it = _instrumentMDPubBySymbol.find(targetSymbol);
	if (it != _instrumentMDPubBySymbol.end())
	{
		it->second->handleDataUpdate(tick);
	}
}
