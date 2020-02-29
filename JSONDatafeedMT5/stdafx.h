//+------------------------------------------------------------------+
//|                                                  Simple Manager  |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//--- Windows versions
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
//--- headers
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <wchar.h>
#include <Winsock2.h>

#include <boost\nowide\convert.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>
//--- API
//#include "..\..\API\MT5APIManager.h"
//#include "API\MT5APIManager.h"
#include "mt5managerapi\Manager\API\MT5APIManager.h"
//+------------------------------------------------------------------+
