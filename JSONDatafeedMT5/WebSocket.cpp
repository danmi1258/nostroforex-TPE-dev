#include "stdafx.h"
#include "WebSocket.h"

void WebSocketS::sendJSONTick(const std::string & symbol, const MTTickShort & tick)
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



connection_data & WebSocketS::get_data_from_hdl(connection_hdl hdl)
{
	// TODO: insert return statement here
	auto it = m_connections.find(hdl);
	if (it == m_connections.end()) {
		// this connection is not in the list. This really shouldn't happen
		// and probably means something else is wrong.
		throw std::invalid_argument("No data available for session");
	}

	return it->second;
}

WebSocketS::WebSocketS(int argument, wchar_t* argv[])
{
	// Initialize Asio Transport
	m_server.init_asio();
	// Register handler callbacks
	m_server.set_open_handler(bind(&WebSocketS::on_open, this, ::_1));
	m_server.set_close_handler(bind(&WebSocketS::on_close, this, ::_1));
	m_server.set_message_handler(bind(&WebSocketS::on_message, this, ::_1, ::_2));

	//TODO expose optional interface on construction
	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.clear_error_channels(websocketpp::log::elevel::all);
}

WebSocketS::~WebSocketS()
{
	for (auto i : connectionlist)
	{
		if (m_server.get_con_from_hdl(i)->get_state() < 3)
		{
			connection_hdl hdl;
			hdl = m_server.get_con_from_hdl(i);//->close;
			m_server.close(hdl, 3, "terminate");
		}
	}
}


void WebSocketS::run(uint16_t port)
{
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

void WebSocketS::stop()
{
	_keepProcessingMessages = false;
	m_server.stop();
	m_server.stop_listening();
}

void WebSocketS::on_open(connection_hdl hdl)
{
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::SUBSCRIBE, hdl));
		connectionlist.push_back(hdl);
	}
	server::connection_ptr con = m_server.get_con_from_hdl(hdl);

	std::cout << "get_host" << con->get_host() << std::endl;
	std::cout << "get_port" << con->get_port() << std::endl;
	std::cout << "get_origin" << con->get_origin() << std::endl;
	std::cout << "get_remote_endpoint" << con->get_remote_endpoint() << std::endl;

	std::unique_ptr<Client> client(new Client(hdl, con->get_remote_endpoint()));
	_client.push_back(std::move(client));
	m_action_cond.notify_one();
	sym_sub.clear();
	std::cout << "Connection Open.." << std::endl;
}

void WebSocketS::on_close(connection_hdl hdl)
{
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::UNSUBSCRIBE, hdl));
	}
	m_action_cond.notify_one();
	sym_sub.clear();
	std::cout << "Connection Close.." << std::endl;
}

void WebSocketS::on_message(connection_hdl hdl, server::message_ptr msg)
{

	isValidRequest(hdl, msg);
	{
		lock_guard<mutex> guard(m_action_lock);
		m_actions.push(action(action::IN_MESSAGE, hdl, msg));
	}
	m_action_cond.notify_one();
}

bool WebSocketS::isValidRequest(connection_hdl hdl, server::message_ptr msg)
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

			//std::pair<bool, std::string> addRet = _addSubscriptionCmd(client.username(), symbol.getString(),
			//	std::bind(&FIXPriceEngine::handleMarketDataUpdate, this, it.second->first, reqID.getString(), symbol.getString(), std::placeholders::_1));

			if (isSubscribe)
			{
				std::pair<bool, std::string> addret = _addSubscriptionCmd(std::string(""), std::string(""),
					std::bind(&WebSocketS::sendJSONTick, this, std::string("BTCUSD"), std::placeholders::_1));
				MTTickShort  tick;
				//else
				//	std::cout << "symbol not found" << std::endl;
				sym_sub[action] = true; //sym_sub[boost::to_upper_copy(action)] = true;
			}
			if (isUnSubscribe)
			{
				auto it = sym_sub.find(action); //sym_sub.find(boost::to_upper_copy(action));
				if (it != sym_sub.end())
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
