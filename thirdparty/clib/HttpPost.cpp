#include "stdafx.h"
#include "HttpPost.h"
//#include <WinInet.h>
#include <atlbase.h>
#include <windows.h>

#include <string>


#include <windows.h>
#include <wininet.h>

namespace aaa
{
	void asyncPostRequest(int trheadid, std::string host, std::string domain, std::string data)
	{
		postRequest(host, domain, data);
	}
	void postRequest(const std::string & host, const std::string & domain, const std::string & data)
	{
		static const std::string hdrs("Content-Type: application/x-www-form-urlencoded");
		static const std::string uA ("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:57.0) Gecko/20100101 Firefox/57.0");

		HINTERNET session = InternetOpenA(uA.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (session != NULL)
		{
			HINTERNET connect = InternetConnectA(session, host.c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
			if (connect != NULL)
			{
				HINTERNET request = HttpOpenRequestA(connect, "POST", domain.c_str(), NULL, "", NULL, 0, 0);
				if (request != NULL)
				{
					const int _SIZE = 1024;
					BYTE _DATA_RECIEVED[_SIZE];
					DWORD NO_BYTES_READ = 0;
					std::string resultPost;
					bool res = HttpSendRequestA(request, hdrs.c_str(), static_cast<DWORD>(hdrs.size()), (LPVOID)data.c_str(), static_cast<DWORD>(data.size()));
					if (NO_BYTES_READ > 0) {
						resultPost.append((char*)_DATA_RECIEVED, NO_BYTES_READ);
					}
					InternetCloseHandle(request);
				}
				InternetCloseHandle(connect);
			}
			InternetCloseHandle(session);
		}
	}
}