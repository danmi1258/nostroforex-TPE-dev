//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "Config.h"
//+------------------------------------------------------------------+
//| Plugin instance class                                            |
//+------------------------------------------------------------------+
class CPluginInstance : public IMTServerPlugin,
						public IMTTradeSink,
                        public IMTConSymbolSink,
                        public IMTDealSink,
						public IMTUserSink,
						public IMTEndOfDaySink
  {
private:
   MTServerInfo			m_info;
   IMTServerAPI*		m_api;
   const IMTDeal*		n_deal;
   IMTAccount			*user;
   ctpl::thread_pool	_threadpool;
   UINT64				_managerLogin;

   std::list<std::unique_ptr<SymbolConfig>> _symbolconfig;
   //std::unordered_map<std::string, std::list<std::unique_ptr<SymbolConfig>>::iterator> _symbolIndexBySymbolmask;
   std::unordered_map<std::string, size_t> _symbolIndexBySymbolmask;

public:
                     CPluginInstance(void);
   virtual          ~CPluginInstance(void);
   //--- IMTServerPlugin interface implementation
   virtual void      Release(void);
   virtual MTAPIRES  Start(IMTServerAPI* server);
   virtual MTAPIRES  Stop(void);
   virtual void OnDealPerform(const IMTDeal* deal, IMTAccount* account, IMTPosition* position);

private:
   void              ParametersShow();
  };


namespace aaa
{
	void processDealRequest(int threadid, IMTServerAPI* api, IMTDeal * deal, UINT64 _managerLogin, UINT64 targetlogin);
}
//+------------------------------------------------------------------+

