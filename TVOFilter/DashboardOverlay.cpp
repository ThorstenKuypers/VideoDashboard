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

//#include "stdafx.h"
#include "stdio.h"
#include "wchar.h"
#include "DashboardOverlay.h"

#pragma unmanaged

using namespace TVOFilter;


#pragma region // HERE BEGINS THE NEW IMPLEMENTATION OF THE OVERLAY RENDERING ENGINE
COverlayRender::COverlayRender(__int64 refFrame, __int64 refSample, int framerate, void* dataLogger)
{
	m_dataLog =(IDataLogging*)dataLogger;

	pLapTiming =NULL;

	this->m_frameRate =framerate;
	this->m_refVidFrame =refFrame;
	this->m_refSampleIdx =refSample;

}

COverlayRender::~COverlayRender(void)
{
	if (pLapTiming != NULL) {
		delete pLapTiming;
		pLapTiming = NULL;
	}

	if (this->m_imgPath != NULL) {
		delete m_imgPath;
		this->m_imgPath = NULL;
	}
	if (this->m_dashBaseImg) {
		delete this->m_dashBaseImg;
		this->m_dashBaseImg = NULL;
	}
}

void COverlayRender::RenderDashElements(int curFrameIdx, Bitmap* frame)
{
	int frameInc =(int)(curFrameIdx - m_refVidFrame);
    float n = (float)((float)frameInc * (((float)1.0f / (float)m_frameRate) / ((float)1.0f / (float)m_dataLog->GetSampleTickRate())));
	m_sampleBufIdx = (int)m_refSampleIdx + (int)round((float)n);

	float chData =0;

	//if (this->m_overlayBuf !=NULL)
	//{
	//	for (int i =0; i < this->m_elementCnt; i++)
	//	{
	//		std::wstring ws(m_overlayBuf[i].channel);
	//		std::string str(ws.begin(), ws.end());
	//		m_dataLog->GetChannelData(str, m_sampleBufIdx, (float*)&chData);

	//		RECT rc ={0};
	//		rc.left =m_overlayBuf[i].pos.x;
	//		rc.top =m_overlayBuf[i].pos.y;
	//		rc.right =m_overlayBuf[i].size.width;
	//		rc.bottom =m_overlayBuf[i].size.height;

	//		switch (this->m_overlayBuf[i].type)
	//		{
	//			case elementType_Bar:

	//				RenderElementBar(chData, (OVERLAY_ELEMENT_BAR*)this->m_overlayBuf[i].element, rc, frame);
	//				break;

	//			case elementType_Gauge:

	//				RenderElementGauge(chData, (OVERLAY_ELEMENT_GAUGE*)this->m_overlayBuf[i].element, rc, frame);
	//				break;
	//		}
	//	}
	//}
}

#pragma endregion
