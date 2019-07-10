//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once

#define WINVER         0x0501           // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT   0x0501           // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINDOWS 0x0501           // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE      0x0600           // Change this to the appropriate value to target other versions of IE.

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#include <windows.h>
#include <limits.h>
//#include "..\Common\MT5APIServer.h"
//#include "..\Common\MT5APITools.h"
#include "mt5serverapi\Server\API\MT5APIServer.h"
#include "mt5serverapi\Common\MT5APITools.h"
#include <boost\nowide\convert.hpp>
#include <boost\algorithm\string.hpp>
//#include "clib\ctpl_stl.h"
#include "ctpl_stl.h"
#include "ServerPlugin.h"
#include <thread>

//+------------------------------------------------------------------+
