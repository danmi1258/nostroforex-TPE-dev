#pragma once

#include "stdafx.h"
#include <mutex>
#include <list>
#include <unordered_map>
#include <websocketpp\config\asio_no_tls.hpp>
#include <websocketpp\common\thread.hpp>
#include <websocketpp\server.hpp>
#include <websocketpp\endpoint.hpp>


typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;
using websocketpp::endpoint;



class Client
{
public:
	Client(connection_hdl & hdl,const std::string & location) :_hdl(hdl), _location(location)
	{
	}
	Client()
	{
	}
	void assign(const Client& c)
	{
		_hdl = c._hdl;
		_con = c._con;
		_location = c._location;
	}
	~Client()
	{
	}
	void getResource()
	{
		lock_guard<mutex> guard(_mtx);
		//_path = _con->get_origin();
	}
	connection_hdl getConnection()
	{
		return _hdl;
	}
private:
	std::mutex					_mtx;
	connection_hdl				_hdl;
	server::connection_ptr		_con;
	std::string					_location;
};


class InstrumentDataPublisher
{
public:
	using DataUpdateHandler = std::function<void(const MTTickShort &)>;
	using ClientByKeyCont = std::unordered_map<std::string, DataUpdateHandler>;
	//using ClientByKeyCont = std::unordered_map<connection_hdl, DataUpdateHandler>;
	using ClientIt = typename ClientByKeyCont::iterator;

	InstrumentDataPublisher()
	{
	}
	~InstrumentDataPublisher()
	{
	}
	void addHandler(DataUpdateHandler _handler)
	{
		//std::lock_guard<std::mutex> lock(_mtx);
		//std::pair<ClientIt, bool> it = _mdUpdateHandlerByClientID.insert(std::pair<std::string, DataUpdateHandler>(std::string(""), _handler));
	}
	bool registerClient(DataUpdateHandler _handler)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		std::pair<ClientIt, bool> it = _mdUpdateHandlerByClientID.insert(std::pair<std::string, DataUpdateHandler>(std::string("1"), _handler));
		return it.second;
	}
	
	void handleDataUpdate(const MTTickShort & upd)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		std::cout << "_mdUpdateHandlerByClientID.size() = " << _mdUpdateHandlerByClientID.size() << std::endl;
		for (const auto & it : _mdUpdateHandlerByClientID)
		{
			it.second(upd);
		}
	}
private:
	std::mutex										_mtx;
	ClientByKeyCont									_mdUpdateHandlerByClientID;
};
