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

}
void WebSocketServer::run(uint16_t port) {
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
}
void WebSocketServer::on_open(connection_hdl hdl) {
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::SUBSCRIBE, hdl));
	}
	m_action_cond.notify_one();
}

void WebSocketServer::on_close(connection_hdl hdl) {
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::UNSUBSCRIBE, hdl));
	}
	m_action_cond.notify_one();
}

void WebSocketServer::on_message(connection_hdl hdl, server::message_ptr msg) {
	// queue message up for sending by processing thread
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::IN_MESSAGE, hdl, msg));
	}
	m_action_cond.notify_one();
}

void WebSocketServer::process_messages() {
	while (_keepProcessingMessages.load()) {
		unique_lock<mutex> lock(m_action_lock);

		while (m_actions.empty()) {
			m_action_cond.wait(lock);
		}

		action a = m_actions.front();
		m_actions.pop();

		lock.unlock();

		if (a.type == action::SUBSCRIBE) {
			lock_guard<mutex> guard(m_connection_lock);
			connection_data data;
			data.sessionid = m_next_sessionid++;
			data.name.clear();
			m_connections[a.hdl] = data;
		}
		else if (a.type == action::UNSUBSCRIBE) {
			lock_guard<mutex> guard(m_connection_lock);
			connection_data& data = get_data_from_hdl(a.hdl);
			m_connections.erase(a.hdl);
		}
		else if (a.type == action::IN_MESSAGE) {
			lock_guard<mutex> guard(m_connection_lock);
			//_msgHandler(a.msg->get_payload(), std::bind(&WebSocketServer::asyncBroadCastMessage, this, std::placeholders::_1));
		}
		else if (a.type == action::OUT_MESSAGE) {
			lock_guard<mutex> guard(m_connection_lock);
			for (con_list::iterator it = m_connections.begin(), itEnd = m_connections.end(); it != itEnd; ++it) {
				try
				{
					std::ostringstream buf;
					boost::property_tree::write_json(buf, *(a._outMsg), false);
					m_server.send(it->first, buf.str(), websocketpp::frame::opcode::text);
				}
				catch (...)
				{

				}
			}
		}
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
