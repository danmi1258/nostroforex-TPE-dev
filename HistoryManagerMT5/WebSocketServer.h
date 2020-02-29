#pragma once
#include <boost\nowide\convert.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

#include <websocketpp/config/asio_no_tls.hpp>
//TODO SSL #include <websocketpp/config/asio.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include <websocketpp/common/thread.hpp>
//#include "TRSignals.h"


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

/* on_open insert connection_hdl into channel
* on_close remove connection_hdl from channel
* on_message queue send to all channels
*/


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
class WebSocketServer {
public:
	WebSocketServer();
	virtual ~WebSocketServer();

	void run(uint16_t port, IMTManagerAPI *m);
	void stop();
	void on_open(connection_hdl hdl);

	void on_close(connection_hdl hdl);

	void on_message(connection_hdl hdl, server::message_ptr msg);

	bool isValidRequest(connection_hdl hdl,server::message_ptr msg);

	void sendJSONTick(const std::string & symbol, const MTTickShort& tick);

private:
	connection_data& get_data_from_hdl(connection_hdl hdl);
	std::vector<connection_hdl> connectionlist;

	//typedef std::set<connection_hdl, std::owner_less<connection_hdl> > con_list;

	std::map<std::string, bool> sym_sub;

	typedef std::map<connection_hdl, connection_data, std::owner_less<connection_hdl>> con_list;

	server m_server;
	IMTManagerAPI *manager = NULL;
	con_list m_connections;
	std::queue<action> m_actions;

	mutex m_action_lock;
	mutex m_connection_lock;
	condition_variable m_action_cond;
	connection_hdl hdl_1;

	long m_next_sessionid;
	std::atomic_bool _keepProcessingMessages = true;

};
