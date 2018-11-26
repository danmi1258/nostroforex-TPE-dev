#pragma once

#ifndef CSERVICE_H
#define CSERVICE_H

#include <windows.h>
#include <atlbase.h>
#include <tchar.h>

#include "quickfix/FileStore.h"
#include "quickfix/ThreadedSocketAcceptor.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"
#include "quickfix/FileLog.h"
#include "Application.h"
#include <string>
#include <iostream>
#include <fstream>
#include "AppStart.h"

TCHAR* serviceName = TEXT("HERA-FX FIXPrice");
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle = 0;
HANDLE stopServiceEvent = 0;

void wait()
{
	//std::cout << "Type Ctrl-C to quit" << std::endl;
	while (true)
	{
		FIX::process_sleep(1);
	}
}

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
		stopServiceEvent = CreateEvent(0, FALSE, FALSE, 0);
		static FIXR::AppStart *App;
		static FIX::SessionSettings *ptr_settings;
		static FIX::FileStoreFactory *ptr_storeFactory;
		static FIX::FileLogFactory *ptr_logFactory;
		static Application *ptr_application;
		static FIX::ThreadedSocketAcceptor *acceptor;

		if (serviceStatus.dwCurrentState == SERVICE_START_PENDING)
		{
			App = new FIXR::AppStart();
			ptr_settings = new FIX::SessionSettings(App->FixSetting());
			ptr_storeFactory = new FIX::FileStoreFactory(*ptr_settings);
			ptr_logFactory = new FIX::FileLogFactory(*ptr_settings);
			ptr_application = new Application(App->ManagerIP(), App->ManagerLogin(), App->ManagerPassword(), *ptr_settings);
			if (App->FileLogRequired() == "N")
				acceptor = new FIX::ThreadedSocketAcceptor(*ptr_application, *ptr_storeFactory, *ptr_settings);
			else if (App->FileLogRequired() == "Y")
				acceptor = new FIX::ThreadedSocketAcceptor(*ptr_application, *ptr_storeFactory, *ptr_settings,*ptr_logFactory);			
		}

		// running
		serviceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(serviceStatusHandle, &serviceStatus);
		if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			try
			{				
				acceptor->start();
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
				acceptor->stop();
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

		if (GetModuleFileName(0, path, sizeof(path) / sizeof(path[0])) > 0)
		{
			SC_HANDLE service = CreateService(serviceControlManager,
				serviceName, serviceName,
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
				0, 0, 0, 0, 0);
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
		if (service)
		{
			SERVICE_STATUS serviceStatus;
			if (QueryServiceStatus(service, &serviceStatus))
			{
				if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
					DeleteService(service);
			}

			CloseServiceHandle(service);
		}

		CloseServiceHandle(serviceControlManager);
	}
}

#endif