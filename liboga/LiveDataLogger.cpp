#include "LiveDataLogger.h"


using namespace libOGA::LiveLogs;

using namespace libOGA;

CLiveDataLogger::CLiveDataLogger(ILiveLoggingCallback& callback) :
_callbackIf(callback)
{
	mathChannels.emplace(std::make_pair(CH_SP_SPEED_KPH, CDataChannel(CH_SP_SPEED_KPH, "kph", "Ground speed in KPH", 0, irsdk_float, 0, ChannelType_Math)));
	mathChannels.emplace(std::make_pair(CH_SP_SPEED_MPH, CDataChannel(CH_SP_SPEED_MPH, "mph", "Ground speed in MPH", 1, irsdk_float, 1, ChannelType_Math)));
	mathChannels.emplace(std::make_pair(CH_SP_LATG, CDataChannel(CH_SP_LATG, "g", "Lateral G forces", 2, irsdk_float, 2, ChannelType_Math)));
	mathChannels.emplace(std::make_pair(CH_SP_LONGG, CDataChannel(CH_SP_LONGG, "g", "Longitudinal G forces", 3, irsdk_float, 3, ChannelType_Math)));
	mathChannels.emplace(std::make_pair(CH_SP_BEST_LAP, CDataChannel(CH_SP_BEST_LAP, "", "Best lap in stint", 4, irsdk_int, 4, ChannelType_Math)));
	mathChannels.emplace(std::make_pair(CH_SP_BEST_LAP_TIME, CDataChannel(CH_SP_BEST_LAP_TIME, "s", "Best lap time in stint", 5, irsdk_double, 5, ChannelType_Math)));
}

void CLiveDataLogger::simConnected()
{
	// enumerate all channels and extract its information to a channels descriptor list
	if (irsdkHeader != nullptr) {

		_sampleRate = irsdkHeader->tickRate;

		for (int i = 0; i < irsdkHeader->numVars; i++) {

			irsdk_varHeader* hdr = (irsdk_varHeader*)((BYTE*)_map + (irsdkHeader->varHeaderOffset + (i*sizeof(irsdk_varHeader))));
			if (hdr != nullptr)
			{
				channels.emplace(std::make_pair(std::string(hdr->name),
					CDataChannel(std::string(hdr->name),
					std::string(hdr->unit),
					std::string(hdr->desc),
					i,
					hdr->type,
					hdr->offset,
					ChannelType_File)));
			}
		}
	}

	_callbackIf.SimConnected();
}

void CLiveDataLogger::simDisconnected()
{
	_callbackIf.SimDisconnected();
}

void CLiveDataLogger::simDataUpdate()
{
	if (_map != nullptr) {
		BYTE* buf = (BYTE*)((BYTE*)_map + irsdkHeader->varBuf[_latest].bufOffset);
		int len = irsdkHeader->bufLen;
		DataSample sample;
		sample = DataSample(buf, buf + len);

		std::unique_lock<std::mutex> ql;

		if (_samplesQueue.size() < 3)
			_samplesQueue.push_front(sample);
		else {
			_samplesQueue.pop_back();
			_samplesQueue.push_front(sample);
		}

		_callbackIf.SimDataUpdate(sample);
	}

#ifdef _DEBUG
	int x = 0;
#endif
}

void CLiveDataLogger::Start()
{
	running = true;

	std::function<void()> f = std::bind(&CLiveDataLogger::dataLoop, this);
	_loop = std::thread(f);

	// detaching the thread makes it independant from its context and thread handle
	// so when the class goes out of scope the thread continues to run!!! with
	// invalid data copies of the class!

	// to detach the thread it MUST have its own data context (e.g. shared_ptr) that handles reference count, thus
	// if the class goes out of scope the reference count drops by 1
	// and when the thread exits it drops to 0 and the shared_ptr object is destroyed!!!

	// USE detach ONLY if the thread has its own data context and exit/abort condition(s)!!!
	//_loop.detach(); <-- DO NOT ENABLE THIS - see above
}

