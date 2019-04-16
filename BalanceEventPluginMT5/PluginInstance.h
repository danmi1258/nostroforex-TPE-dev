//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "WebSocketServer.h"
#define URLHOST 0
#define URLPATH 1

//+------------------------------------------------------------------+
//| Plugin instance class                                            |
//+------------------------------------------------------------------+
class CPluginInstance : public IMTServerPlugin,
                        public IMTConSymbolSink,
                        public IMTDealSink,
						public IMTUserSink
  {
private:
   MTServerInfo      m_info;
   IMTServerAPI*     m_api;
   const IMTDeal*	 n_deal;
   IMTAccount		 *user;
   CMTStr256         url_host;
   CMTStr256         url_domain;
   INT				 typeOfDeal=0;
   char				 UrlHost[50];
   char				 UrlPath[50];
   std::string		 _UrlHost;
   std::string		 _UrlPath;
   bool				 isGroupMaskFound = false;
   std::map<std::string, std::vector<std::string>> group_map;

public:
                     CPluginInstance(void);
   virtual          ~CPluginInstance(void);
   //--- IMTServerPlugin interface implementation
   virtual void      Release(void);
   virtual MTAPIRES  Start(IMTServerAPI* server);
   virtual MTAPIRES  Stop(void);

private:
   void              ParametersShow();
   WebSocketServer	 ws_server;
  };

//+------------------------------------------------------------------+

