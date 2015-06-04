#pragma once
/*** file: DShow.h ***/

#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>
#include <Dshowasf.h>

#include "..\\TVOFilter\\ITVOFilterConfig.h"

// {656171CA-88B2-4cef-8B68-B8F632B8D616}
static const GUID CLSID_TVOFilter = 
{ 0x656171ca, 0x88b2, 0x4cef, { 0x8b, 0x68, 0xb8, 0xf6, 0x32, 0xb8, 0xd6, 0x16 } };

#define ONE_SECOND 10000000


	//	typedef struct VideoSyncData
	//	{
	//		int startFrame;	// start at video frame #:
	//		int endFrame;	// to video frame #:
	//		int startSample;	// data sample index to start with
	//		int endSample;		// data sample index last
	//		int dataSampleInc;	// data sample index increment (depending on video framerate to data tick rate)
	//	}VIDEO_SYNC_OPTIONS;

	//	typedef struct RenderConfigData {

	//		int outputWidth;	// target video width
	//		float scale;			// selected scale factor
	//		float transp;			// selected transparency value
	//		int margin;			// selected bounding border value
	//	}VIDEO_RENDER_OPTIONS;

	//	typedef struct FilterConfig {
	//		wchar_t* appPath;
	//		wchar_t* videoFileName;
	//		wchar_t* dataFile;

	//		void* DataLoggerObj;
	//		void* DashboardOverlayObj;

	//		VIDEO_SYNC_OPTIONS syncData;
	//		VIDEO_RENDER_OPTIONS renderCfg;				
	//}TVOFILTERCONFIG, *PTVOFILTERCONFIG;


#define WM_DS_PLAYBACK_GRAPH_NOTIFY	WM_USER+0x10
#define WM_DS_OVERLAY_GRAPH_NOTIFY	WM_USER+0x20
#define WM_DS_CAPTURE_GRAPH_NOTIFY	WM_USER+0x30

#define GRAPHID_PLAYBACK_GRAPH	0x1000
#define GRAPHID_OVERLAY_GRAPH	0x2000
#define GRAPHID_CAPTURE_GRAPH	0x4000

enum VideoStatus {
	VideoStopped =0,
	VideoRunning,
	VideoPaused
};

namespace DShow {

using namespace TVOFilter;

#pragma region // CInputPlayerGraph
class CInputPlayerGraph // input video playback graph
{
public:
	CSourcePlayerGraph(HWND parentWindow, wchar_t* videoFileName, HWND renderWindow, RECT& windowRect);
	~CSourcePlayerGraph();

	int Play();
	int Pause();
	int Stop();

	long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
	void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);

	bool CheckCanFrameSeek();
	bool SetFrameSeekMode();

	int GetVideoFrameRate();
	LONGLONG GetAvgFrameTime();

	BYTE* GetVideoImage();
	bool CheckCanSeek();
	int GetVideoDuration();
	int GetVideoPos();
	int SetVideoPos(int pos);
	int SetVideoFrame(int frame);
	int GetVideoSize(RECT* rc);

	void RepaintVideo(HDC hdc);

	DWORD GetLastError(HRESULT* lastComErr);

	VideoStatus GetPlaybackStatus();

	int m_isVideoOpen;

private:

	int BuildFilterGraph();
	void ReleaseFilterGraph(void);
	int GetVideoStatus();
	int GetSourceVideoInfo();

	HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
	HRESULT GetSourceFilter (IBaseFilter** ppFilter);

	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IMediaEventEx* m_pEvent;
	IMediaSeeking* m_pSeek;

	IBaseFilter* m_pVmr;
	IVMRWindowlessControl9* m_pWindowless;
	IVMRFilterConfig9* m_pConfig;

	AM_MEDIA_TYPE* m_sourceMediaType;
	VIDEOINFOHEADER* m_videoInfo;
	float m_sourceFrameRate;

	wchar_t* m_fname;	// w/o path
	wchar_t* m_VideoFileName;	// including path

	HWND m_parentWnd;	// (parent) window which receives DirectShow events/messages
	LONG_PTR m_graphId;

	HWND m_renderWnd;	// rendering window for VMR9 windowless mode
	RECT m_targetRect;	// rendering window rectangle (size)

	HRESULT hr;		// last COM error
	DWORD lastErr;	// last internal application error

	VideoStatus status;

	BOOLEAN m_canSeek;
	BOOLEAN m_seekModeFrame;

	DWORD m_seekCaps;

	REFERENCE_TIME m_duration;
	REFERENCE_TIME m_endPos;
	REFERENCE_TIME m_curPos;


};
#pragma endregion


//#pragma region // IDsFilterGraph interface
//	/*** This interface defines the public structure of a DirectShow filter graph Object. It
//	 *** is implemented in CBaseFilterGraph which represents the Object 
//	***/
//	struct IDsFilterGraph
//	{
//	public:
//		long GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2) =0;
//		void FreeDShowEventParams(long evCode, LONG_PTR param1,
//								  LONG_PTR param2) =0;
//		virtual void Start() =0;
//		virtual void Pause() =0;
//		virtual void Stop() =0;
//
//		virtual bool CheckCanSeek() =0;
//		virtual __int64 GetVideoDuration() =0;
//		virtual __int64 GetCurrentVideoPos() =0;
//		virtual void SetCurrentVideoPos(int pos) =0;
//
//	protected:
//	};
//
//#pragma endregion
//
//	class CBaseFilterGraph
//	{
//	private:
//
//	protected:
//		IGraphBuilder* m_pGraph;
//		IMediaControl* m_pControl;
//		IMediaEventEx* m_pEvent;
//		IMediaSeeking* m_pSeek;
//
//		virtual void buildGraph() =0;
//		virtual void configGraph() =0;
//		virtual void connectGraph() =0;
//		virtual void releaseGraph() =0;
//
//		HWND m_parentWnd;
//		LONG_PTR m_graphId;
//		REFERENCE_TIME m_duration;
//		bool m_canSeek;
//		
//
//	public:
//
//	};
//
//	class CBaseCaptureGraph
//	{
//	protected:
//		ICaptureGraphBuilder2* m_pGraphBuilder;
//		IBaseFilter* m_pTvoFilter;
//		ITVOFilterConfig* m_pTvoFilterCfg;
//		TVOFILTERCONFIG* m_filterCfg;
//	};
//
//	class CBaseEncoderGraph : public CBaseFilterGraph, public CBaseCaptureGraph
//	{
//	private:
//
//	protected:
//		wchar_t* m_outFileName;
//
//	public:	
//		virtual void SetProgressNotifyCallback(void* pNotifyCallback) =0;
//		virtual void StartEncoding() =0;
//		virtual void StopEncoding() =0;
//	};
//
//	class CWMEncoderGraph : public CBaseEncoderGraph
//	{
//	private:
//
//	protected:
//		IBaseFilter* m_pAsfWriter;
//		IConfigAsfWriter2* m_pAsfWriterConfig;
//		IFileSinkFilter2* m_pFileSink;
//
//		virtual void buildGraph();
//		virtual void configGraph();
//		virtual void releaseGraph();
//
//	public:
//		CWMEncoderGraph(wchar_t* filename, TVOFILTERCONFIG* cfg, HWND parent);
//		~CWMEncoderGraph();
//
//		virtual bool CheckCanSeek();
//		virtual __int64 GetVideoDuration();
//		virtual int GetCurrentVideoPos();
//		virtual int SetCurrentVideoPos(int pos);
//	};
//
//	class CRawEncoderGraph : public CBaseEncoderGraph
//	{
//	public:
//
//	protected:
//
//	private:
//
//	};
}