#pragma once

#include <memory>
#include "stdafx.h"
template<typename T>
struct MT5Deleter
{
	void operator()(T* obj) { obj->Release(); }
};

typedef std::unique_ptr<IMTConParam, MT5Deleter<IMTConParam>> MT5ConParam;
typedef std::unique_ptr<IMTConfirm, MT5Deleter<IMTConfirm>> MT5Confirm;
typedef std::unique_ptr<IMTConSymbol, MT5Deleter<IMTConSymbol>> MT5ConSymbol;
typedef std::unique_ptr<IMTExecution, MT5Deleter<IMTExecution>> MT5Execution;
typedef std::unique_ptr<IMTOrder, MT5Deleter<IMTOrder>> MT5Order;
typedef std::unique_ptr<IMTDeal, MT5Deleter<IMTDeal>> MT5Deal;
typedef std::unique_ptr<IMTManagerAPI, MT5Deleter<IMTManagerAPI>> MT5ManagerAPI;
typedef std::unique_ptr<IMTConGroup, MT5Deleter<IMTConGroup>> MT5ConGroup;
typedef std::unique_ptr<IMTConGroupSymbol, MT5Deleter<IMTConGroupSymbol>> MT5ConGroupSymbol;
typedef std::unique_ptr<IMTConSymbol, MT5Deleter<IMTConSymbol>> MT5ConSymbol;
typedef std::unique_ptr<IMTUser, MT5Deleter<IMTUser>> MT5User;
typedef std::unique_ptr<IMTRequest, MT5Deleter<IMTRequest >> MT5OrderRequest;
typedef std::unique_ptr<IMTConSymbolSession, MT5Deleter<IMTConSymbolSession >> MT5ConSymbolSession;
typedef std::unique_ptr<IMTConTime, MT5Deleter<IMTConTime>> MT5ConTime;

 

