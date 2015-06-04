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

#include <windows.h>
#include <gdiplus.h>
#include <cmath>

#include "filterCfg.h"
#include "..\\libDataLogging\\libDataLogging.h"
#include "..\\libLDF\ldf.h"


using namespace std;
using namespace Gdiplus;
using namespace TVOFilter;
using namespace libDataLogging::DataLogger;
//using namespace LayoutDefinitionFormat::Native;

#define M_PI	3.141592654f

namespace TVOFilter {

typedef struct _LapTimingInfo {
	float syncStartTimeIdx;
	float lapStartTimeIdx;
	int currentLap;
	int currentLapIdx;
	int lastLap;
	float currentLapTime;
	float lastLapTime;

	float fastestStintLapTime;
	float fastestSessionLapTime;

}LAP_TIMING_INFO, *PLAP_TIMING_INFO;


#pragma region // Class COverlayRender
class COverlayRender
{
public:
	COverlayRender(TVOFILTERCONFIG* tvoCfg, FILTER_DATA* filterData);

	// * added overloaded contructor for experimental version
	// TODO: remove old constructor once the new version is finally implemented.
	COverlayRender(__int64 refFrame, __int64 refSample, int framerate, void* dataLogger);

	~COverlayRender(void);

	//void SetOverlayLayout(OVERLAY_ELEMENT* buf, int cnt);
	//void Test();
	void RenderDashElements(int curFrameIdx, Bitmap* frame);

	// rcSource: input video (original) frame size
	// rcTarget: output video frame size (on input)
	//			 overlay pos and size (on output)
	//Bitmap* GetOverlay2(Rect rcSource, Rect* rcTarget, ColorMatrix* mtx);

	//Bitmap* GetOverlay(Rect rcSource, Rect* rcTarget);
	//void ReleaseOverlay(Bitmap* overlay);

	//int GetDataLogSampleCount();

private:
/*******************************************************************************************************
** NEW OVERLAY DATA
*******************************************************************************************************/
	IDataLogging* m_dataLog;
	//OVERLAY_ELEMENT* m_overlayBuf; // overlay layout buffer (array)
	//int m_elementCnt;	// number of elements in buffer

/*********************** OLD ***********************************/
	TVOFILTERCONFIG* m_filterCfg;
	//DASHBOARD_OVERLAY* m_dashboard;
	int m_sampleBufIdx;
	int m_sampleBufInc;
	int avgTimePerFrame;
	bool eol;	// end-of-lap marker;

	__int64 m_refSampleIdx;
	__int64 m_refVidFrame;

	int m_curFrameInStream;
	int m_frameRate;
	float m_sample2FrameRateRatio;	// ratio of sample rate to frame rate

	PLAP_TIMING_INFO pLapTiming;
	
	wchar_t* m_imgPath;

	Bitmap* m_dashImg;
	Bitmap* m_dashBaseImg;
	Bitmap* m_needleBaseImg;
	Bitmap** m_elementImgs;	// array belonging to DashElementBuf
	
	Graphics* m_overlayGfx;

	//void loadBaseImages();
	//void drawOverlayElements();
	//void drawElement(wchar_t* filename, DASH_ELEMENT_GAUGE& element);
	//void applyRenderCfg(Rect& src, Rect* tgt);
	//void updateOverlay();
	//void drawF1Overlay();

	//void updateSpeed(float speed, DASH_ELEMENT_GAUGE& element);
	//void updateRpm(float rpm, DASH_ELEMENT_GAUGE& element);
	//void updateSlider(float val, DASH_ELEMENT_SLIDER& element);
	//void updateTextBox(float val, DASH_ELEMENT_TEXTBOX& element);

	// -- @ v0.8.1 -->
	//void RenderElementDisplay(PDATASAMPLE sample, DASH_ELEMENT_DISPLAY& element);
	//void RenderElementGCircle(PDATASAMPLE sample, DASH_ELEMENT_GCIRCLE& element);
	//void RenderElementLabel(PDATASAMPLE sample, DASH_ELEMENT_LABEL& element);
	//void RenderElementSlider(PDATASAMPLE sample, DASH_ELEMENT_SLIDER& element);
	//void RenderElementTextBox(PDATASAMPLE sample, DASH_ELEMENT_TEXTBOX& element);
	//void RenderElementSweeper(PDATASAMPLE sample, DASH_ELEMENT_SWEEPER& element);
	//void RenderElementLed(PDATASAMPLE sample, DASH_ELEMENT_LED& element);

	// -- @v0.9 -->
	//void RenderElementBar(float chData, OVERLAY_ELEMENT_BAR* e, RECT bounds, Bitmap* frame);
	//void RenderElementGauge(float chData, OVERLAY_ELEMENT_GAUGE* g, RECT bounds, Bitmap* frame);


	wchar_t* FormatLapTimeString(float lapTime, int* strlen, bool def);

	void RenderDashElements();

	wchar_t* GetFullFileName(wchar_t* fname);

	// <-- //


	//void CreateBasicOverlay();

	//void DrawSpeedGauge(float speed);
	//void DrawRpmGauge(int rpm);
	//void DrawThrottleBar(float pos);
	//void DrawBrakeBar(float pos);
	//void DrawGCircle(float gLat, float gLong);
	//void DrawGearDisplay(int gear);
	//void DrawLapTime(float time);
};
#pragma endregion

}