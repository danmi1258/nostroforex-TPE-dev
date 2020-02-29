#pragma once
#include <iostream>
#include <set>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "stdafx.h"
#include "Constant.h"
#include "ClientInstrument.h"

#include <boost\nowide\convert.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

#include <websocketpp\config\asio_no_tls.hpp>
#include <websocketpp\common\thread.hpp>
#include <websocketpp\server.hpp>


typedef websocketpp::server<websocketpp::config::asio> server;
namespace pt = boost::property_tree;
using namespace pt;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

struct action {
	enum action_type {
		SUBSCRIBE,
		UNSUBSCRIBE,
		IN_MESSAGE,
		OUT_MESSAGE
	};
	action(action_type t, connection_hdl h = connection_hdl()) : type(t), hdl(h) {}
	action(action_type t, std::shared_ptr<boost::property_tree::ptree> outmsg, connection_hdl h = connection_hdl()) : type(t), hdl(h), _outMsg(outmsg) {}
	action(action_type t, connection_hdl h, server::message_ptr m)
		: type(t), hdl(h), msg(m) {}

	action_type type;
	websocketpp::connection_hdl hdl;
	server::message_ptr msg;
	std::shared_ptr<boost::property_tree::ptree> _outMsg;
};
struct connection_data {
	int sessionid;
	std::string name;

};

class WebSocketS
{
public:
	using AddSubscriptionCmd = std::function<std::pair<bool, std::string>(std::string &, std::string &, InstrumentDataPublisher::DataUpdateHandler)>;
	//using AddSubscriptionCmd = std::function<void(std::string &, std::string &, InstrumentDataPublisher::DataUpdateHandler)>;

	void setAddSubscriptionCmd(AddSubscriptionCmd cmd)
	{
		_addSubscriptionCmd = cmd;
	}

	WebSocketS(int argument, wchar_t* argv[]);
	virtual ~WebSocketS();

	void run(uint16_t port);
	void stop();
	void on_open(connection_hdl hdl);

	void on_close(connection_hdl hdl);

	void on_message(connection_hdl hdl, server::message_ptr msg);

	bool isValidRequest(connection_hdl hdl, server::message_ptr msg);

	void sendJSONTick(const std::string & symbol, const MTTickShort& tick);

private:

	AddSubscriptionCmd _addSubscriptionCmd;

	//--- Lock mechanism
	mutex m_action_lock;
	mutex m_connection_lock;

	std::list<std::unique_ptr<Client>> _client;

	connection_data & get_data_from_hdl(connection_hdl hdl);
	std::vector<connection_hdl> connectionlist;

	std::map<std::string, bool> sym_sub;

	typedef std::map<connection_hdl, connection_data, std::owner_less<connection_hdl>> con_list;

	server m_server;
	con_list m_connections;
	std::queue<action> m_actions;

	
	condition_variable m_action_cond;
	connection_hdl hdl_1;

	long m_next_sessionid;
	std::atomic_bool _keepProcessingMessages = true;

};
