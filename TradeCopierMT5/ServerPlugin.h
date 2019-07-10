#ifndef _HELPER_SERVER_PLUGIN_
#define _HELPER_SERVER_PLUGIN_

//#include "..\Common\Helper.h"
#include "Helper.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "33333"


string const LOG_FILE_SUFFIX = "_SERVER_PLUGIN";

string const PLUGIN_FOLDER_NAME = "serverplugin";


static class BalanceServerPlugin :Helper
{

};


#endif
