#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011 Thorsten Kuypers
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You should have obtained a copy of the License with this Software. If not,
//	you may obtain a copy of the License at
//
//				http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
// * version history:
//   -----------------
//
//	0.4 - first public release
//
///////////////////////////////////////////////////////////////////////////////

#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>
#include <Dshowasf.h>
#include <typeinfo.h>

/*****************************************************************************************************/
// these definitions are needed because dxtrans.h is no longer shipped with the Platform SDK nor
// the DirectX SDK, but Qedit.h is still dependant on dxtrans.h!!!
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

#include "Qedit.h"
/******************************************************************************************************/

#include <gdiplus.h>
#include <wingdi.h>

#include "..\\TVOFilter\\ITVOFilterConfig.h"
#include "..\\TVOFilter\\ProgressCallback.h"

#include "errex.h"

using namespace TVOFilter;

// {656171CA-88B2-4cef-8B68-B8F632B8D616}
static const GUID CLSID_TVOFilter = 
{ 0x656171ca, 0x88b2, 0x4cef, { 0x8b, 0x68, 0xb8, 0xf6, 0x32, 0xb8, 0xd6, 0x16 } };

#define __MODULE__ "libds.dll"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define WM_DS_PLAYBACK_GRAPH_NOTIFY	WM_USER+0x10
#define WM_DS_OVERLAY_GRAPH_NOTIFY	WM_USER+0x20
#define WM_DS_CAPTURE_GRAPH_NOTIFY	WM_USER+0x30

#define GRAPHID_PLAYBACK_GRAPH	0x1000
#define GRAPHID_OVERLAY_GRAPH	0x2000
#define GRAPHID_CAPTURE_GRAPH	0x4000

#define ONE_SECOND 10000000

typedef void (__stdcall * fnProgressCallback)(int);

namespace DShow {

#pragma unmanaged

using namespace Gdiplus;
using namespace TVOFilter;


typedef struct VideoEncoderConfig
{
	int encoderType;
	HWND parentWnd;
	void* wm9_profilerObj;
	void* videoCompressor;
	bool preview;

	wchar_t* appPath;
	wchar_t* srcVideoFileName;
	wchar_t* outVideoFileName;

	int videoWidth;
	int videoHeight;
	float aspectRatio;
	int framerate;

	void* dashboardLayoutObj;
	int dataLogTickRate;

	OVERLAY_CONFIG_DATA overlayCfg;
	VIDEO_SYNC_OPTIONS syncCfg;
}VIDEO_ENCODER_CONFIG, *PVIDEO_ENCODER_CONFIG;

enum VideoStatus {
	VideoStopped =0,
	VideoRunning,
	VideoPaused
};

class CPreviewPlayerGraph
{
public:
	CPreviewPlayerGraph(HWND parentWindow, DShow::TVOFILTERCONFIG* filterCfg, LONGLONG avgTimePerFrame);
	~CPreviewPlayerGraph();

	int Play();
	int Pause();
	int Stop();

	long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
	void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);

	int CheckCanSeek();
	bool CheckCanFrameSeek();

	int GetVideoPos();

	int RepaintVideo(HWND Window, RECT& windowRect);
	VideoStatus GetPlaybackStatus();
	BYTE* GetVideoImage();

private:

	void BuildFilterGraph(void);
	void ReleaseFilterGraph(void);
	int GetVideoStatus();

	void connectFilterGraph();

	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IMediaEventEx* m_pEvent;
	IMediaSeeking* m_pSeek;

	IBaseFilter* m_pVmr;
	IVMRWindowlessControl9* m_pWindowless;
	IVMRFilterConfig9* m_pConfig;
	IVideoWindow* m_pRenderWnd;
	IBaseFilter* m_pCsc;	// color-space-converter
	IBaseFilter* m_pAviDec;

	IBaseFilter* m_pTvoFilter;
	ITVOFilterConfig* m_pTvoFilterCfg;

	HWND m_parentWnd;	// (parent) window which receives DirectShow events/messages
	LONG_PTR m_graphId;

	HWND m_renderWnd;	// rendering window for VMR9 windowless mode
	RECT m_targetRect;	// rendering window rectangle (size)

	HRESULT hr;		// last COM error

	VideoStatus status;

	int m_canSeek;
	DWORD m_seekCaps;

	REFERENCE_TIME m_duration;
	LONGLONG m_avgTimePerFrame;
	REFERENCE_TIME m_endPos;
	REFERENCE_TIME m_curPos;

	TVOFILTERCONFIG* m_filterCfg;
};

class CInputPlayerGraph // input video playback graph
{
public:
	CInputPlayerGraph(HWND parentWindow, wchar_t* videoFileName, HWND renderWindow, RECT& windowRect, wchar_t* appRootPath);
	~CInputPlayerGraph();

	int Play();
	int Pause();
	int Stop();

