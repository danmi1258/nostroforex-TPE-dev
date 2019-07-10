#pragma once
#include <mutex>
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>
#include "stdafx.h"


template<typename T>
struct MT5Deleter
{
	void operator()(T* obj) { obj->Release(); }
};

typedef std::unique_ptr<IMTConGroup, MT5Deleter<IMTConGroup>> MT5UserGroup;
typedef std::unique_ptr<IMTUser, MT5Deleter<IMTUser>> MT5User;

struct Logins
{
	Logins(const std::string & loginmask, const long targetlogin): _loginmask(loginmask), _targetlogin(targetlogin)
	{
	}
	std::string		_loginmask;
	long			_targetlogin;
};

class GroupConfig
{
public:
	GroupConfig(std::string & groupmask): _groupmask(groupmask){}
	~GroupConfig(){}
	void insertGroupConfig(const std::string & loginmask, const long & targetlogin)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		Logins _login(loginmask, targetlogin);
		_loginconfig.push_back(_login);
	}
	void print(IMTServerAPI* api)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_api = api;
		_api->LoggerOut(MTLogOK, L"GroupSetting(%d) :\t%s", _loginconfig.size(), boost::nowide::widen(_groupmask).c_str());
		for (auto & i : _loginconfig)
		{
			_api->LoggerOut(MTLogOK, L"LoginMask(%s) : TargetAccount(%d)",boost::nowide::widen(i._loginmask),i._targetlogin);//api->UserGet
		}
	}
	std::string getGroup()
	{
		return _groupmask;
	}
	std::pair<long, bool> searchLogin(std::string & login)
	{
		for (auto & i : _loginconfig)
		{
			if (CMTStr::CheckGroupMask(boost::nowide::widen(i._loginmask).c_str(), boost::nowide::widen(login).c_str()) == true)
			{
				return (std::pair<long, bool>(std::make_pair(i._targetlogin,true)));
			}
		}
		return (std::pair<long, bool>(std::make_pair(0, false)));
	}
private:
	std::mutex _mtx;
	std::string _groupmask;
	std::vector<Logins> _loginconfig;
	IMTServerAPI* _api;
};

class SymbolConfig
{
public:
	SymbolConfig(std::string & symbolmask) :_symbolmask(symbolmask) {}
	SymbolConfig(std::string & symbolmask,IMTServerAPI * api) :_symbolmask(symbolmask),_api(api) {}
	~SymbolConfig() {}
	std::string getSymbol()
	{
		return _symbolmask;
	}
	bool insertSymbolConfig(std::string & groupmask, const std::string & loginmask, const long & targetlogin)
	{
		std::lock_guard<std::mutex> lock(_mtx);

		MT5User _user(_api->UserCreate());
		if (MT_RET_OK != _api->UserGet(targetlogin, _user.get()))
			_api->LoggerOut(MTLogOK, L"UserGet Failed.");
		if (CMTStr::CheckGroupMask(boost::nowide::widen(groupmask).c_str(), _user.get()->Group()) == true)
		{
			// groupmask and target login group are same. 
			// loginmask and login should not same.
			//_api->LoggerOut(MTLogOK, L"Groupmask[%s] and user group[%s] matched.", boost::nowide::widen(groupmask).c_str(), _user.get()->Group());
			if (CMTStr::CheckGroupMask(boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str()) == true)
			{
				// discard this entry.
				//_api->LoggerOut(MTLogOK, L"Loginmask[%s] and targetlogin[%s] matched.",boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str());
				_api->LoggerOut(MTLogOK, L"Failed === {Groupmask[%s]|Usergroup[%s] : Loginmask[%s]|Targetlogin[%s]}", boost::nowide::widen(groupmask).c_str(), _user.get()->Group(), boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str());
				return false;
			}

		}
		else
		{
			//_api->LoggerOut(MTLogOK, L"Groupmask[%s] and user group[%s] failed.", boost::nowide::widen(groupmask).c_str(), _user.get()->Group());
			if (CMTStr::CheckGroupMask(boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str()) == false)
			{
				// discard this entry.
				//_api->LoggerOut(MTLogOK, L"Loginmask[%s] and targetlogin[%s] failed.", boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str());
				_api->LoggerOut(MTLogOK, L"Failed === {Groupmask[%s]|Usergroup[%s] : Loginmask[%s]|Targetlogin[%s]}", boost::nowide::widen(groupmask).c_str(), _user.get()->Group(), boost::nowide::widen(loginmask).c_str(), boost::nowide::widen(std::to_string(targetlogin)).c_str());
				return false;
			}
		}

		auto it = findGroupConfig(groupmask);
		if (it.second == false)
		{
			// Insert new entry.
			size_t index = _groupconfig.size();
			std::unique_ptr<GroupConfig> grConfig(new GroupConfig(groupmask));
			grConfig->insertGroupConfig(loginmask, targetlogin);
			_groupconfig.push_back(std::move(grConfig));
			_groupIndexByGroupmask[groupmask] = index;
		}
		else if (it.second == true)
		{
			// update into existing settings.
			auto & it1 = _groupconfig.begin();
			std::advance(it1, it.first);
			it1->get()->insertGroupConfig(loginmask, targetlogin);
		}
		return true;
	}
	void print(IMTServerAPI* api)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_api = api;
		_api->LoggerOut(MTLogOK, L"SymbolSetting(%d) :\t%s", _groupconfig.size(),boost::nowide::widen(_symbolmask).c_str());
		for (auto & i : _groupconfig)
		{
			i.get()->print(api);
		}
	}
	std::pair<long, bool> searchGroupConfig(const std::string & groupname,const long login)
	{
		for (auto & i : _groupconfig)
		{
			if (CMTStr::CheckGroupMask(boost::nowide::widen(i.get()->getGroup()).c_str(), boost::nowide::widen(groupname).c_str()) == true)
			{
				auto & x = i.get()->searchLogin(std::to_string(login));
				if (x.second == true)
					return (std::pair<long, bool>(std::make_pair(x.first, true)));
				else
					return (std::pair<long, bool>(std::make_pair(0, false)));
			}
		}
		return (std::pair<long, bool>(std::make_pair(0, false)));
	}
private:
	std::mutex _mtx;
	std::string _symbolmask;
	IMTServerAPI*  _api;
	std::list<std::unique_ptr<GroupConfig>> _groupconfig;
	std::unordered_map<std::string, size_t> _groupIndexByGroupmask;


	std::pair<size_t, bool> findGroupConfig(const std::string & groupmask)
	{
		auto i = _groupIndexByGroupmask.find(groupmask);
		if (i != _groupIndexByGroupmask.end())
		{
			return (std::pair<size_t, bool>(std::make_pair(i->second, true)));
		}
		else
		{
			return (std::pair<size_t, bool>(std::make_pair(i->second, false)));
		}
	}
};