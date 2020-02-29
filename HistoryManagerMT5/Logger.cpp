
#include "Logger.h"
#include "stdafx.h"
#include <iostream>

Logger::Logger()
{
	//logFile.open(LOG_FILE, std::ios::out|std::ios::in);
}

Logger::~Logger()
{
	//logFile.close();
}

void Logger::writeLog(char * msg)
{
	ofstream myfile;
	//time_t t = time(0);
	//now = localtime(&t);
	//logFile << now->tm_mday << "-" << now->tm_mon << "-" << now->tm_year << "  " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "==" << msg << std::endl;
	myfile.open(LOG_FILE, std::ofstream::out|std::ofstream::app);
	myfile << msg << std::endl;
	myfile.close();
}
