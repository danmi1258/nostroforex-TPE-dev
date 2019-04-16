//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "PluginInstance.h"
//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
using namespace std;

MTPluginInfo ExtPluginInfo=
  {
   2.0,
   MTServerAPIVersion,
   L"Balance Event Plugin",
   L"AAATrade Ltd.",
   L"MetaTrader 5 Plugin for Balance Event POST Message Notification."
   };

MTPluginParam ExtPluginDefaults[] =
{
	{ MTPluginParam::TYPE_SYMBOLS, L"Symbols"               ,L"*" },
	{ MTPluginParam::TYPE_GROUPS,  L"Groups"                ,L"*,!demo*,!contest*,!manager*" },
	{ MTPluginParam::TYPE_FLOAT,   L"Monthly cost"          ,L"5" },
	{ MTPluginParam::TYPE_SYMBOLS, L"Currency"              ,L"USD" },
	{ MTPluginParam::TYPE_STRING,  L"Skip disabled users"   ,L"Yes" },
	{ MTPluginParam::TYPE_FLOAT,   L"Max deposit to charge" ,L"10000" },
	{ MTPluginParam::TYPE_FLOAT,   L"Max overturn to charge",L"100000" },
};
//+------------------------------------------------------------------+
//| DLL Main Function                                                |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HMODULE hModule,DWORD reason,LPVOID lpReserved)
  {

//---
   switch(reason)
     {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
         break;
     }
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Plugin About entry function                                      |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTServerAbout(MTPluginInfo& info)
  {
   info=ExtPluginInfo;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Plugin instance creation entry point                             |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTServerCreate(UINT apiversion,IMTServerPlugin **plugin)
  {
//--- check parameters
   if(!plugin) return(MT_RET_ERR_PARAMS);
//--- create plugin instance
   if(((*plugin)=new(std::nothrow) CPluginInstance())==NULL)
      return(MT_RET_ERR_MEM);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
