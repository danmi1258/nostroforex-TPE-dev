//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "StdAfx.h"
#include "PluginInstance.h"
#include <thread> 
#include <string>
#include <sstream>
#include <WinInet.h>
#include <mutex>
#include <fstream>>

#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")

std::mutex m1;

typedef std::unique_ptr<IMTRequest, MT5Deleter<IMTRequest>> MT5Request;
typedef std::unique_ptr<IMTDeal, MT5Deleter<IMTDeal>> MT5Deal;

std::vector<std::string> Split(std::string strToSplit, char delimeter)
{
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;
	while (std::getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

namespace aaa
{
	void processDealRequest(int threadid, IMTServerAPI * api, IMTDeal * deal, UINT64 _managerLogin, UINT64 targetlogin)
	{
		MT5Request req(api->TradeRequestCreate());
		req.get()->Clear();
		req.get()->Login(targetlogin);
		req.get()->Action(IMTRequest::TA_DEALER_POS_EXECUTE);
		if (deal->Action() % 2 == 0)
			req.get()->Type(IMTOrder::OP_BUY);
		else if (deal->Action() % 2 == 1)
			req.get()->Type(IMTOrder::OP_SELL);
		req.get()->Volume(deal->Volume());
		req.get()->Symbol(deal->Symbol());
		req.get()->Flags(IMTRequest::TA_FLAG_MARKET);
		req.get()->PriceOrder(deal->Price());
		req.get()->SourceLogin(_managerLogin);
		if (api->TradeRequest(req.get()) != MT_RET_OK)
		{
			api->LoggerOut(MTLogOK, L"Trade Request Send Fail.");
		}
		else
		{
			api->LoggerOut(MTLogOK, L"processDealRequest : Order(%d) Placed @ %f",req.get()->ResultOrder(), deal->Price());
		}
	}
}

//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CPluginInstance::CPluginInstance(void) : m_api(NULL)
  {
   ZeroMemory(&m_info,sizeof(m_info));
   //StartCapturing();
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CPluginInstance::~CPluginInstance(void)
  {
   Stop();
  }
//+------------------------------------------------------------------+
//| Plugin release function                                          |
//+------------------------------------------------------------------+
void CPluginInstance::Release(void)
  {
   delete this;
  }
//+------------------------------------------------------------------+
//| Plugin start notification function                               |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::Start(IMTServerAPI* api)
  {
   MTAPIRES retcode;
   _threadpool.resize(1);
//--- check
   if(!api) return(MT_RET_ERR_PARAMS);
//--- store API pointer
   m_api=api;
   m_api->LoggerOut(MTLogOK, L"ZZ.CPluginInstance::Start");
   IMTConCommon*  commonConf = m_api->CommonCreate();

   SERVER_ROOT = "C:\\MetaTrader 5 Platform\\MainTrade\\";

   LOG_FOLDER = SERVER_ROOT + "logs\\";
   LOG_ERRORS = LOG_FOLDER + "LOG_INFO";

   m_api->LoggerOut(MTLogOK, L"ZZ.SERVER_ROOT = [%s]", SERVER_ROOT.c_str());
   m_api->LoggerOut(MTLogOK, L"ZZ.LOG_WARNINGS = [%s]", LOG_WARNINGS.c_str());
   m_api->LoggerOut(MTLogOK, L"ZZ.LOG_ERRORS = [%s]", LOG_ERRORS.c_str());
   m_api->LoggerOut(MTLogOK, L"ZZ.commonConf->Name = [%s]", commonConf->Name());

   commonConf->Release();

//--- print plugin parameters
   //ParametersAdd();

//--- receive server information
   if((retcode=m_api->About(m_info))!=MT_RET_OK)
     m_api->LoggerOut(MTLogOK,L"Server info failed [%d]",retcode);
//--- subscribe plugin on deal events
   if((retcode=m_api->DealSubscribe(this))!=MT_RET_OK)
     m_api->LoggerOut(MTLogOK,L"Deal subscribe failed [%d]",retcode);
//--- subscribe plugin on symbol events
   if((retcode=m_api->SymbolSubscribe(this))!=MT_RET_OK)
     m_api->LoggerOut(MTLogOK,L"Symbol subscribe failed [%d]",retcode);
//--- subscribe plugin on account events
   if ((retcode = m_api->UserSubscribe(this)) != MT_RET_OK)
	   m_api->LoggerOut(MTLogOK, L"User subscribe failed [%d]", retcode);
   if ((retcode = m_api->TradeSubscribe(this)) != MT_RET_OK)
	   m_api->LoggerOut(MTLogOK, L"Trade subscribe failed [%d]", retcode);
   //--- subscribe plugin on account events
//--- print plugin parameters
   ParametersShow();
   m_api->LoggerOut(MTLogOK, L"ZZ.CPluginInstance::Start.ENDD");
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Plugin stop notification function                                |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::Stop(void)
  {
	_threadpool.stop(true);
//--- unsubscribe from all events
   if(m_api)
     {
      m_api->DealUnsubscribe(this);
      m_api->SymbolUnsubscribe(this);
	  m_api->UserUnsubscribe(this);
	  m_api->TradeUnsubscribe(this);
      //--- clear API 
	  m_api->LoggerOut(MTLogOK, L"Plugin Stop End");
      m_api=NULL;
	  
     }
//--- ok
   return(MT_RET_OK);
  }

void CPluginInstance::OnDealPerform(const IMTDeal * deal, IMTAccount* account, IMTPosition* position)
{
	m_api->LoggerOut(MTLogOK, L"OnDealAdd : START = %d",deal->Reason());
	if ((deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_CLIENT) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_EXPERT) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_DEALER) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_SL) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_TP) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_GATEWAY) ||
		(deal->Reason() == IMTDeal::EnDealReason::DEAL_REASON_MOBILE))
	{
		m_api->LoggerOut(MTLogOK, L"OnDealAdd : IF-Block");
		for (auto & i : _symbolconfig)
		{
			if (CMTStr::CheckGroupMask(boost::nowide::widen(i.get()->getSymbol()).c_str(), deal->Symbol()) == true)
			{
				m_api->LoggerOut(MTLogOK, L"OnDealAdd : SymbolMask=%s, DealSymbol=%s", boost::nowide::widen(i.get()->getSymbol()).c_str(), deal->Symbol());
				MT5User _user(m_api->UserCreate());
				if (MT_RET_OK != m_api->UserGet(deal->Login(), _user.get()))
					m_api->LoggerOut(MTLogOK, L"UserGet Failed.");

				auto & it1 = i.get()->searchGroupConfig(boost::nowide::narrow(_user.get()->Group()), deal->Login());
				if (it1.second == true)
				{
					MT5Deal _deal(m_api->DealCreate());
					if (_deal.get()->Assign(deal) != MT_RET_OK)
						m_api->LoggerOut(MTLogOK, L"Deal Assign Fail.");
					_threadpool.push(aaa::processDealRequest, std::move(m_api), std::move(_deal.get()), std::move(_managerLogin), std::move(it1.first));
				}
			}
		}
	}
	m_api->LoggerOut(MTLogOK, L"OnDealAdd : STOP");
}

