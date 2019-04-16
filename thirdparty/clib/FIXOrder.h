#pragma once
#pragma once
#include <string>
#include <functional>
#include <iostream>
namespace aaa
{
	struct FIXOrder
	{
		enum Side
		{
			Side_BUY = '1',
			Side_SELL = '2'
		};
		enum Type
		{
			Type_MARKET = '1',
			Type_LIMIT = '2'
		};
		enum TimeInForce
		{
			TimeInForce_IOC = '3',
			TimeInForce_FOK = '4'
		};
		enum Status
		{
			Status_PLACED = '0',
			Status_PARTIALLY_FILLED = '1',
			Status_FILLED = '2',
			Status_CANCELLED = '4',
			Status_REJECTED = '8'
		};
		enum ExecType
		{
			ExecType_NONE = '0',
			ExecType_CANCELLED = '4',
			ExecType_REJECTED = '8',
			ExecType_TRADE = 'F'
		};
		FIXOrder()
		{
			
		}
		FIXOrder(const std::string & clientId, 
			const std::string & clientOrderId,
			const std::string & symbol,
			const char side,
			const double & qty,
			const double & price,
			const char type = Type_MARKET,
			const char timeinforce = TimeInForce_IOC,
			const char status = FIXOrder::Status_PLACED,
			const char exectype = FIXOrder::ExecType_NONE,
			const double leavesqty = 0.0,
			const double cumqty = 0.0) :
			_clientId(clientId),
			_clientOrderId(clientOrderId),
			_symbol(symbol),
			_side(side),
			_qty(qty),
			_price(price),
			_type(type),
			_timeinforce(timeinforce),
			_execType(exectype),
			_status(status),
			_lastPx(0.0),
			_avgPx(0.0),
		    _lastQty(0.0),
		    _leavesQty(leavesqty),
		    _cumQty(cumqty),
			_orderID("0"),
			_execID("0")

		{

		}
		void handleFill(double fillVolume, double fillPrice, double residualVolume,  const std::string & execID)
		{
			_lastQty = fillVolume;
			_lastPx = fillPrice;
			_leavesQty = residualVolume;
			_avgPx = (_avgPx * _cumQty + fillPrice*fillVolume) / (_cumQty + fillVolume);
			_cumQty += fillVolume;
			_execID = execID;
			
		}
		//TODO make private and provide interface
		std::string _clientId;
		std::string _clientOrderId;
		std::string _symbol;
		char _side;
		double _qty;
		double _price;
		char _type;
		char _timeinforce;

		//Updated on execution report
		char _execType;
		char _status;
		double _lastPx;
		double _avgPx;
		double _lastQty;
		double _leavesQty;
		double _cumQty;
		std::string _orderID;
		std::string _execID;
		std::string _text;

		void print(std::ostream &os) const
		{
			os << _clientId << "|" <<
				_clientOrderId << "|" <<
				_symbol << "|" <<
				_side << "|" <<
				_qty << "|" <<
				_price << "|" <<
				_type << "|" <<
				_timeinforce << "|" <<
				_execType << "|" <<
				_status << "|" <<
				_lastPx << "|" <<
				_avgPx << "|" <<
				_lastQty << "|" <<
				_leavesQty << "|" <<
				_cumQty << "|" <<
				_orderID << "|" <<
				_execID << "|" <<
				_text;
		}
		bool operator ==(const FIXOrder& rhs)
		{
			/*std::cout << "*****" << std::endl;
			std::cout << _status << "::" << rhs._status << std::endl;
			std::cout << _leavesQty << "::" << rhs._leavesQty << std::endl;
			std::cout << _cumQty << "::" << rhs._cumQty << std::endl;
			std::cout << "*****" << std::endl;*/
			return _clientId == rhs._clientId &&
				_clientOrderId == rhs._clientOrderId &&
				_symbol == rhs._symbol &&
				_side == rhs._side &&
				_qty == rhs._qty &&
				_type == rhs._type &&
				_execType == rhs._execType &&
				//_status == rhs._status &&
				(std::fabs(_leavesQty - rhs._leavesQty) < 0.00001) &&
				(std::fabs(_cumQty - rhs._cumQty) < 0.00001);
		}
		friend std::ostream& operator<< (std::ostream& stream, const FIXOrder& order)
		{
			order.print(stream);
			return stream;
		}
		friend bool operator ==(const FIXOrder& lhs, const FIXOrder& rhs)
		{
			return lhs == rhs;
		}
	};
	using FixOrderUpdateCallBack = std::function<void(const aaa::FIXOrder &)>;
}