void CLiveDataLogger::Stop()
{
	if (running) {
		running = false;

		if (_loop.joinable())
			_loop.join(); // wait for the data pump thread to cleanup and exit
	}
}

int CLiveDataLogger::GetSampleTickRate()
{
	return irsdkHeader->tickRate;
}

int CLiveDataLogger::ShiftRpm()
{
	return 0;
}

int CLiveDataLogger::RedlineRpm()
{
	return 0;
}

void CLiveDataLogger::dataLoop()
{
	while (running)
	{
		if (!running)
			break;

		if (!_initialized) {

			if (_memMap == NULL)
				_memMap = OpenFileMappingA(FILE_MAP_READ, false, IRSDK_MEMMAPFILENAME);

			if (_memMap != NULL) {

				if (_map == nullptr)
					_map = (BYTE*)MapViewOfFile(_memMap, FILE_MAP_READ, 0, 0, 0);

				if (_map != nullptr) {

					if (_dataEvent == NULL)
						_dataEvent = OpenEventA(SYNCHRONIZE, false, IRSDK_DATAVALIDEVENTNAME);

					if (_dataEvent != NULL) {

						_initialized = true;
						_lastTick = INT_MAX;

						if (!irsdkHeader)
							irsdkHeader = (irsdk_header*)_map;
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500)); // retry in 0.5 seconds
		}
		else {

			if (irsdkHeader != nullptr) {

				// we have been successfully initialized so the mapping has its address and 
				// a handle to the data event is open
				// check if sim is already connected
				if (irsdkHeader->status & irsdk_StatusField::irsdk_stConnected) {
					// now we are fully connected to the sim
					if (!isConnected) {
						isConnected = true;
						simConnected();
					}

					// now we need to wait until the event signals new data
					WaitForSingleObject(_dataEvent, 8); // wait for 20ms 

					// try to recieve latest data
					// first find latest valid buffer

					// NOTE: no need for a lock since the called function runs on the same thread
					// so no changes to data possible during that time!
					_latest = 0;

					for (int i = 1; i < irsdkHeader->numBuf; i++) {

						if (irsdkHeader->varBuf[_latest].tickCount < irsdkHeader->varBuf[i].tickCount)
							_latest = i;
					}

					if (_lastTick == INT_MAX)
						_lastTick = irsdkHeader->varBuf[_latest].tickCount;

					if (irsdkHeader->varBuf[_latest].tickCount > _lastTick) {
						_lastTick = irsdkHeader->varBuf[_latest].tickCount;

						simDataUpdate();

					}
				}
				else {

					if (isConnected) {

						isConnected = false;
						_initialized = false;
						simDisconnected();
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::microseconds(2));
		}

	}
	if (_dataEvent != NULL) {
		CloseHandle(_dataEvent);
		_dataEvent = NULL;
	}
	if (_map != nullptr)
	{
		UnmapViewOfFile(_map);
		_map = nullptr;
		irsdkHeader = nullptr;
	}
	if (_memMap != NULL)
	{
		CloseHandle(_memMap);
		_memMap = NULL;
	}

#ifdef _DEBUG
	int x = 0;
#endif
}

bool CLiveDataLogger::GetSample(DataSample& s)
{
	if (!_samplesQueue.empty()) {

		std::unique_lock<std::mutex> ql;

		DataSample _s = _samplesQueue.front();
		s = std::move(_s);

		ql.unlock();
		return true;
	}

	return false;
}

CDataChannel& CLiveDataLogger::GetChannel(std::string& name)
{
	try {

		// check if name is one of the math channels
		if (name == CH_SP_SPEED_KPH || name == CH_SP_SPEED_MPH || name == CH_SP_LATG || name == CH_SP_LONGG || name == CH_SP_LAP_TIME ||
			name == CH_SP_LAP || name == CH_SP_BEST_LAP_TIME || name == CH_SP_BEST_LAP || name == CH_SP_CURRENT_SECTOR || name == CH_SP_SECTOR_TIME)
			return mathChannels[name];
		else
			return channels[name];
	}
	catch (std::exception)
	{
		throw;
	}

}
