#pragma once
#include "StdAfx.h"
#include "WebSocketServer.h"

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

/* on_open insert connection_hdl into channel
* on_close remove connection_hdl from channel
* on_message queue send to all channels
*/


WebSocketServer::WebSocketServer() :m_next_sessionid(1) {
	// Initialize Asio Transport
	m_server.init_asio();
	// Register handler callbacks
	m_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
	m_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
	m_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));

	//TODO expose optional interface on construction
	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.clear_error_channels(websocketpp::log::elevel::all);
}
WebSocketServer::~WebSocketServer()
{
	for (auto i : connectionlist)
	{
		if (m_server.get_con_from_hdl(i)->get_state() < 3)
		{
			connection_hdl hdl;
			hdl = m_server.get_con_from_hdl(i);//->close;
			m_server.close(hdl,3,"terminate");
		}
	}
}
void WebSocketServer::run(uint16_t port, IMTManagerAPI *m) {
	if (m != NULL)
		manager = m;
	else
		std::cout << "Manager api is null" << std::endl;
	// listen on specified port
	m_server.listen(port);

	// Start the server accept loop
	m_server.start_accept();

	// Start the ASIO io_service run loop
	try {
		m_server.run();
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}
void WebSocketServer::stop()
{
	_keepProcessingMessages = false;
	m_server.stop();
	m_server.stop_listening();
}
void WebSocketServer::on_open(connection_hdl hdl) {
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::SUBSCRIBE, hdl));
		connectionlist.push_back(hdl);
	}
	m_action_cond.notify_one();
	sym_sub.clear();
	std::cout << "Connection Open.." << std::endl;
	manager->LoggerOut(MTLogOK, L"Connection Open.");
}

void WebSocketServer::on_close(connection_hdl hdl) {
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::UNSUBSCRIBE, hdl));
	}
	m_action_cond.notify_one();
	sym_sub.clear();
	std::cout << "Connection Close.." << std::endl;
	manager->LoggerOut(MTLogOK, L"Connection Close.");
}

void WebSocketServer::on_message(connection_hdl hdl, server::message_ptr msg) {
	// queue message up for sending by processing thread
	isValidRequest(hdl,msg);
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::IN_MESSAGE, hdl, msg));
	}
	m_action_cond.notify_one();
}

bool WebSocketServer::isValidRequest(connection_hdl hdl,server::message_ptr msg)
{
	pt::ptree tree;
	hdl_1 = hdl;
	try
	{
		bool isSubscribe = false;
		bool isUnSubscribe = false;
		bool isSendPrice = false;
		std::stringstream ss;
		std::string action;
		ss << msg->get_payload();
		std::cout << ss.str() << std::endl;
		manager->LoggerOut(MTLogOK, L"Message Received :\t%s.", boost::nowide::widen(ss.str()).c_str());
		while (std::getline(ss, action, ','))
		{
			if (!action.compare("SUBSCRIBE") || !action.compare("subscribe"))
			{
				isSubscribe = true;
				// send accept or reject.
				continue;
			}
			else if (!action.compare("UNSUBSCRIBE") || !action.compare("unsubscribe"))
			{
				isUnSubscribe = true;
				// send accept or reject.
				continue;
			}
			/*else if (!action.compare("SENDPRICE") || !action.compare("sendprice"))
			{
				isSendPrice = true;
				// send accept or reject.
				continue;
			}*/

			if (isSubscribe)
			{
				MTTickShort  tick;
				MTAPIRES ret = manager->TickLast(boost::nowide::widen(action).c_str(), tick);
				if (ret != MT_RET_OK)
					ret = manager->SelectedAdd(boost::nowide::widen(action).c_str());
				//else
				//	std::cout << "symbol not found" << std::endl;
				sym_sub[action] = true; //sym_sub[boost::to_upper_copy(action)] = true;
			}
			if (isUnSubscribe)
			{
				auto it = sym_sub.find(action); //sym_sub.find(boost::to_upper_copy(action));
				if (it !=sym_sub.end())
					sym_sub.erase(it);
			}
			/*if (isSendPrice)
			{
				MTTickShort tick;
				tick = 
			}*/
			
		}
		return true;
	}
	catch (std::exception &e)
	{
		return false;
	}	
	//return false;
}
void WebSocketServer::sendJSONTick(const std::string & symbol, const MTTickShort & tick)
{
	auto it = sym_sub.find(symbol);
	if (it != sym_sub.end())
	{
		pt::ptree tree;
		tree.clear();
		tree.put("symbol", symbol);
		tree.put("bidPx", tick.bid);
		tree.put("askPx", tick.ask);

		std::stringstream ss;
		boost::property_tree::write_json(ss, tree, false);
		std::cout << ss.str() << std::endl;
		m_server.send(hdl_1, ss.str(), websocketpp::frame::opcode::text);
		ss.clear();
		tree.clear();
	}
	
}
connection_data& WebSocketServer::get_data_from_hdl(connection_hdl hdl) {
	auto it = m_connections.find(hdl);

	if (it == m_connections.end()) {
		// this connection is not in the list. This really shouldn't happen
		// and probably means something else is wrong.
		throw std::invalid_argument("No data available for session");
	}

	return it->second;
}
