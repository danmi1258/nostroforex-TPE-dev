/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef EXECUTOR_APPLICATION_H
#define EXECUTOR_APPLICATION_H

#include "Bitstamp.h"

#include <string>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <fstream>
#include <cstdarg>
#include <time.h>

// FIX Application run as server or acceptor.

class Application: public Bitstamp
{
public:
	Application(const std::string & sett, bool log)
	{
		this->init(sett, true);
	}

	virtual void onTick(Tick & t) 
	{
		t.Print();
	}
private:
	//Bitstamp * bt;
};

#endif
