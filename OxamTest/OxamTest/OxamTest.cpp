// OxamTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MarketData.h"


int main()
{
	MarketData m("test.txt");
	/*m.insertOrderEntry(1000, 100, 10.0);
	m.insertOrderEntry(2000, 101, 13.0);
	m.insertOrderEntry(2200, 102, 13.0);
	m.eraseOrderEntry(2400, 101);
	m.eraseOrderEntry(2500, 102);
	m.eraseOrderEntry(4000, 100);*/
	//m.printWeightedPx();
	//m.getHighPrice();
	std::cout << "Weighted Price:\t" << m.printWeightedPx() << std::endl;
	std::cout << "Highest Order Price:\t" << m.getHighPrice() << std::endl;
	//m.insertOrderEntry(5000, 103, 10.0);
	//m.eraseOrderEntry(5200, 103);
	//m.printWeightedPx();
    return 0;
}

