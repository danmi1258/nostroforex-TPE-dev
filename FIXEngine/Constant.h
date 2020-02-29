
#ifndef _CONSTANT_
#define _CONSTANT_
#pragma once
#include<string>
#include<ctime>

struct Tick
{
	Tick(std::string symbol,double bid, double offer,double bidQty, double offerQty,std::string source):_symbol(symbol),_bidPx(bid),_offerPx(offer),_bidQty(bidQty),_offerQty(offerQty),_source(source)
	{
		_timestamp = time(0);
	}
	void Print()
	{
		std::cout << "Symbol:\t" << _symbol << std::endl;
		std::cout << "BidPx:\t" << _bidPx << std::endl;
		std::cout << "BidSize:\t" << _bidQty << std::endl;
		std::cout << "OfferPx:\t" << _offerPx << std::endl;
		std::cout << "OfferSize:\t" << _offerQty << std::endl;
		std::cout << "Time:\t" << _timestamp << std::endl;
		std::cout << "Source:\t" << _source << std::endl;
	}
	std::string _symbol;
	double      _bidPx;
	double		_offerPx;
	double		_bidQty;
	double		_offerQty;
	std::string _source;
	time_t		_timestamp;
};

#endif