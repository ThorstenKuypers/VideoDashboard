#pragma once

#include <queue>
#include <memory> 
#include <thread>
#include <future>
#include <string>
#include <functional>

#include "events.h"
#include "libDataLogging.h"
#include "liveLogging.h"
#include "LiveChannels.h"

using namespace std;

#define MAX_DATA_QUEUE_SIZE		10

namespace libDataLogging {
	namespace LiveLogging {

		struct sampleData
		{

		};

		class CLiveDataLogger : public ILiveLogger
		{

		public:
			CLiveDataLogger();
			~CLiveDataLogger();
			//CLiveDataLogger(CLiveDataLogger&) = delete;
			//CLiveDataLogger& operator=(const CLiveDataLogger&) = delete;

			//CLiveDataLogger(CLiveDataLogger&& t) :
			//	_sampleRate(t._sampleRate),
			//	_running(t._running),
			//	_samplesQueue(t._samplesQueue)
			//{
			//	t._sampleRate = 0;
			//	t._samplesQueue.empty();
			//	t._running = false;
			//}

			//CLiveDataLogger& operator=(CLiveDataLogger&& t) {
			//	_sampleRate = t._sampleRate;
			//	t._sampleRate = 0;

			//	_samplesQueue = t._samplesQueue;
			//	t._samplesQueue.empty();

			//	_running = t._running;
			//	t._running = false;

			//	return *this;
			//}

			virtual int GetLoggerSampleRate() override;
			virtual void Start() override;
			virtual void Stop() override;

		private:

			void _dataLoop();

			void _simConnected();
			void _simDisconnected();
			void _simDataUpdate(DWORD);

			template<typename T>
			T GetChannelData(const std::string channel)
			{

			}
			//template <typename Value>
			//std::function<Value(const std::string)> s;

			std::thread _loop;
			std::mutex mtx; // synchronization mutex

			bool running;
			bool isConnected; // wether connected to irsdk memserver
			int _sampleRate;
			HANDLE _memMap; // handle to file mapping
			BYTE* _map; // pointer to mapping base address
			HANDLE _dataEvent; // handle to irsdk data event
			int _lastTick; // timestamp of last data update
			bool _initialized; // whether irsdk has been initialized
			int _latest; // latest valid varbuf index
			irsdk_header* irsdkHeader;

			ChannelsMap channelList;
		};

	};
};