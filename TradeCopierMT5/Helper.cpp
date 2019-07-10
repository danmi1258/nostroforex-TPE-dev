#include "stdafx.h"
#include "Helper.h"
#include <string>
#include <fstream>
#include <iostream>
#include <thread>         // thread
#include <mutex>          // mutex
#include <vector>
#include <set>

mutex mtx;
mutex mtx2;
//string SERVER_ROOT = "C:\\MetaTrader 5 Platform\\MainTrade\\";
string SERVER_ROOT = "";
string LOG_FOLDER = SERVER_ROOT + "logs\\";
string PLUGIN_FOLDER = SERVER_ROOT + "plugins\\";
string LOG_ERRORS = LOG_FOLDER + "LOG_INFO";
//string LOG_WARNINGS = LOG_FOLDER + "LOG_WARNINGS";
string LOG_WARNINGS = "LOG_WARNINGS";
string LOG_PLUGIN_EVENTS = LOG_FOLDER + "LOG_PLUGIN_EVENTS";

using namespace std;


string Helper::GetExeFolder(const string & inStr)
{
	size_t lastPos = inStr.find_last_of('\\');
	string res0 = inStr.substr(0, lastPos + 1);
	string res = "";
	vector<string> res0Splited = Helper::split(res0, '\\');

	for (int i = 0; i < res0Splited.size() - 1; i++)
	{
		res += res0Splited.at(i) + "\\";
	}


	return res ;
}


vector<string> Helper:: split(const string &text, char sep) {
	vector<string> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}
string Helper::TimeStampToStr(const time_t&   inTime)
{
	string res = "";
	char* buf = new char[100];
	struct tm *ts = gmtime((&inTime));

	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour, ts->tm_min,
		ts->tm_sec);
	res = string(buf);

	return res;
}




set<string> Helper::deserialize(const string & fileName, const int & firstLinesToSkip)
{
	set<string> res;

	ifstream inFile;

	inFile.open(fileName);
	if (inFile.is_open()) {
		Helper::logMessage(false, "file opened for serialization");
		string line;
		string Surname;
		string Initial;
		int i = 0;
		while (getline(inFile, line)) {

			if (i >= firstLinesToSkip && strcmp(line.c_str(), "") != 0)
			{
				res.emplace(line);
				Helper::logMessage(false, "line i = " + line);
			}				
			i++;
		}
		inFile.close();

	}
	else {
		Helper::logMessage(false, "Failed to open file for serialization : " + fileName);
	}

	
	return res;
}

vector<string> Helper::deserializeVect(const string & fileName, const int & firstLinesToSkip)
{
	vector<string> res;

	ifstream inFile;

	Helper::logMessage(false, "deserializeVect.fileName = " + fileName);

	inFile.open(fileName);
	if (inFile.is_open()) {
		Helper::logMessage(false, "file opened for serialization");
		string line;
		string Surname;
		string Initial;
		int i = 0;
		while (getline(inFile, line)) {

			if (i >= firstLinesToSkip && strcmp(line.c_str(), "") != 0)
			{
				res.push_back(line);
				Helper::logMessage(false, "line i = " + line);
			}
			i++;
		}
		inFile.close();

	}
	else {
		Helper::logMessage(false, "Failed to open file for serialization");
	}


	return res;
}


string Helper::BoolToStr(const bool & inBool)
{
	string res = CODE_EXPIRED_FALSE;
	if (inBool)
		res = CODE_EXPIRED_TRUE;
	return res;
}

bool Helper::StrToBool(const string & inStr)
{
	bool res = false;
	if (strcmp(inStr.c_str(), CODE_EXPIRED_TRUE.c_str()) == 0)
	{
		Helper::logMessage(false, "StrToBool.res = true");
		res = true;
	}
		
	return res;
}

//void Helper::logMessage(const bool& isError, const string & msg)
//{
//#ifdef _DEBUG
//	mtx.lock();
//
//	try
//	{
//		string logFileName = LOG_ERRORS + LOG_FILE_SUFFIX + LOG_FILE_EXT;
//		if (isError)
//			logFileName = LOG_WARNINGS + LOG_FILE_SUFFIX + LOG_FILE_EXT;
//
//		ofstream myfile;
//		myfile.open(logFileName, ofstream::out | ofstream::app);
//		myfile << msg << endl;
//		myfile.close();
//	}
//	catch (const std::exception& e)
//	{
//
//	}
//
//
//
//	mtx.unlock();
//#endif	
//
//}

void Helper::logMessage(const string & msg)
{
//#ifdef _DEBUG
	mtx.lock();

	try
	{
		string logFileName = LOG_ERRORS + LOG_FILE_SUFFIX + LOG_FILE_EXT;
	

		ofstream myfile;
		myfile.open(logFileName, ofstream::out | ofstream::app);
		myfile << msg << endl;
		myfile.close();
	}
	catch (const std::exception& e)
	{

	}



	mtx.unlock();
//#endif	

}

void Helper::logMessage(const string & tag, const string & msg)
{
//#ifdef _DEBUG
	mtx.lock();

	try
	{
		string logFileName = LOG_ERRORS + LOG_FILE_SUFFIX + LOG_FILE_EXT;


		ofstream myfile;
		myfile.open(logFileName, ofstream::out | ofstream::app);
		myfile << tag + " = " + msg << endl;
		myfile.close();
	}
	catch (const std::exception& e)
	{

	}



	mtx.unlock();
//#endif	

}
void Helper::logMessage(LPCWSTR msg)
{
//#ifdef _DEBUG
	mtx.lock();

	try
	{
		string logFileName = LOG_ERRORS + LOG_FILE_SUFFIX + LOG_FILE_EXT;


		ofstream myfile;
		myfile.open(logFileName, ofstream::out | ofstream::app);
		myfile << LPCWSTRtoStr(msg).c_str() << endl;
		myfile.close();
	}
	catch (const std::exception& e)
	{

	}



	mtx.unlock();
//#endif	

}

void Helper::logMessage(const string & tag, LPCWSTR msg)
{
//#ifdef _DEBUG
	mtx.lock();

	try
	{
		string logFileName = LOG_ERRORS + LOG_FILE_SUFFIX + LOG_FILE_EXT;


		ofstream myfile;
		myfile.open(logFileName, ofstream::out | ofstream::app);
		myfile << (tag + " = " + LPCWSTRtoStr(msg)).c_str() << endl;
		myfile.close();
	}
	catch (const std::exception& e)
	{

	}



	mtx.unlock();
//#endif	

}

std::string Helper::WchartToStr(wchar_t *inParam)
{
	wstring inParamWS(inParam);
	string res(inParamWS.begin(), inParamWS.end());
	return res;
}

std::string Helper::LPCWSTRtoStr(LPCWSTR inParam)
{
	wstring inParamWS(inParam);
	string res(inParamWS.begin(), inParamWS.end());
	return res;
}

std::string Helper::LPTSTRtoStr(LPTSTR inParam)
{
	wstring inParamWS(inParam);
	string res(inParamWS.begin(), inParamWS.end());
	return res;
}