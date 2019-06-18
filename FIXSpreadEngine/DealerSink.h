#ifndef _DEALERSINK_
#define _DEALERSINK_
#pragma once
#include "mt5managerapi\Manager\API\MT5APIManager.h"

class CDealerSink : public IMTDealerSink
{
private:
	IMTRequest		*m_request;
	HANDLE			m_event;


public:
	UINT			orderStatus = 0;

	CDealerSink(void);
	~CDealerSink(void);
	bool Initialize(IMTRequest *request);
	MTAPIRES	Wait(DWORD timeout);
	virtual void	OnDealerResult(const IMTConfirm* result);
	virtual void	OnDealerAnswer(const IMTRequest* result);

};

inline CDealerSink::CDealerSink(void) : m_request(NULL), m_event(NULL)
{
}

inline CDealerSink::~CDealerSink(void)
{
	if (m_event)
		CloseHandle(m_event);
	m_event = NULL;
	m_request = NULL;
}

inline bool CDealerSink::Initialize(IMTRequest *request)
{
	m_request = request;
	m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	return(m_request && m_event);
}

inline MTAPIRES CDealerSink::Wait(DWORD timeout)
{
	MTAPIRES res = MT_RET_ERR_DATA;
	//--- wait for result
	if (m_event)
		if (WaitForSingleObject(m_event, timeout) == WAIT_OBJECT_0)
			res = m_request->ResultRetcode();
		else
			res = MT_RET_REQUEST_TIMEOUT;
	//--- done
	return(res);
}

inline void CDealerSink::OnDealerResult(const IMTConfirm* result)
{
}

inline void CDealerSink::OnDealerAnswer(const IMTRequest* request)
{
	orderStatus = request->ResultDeal();
	if (m_request) m_request->Assign(request);
	if (m_event)   SetEvent(m_event);
}




#endif
