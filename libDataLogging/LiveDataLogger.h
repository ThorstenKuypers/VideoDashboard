#pragma once

#include <queue>
#include <array>
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

#pragma region "CLiveDataLogger"
		// This is the "Core" class of the live data acquisition system. It hosts and encapsulates the data loop thread
		// and all other sim SDK specific information, that is transparent to the using application.

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

			// returns a raw sample buffer from the sample queue
			virtual bool GetSample(std::vector<BYTE>& buf);

			virtual SampleValue GetSampleData(DataSample& s, CLiveChannel& c)
			{
				int n = c.get_Offset();
				SampleValue val;

				try {

					if (c.get_Type() == irsdk_double) {

						double* v = (double*)(&s.at(n));
						return std::move(SampleValue(*v));
					}
					else if (c.get_Type() == irsdk_float) {

						float* f = (float*)(&s.at(n));
						return std::move(SampleValue(*f));
					}
					else if (c.get_Type() == irsdk_int) {

						int* i = (int*)(&s.at(n));
						return std::move(SampleValue(*i));
					}
					else if (c.get_Type() == irsdk_char) {

						char* c = (char*)(&s.at(n));
						return std::move(SampleValue(*c));
					}
					else if (c.get_Type() == irsdk_bool) {

						bool* b = (bool*)(&s.at(n));
						return std::move(SampleValue(*b));
					}
					else if (c.get_Type() == irsdk_bitField) {

						DWORD* u = (DWORD*)(&s.at(n));
						return std::move(SampleValue(*u));
					}

				}
				catch (std::out_of_range ex) {
					throw ex;
				}

				return val;
			}

		private:

			void _dataLoop();

			void _simConnected();
			void _simDisconnected();
			void _simDataUpdate();

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

			std::deque<DataSample> _samplesQueue;
		};

#pragma endregion

	}
}