#pragma once

#ifndef CSERVICE_H
#define CSERVICE_H

#include <windows.h>
#include <atlbase.h>
#include <tchar.h>

#include "Manager.h"
#include <string>
#include <iostream>
#include <fstream>
//#include "AppStart.h"

TCHAR* serviceName = TEXT("JSON Pricefeed");
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle = 0;
HANDLE stopServiceEvent = 0;


void WINAPI ServiceControlHandler(DWORD controlCode)
{
	switch (controlCode)
	{
	case SERVICE_CONTROL_INTERROGATE:
		break;

	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		SetEvent(stopServiceEvent);
		return;

	case SERVICE_CONTROL_PAUSE:
		break;

	case SERVICE_CONTROL_CONTINUE:
		break;

	default:
		if (controlCode >= 128 && controlCode <= 255)
			// user defined control code
			break;
		else
			// unrecognised control code
			break;
	}

	SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void WINAPI ServiceMain(DWORD /*argc*/, TCHAR* /*argv*/[])
{
	std::stringstream ss;
	// initialise service status
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwControlsAccepted = 0;
	serviceStatus.dwWin32ExitCode = NO_ERROR;
	serviceStatus.dwServiceSpecificExitCode = NO_ERROR;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, ServiceControlHandler);

	if (serviceStatusHandle)
	{
		// service is starting
		serviceStatus.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		// do initialisation here
		Manager *manager;
		stopServiceEvent = CreateEvent(0, FALSE, FALSE, 0);
		// declaration

		if (serviceStatus.dwCurrentState == SERVICE_START_PENDING)
		{
			// initialization	
			std::vector<std::wstring> va{ boost::nowide::widen("HistoryManager64.exe"),
				boost::nowide::widen("/server:188.72.227.210:443"),
				boost::nowide::widen("/login:1005"),
				boost::nowide::widen("/password:trt487598"),
				boost::nowide::widen("/port:25500")
			};


			const wchar_t* argv1[5] = { va[0].c_str(),
				va[1].c_str(),
				va[2].c_str(),
				va[3].c_str(),
				va[4].c_str()
			};
			manager = new Manager(5, argv1);
			if (manager->createManagerInterface() != MT_RET_OK)
				goto stop_service;
		}

		// running
		serviceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			// server running			
			try
			{
				manager->StartWS();
			}
			catch (...)
			{
				goto stop_service;
			}
		}

		do
		{
		} while (WaitForSingleObject(stopServiceEvent, 5000) == WAIT_TIMEOUT);
		

		

		// service was stopped
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);

		if (serviceStatus.dwCurrentState == SERVICE_STOP_PENDING)
		{	
			try
			{
				// stopping service
				manager->StopWS();
			}
			catch (...)
			{
				// do cleanup here
				CloseHandle(stopServiceEvent);
				stopServiceEvent = 0;
				goto stop_service;
			}
		}		

		// do cleanup here
		CloseHandle(stopServiceEvent);
		stopServiceEvent = 0;
		goto stop_service;

		stop_service:
		// service is now stopped
		serviceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
	}
}

void RunService()
{
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ serviceName, ServiceMain },
		{ 0, 0 }
	};

	StartServiceCtrlDispatcher(serviceTable);
}

void InstallService()
{
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);

	if (serviceControlManager)
	{
		TCHAR path[_MAX_PATH + 1];
		std::cout << "service control manager" << std::endl;

		if (GetModuleFileName(0, path, sizeof(path) / sizeof(path[0])) > 0)
		{
			SC_HANDLE service = CreateService(serviceControlManager,
				serviceName, serviceName,
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
				0, 0, 0, 0, 0);
			std::cout << "create service" <<service << std::endl;
			if (service)
				CloseServiceHandle(service);
		}

		CloseServiceHandle(serviceControlManager);
	}
}

void UninstallService()
{
	SC_HANDLE serviceControlManager = OpenSCManager(0, 0, SC_MANAGER_CONNECT);

	if (serviceControlManager)
	{
		SC_HANDLE service = OpenService(serviceControlManager,
			serviceName, SERVICE_QUERY_STATUS | DELETE);
		std::cout << "open service" << std::endl;
		if (service)
		{
			SERVICE_STATUS serviceStatus;
			if (QueryServiceStatus(service, &serviceStatus))
			{
				std::cout << "delete" << std::endl;
				if ((serviceStatus.dwCurrentState == SERVICE_STOPPED)|| (serviceStatus.dwCurrentState == SERVICE_STOP_PENDING))//SERVICE_STOP_PENDING
					DeleteService(service);
			}

			CloseServiceHandle(service);
		}

		CloseServiceHandle(serviceControlManager);
	}
}

#endif