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

#include "includes.h"

#include "ITVOFilterConfig.h"
#include "ProgressCallback.h"
#include "libDataLogging.h"
#include "DashboardLayout.h"
#include "libLDF.h"

//using namespace std;
using namespace Gdiplus;
using namespace libLDF;
using namespace libDataLogging::DataLogger;

#define FILTER_NAME L"TVO Filter"

#define ONE_SECOND 10000000

// {656171CA-88B2-4cef-8B68-B8F632B8D616}
static const GUID CLSID_TVOFilter = 
{ 0x656171ca, 0x88b2, 0x4cef, { 0x8b, 0x68, 0xb8, 0xf6, 0x32, 0xb8, 0xd6, 0x16 } };

namespace TVOFilter {

	class CTVOFilter : public CTransformFilter, public ITVOFilterConfig
{
public:
    DECLARE_IUNKNOWN
	CTVOFilter(LPUNKNOWN pUnk, HRESULT *phr);
	~CTVOFilter(void);

    STDMETHODIMP NonDelegatingQueryInterface(REFIID iid, void **ppv);

	static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);

	virtual HRESULT CheckInputType(const CMediaType *mtIn);
	virtual HRESULT Transform(IMediaSample* pIn, IMediaSample* pOut);
	virtual HRESULT SetMediaType(PIN_DIRECTION direction, const CMediaType *pmt);
	virtual HRESULT GetMediaType(int iPos, CMediaType* pMediaType);
	virtual HRESULT CheckTransform(const CMediaType* mtIn, 
									const CMediaType* mtOut);
	virtual HRESULT DecideBufferSize(IMemAllocator * pAlloc, 
										ALLOCATOR_PROPERTIES *pProp);

	virtual HRESULT StopStreaming();
	HRESULT __stdcall Run(REFERENCE_TIME s);

	virtual int SetPreviewConfig(__int64 startFrame, int startSample, void* DataLogger);
	virtual int SetOverlay(void* overlay, int cnt);
	virtual int SetFilterConfig(void* cfg);
	virtual void SetNotifyCallback(IProgressCallback* notify);

private:
	ULONG_PTR m_gdiplusToken;

	VIDEOINFOHEADER m_vihIn;
	VIDEOINFOHEADER m_vihOut;

	PixelFormat m_pixFmt;
	int m_stride;

	TVOFILTERCONFIG* m_filterCfg;

	int m_framerate;
	__int64 m_avgTimePerFrame;

	__int64 m_refFrame; // reference video frame (absolute 0 in stream)
	int m_refSample;	// reference sample index (absolute 0 in samples)
	__int64 m_curFrame;	// current frame in stream (frame to be rendered)

	IDashboardLayout* m_dashOverlay;
	int m_sampleBufIdx;

	int m_flipInputImage;	// is the video image inverted, so we have to flip it?

	IProgressCallback* m_progressNotify;
	int m_progressCnt; // progress counter

};

}