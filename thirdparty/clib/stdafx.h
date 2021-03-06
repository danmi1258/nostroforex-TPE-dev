#pragma once
//---
//#define WINVER               0x0600
//#define _WIN32_WINNT         0x0600
#define _WIN32_WINDOWS       0x0600
//#define _WIN32_IE            0x0600
//---

//#include "targetver.h"  //TODO Check this

#include <tchar.h>
#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <new.h>
#include <wchar.h>
#include <process.h>
#include <Winsock2.h>
#include <iostream>

//--- API
#include "mt5gatewayapi/Gateway/API/MT5APIGateway.h"
#include "mt5gatewayapi/Manager/API/MT5APIManager.h"