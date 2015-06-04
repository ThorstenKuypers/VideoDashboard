#include "LiveDataLogger.h"
#include <memory>
#include <sstream>

using namespace libDataLogging::LiveLogging;


CLiveDataLogger::CLiveDataLogger()
{
	running = false;
	isConnected = false;
	_initialized = false;
	_sampleRate = -1;
	_memMap = NULL;
	_map = nullptr;
	_dataEvent = NULL;
	_lastTick = INT_MAX;
	_latest = 0;
	irsdkHeader = nullptr;
}

CLiveDataLogger::~CLiveDataLogger()
{
	if (running)
		Stop();
}

void CLiveDataLogger::_simConnected()
{
	// enumerate all channels and extract its information to a channels descriptor list
	if (irsdkHeader != nullptr) {

		_sampleRate = irsdkHeader->tickRate;

		for (int i = 0; i < irsdkHeader->numVars; i++) {

			irsdk_varHeader* hdr = (irsdk_varHeader*)((BYTE*)_map + (irsdkHeader->varHeaderOffset + (i*sizeof(irsdk_varHeader))));
			if (hdr != nullptr)
			{
				channelList[std::string(hdr->name)] = LiveChannelDescriptor(std::string(hdr->name), std::string(hdr->desc), std::string(hdr->unit), hdr->type, i);
				AvailableChannels[std::string(hdr->name)] = LiveChannelDescriptor(std::string(hdr->name), std::string(hdr->desc), std::string(hdr->unit), hdr->type, i);
				//switch (hdr->type)
				//{
				//case ChannelType::irsdk_bool:
				//	channelData[i] = static_cast<BaseLiveChannel>(LiveChannel<bool>(mtx));
				//	break;
				//case ChannelType::irsdk_char:
				//	channelData[i] = static_cast<BaseLiveChannel>(LiveChannel<char>(mtx));
				//	break;
				//case ChannelType::irsdk_double:
				//	channelData[i] = static_cast<BaseLiveChannel>(LiveChannel<double>(mtx));
				//	break;
				//case ChannelType::irsdk_float:
				//	channelData[i] = static_cast<BaseLiveChannel>(LiveChannel<float>(mtx));
				//	break;
				//case ChannelType::irsdk_int:
				//	channelData[i] = static_cast<BaseLiveChannel>(LiveChannel<int>(mtx));
				//	break;
				//}
			}
		}
	}

	EventArgs e;
	e.Channels = channelList;
	Connected(e);
}

void CLiveDataLogger::_simDisconnected()
{
	EventArgs e;
	Disconnected(e);
}

void CLiveDataLogger::_simDataUpdate(DWORD data)
{
	SampleDataUpdateEventArgs e;

	if (_map != nullptr) {
		BYTE* buf = (BYTE*)((BYTE*)_map + irsdkHeader->varBuf[_latest].bufOffset);

		for (int i = 0; i < irsdkHeader->numVars; i++)
		{
			irsdk_varHeader* hdr = (irsdk_varHeader*)((BYTE*)_map + (irsdkHeader->varHeaderOffset + (i*sizeof(irsdk_varHeader))));
			if (hdr != nullptr)
			{
				switch (hdr->type)
				{
				case ChannelType::irsdk_bool:
					e.ChannelData[i].type = hdr->type;
					e.ChannelData[i].SampleData._bool = *((int*)(buf + hdr->offset));
					break;
				case ChannelType::irsdk_char:
					e.ChannelData[i].type = hdr->type;
					e.ChannelData[i].SampleData._char = *((char*)(buf + hdr->offset));
					break;
				case ChannelType::irsdk_double:
					e.ChannelData[i].type = hdr->type;
					e.ChannelData[i].SampleData._double = *((double*)(buf + hdr->offset));
					break;
				case ChannelType::irsdk_float:
					e.ChannelData[i].type = hdr->type;
					e.ChannelData[i].SampleData._float = *((float*)(buf + hdr->offset));
					break;
				case ChannelType::irsdk_int:
					e.ChannelData[i].type = hdr->type;
					e.ChannelData[i].SampleData._int = *((int*)(buf + hdr->offset));
					break;
				}
			}
		}

		e.Channels = channelList;
		DataUpdate(e);
	}

#ifdef _DEBUG
	int x = 0;
#endif
}

void CLiveDataLogger::Start()
{
	running = true;

	std::function<void()> f = std::bind(&CLiveDataLogger::_dataLoop, this);
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

int CLiveDataLogger::GetLoggerSampleRate()
{
	return -1;
}

void CLiveDataLogger::_dataLoop()
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
						_simConnected();
					}

					// now we need to wait until the event signals new data
					WaitForSingleObject(_dataEvent, 8); // wait for 20ms 

					// try to recieve latest data
					// first find latest valid buffer

					// NOTE: no need for a lock since the called function runs on the same thread
					// so no changes to data possible during that time!
					//data->mtx.lock();
					_latest = 0;

					for (int i = 1; i < irsdkHeader->numBuf; i++) {

						if (irsdkHeader->varBuf[_latest].tickCount < irsdkHeader->varBuf[i].tickCount)
							_latest = i;
					}

					if (_lastTick == INT_MAX)
						_lastTick = irsdkHeader->varBuf[_latest].tickCount;

					if (irsdkHeader->varBuf[_latest].tickCount > _lastTick) {
						_lastTick = irsdkHeader->varBuf[_latest].tickCount;

						_simDataUpdate((DWORD)_map);

					}
				}
				else {

					if (isConnected) {

						isConnected = false;
						_initialized = false;
						_simDisconnected();
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
