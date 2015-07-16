#ifndef _LIVE_DASH_H_
#define _LIVE_DASH_H_


#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <mutex>

#include "libLDF.h"
#include "liboga.h"
#include "config.h"


struct ImageInfo
{
	UINT width;
	UINT height;
	UINT stride;
	Gdiplus::PixelFormat pixelFmt;
	BYTE* scan0;
};


class CLiveDashboard : public libOGA::LiveLogs::ILiveLoggingCallback
{
public:
	CLiveDashboard();
	~CLiveDashboard();

	CLiveDashboard(CLiveDashboard&) = delete;
	CLiveDashboard(CLiveDashboard&& o)
	{

	}

	CLiveDashboard& operator=(CLiveDashboard&) = delete;
	CLiveDashboard& operator=(CLiveDashboard&& o)
	{

	}

	void OnStartStream();
	void OnStopStream();
	void OnStartStreaming();
	void OnStopStreaming();

	virtual void SimConnected();
	virtual void SimDataUpdate(libOGA::DataSample& sample);
	virtual void SimDisconnected();

	bool IsSimConnected() { return _simConnected; }
	LIVEDASH_CONFIG& GetConfig() { return _cfg; }

	libOGA::DataSample& GetLatestSample()
	{
		std::lock_guard<std::mutex> lock{ mtx };
		return _latestSample;
	}

	void Init();

private:
	libOGA::LiveLogs::ILiveDataLogger* _logger;
	libLDF::IDashboardLayout* _dash;
	bool _simConnected;
	LIVEDASH_CONFIG _cfg;
	std::mutex mtx;
	libOGA::DataSample _latestSample;
};


ImageSource* STDCALL CreateLDImage(XElement* data);

#endif // _LIVE_DASH_H_