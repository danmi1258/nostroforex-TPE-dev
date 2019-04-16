#pragma once
#include <string>
#include <vector>

namespace aaa
{
	enum class SubscriptionRequestType
	{
		SUB = 0,
		UNSUB = 1
	};
	static const std::vector<std::string> SubscriptionRequestTypeNames{ "SUB","UNSUB" };

	enum class SubscriptionResponse
	{
		OK = 0,
		FAIL = 1
	};
	static const std::vector<std::string> SubscriptionResponseNames{ "OK","FAIL" };

	enum class StreamState
	{
		SUBSCRIBED = 0,
		SUBREJECTED = 1,
		STARTED = 2,
		STOPPED = 3,
		STALE = 4,
		UNSUBSCRIBED = 5
	};
	static const std::vector<std::string> StreamStateNames{
		"SUBSCRIBED",
		"SUBREJECTED",
		"STARTED",
		"STOPPED",
		"STALE",
		"UNSUBSCRIBED" };

	enum class LogLevel
	{
		MTLogOK = 0,
		MTLogWarn = 1,
		MTLogErr = 2
	};

}