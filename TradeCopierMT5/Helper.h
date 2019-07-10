#ifndef _HELPER_
#define _HELPER_

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

string const CODE_EXPIRED_FALSE = "false";
string const CODE_EXPIRED_TRUE = "true";
string const LOG_FILE_EXT = ".txt";
string const LOG_FILE_EXT_CSV = ".csv";


extern string SERVER_ROOT;
extern string LOG_FOLDER;
extern string PLUGIN_FOLDER;
extern string LOG_ERRORS;
extern string LOG_WARNINGS;
extern string LOG_PLUGIN_EVENTS;

static class Helper
{

public:

	static string GetExeFolder(const string & inStr);
	
	//static void logMessage(const bool& isError, const string & msg);
	static void logMessage(const string & msg);
	static void logMessage(const string & tag, const string & msg);
	static void logMessage(LPCWSTR  msg);
	static void logMessage(const string & tag, LPCWSTR  msg);

	static set<string> deserialize(const string & fileName, const int & firstLinesToSkip);
	static vector<string> deserializeVect(const string & fileName, const int & firstLinesToSkip);
	static vector<string> split(const string &text, char sep);
	static string TimeStampToStr(const time_t&   inTime);
	static string BoolToStr(const bool & inBool);
	static bool StrToBool(const string & inStr);

	static std::string WchartToStr(wchar_t *inParam);
	static std::string LPCWSTRtoStr(LPCWSTR inParam);
	static std::string LPTSTRtoStr(LPTSTR inParam);
	


};

#endif // !1

