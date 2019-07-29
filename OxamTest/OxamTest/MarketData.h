#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <sstream>
#include <fstream>

class MarketData
{
private:
	std::multiset<double> _orderPx;
	std::map<int, double> _orderEntry;

	int total_timestamp;
	double weightPrice;
	int prev_timestamp;
	double prev_price;

public:
	MarketData(const std::string & inputfile) : total_timestamp(0),weightPrice(0.0),prev_timestamp(0),prev_price(0.0)
	{
		std::ifstream ifile(inputfile);
		std::string str;
		while (std::getline(ifile, str))
		{			
			std::stringstream ss(str);
			std::string item;
			std::vector<std::string> splittedStrings;
			while (std::getline(ss, item, ' '))
			{
				splittedStrings.push_back(item);
			}
			if (splittedStrings[1] == "I")
				insertOrderEntry(std::stoi(splittedStrings[0]), std::stoi(splittedStrings[2]), std::stod(splittedStrings[3]));
			else if (splittedStrings[1] == "E")
				eraseOrderEntry(std::stoi(splittedStrings[0]), std::stoi(splittedStrings[2]));
		}
		ifile.close();
	}
	void insertOrderEntry(int timestamp, int id, double price);
	void eraseOrderEntry(int timestamp, int id);
	double getHighPrice()
	{
		if (_orderEntry.size() == 0)
			return std::numeric_limits<double>::quiet_NaN();
		else
			return *(_orderPx.rbegin());
	}
	double printWeightedPx()
	{
		return double(weightPrice / total_timestamp);
	}
};

void MarketData::insertOrderEntry(int timestamp, int id, double price)
{
	weightPrice += prev_price * (timestamp - prev_timestamp);
	if (_orderEntry.size() > 0)
		total_timestamp += (timestamp - prev_timestamp);
	prev_price = price;
	prev_timestamp = timestamp;
	_orderEntry[id] = price;
	_orderPx.insert(price);
}

void MarketData::eraseOrderEntry(int timestamp, int id)
{
	double price = 0.0;

	auto iter = _orderEntry.find(id);
	if (iter != _orderEntry.end())
	{
		price = iter->second;
		_orderPx.erase(price);
		_orderEntry.erase(iter);
	}

	if (_orderEntry.size() > 0)
	{
		weightPrice += prev_price * (timestamp - prev_timestamp);
		total_timestamp += (timestamp - prev_timestamp);
		prev_price = price;
		prev_timestamp = timestamp;
	}
	else
	{
		weightPrice += price * (timestamp - prev_timestamp);
		total_timestamp += (timestamp - prev_timestamp);
		prev_price = 0.0;
		prev_timestamp = 0;
	}
}
