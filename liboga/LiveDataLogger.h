#pragma once
#ifndef _LIVEDATALOGGER_H_
#define _LIVEDATALOGGER_H_

#include <thread>
#include <mutex>
#include <queue>
#include <map>

#include "liboga.h"


namespace libOGA {
	namespace LiveLogs {

		class CLiveDataLogger : public ILiveDataLogger, public CBaseChannel
		{
		public:
			CLiveDataLogger(ILiveLoggingCallback& callback);

			CLiveDataLogger(CLiveDataLogger&) = delete;
			CLiveDataLogger(CLiveDataLogger&& o) :
				_callbackIf(std::move(o._callbackIf))
			{
				running = o.running;
				o.running = false;

				isConnected = o.isConnected;
				o.isConnected = false;

				_initialized = o._initialized;
				o._initialized = false;

				_sampleRate = o._sampleRate;
				o._sampleRate = 0;

				_memMap = o._memMap;
				o._memMap = INVALID_HANDLE_VALUE;

				_map = o._map;
				o._map = nullptr;

				_dataEvent = o._dataEvent;
				o._dataEvent = INVALID_HANDLE_VALUE;

				_lastTick = o._lastTick;
				o._lastTick = 0;

				_latest = o._latest;
				o._latest = 0;

				irsdkHeader = std::move(o.irsdkHeader);

				//_callbackIf = std::move(o._callbackIf);

				channels = std::move(o.channels);
			}

			CLiveDataLogger& operator=(CLiveDataLogger&& o)
			{
				running = o.running;
				o.running = false;

				isConnected = o.isConnected;
				o.isConnected = false;

				_initialized = o._initialized;
				o._initialized = false;

				_sampleRate = o._sampleRate;
				o._sampleRate = 0;

				_memMap = o._memMap;
				o._memMap = INVALID_HANDLE_VALUE;

				_map = o._map;
				o._map = nullptr;

				_dataEvent = o._dataEvent;
				o._dataEvent = INVALID_HANDLE_VALUE;

				_lastTick = o._lastTick;
				o._lastTick = 0;

				_latest = o._latest;
				o._latest = 0;

				irsdkHeader = std::move(o.irsdkHeader);

				_callbackIf = std::move(o._callbackIf);

				channels = std::move(o.channels);

				return *this;
			}

			~CLiveDataLogger()
			{
				if (running)
					Stop();
			}

			virtual int GetLoggerSampleRate();
			virtual void Start();
			virtual void Stop();

			virtual bool GetSample(DataSample& s);
			virtual bool GetSample(DataSample& s, size_t idx) { return false; }

			virtual CDataChannel& GetChannel(std::string& name);

		private:

			void dataLoop();

			void simConnected();
			void simDisconnected();
			void simDataUpdate();

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
			ChannelsMap channels;
			ChannelsMap mathChannels;

			std::deque<DataSample> _samplesQueue;

			ILiveLoggingCallback& _callbackIf;
		};

	}
}
#endif // _LIVEDATALOGGER_H_