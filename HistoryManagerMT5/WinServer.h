#ifndef WinServer_h
#define WinServer_h

#pragma once

#include "stdafx.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "33333"

#define MAX_PACKET_SIZE 1024


class WinServer
{
public:

	
	SOCKET ListenSocket;	// Socket to listen for new connections	
	SOCKET ClientSocket;	// Socket to give to the clients				  
	int iResult;			// for error checking return values

	WinServer();
	~WinServer();
	bool acceptClient();
	void sendDataToClient(char * packets, int totalSize);
	int receiveData(char * recvbuf);
	void serverShutdown();

	static int sendMessage(SOCKET curSocket, char * message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};

#endif