// libDataLogging.cpp : Defines the exported functions for the DLL application.
//

#include <windows.h>

#include "stdafx.h"
#include "DataLogger.h"
#include "LiveDataLogger.h"
#include "libDataLogging.h"
#include "liveLogging.h"

using namespace libDataLogging;
using namespace libDataLogging::DataLogger;
using namespace libDataLogging::LiveLogging;

LIBDATALOGGING_API struct IDataLogging* init_instance(wchar_t* logfile)
{
	try {
		struct IDataLogging* p =new CDataLogger(logfile);
		if (p) {
			return p;
		}
	}
	catch (...)
	{
		return NULL;
	}

	return NULL;
}

LIBDATALOGGING_API void release_instance(struct IDataLogging* inst)
{
	if (inst) {
		CDataLogger* p =(CDataLogger*)inst;

		delete p;
		p =NULL;
		inst =NULL;
	}
}

LIBDATALOGGING_API ILiveLogger* make_liveLogger()
{
	return new CLiveDataLogger();
}

LIBDATALOGGING_API void release_live(libDataLogging::LiveLogging::ILiveLogger* inst)
{
	if (inst != nullptr)
	{
		CLiveDataLogger* l = static_cast<CLiveDataLogger*>(inst);
		delete l;
		l = nullptr;
	}
}

