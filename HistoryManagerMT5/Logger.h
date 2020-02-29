#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include "Constant.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>


using namespace std;

class Logger
{
public:
	Logger();
	~Logger();
	void writeLog(char * msg);
}

#endif