	long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
	void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);

	//bool CheckCanFrameSeek();
	//bool SetFrameSeekMode();

	int GetVideoFrameRate();
	LONGLONG GetAvgFrameTime();

	BYTE* GetVideoImage();
	bool CheckCanSeek();
	__int64 GetVideoDuration();
	__int64 GetVideoPos();
	int SetVideoPos(__int64 frame);
	int GetVideoSize(RECT* rc);

	void RepaintVideo(HDC hdc);

	DWORD GetLastError(HRESULT* lastComErr);

	VideoStatus GetPlaybackStatus();

	int m_isVideoOpen;

private:

	void BuildFilterGraph();
	void ReleaseFilterGraph(void);
	int GetVideoStatus();
	int GetSourceVideoInfo();

	REFERENCE_TIME frames2time(REFERENCE_TIME frame);
	REFERENCE_TIME time2frames(REFERENCE_TIME time);

	void ConfigureFilterGraph();
	HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
	HRESULT GetSourceFilter (IBaseFilter** ppFilter);

	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) ;

	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IMediaEventEx* m_pEvent;
	IMediaSeeking* m_pSeek;

	IBaseFilter* m_pVmr;
	IVMRWindowlessControl9* m_pWindowless;
	IVMRFilterConfig9* m_pConfig;

	AM_MEDIA_TYPE* m_sourceMediaType;
	VIDEOINFOHEADER* m_videoInfo;
	__int64 m_sourceFrameRate;

	wchar_t* m_fname;	// w/o path
	wchar_t* m_VideoFileName;	// including path

	HWND m_parentWnd;	// (parent) window which receives DirectShow events/messages
	LONG_PTR m_graphId;

	HWND m_renderWnd;	// rendering window for VMR9 windowless mode
	RECT m_targetRect;	// rendering window rectangle (size)

	HRESULT hr;		// last COM error
	DWORD lastErr;	// last internal application error

	wchar_t* m_appRootPath;	// application root path

	VideoStatus status;

	bool m_canSeek;
	bool m_seekModeFrame;

	DWORD m_seekCaps;

	REFERENCE_TIME m_duration;
	REFERENCE_TIME m_framesCnt;
	REFERENCE_TIME m_endPos;
	REFERENCE_TIME m_curPos;
};


class CBaseEncoderGraph
{
protected:
	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IMediaEventEx* m_pEvent;
	IMediaSeeking* m_pSeek;

	IBaseFilter* m_pVmr;
	IVMRWindowlessControl9* m_pWindowless;
	IVMRFilterConfig9* m_pConfig;
	IVideoWindow* m_pRenderWnd;

	IBaseFilter* m_pTvoFilter;
	ITVOFilterConfig* m_pTvoFilterCfg;

	TVOFILTERCONFIG* m_filterCfg;
	wchar_t* m_outFileName;

	HWND m_parentWnd;	// (parent) window which receives DirectShow events/messages
	HWND m_renderWnd;	// rendering window for VMR9 windowless mode
	RECT m_targetRect;	// rendering window rectangle (size)

	LONG_PTR m_graphId;

	REFERENCE_TIME m_duration;
	REFERENCE_TIME m_endPos;
	REFERENCE_TIME m_startPos;

	VIDEOINFOHEADER* m_srcVideoInfo;

	HRESULT m_hr;

	int GetSourceVideoInfo();
	HRESULT GetSourceFilter(IBaseFilter** ppFilter) ;
	HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

	virtual void buildGraph() =0;
	virtual void configGraph() =0;
	virtual void connectGraph() =0;
	virtual void releaseGraph() =0;

public:
	CBaseEncoderGraph();
	~CBaseEncoderGraph();

	virtual long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
	virtual void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);
};


// rendering progress interface implementation
class CProgressCallback : IProgressCallback
{
	public:
		virtual void ProgressNotify(int curProgress);

		void SetCallbackRoutine(/*fnProgressCallback*/void* fnCallback);

	private:
		/*fnProgressCallback*/void* m_fnProgressCallback; // function pointer to higher progress callback routine
};

/*** IEncoder ***/
/** video encoder interface **/
struct IVideoEncoder {

	virtual void SetFileName(wchar_t* fileName) =0;
	virtual void SetEncoderConfig(PVIDEO_ENCODER_CONFIG cfg) =0;

	virtual int EncodeStart() =0;
	virtual void EncodeCancel() =0;

	virtual void SetProgressCallback(void* cb) =0;

	virtual long GetDShowEventCode(LONG_PTR*, LONG_PTR*) =0;
	virtual void FreeDShowEventParams(long, LONG_PTR, LONG_PTR) =0;

protected:
	// Progress info interface
	CProgressCallback* m_progressCb;

	bool m_preview;
	int m_previewLength;	// length of video if preview is rendered
};


}
