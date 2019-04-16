#pragma once
#include <string>

namespace aaa
{
	void asyncPostRequest(int trheadid, std::string host, std::string domain, std::string data);
	void postRequest(const std::string & host, const std::string & domain, const std::string & data);
}