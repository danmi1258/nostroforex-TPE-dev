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

#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")

std::mutex m1;


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
//--- check
   if(!api) return(MT_RET_ERR_PARAMS);
//--- store API pointer
   m_api=api;
   m_api->LoggerOut(MTLogOK, L"ZZ.CPluginInstance::Start");
   IMTConCommon*  commonConf = m_api->CommonCreate();
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
	   m_api->LoggerOut(MTLogOK, L"Symbol subscribe failed [%d]", retcode);
//--- print plugin parameters
   ParametersShow();
   m_api->LoggerOut(MTLogOK, L"ZZ.CPluginInstance::Start.ENDD");

   ws_server.run(25500);

   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Plugin stop notification function                                |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::Stop(void)
  {
	typeOfDeal = 2;
//--- unsubscribe from all events
   if(m_api)
     {
      m_api->DealUnsubscribe(this);
      m_api->SymbolUnsubscribe(this);
	  m_api->UserUnsubscribe(this);
      //--- clear API 
	  m_api->LoggerOut(MTLogOK, L"Plugin Stop End");
      m_api=NULL;
	  
     }
//--- ok
   return(MT_RET_OK);
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
		  //--- get a parameter by index.
		  if (CMTStr::Compare(L"URLHOST", param->Name()) == 0) 
		  {
			  _UrlHost = boost::nowide::narrow(param->Value());
			  continue;
		  }
		  else if (CMTStr::Compare(L"URLPATH", param->Name()) == 0) 
		  {
			  _UrlPath = boost::nowide::narrow(param->Value());
			  continue;
		  }
		  else
		  {
			  //--- This block is for group mask.
			  group_map[boost::nowide::narrow(param->Name())] = Split(boost::nowide::narrow(param->Value()), ',');
			  m_api->LoggerOut(MTLogOK, L"[%s] = [%s]", param->Name(), param->Value());
		  }
	  }
      //--- release parameter
      param->Release();
     }
//--- release plugin config
   plugin->Release();
  }

//+------------------------------------------------------------------+



