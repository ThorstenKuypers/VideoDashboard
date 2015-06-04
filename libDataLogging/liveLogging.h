#pragma once

#include "libDataLogging.h"
#include "events.h"

#pragma region "Live data aquisition interface"

namespace libDataLogging {
	namespace LiveLogging {

		using namespace libDataLogging::DataLogger;

#define VAR_TYPE(t) decltype(t)

		struct ILiveLogger
		{
		public:
			SimConnectedEventHandler Connected;
			SimDisconnectedEventHandler Disconnected;
			SampleDataUpdateEventHandler DataUpdate;

			ChannelsMap AvailableChannels;

			virtual void Start() = 0;
			virtual void Stop() = 0;

			virtual int GetLoggerSampleRate() = 0;
		};

	}
}

#pragma endregion

extern "C" LIBDATALOGGING_API libDataLogging::LiveLogging::ILiveLogger* make_liveLogger();
extern "C" LIBDATALOGGING_API void release_live(libDataLogging::LiveLogging::ILiveLogger*);