//+------------------------------------------------------------------+
//| Show plugin parameters                                           |
//+------------------------------------------------------------------+
void CPluginInstance::ParametersShow(void)
  {
   IMTConPlugin* plugin=NULL;
   IMTConParam*  param =NULL;
   UINT          i;
//--- check API
   if(!m_api) return;
//--- create plugin config object
   if((plugin=m_api->PluginCreate())==NULL) return;
//--- receive current plugin description
   if(m_api->PluginCurrent(plugin)!=MT_RET_OK)
     {
      plugin->Release();
      return;
     }
//--- print plugin name
   m_api->LoggerOut(MTLogOK,L"ZZ.Plugin: %s",plugin->Name());
//--- create parameters object
   if((param=m_api->PluginParamCreate())!=NULL)
     {
      m_api->LoggerOut(MTLogOK,L"ZZ.Parameters: ");
      //--- enumerate parameters
	  for (i = 0; plugin->ParameterNext(i, param) == MT_RET_OK; i++)
	  {
		  m_api->LoggerOut(MTLogOK, L"Name: %s, Value: %s", param->Name(), param->Value());

		  if (CMTStr::Compare(L"ManagerLogin", param->Name()) == 0)
		  {
			  _managerLogin = param->ValueInt();
			  m_api->LoggerOut(MTLogOK, L"ManagerLogin =[%d]", _managerLogin);
			  continue;
		  }

		  // symbolmask(0)|groupmask(1) ==> loginmask(0)|targetaccount(1)

		  std::vector<std::string> _paramName = Split(boost::nowide::narrow(param->Name()), '|');
		  std::vector<std::string> _paramValue = Split(boost::nowide::narrow(param->Value()), '|');
		  bool retValue;

		  auto i = _symbolIndexBySymbolmask.find(_paramName[0]);
		  if (i != _symbolIndexBySymbolmask.end())
		  {
			  auto & it = _symbolconfig.begin();
			  std::advance(it, i->second);
			  // symbol settings exists.
			  retValue = it->get()->insertSymbolConfig(_paramName[1], _paramValue[0], std::stoul(_paramValue[1]));
			  if (!retValue)
			  {
				  // Insertion failed due to setting mismatch.
				  m_api->LoggerOut(MTLogOK, L"Setting Failed = %s : %s", param->Name(), param->Value());
				  //param->Release();
				  continue;
			  }
		  }
		  else
		  {
			  // symbol settings not exists.
			  size_t index = _symbolconfig.size();
			  std::unique_ptr<SymbolConfig> symConfig(new SymbolConfig(_paramName[0],m_api));
			  retValue = symConfig.get()->insertSymbolConfig(_paramName[1], _paramValue[0], std::stoul(_paramValue[1]));
			  if (!retValue)
			  {
				  // Insertion failed due to setting mismatch.
				  m_api->LoggerOut(MTLogOK, L"Setting Failed = %s : %s", param->Name(), param->Value());
				  //param->Release();
				  continue;
			  }
			  _symbolconfig.push_back(std::move(symConfig));
			  _symbolIndexBySymbolmask[_paramName[0]] = index;
		  }
	  }
	  for (auto & x : _symbolconfig)
	  {
		  m_api->LoggerOut(MTLogOK, L"Print configurations..");
		  x.get()->print(m_api);
	  }
      //--- release parameter
      param->Release();
     }
//--- release plugin config
   plugin->Release();
  }
//+------------------------------------------------------------------+

