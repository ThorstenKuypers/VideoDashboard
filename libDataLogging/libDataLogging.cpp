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
using namespace libDataLogging::LiveDataLogger;

LIBDATALOGGING_API bool make_instance(LIBDATALOGGING_ID iid, void** inst, void* param)
{
	if (IsEqualGUID(iid, IID_DATALOGGER)) {

		if (param != nullptr) {

			IDataLogger* p = new CDataLogger(static_cast<wchar_t*>(param));
			if (p != nullptr) {

				*inst = nullptr;
				*inst = p;
				return true;
			}
		}
	}

	if (IsEqualGUID(iid, IID_LIVELOGGER)) {

		ILiveLogger* p = new CLiveDataLogger();
		if (p != nullptr) {

			*inst = nullptr;
			*inst = p;
			return true;
		}
	}
	return false;
}

LIBDATALOGGING_API void release_instance(void* inst)
{

}