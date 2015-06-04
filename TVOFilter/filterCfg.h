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

namespace TVOFilter {

#pragma region // TVOFilter configuration

	typedef struct VideoSyncData
	{
		int startFrame;	// start at video frame #:
		int endFrame;	// to video frame #:
		int startSample;	// data sample index to start with
		int endSample;		// data sample index last
		//int dataSampleInc;	// data sample index increment (depending on video framerate to data tick rate)
	}VIDEO_SYNC_OPTIONS;

	typedef struct OverlayConfigData {

		int posX;				// X-position of overlay
		int posY;				// Y-position of overlay
		float scale;			// selected scale factor
		float transp;			// selected transparency value
	}OVERLAY_CONFIG_DATA;

	typedef struct RenderConfigData
	{
		int width;
		int height;

	}VIDEO_RENDER_OPTIONS;

	typedef struct FilterConfig {
		wchar_t* appPath;
		//wchar_t* imgPath;
		wchar_t* videoFileName;

		void* DashboardLayoutObj;
		int dataLogTickRate;
		//wchar_t* DashboardOverlayFileName;

		VIDEO_SYNC_OPTIONS syncData;
		OVERLAY_CONFIG_DATA overlayCfg;
		VIDEO_RENDER_OPTIONS renderCfg;
	}TVOFILTERCONFIG, *PTVOFILTERCONFIG;

	typedef struct FilterData {
		int framerate;
		__int64 avgTimePerFrame;

	}FILTER_DATA, *PFILTER_DATA;

#pragma endregion

#pragma region // Callback function data block

	typedef struct ProgressCallbackDataBlock
	{
		void* currentVideoFrame;
		int width;
		int height;
		int stride;
		int pixelFmt;

		int sampleCnt;
		int sampleIdx;

	}PROGRESS_CALLBACK_DATA, *PPROGRESS_CALLBACK_DATA;

#pragma endregion

}