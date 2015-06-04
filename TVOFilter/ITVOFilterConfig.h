#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVOFilter -- DirectShow Transform Filter implementation
//
//	Copyright 2011-2012 Thorsten Kuypers
//	All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

#include "filterCfg.h"
#include "ProgressCallback.h"

namespace TVOFilter {

// {C0EA2171-8CA1-478a-9A1E-93751EB18131}
static const GUID IID_ITVOFilterConfig = 
{ 0xc0ea2171, 0x8ca1, 0x478a, { 0x9a, 0x1e, 0x93, 0x75, 0x1e, 0xb1, 0x81, 0x31 } };

extern "C++" {
	struct ITVOFilterConfig : public IUnknown {
public:
	virtual int SetFilterConfig(void* cfg) =0;
	virtual int SetPreviewConfig(__int64 startFrame, int startSample, void* DataLogger) =0;
	//virtual int SetOverlay(void* overlay, int cnt) =0;
	virtual void SetNotifyCallback(IProgressCallback* notify) =0;

protected:
	//void* m_filterConf;

	//void* m_NotifyCallback;
};
}
}