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

//inline int round(float x) { return (int)(floor(x + 0.5)); }


#pragma region // HERE BEGINS THE NEW IMPLEMENTATION OF THE OVERLAY RENDERING ENGINE
COverlayRender::COverlayRender(__int64 refFrame, __int64 refSample, int framerate, void* dataLogger)
{
	m_dataLog =(IDataLogging*)dataLogger;

	m_overlayBuf =NULL;
	m_elementCnt =0;

	pLapTiming =NULL;

	this->m_frameRate =framerate;
	this->m_refVidFrame =refFrame;
	this->m_refSampleIdx =refSample;

	this->m_imgPath =NULL;
	this->m_dashBaseImg =NULL;
}

void COverlayRender::SetOverlayLayout(OVERLAY_ELEMENT* buf, int cnt)
{
	if (buf !=NULL && cnt > 0)
	{
		m_overlayBuf =buf;
		m_elementCnt =cnt;
	}
}

void COverlayRender::RenderDashElements(int curFrameIdx, Bitmap* frame)
{
	int frameInc =(int)(curFrameIdx - m_refVidFrame);
    float n = (float)((float)frameInc * (((float)1.0f / (float)m_frameRate) / ((float)1.0f / (float)m_dataLog->GetSampleTickRate())));
	m_sampleBufIdx = m_refSampleIdx + (int)round((float)n);

	float chData =0;

	if (this->m_overlayBuf !=NULL)
	{
		for (int i =0; i < this->m_elementCnt; i++)
		{
			std::wstring ws(m_overlayBuf[i].channel);
			std::string str(ws.begin(), ws.end());
			m_dataLog->GetChannelData(str, m_sampleBufIdx, (float*)&chData);

			RECT rc ={0};
			rc.left =m_overlayBuf[i].pos.x;
			rc.top =m_overlayBuf[i].pos.y;
			rc.right =m_overlayBuf[i].size.width;
			rc.bottom =m_overlayBuf[i].size.height;

			switch (this->m_overlayBuf[i].type)
			{
				case elementType_Bar:

					RenderElementBar(chData, (OVERLAY_ELEMENT_BAR*)this->m_overlayBuf[i].element, rc, frame);
					break;

				case elementType_Gauge:

					RenderElementGauge(chData, (OVERLAY_ELEMENT_GAUGE*)this->m_overlayBuf[i].element, rc, frame);
					break;
			}
		}
	}
}

void COverlayRender::RenderElementGauge(float chData, OVERLAY_ELEMENT_GAUGE* g, RECT bounds, Bitmap* frame)
{
	if (g !=NULL)
	{
        int w = bounds.right;
		int h = bounds.bottom;
        int x = bounds.left;
        int y = bounds.top;

	}
}

void COverlayRender::RenderElementBar(float chData, OVERLAY_ELEMENT_BAR* e, RECT bounds, Bitmap* frame)
{
	float val =chData;

	if (e !=NULL)
	{		
        int w = bounds.right;
		int h = bounds.bottom;
        int x = bounds.left;
        int y = bounds.top;

		Bitmap bmp(w, h, PixelFormat32bppARGB);
        //if (bmp != null)
        //{
		Graphics* gfx = Graphics::FromImage(&bmp);
        SolidBrush br(Color(e->backColor.r, e->backColor.g, e->backColor.b));
        gfx->FillRectangle(&br, 0, 0, w, h);

		if (val > e->valRange.max)
            val = e->valRange.max;
		else if (val < e->valRange.min)
            val = e->valRange.min;

        float inc = 0;
        float range = (float)(abs(e->valRange.min) + abs(e->valRange.max));
        float v = 0;    // intermediate var for calculating actual value range w.r.t to min/max value

		switch (e->type)
        {
                // TODO: check that all variations of this element are drawn correctly
                //       fix relations between min/max values

            case SliderType_min_max:
				if (e->horizontal)
                {
                    inc = (float)((float)w / (float)range);
                    int bar = (int)round((float)inc * (float)val);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    gfx->FillRectangle(&br, 0, 0, bar, h);
                }
                else
                {
                    inc = (float)((float)h / (float)range);
                    int bar = (int)round((float)inc * (float)val);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    gfx->FillRectangle(&br, 0, (h - bar), w, bar);
                }
                break;

            case SliderType_max_min:
                //if (m_maxVal < 0)
                //    v = (float)Math.Abs(val - m_maxVal);
                //else
                //    v = (float)Math.Abs(val) + (float)Math.Abs(m_maxVal);
				v = (float)abs(e->valRange.max - val);

                if (e->horizontal)
                {
                    inc = (float)((float)w / (float)range);
                    int bar = (int)round((float)inc * (float)v);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    gfx->FillRectangle(&br, (w - bar), 0, bar, h);
                }
                else
                {
                    inc = (float)((float)h / (float)range);
                    int bar = (int)round((float)inc * (float)val);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    gfx->FillRectangle(&br, 0, 0, w, bar);
                }
                break;

            case SliderType_min0max:
                if (e->horizontal)
                {
                    inc = (float)((float)w / (float)range);
					float zp = (float)(e->valRange.min * inc);
                    if (zp < 0)
                        zp *= -1;
                    int bar = (int)round((float)inc * (float)val);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    if (val>0)
                        gfx->FillRectangle(&br, (int)zp, 0, bar, h);
                    else
                        gfx->FillRectangle(&br, (int)(zp + (float)bar), 0, bar * -1, h);
                }
                else
                {
                    inc = (float)((float)h / (float)range);
					float zp = (float)(e->valRange.min * inc);
                    if (zp < 0)
                        zp *= -1;
                    int bar = (int)round((float)inc * (float)val);

					SolidBrush br(Color(e->fillColor.r, e->fillColor.g, e->fillColor.b));
                    if (val > 0)
                        gfx->FillRectangle(&br, 0, (int)(zp - (float)bar), w, bar);
                    else
                        gfx->FillRectangle(&br, (float)0, (float)zp, (float)w, (float)(bar) * -1.0f);
                }
                break;
        }

		if (e->useFrame)
        {
			Pen pen(Color(e->frameColor.r, e->frameColor.g, e->frameColor.b), (float)e->frameWidth);

            gfx->DrawRectangle(&pen, (float)((float)e->frameWidth / 2.0f), ((float)e->frameWidth / 2.0f), (float)((float)w - (float)e->frameWidth), (float)((float)h - (float)e->frameWidth));
        }

		if (e->useLabel)
        {
			float fh =(float)(e->labelFontHeight * (float)frame->GetWidth());
			Font fnt((WCHAR*)e->labelFontName, (float)fh, (FontStyle)e->labelFontStyle, UnitPoint);
            SolidBrush tbr(Color(e->labelTextColor.r, e->labelTextColor.g, e->labelTextColor.b));

			RectF sz;
			StringFormat sf;
			sf.SetAlignment(StringAlignmentCenter);

			gfx->MeasureString(e->labelText, wcslen(e->labelText), &fnt, PointF(0.0f, 0.0f), &sf, &sz);
            gfx->DrawString(e->labelText, wcslen(e->labelText), &fnt, PointF((float)(w - sz.Width) / 2.0f, (float)0), &tbr);
        }
        
		Graphics* g =Graphics::FromImage(frame);
		g->DrawImage(&bmp, x, y, bmp.GetWidth(), bmp.GetHeight());

		delete g;
		g =NULL;
        delete gfx;
        gfx = NULL;
	}
}

#pragma endregion


//COverlayRender::COverlayRender(TVOFILTERCONFIG* tvoCfg, FILTER_DATA* filterData)
//{
//	if (tvoCfg ==NULL)
//		throw -1;
//
//	this->eol =false;
//	this->m_filterCfg =tvoCfg;
//	this->m_dashboard =(DASHBOARD_OVERLAY*)tvoCfg->DashboardOverlayObj;
//	this->m_dataLog =(IDataLogging*)tvoCfg->DataLoggerObj;
//	this->m_imgPath =NULL;
//	this->m_dashBaseImg =NULL;
//
//	loadBaseImages();
//
//	// setup 
//	//////////////////////////////////////////////////////////////////////////////////
//	this->m_sampleBufIdx =tvoCfg->syncData.startSample;
//	//////////////////////////////////////////////////////////////////////////////////
//	avgTimePerFrame =filterData->avgTimePerFrame;
//	m_frameRate =filterData->framerate;
//
//	this->m_refSampleIdx =tvoCfg->syncData.startSample;
//	this->m_refVidFrame =tvoCfg->syncData.startFrame;
//	this->m_curFrameInStream =m_refVidFrame;
//
//	pLapTiming =new LAP_TIMING_INFO;
//	memset(pLapTiming, 0, sizeof(LAP_TIMING_INFO));
//	pLapTiming->syncStartTimeIdx =(float)(this->m_dataLog->GetSample(this->m_sampleBufIdx))->time;
//	pLapTiming->currentLap =(this->m_dataLog->GetSample(this->m_sampleBufIdx))->LapNo;
//
//	// find the lap index of the current lap and receive the corresponding lapInfo struct from the array
//	PLAPINFO lap =NULL;
//	for (int i =0; i < this->m_dataLog->GetSessionLapCount(); i++) {
//		lap =this->m_dataLog->GetLapInfo(i);
//		if (lap) {
//			if ((this->m_dataLog->GetSample(this->m_sampleBufIdx))->LapNo ==lap->lapNo) {
//				pLapTiming->currentLapIdx =i;
//				break;
//			}
//		}
//	}
//	// setup other members of timing struct
//	pLapTiming->lapStartTimeIdx =(float)lap->SessTimeIdx;
//	// calculate current time into the lap
//	pLapTiming->currentLapTime =(float)((float)pLapTiming->syncStartTimeIdx - (float)pLapTiming->lapStartTimeIdx);
//
//	// get information about previous lap, if available
//	if (pLapTiming->currentLapIdx > 0) {
//		lap =this->m_dataLog->GetLapInfo(pLapTiming->currentLapIdx-1);
//
//		pLapTiming->lastLap =lap->lapNo;
//		pLapTiming->lastLapTime =(float)((float)pLapTiming->lapStartTimeIdx - (float)lap->SessTimeIdx);
//	}
//
//	this->m_sampleBufInc =this->m_dataLog->GetSampleTickRate() / filterData->framerate;
//}
//
//void COverlayRender::loadBaseImages(void)
//{
//	//int len =wcslen(this->m_filterCfg->appPath) + wcslen(L"\\dashboards\\images\\")+2;
//	//this->m_imgPath =new wchar_t[len];
//	//wcscpy_s(this->m_imgPath, len, this->m_filterCfg->appPath);
//	//wcscat_s(this->m_imgPath, len, L"\\dashboards\\images\\");
//
//	int len = wcslen(this->m_filterCfg->imgPath) + wcslen(this->m_dashboard->fileName) +2; 
//
//	wchar_t* file =new wchar_t[len];
//	wcscpy_s(file, len, this->m_filterCfg->imgPath);
//	wcscat_s(file, len, this->m_dashboard->fileName);
//
//	this->m_dashBaseImg =new Bitmap(file);
//	if (this->m_dashBaseImg ==NULL)
//		throw "base dashboard image not found! Bad Path!";
//
//	delete file;
//	file =NULL;
//
//}
//
//void COverlayRender::drawF1Overlay()
//{
//	PDATASAMPLE sample =this->m_dataLog->GetSample(this->m_sampleBufIdx);
//	PDASHELEMENT* dashElem =(PDASHELEMENT*)this->m_dashboard->pDashElementBuf;
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//
//	if (sample !=NULL) {
//		// m_dashImg is empty for now, so render only throttle, brake and sweeper
//		for (int i=0; i < this->m_dashboard->DashElementBufCnt; i++) {
//			if (dashElem[i]->type ==elementType_Slider)
//			{
//				dashElem[i]->slider.channelName = dashElem[i]->id;
//				RenderElementSlider(sample, dashElem[i]->slider);
//			}
//			if (dashElem[i]->type ==elementType_Sweeper)
//			{
//				dashElem[i]->sweeper.channelName = dashElem[i]->id;
//				RenderElementSweeper(sample, dashElem[i]->sweeper);
//			}
//		}
//
//		// draw dashboard image over the already rendered elements
//		if (gfx) 
//		{
//			gfx->DrawImage(this->m_dashBaseImg, 0, 0, this->m_dashBaseImg->GetWidth(), this->m_dashBaseImg->GetHeight());
//
//			// now render the rest of the elements
//			for (int i=0; i < this->m_dashboard->DashElementBufCnt; i++) {
//				//if (dashElem[i]->type ==elementType_Display)
//				//{
//				//	dashElem[i]->display.channelName = dashElem[i]->id;
//				//	RenderElementDisplay(sample, dashElem[i]->display);
//				//}
//				if (dashElem[i]->type ==elementType_gCircle)
//				{
//					RenderElementGCircle(sample, dashElem[i]->gCircle);
//				}
//				if (dashElem[i]->type ==elementType_Label)
//				{
//					dashElem[i]->label.channelName = dashElem[i]->id;
//					RenderElementLabel(sample, dashElem[i]->label);
//				}
//				//if (dashElem[i]->type ==elementType_TextBox)
//				//{
//				//	RenderElementTextBox(sample, dashElem[i]->txtBox);
//				//}
//				//if (dashElem[i]->type ==elementType_Sweeper) {
//				//	dashElem[i]->sweeper.channelName =dashElem[i]->id;
//				//	RenderElementSweeper(sample, dashElem[i]->sweeper);
//				//}
//			}
//		}
//	}
//
//	delete gfx;
//}
//
//Bitmap* COverlayRender::GetOverlay2(Rect rcSource, Rect* rcTarget, ColorMatrix* mtx)
//{
//	// create copy of dashBaseImg
//	this->m_dashImg =new Bitmap(this->m_dashBaseImg->GetWidth(), this->m_dashBaseImg->GetHeight(), this->m_dashBaseImg->GetPixelFormat());
//	// cretae graphics object from overlay iamge
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//
//	if (wcscmp(this->m_dashboard->name, L"F1 Season 2012") ==0) {
//		drawF1Overlay();
//	}
//	else {
//		gfx->DrawImage(this->m_dashBaseImg, 0, 0, this->m_dashBaseImg->GetWidth(), this->m_dashBaseImg->GetHeight());
//		// draw overlay and update it
//		RenderDashElements();
//		//...
//	}
//
//	ColorMatrix m ={ 
//				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//				0.0f, 0.0f, 0.0f, (float)m_filterCfg->overlayCfg.transp, 0.0f,
//				0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
//	if (mtx)
//		memcpy(mtx, &m, sizeof(ColorMatrix));
//
//	rcTarget->X =(int)(float)this->m_filterCfg->overlayCfg.posX;;
//	rcTarget->Y =(int)(float)this->m_filterCfg->overlayCfg.posY;;
//	rcTarget->Width =(int)(float)((float)m_dashImg->GetWidth() * this->m_filterCfg->overlayCfg.scale);;
//	rcTarget->Height =(int)(float)((float)m_dashImg->GetHeight() * this->m_filterCfg->overlayCfg.scale);;
//
//	delete gfx;
//	gfx =NULL;
//
//	m_curFrameInStream++; 
//
//	int frameInc =(int)(m_curFrameInStream - m_refVidFrame);
//    float n = (float)((float)frameInc * (((float)1.0f / (float)m_frameRate) / ((float)1.0f / (float)m_dataLog->GetSampleTickRate())));
//	m_sampleBufIdx = m_refSampleIdx + (int)round((float)n);
//
//	return this->m_dashImg;
//}
//
//Bitmap* COverlayRender::GetOverlay(Rect rcSource, Rect* rcTarget)
//{
//	// create copy of dashBaseImg
//	this->m_dashImg =new Bitmap(this->m_dashBaseImg->GetWidth(), this->m_dashBaseImg->GetHeight(), this->m_dashBaseImg->GetPixelFormat());
//	// cretae graphics object from overlay iamge
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//
//	gfx->DrawImage(this->m_dashBaseImg, 0, 0, this->m_dashBaseImg->GetWidth(), this->m_dashBaseImg->GetHeight());
//
//	// draw overlay and update it
//	//updateOverlay();
//	RenderDashElements();
//	//...
//
//	// calculations for position and size of final overlay image
//	float dash_ar =(float)((float)this->m_dashImg->GetWidth() / (float)this->m_dashImg->GetHeight());
////	float tw =(float)((float)this->m_filterCfg->renderCfg.outputWidth / ); // target width of dashboard (1/2 video frame width)
////	float scale =(float)(tw / (float)this->m_dashImg->GetWidth());
//
//	float w =(float)(((float)this->m_dashImg->GetWidth() / (float)rcSource.Width) * (float)this->m_filterCfg->renderCfg.width); //this->m_dashImg->GetWidth() * scale;
//	float h =w / dash_ar;
//
//	float x, y, sx, sy;
//
//	sx =(float)((float)w * this->m_filterCfg->overlayCfg.scale);
//	sy =sx / dash_ar;
//
//	//x =0;
//	//y =rcTarget->Height - sy;
//	//if (this->m_filterCfg->renderCfg.margin > 0) {
//	//	x =(float)this->m_filterCfg->renderCfg.margin;
//	//	y =(float)rcTarget->Height - sy - this->m_filterCfg->renderCfg.margin;
//	//}
//
//	// TODO: Add Transparency
//	// ...
//
//	x =(float)this->m_filterCfg->overlayCfg.posX;
//	y =(float)this->m_filterCfg->overlayCfg.posY;
//	rcTarget->X =(int)x;
//	rcTarget->Y =(int)y;
//	rcTarget->Width =(int)w;
//	rcTarget->Height =(int)h;
//
//	delete gfx;
//
//	this->m_sampleBufIdx +=this->m_sampleBufInc;
//
//	return this->m_dashImg;
//}
//
//void COverlayRender::ReleaseOverlay(Gdiplus::Bitmap* overlay)
//{
//	if (overlay ==this->m_dashImg) {
//		delete this->m_dashImg;
//		this->m_dashImg =NULL;
//	}
//	else {
//		delete overlay;
//		overlay =NULL;
//	}
//}
//
//int COverlayRender::GetDataLogSampleCount()
//{
//	return this->m_dataLog->GetSampleCount();
//}
//
//void COverlayRender::RenderDashElements()
//{
//	PDATASAMPLE sample =this->m_dataLog->GetSample(this->m_sampleBufIdx);
//	PDASHELEMENT* dashElem =(PDASHELEMENT*)this->m_dashboard->pDashElementBuf;
//
//	if (sample != NULL) {
//
//		// update current lap time
//		pLapTiming->currentLapTime =(float)((float)sample->time - (float)pLapTiming->lapStartTimeIdx); 
//		if (pLapTiming->currentLap < sample->LapNo) {
//			// new lap started
//			pLapTiming->lastLap =pLapTiming->currentLap;
//			pLapTiming->lastLapTime =pLapTiming->currentLapTime;
//			pLapTiming->currentLap =sample->LapNo;
//			pLapTiming->currentLapTime =0;
//			
//			if (pLapTiming->currentLapIdx < (this->m_dataLog->GetSessionLapCount() - 1))
//				pLapTiming->currentLapIdx++;
//			pLapTiming->lapStartTimeIdx =(float)(this->m_dataLog->GetLapInfo(pLapTiming->currentLapIdx))->SessTimeIdx;
//
//			this->eol =true;
//		}
//		else {
//			this->eol =false;
//		}
//
//		// draw other elements
//		for (int i=0; i < this->m_dashboard->DashElementBufCnt; i++) {
//			////
//			// -- @ v0.6.1 -->
//			if (dashElem[i]->type ==elementType_Gauge)
//			{
//				dashElem[i]->gauge.channelName = dashElem[i]->id;
////				RenderElementGauge(sample, dashElem[i]->gauge);
//			}
//			if (dashElem[i]->type ==elementType_Slider)
//			{
//				dashElem[i]->slider.channelName = dashElem[i]->id;
//				RenderElementSlider(sample, dashElem[i]->slider);
//			}
//			if (dashElem[i]->type ==elementType_Display)
//			{
//				dashElem[i]->display.channelName = dashElem[i]->id;
//				RenderElementDisplay(sample, dashElem[i]->display);
//			}
//			if (dashElem[i]->type ==elementType_gCircle)
//			{
//				RenderElementGCircle(sample, dashElem[i]->gCircle);
//			}
//			if (dashElem[i]->type ==elementType_Label)
//			{
//				dashElem[i]->label.channelName = dashElem[i]->id;
//				RenderElementLabel(sample, dashElem[i]->label);
//			}
//			if (dashElem[i]->type ==elementType_TextBox)
//			{
//				RenderElementTextBox(sample, dashElem[i]->txtBox);
//			}
//			if (dashElem[i]->type ==elementType_Sweeper) {
//				dashElem[i]->sweeper.channelName =dashElem[i]->id;
//				RenderElementSweeper(sample, dashElem[i]->sweeper);
//			}
//			if (dashElem[i]->type ==elementType_Led) {
//				dashElem[i]->led.channelName =dashElem[i]->id;
//				RenderElementLed(sample, dashElem[i]->led);
//			}
//
//			// <-- //
//		} // for
//	} // if
//}
//
//wchar_t* COverlayRender::GetFullFileName(wchar_t* fname)
//{
//	if (!fname)
//		return NULL;
//
//	int len =wcslen(this->m_filterCfg->imgPath) + wcslen(fname) + 2;
//
//	wchar_t* file =new wchar_t[len];
//	if (file) {
//		wcscpy_s(file, len, this->m_filterCfg->imgPath);
//		wcscat_s(file, len, fname);
//
//		return file;
//	}
//
//	return NULL;
//}
//
//void COverlayRender::RenderElementDisplay(PDATASAMPLE sample, DASH_ELEMENT_DISPLAY& element)
//{
//	Point displaySize(element.size.width, element.size.height);
//	Bitmap* bmp =new Bitmap(displaySize.X, displaySize.Y, this->m_dashBaseImg->GetPixelFormat());
//	Graphics* g =Graphics::FromImage(bmp);
//	SolidBrush* bkg =new SolidBrush(Color::Black);
//	Pen* digitBkg =new Pen(Color(50, 255, 0, 0), 5.0f);
//	Pen* digit =new Pen(Color::Red, 5.0f);
//	float margin =8.0f;
//	int gear =0;
//
//	digitBkg->SetStartCap(LineCapTriangle);
//	digitBkg->SetEndCap(LineCapTriangle);
//	digit->SetStartCap(LineCapTriangle);
//	digit->SetEndCap(LineCapTriangle);
//
//	this->m_dataLog->GetChannelData(element.channelName, sample, &gear);
//
////	g->FillRectangle(bkg, (float)0, (float)0, (float)bmp->GetWidth(), (float)bmp->GetHeight());
//
//
//		// 1
//		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
//										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
//		// 2
//		g->DrawLine(digitBkg, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
//										 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//		// 3
//		g->DrawLine(digitBkg, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//										 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//		// 4
//		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
//										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
//		// 5
//		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//		// 6
//		g->DrawLine(digitBkg, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
//										 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//		// 7
//		g->DrawLine(digitBkg, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//										 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//
//		if (gear ==1) {
//			// 2
//			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//			// 3
//			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//		}
//		else if (gear ==2) {
//			// 1
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
//			// 2
//			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//			// 4
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
//			// 5
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//			// 7
//			g->DrawLine(digit, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//		}
//		else if (gear ==3) {
//			// 1
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
//			// 2
//			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//			// 3
//			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//			// 4
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
//			// 5
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//		}
//		else if (gear ==4) {
//			// 2
//			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//			// 3
//			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//			// 5
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//			// 6
//			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//		}
//		else if (gear ==5) {
//			// 1
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
//			// 3
//			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//			// 4
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
//			// 5
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//			// 6
//			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//		}
//		else if (gear ==6) {
//			// 1
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
//			// 3
//			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//			// 4
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
//			// 5
//			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
//											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
//			// 6
//			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
//			// 7
//			g->DrawLine(digit, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
//											 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
//		}
//
//
//		Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//		if (gfx) {
//			gfx->DrawImage(bmp, (REAL)element.pos.x, (REAL)element.pos.y);
//			delete gfx;
//		}
//		//Dashboard.gfx->DrawImage(bmp, PointF(Dashboard.bmp->GetWidth() - RpmGauge.ptCenter.X + 10,
//		//									 RpmGauge.ptCenter.Y + 20));
//
//	delete digit;
//	delete digitBkg;
//	delete bkg;
//	delete g;
//	delete bmp;
//
//}
//
//void COverlayRender::RenderElementGCircle(PDATASAMPLE sample, DASH_ELEMENT_GCIRCLE& element)
//{
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//	Pen* grid_pen =new Pen(Color(element.GridColor.r, element.GridColor.g, element.GridColor.b), 1.0f);
//	Pen* ball_pen =new Pen(Color(element.ballColor.r, element.ballColor.g, element.ballColor.b), 3.0f);
//	SolidBrush* br =new SolidBrush(Color(element.ballColor.r, element.ballColor.g, element.ballColor.b));
//	PointF center =PointF((float)element.center.x, (float)element.center.y);
//	float inc_pix =	(float)((REAL)element.latRange.max / (REAL)element.radius);	// increment per pixel
//	//
//
//	float ax =(REAL)center.X - element.radius;	// x-axis start point (grid)
//	float ay =(REAL)center.Y - element.radius;	// y-axis start point (grid)
//
//	// draw grid
//	gfx->DrawLine(grid_pen, (REAL)center.X, ay, (REAL)center.X, (REAL)(ay + element.size.height));
//	gfx->DrawLine(grid_pen, ax, (REAL)center.Y, (REAL)(ax + element.size.width), (REAL)center.Y);
//	// draw grid circles
//	float pt =(float)( (element.radius - 5) / element.latRange.max);
//	for (int i =1; i < (element.latRange.max + 1); i++)
//	{
//		gfx->DrawEllipse(grid_pen, RectF((REAL)element.center.x - (i*pt), (REAL)element.center.y - (i*pt), (2*i*pt), (2*i*pt)));
//	}
//
//	float latG =0;
//	float longG =0;
//	this->m_dataLog->GetChannelData(L"LatG", sample, &latG);
//	this->m_dataLog->GetChannelData(L"LongG", sample, &longG);
//
//	// draw indicator ball
//	float ball_size =(float)element.BallSize;
//
//	// TODO: check for style
//	ball_pen->SetEndCap(LineCapRound);
//	////
//
//	PointF dst =PointF((REAL)((REAL)latG / (REAL)inc_pix), (REAL)((REAL)longG / (REAL)inc_pix));
//	if (element.BallType ==1) {
//		gfx->DrawLine(ball_pen, (REAL)center.X, (REAL)center.Y, (REAL)dst.X, (REAL)dst.Y);
//	}
//	else if (element.BallType ==0) {
//		float sx =(float)((dst.X + center.X) - ((REAL)element.BallSize / 2.0f));
//		float sy =(float)((dst.Y + center.Y) - ((REAL)element.BallSize / 2.0f));
//		gfx->FillEllipse(br, RectF(sx, sy, (REAL)element.BallSize, (REAL)element.BallSize));
//	}
//
//	delete br;
//	delete ball_pen;
//	delete grid_pen;
//	delete gfx;
//}
//
//wchar_t* COverlayRender::FormatLapTimeString(float lapTime, int* strlen, bool def)
//{
//	wchar_t* str =new wchar_t[11];
//	memset(str, 0, sizeof(wchar_t) * 11);
//
//	float s =fmod((float)lapTime, (float)60.0f);
//	int min =(int)(((float)lapTime - (float)s) / 60.0f);
//
//	float _ms =(float)fmod((float)lapTime, (float)1.0f);
//	int ms =_ms * 10;
//
//	if (this->eol) {
//		_snwprintf_s(str, 11, 10, L"%.02d:%#02.3f", (int)min, s);
//		this->eol =false;
//	}
//	else {
//		_snwprintf_s(str, 11, 10, L"%.02d:%.02d.%.1d", (int)min, (int)s, (int)ms);
//	}
//	
//	if (def) {
//		_snwprintf_s(str, 11, 10, L"%.02d:%#02.3f", (int)min, s);
//	}
//
//	if (strlen)
//		*strlen =wcslen(str);
//
//	//float _ms =(float)fmod((float)lapTime, (float)1.0f);
//	//int ms =_ms * 10;
//	//lapTime -=_ms;
//	//int s =(int)fmod((float)lapTime, (float)60.0f);
//	//int min =(int)(((float)lapTime - (float)s) / 60.0f);
//
//	//if (this->eol) {
//	//	_snwprintf_s(str, 11, 10, L"%.02d:%.02d.%.3d", (int)min, (int)s, (int)_ms*1000);
//	//	this->eol =false;
//	//}
//	//else {
//	//	_snwprintf_s(str, 11, 10, L"%.02d:%.02d.%.1d", (int)min, (int)s, (int)ms);
//	//}
//	//
//	//if (def) {
//	//	_snwprintf_s(str, 11, 10, L"%.02d:%.02d.%.3d", (int)min, (int)s, (int)(_ms*1000));
//	//}
//
//	//if (strlen)
//	//	*strlen =wcslen(str);
//
//	return str;
//}
//
//void COverlayRender::RenderElementLabel(PDATASAMPLE sample, DASH_ELEMENT_LABEL& element)
//{
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//	Font font(element.font, (float)element.fontSize, element.fontStyle, UnitPixel);
//	SolidBrush br(Color(element.color.r, element.color.g, element.color.b));
//	float cval =0;
//	this->m_dataLog->GetChannelData(element.channelName, sample, &cval);
//	wchar_t* str =NULL;
//	int slen =0;
//	PointF pt =PointF((float)element.pos.x, (float)element.pos.y);
//	RectF strSize(0,0,0,0);
//
//	int curLapNo =0;
//	if (wcscmp(element.channelName, L"CurrentLapNo") ==0) {
//
//		// 
//		str =new wchar_t[5];
//		if (str) {
//			memset(str, 0, sizeof(wchar_t) * 5);
//			_swprintf(str, L"%d", sample->LapNo);
//			slen =wcslen(str)+1;
//
//			gfx->DrawString(str, slen, &font, pt, &br);
//
//			delete str;
//			str =NULL;
//		}
//	}
//	else if (wcscmp(element.channelName, L"CurrentLapTime") ==0) {
//
//		// calculate and update current lap time
//		str =FormatLapTimeString(pLapTiming->currentLapTime, &slen, false);
//		if (str) {
//			
//			gfx->DrawString(str, slen, &font, pt, &br);
//
//			delete str;
//			str =NULL;
//		}
//	}
//	else if (wcscmp(element.channelName, L"BestLapNo") ==0) {
//
//		PFAST_LAP_INFO fli =this->m_dataLog->GetFastLapInfo();
//		if (fli) {
//			str =new wchar_t[5];
//			if (str) {
//				memset(str, 0, sizeof(wchar_t) * 5);
//				_snwprintf_s(str, 5, 4, L"%d", fli->fastestLapStint);
//				slen =wcslen(str)+1;
//
//				gfx->DrawString(str, slen, &font, PointF((REAL)element.pos.x, (REAL)element.pos.y), &br);
//
//				delete str;
//				str =NULL;
//			}
//
//		}
//	}
//	else if (wcscmp(element.channelName, L"BestLapTime") ==0) {
//
//		int slen =0;
//
//		PFAST_LAP_INFO fli =this->m_dataLog->GetFastLapInfo();
//		if (fli) {
//			str =this->FormatLapTimeString(fli->fastestLapTimeInStint, &slen, true);
//			if (str) {
//
//				gfx->DrawString(str, slen, &font, PointF((REAL)element.pos.x, (REAL)element.pos.y), &br);
//
//				delete str;
//				str =NULL;
//			}
//
//		}
//	}
//	else if (wcscmp(element.channelName, L"LastLapNo") ==0) {
//
//		int slen =0;
//
//		if (sample->LapNo > pLapTiming->lastLap) {
//			if (pLapTiming->lastLap > 0) {
//				str =new wchar_t[5];
//				if (str) {
//					memset(str, 0, sizeof(wchar_t) * 5);
//					_swprintf(str, L"%d", pLapTiming->lastLap);
//					slen =wcslen(str)+1;
//
//					gfx->DrawString(str, slen, &font, pt, &br);
//
//					delete str;
//					str =NULL;
//				}
//			}
//		}
//
//		//PFAST_LAP_INFO fli =this->m_dataLog->GetFastLapInfo();
//		//if (fli) {
//		//	str =this->FormatLapTimeString(fli->fastestLapTimeInStint, &slen, true);
//		//	if (str) {
//
//		//		gfx->DrawString(str, slen, &font, PointF((REAL)element.pos.x, (REAL)element.pos.y), &br);
//
//		//		delete str;
//		//		str =NULL;
//		//	}
//		//}
//	}
//	else if (wcscmp(element.channelName, L"LastLapTime") ==0) {
//
//		int slen =0;
//
//		if (sample->LapNo > pLapTiming->lastLap) {
//			if (pLapTiming->lastLap > 0) {
//				str =this->FormatLapTimeString(pLapTiming->lastLapTime, &slen, true);
//				if (str) {
//
//					gfx->DrawString(str, slen, &font, PointF((REAL)element.pos.x, (REAL)element.pos.y), &br);
//
//					delete str;
//					str =NULL;
//				}
//			}
//		}
//
//		//PFAST_LAP_INFO fli =this->m_dataLog->GetFastLapInfo();
//		//if (fli) {
//		//	str =this->FormatLapTimeString(fli->fastestLapTimeInStint, &slen, true);
//		//	if (str) {
//
//		//		gfx->DrawString(str, slen, &font, PointF((REAL)element.pos.x, (REAL)element.pos.y), &br);
//
//		//		delete str;
//		//		str =NULL;
//		//	}
//		//}
//	}
//
//	// TODO: add last lap number and time hwen crossing s/f line
//	// ...
//
//	else {
//		// none of the special channel names, so check if one of the defaults
//
//		str =new wchar_t[32];
//		if (wcscmp(element.channelName, L"Speed")==0) {
//			float val =0;
// 			this->m_dataLog->GetChannelData(element.channelName, sample, &val);
//
//			if (element.units ==units_kph) {
//				val *=3.6f;
//			}
//			else if (element.units ==units_mph) {
//				val *=2.23694f;
//			}
//
//			memset(str, 0, sizeof(wchar_t) * 32);
//			_snwprintf_s(str, 32, 31, L"%d", (int)val);
//		}
//		else if (wcscmp(element.channelName, L"Gear")==0) {
//			int val =0;
// 			this->m_dataLog->GetChannelData(element.channelName, sample, &val);
//
//			memset(str, 0, sizeof(wchar_t) * 32);
//			_snwprintf_s(str, 32, 31, L"%d", (int)val);
//		}
//		else if (wcscmp(element.channelName, L"LongG") ==0) {
//			float val =0;
//			this->m_dataLog->GetChannelData(element.channelName, sample, &val);
//
//			memset(str, 0, sizeof(wchar_t) * 32);
//			if (val <0)
//				_snwprintf_s(str, 32, 31, L"%#2.1f", (float)val);
//			else
//				_snwprintf_s(str, 32, 31, L"+%#2.1f", (float)val);
//		}
//		else if (wcscmp(element.channelName, L"LatG") ==0) {
//			float val =0;
//			this->m_dataLog->GetChannelData(element.channelName, sample, &val);
//
//			memset(str, 0, sizeof(wchar_t) * 32);
//			if (val <0)
//				_snwprintf_s(str, 32, 31, L"%#2.1f", (float)val);
//			else
//				_snwprintf_s(str, 32, 31, L"+%#2.1f", (float)val);
//		}
//
//		if (str) {
//			//memset(str, 0, sizeof(wchar_t) * 32);
//			//_snwprintf_s(str, 32, 31, L"%d", (int)val);
//
//			gfx->DrawString(str, wcslen(str), &font, pt, &br);
//
//			delete str;
//			str =NULL;
//		}
//	}
//
//	delete gfx;
//}
//
//void COverlayRender::RenderElementSlider(PDATASAMPLE sample, DASH_ELEMENT_SLIDER& element)
//{
//	float cval =0;
//	int zp =0; // slider zero point (left, center or right)
//	float inc =0;
//	Color cl(element.color.r, element.color.g, element.color.b);
//	SolidBrush* brush =new SolidBrush(cl);
//	float x, y, w, h;
//	RectF bar((REAL)element.pos.x, (REAL)element.pos.y, (REAL)element.size.width, (REAL)element.size.height);
//	int valRange =element.valRange.max - element.valRange.min;
//	if (valRange < 0)
//		valRange *=-1;
//	
//	this->m_dataLog->GetChannelData(element.channelName, sample, &cval);
//
//	// check if bar is drawn horizontal or vertical
//	if (element.barType =='h') // horizontal
//	{
//		// check type of bar
//		if (element.type ==SliderType_min_max) {
//			// the bar is rendered from left to right
//			// ...
//			//inc = (float)((float)element.valRange.max / (float)element.size.width);
//			if ((cval >= element.valRange.min) && (cval <= element.valRange.max)) {
//				inc = (float)((float)valRange / (float)element.size.width);
//				x =(REAL)bar.X;
//				y =(REAL)bar.Y;
//				w =(REAL)(abs((float)(cval / inc)));
//				h =(REAL)bar.Height;
//			}
//		}
//		if (element.type ==SliderType_min0max) {
//			//inc =(float)((float)element.valRange.max / (float)(element.size.width / 2));
//			zp =(element.size.width / valRange) * element.valRange.min;
//			if (zp < 0)
//				zp *= -1;
//			inc =(float)((float)valRange / (float)element.size.width);
//			if (cval > 0) {
//				x =(REAL)bar.X + zp;
//				y =(REAL)bar.Y;
//				w =(REAL)(abs((float)(cval / inc)));
//				h =(REAL)bar.Height;
//			}
//			else {
//				w =(REAL)(abs((float)(cval / inc)));
//				//zp =element.size.width / 2;
//				x =(REAL)bar.X + (zp - w);
//				y =(REAL)bar.Y;
//				h =(REAL)bar.Height;
//			}
//		}
//		if (element.type ==SliderType_max_min) {
//			// the bar is rendered from right to left
//			if ((cval >= element.valRange.min) && (cval <= element.valRange.max)) {
//				inc = (float)((float)valRange / (float)element.size.width);
//				w =(REAL)(abs((float)(cval / inc)));
//				h =(REAL)bar.Height;
//				x =(REAL)bar.X + (bar.Width - w);
//				y =(REAL)bar.Y;
//			}
//		}
//	}
//	else if (element.barType =='v') // vertical
//	{
//		if (element.type == SliderType_min_max) {
//			// render bar from bottom to top
//			if ((cval >= element.valRange.min) && (cval <= element.valRange.max)) {
//				inc =(float)((float)valRange / (float)element.size.height);
//				w =(REAL)element.size.width;
//				h =(REAL)(abs((float)(cval / inc)));
//				x =(REAL)element.pos.x;
//				y =(REAL)(element.pos.y + (element.size.height - h));
//			}
//		}
//		if (element.type == SliderType_max_min) {
//			// render bar from top to bottom
//			if ((cval >= element.valRange.min) && (cval <= element.valRange.max)) {
//				//if (valRange < 0)
//				//	valRange *= -1;
//
//				inc =(float)((float)valRange / (float)element.size.height);
//				w =(REAL)element.size.width;
//				h =(REAL)(abs((float)(cval / inc)));
//				x =(REAL)element.pos.x;
//				y =(REAL)element.pos.y;
//			}
//		}
//		if (element.type == SliderType_min0max) {
//			// ...
//			zp =(int)(element.size.height / valRange) * element.valRange.max;
//			if (zp < 0)
//				zp *= -1;
//
//			inc =(float)((float)valRange / (float)element.size.height);
//			if (cval > 0) {
//				w =(REAL)element.size.width;
//				h =(REAL)(abs((float)(cval / inc)));
//				x =(REAL)element.pos.x;
//				y =(REAL)element.pos.y + (zp - h);
//			}
//			else {
//				w =(REAL)element.size.width;
//				h =(REAL)(abs((float)(cval / inc)));
//				x =(REAL)element.pos.x;
//				y =(REAL)element.pos.y + zp;
//			}
//		}
//	}
//
//	RectF rc(x,y,w,h);
//	Graphics* gfx;
//	if (this->m_dashImg) {
//		gfx =Graphics::FromImage(this->m_dashImg);
//		if (gfx) {
//			gfx->FillRectangle(brush, rc);
//			delete gfx;
//		}
//	}
//
//	delete brush;
//}
//
//void COverlayRender::RenderElementLed(PDATASAMPLE sample, LayoutDefinitionFormat::Native::DASH_ELEMENT_LED &element)
//{
//	Graphics* gfx =Graphics::FromImage(m_dashImg);
//	Bitmap* led =NULL;
//	wchar_t* fname =NULL;
//
//	if (wcscmp(element.channelName, L"ShiftLight") ==0) {
//		// render shift indicator light
//		int rpm =m_dataLog->ShiftLightRPM();
//
//		if ((int)sample->engineRpm >= rpm) {
//			fname =GetFullFileName(element.imgFileOn);
//			if (fname !=NULL) {
//				led =new Bitmap(fname);
//				delete fname;
//				fname =NULL;
//			}
//		}
//		else {
//			fname =GetFullFileName(element.imgFileOff);
//			if (fname !=NULL) {
//				led =new Bitmap(fname);
//				delete fname;
//				fname =NULL;
//			}
//		}
//
//
//	}
//	else if (wcscmp(element.channelName, L"Rpm") ==0) {
//		// check values and render on or off
//		if (sample->engineRpm >=element.onOffVals.max) {
//			// render LED as ON
//			fname =GetFullFileName(element.imgFileOn);
//			if (fname !=NULL) {
//				led =new Bitmap(fname);
//				delete fname;
//				fname =NULL;
//			}
//		}
//		else if (sample->engineRpm <=element.onOffVals.min) {
//			// render LED as OFF
//			fname =GetFullFileName(element.imgFileOff);
//			if (fname !=NULL) {
//				led =new Bitmap(fname);
//				delete fname;
//				fname =NULL;
//			}
//		}
//	}
//
//	if ( (led !=NULL) && gfx !=NULL) {
//		Rect rc(element.pos.x - (led->GetWidth() / 2), element.pos.y - (led->GetHeight() / 2), led->GetWidth(), led->GetHeight());
//		gfx->DrawImage(led, rc);
//	}
//
//	if (led !=NULL) {
//		delete led;
//		led =NULL;
//	}
//	if (gfx !=NULL) {
//		delete gfx;
//		gfx =NULL;
//	}
//}
//
//void COverlayRender::RenderElementSweeper(PDATASAMPLE sample, DASH_ELEMENT_SWEEPER& element)
//{
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//	Pen dp(Color(element.color.r, element.color.g, element.color.b), element.width); // drawing pen
//	Pen thp(Color(element.threshColor.r, element.threshColor.g, element.threshColor.b), element.width); // threshold drawing pen
//	
//	RectF pos((float)element.pos.x + (float)(element.width / 2.0f),
//			  (float)element.pos.y + (float)(element.width / 2.0f),
//			  (float)element.size.width - element.width,
//			  (float)element.size.height - element.width);
//
//	int val =0;
//	if (wcscmp(element.channelName, L"Rpm") ==0) {
//		float _v =0;
//		m_dataLog->GetChannelData(element.channelName, sample, &_v);
//		val =(int)_v;
//	}
//
//	float inc =(float)((float)element.angles.sweepAngle / (float)element.values.max);
//	float sa =(float)(((float)val - (float)element.values.min) * inc);
//
//	if (val <= element.threshVal)
//		gfx->DrawArc(&dp, pos, (float)((float)element.angles.startAngle - 90.0f), sa);
//	else {
//		gfx->DrawArc(&dp, pos, (float)((float)element.angles.startAngle - 90.0f), sa);
//		int diff =(val - element.threshVal);
//		sa =(float)(diff * inc);
//		float sta =(float)(element.angles.startAngle - 90.0f + ((float)(element.threshVal - element.values.min) * inc));
//		gfx->DrawArc(&thp, pos, sta, sa);
//	}
//
//	delete gfx;
//}
//
//void COverlayRender::RenderElementTextBox(PDATASAMPLE sample, DASH_ELEMENT_TEXTBOX& element)
//{
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//	Font font(element.font, (float)element.fontSize, element.fontStyle, UnitPixel);
//	SolidBrush br(Color(element.color.r, element.color.g, element.color.b));
//	PointF pt((REAL)element.pos.x, (REAL)element.pos.y);
//	int text_len =wcslen(element.text)+1; // length of user text
//	wchar_t* str =new wchar_t[text_len];
//
//	if (str) {
//		memset(str, 0, text_len);
//
//		_snwprintf_s(str, text_len, text_len-1, L"%ws", element.text);
//		gfx->DrawString(str, text_len, &font, pt, &br);
//
//		delete str;
//		str =NULL;
//	}
//
//	delete gfx;
//}
//
//
//void COverlayRender::drawElement(wchar_t* filename, DASH_ELEMENT_GAUGE& element)
//{
//	Graphics* gfx;
//
//	wchar_t* file =NULL;
//	Bitmap* bmp =NULL;
//	int len =wcslen(filename) + wcslen(this->m_filterCfg->imgPath) +2;
//	 
//	file =new wchar_t[len];
//	wcscpy_s(file, len, this->m_filterCfg->imgPath);
//	wcscat_s(file, len, filename);
//
//	if (this->m_dashBaseImg) {
//		gfx =Graphics::FromImage(this->m_dashBaseImg);
//	}
//	bmp =new Bitmap(file);
//	if (bmp !=NULL) {
//		gfx->DrawImage(bmp, element.pos.x, element.pos.y, element.size.width, element.size.height);
//	}
//
//	delete gfx;
//	delete bmp;
//	delete file;
//}
//
COverlayRender::~COverlayRender(void)
{
	if (pLapTiming !=NULL) {
		delete pLapTiming;
		pLapTiming =NULL;
	}

	if (this->m_imgPath !=NULL) {
		delete m_imgPath;
		this->m_imgPath =NULL;
	}
	if (this->m_dashBaseImg) {
		delete this->m_dashBaseImg;
		this->m_dashBaseImg =NULL;
	}
}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void COverlayRender::applyRenderCfg(Rect& src, Rect* tgt)
//{
//	
//}
//
//void COverlayRender::updateSpeed(float speed, DASH_ELEMENT_GAUGE& element)
//{	
//	float angle_inc =(float)((float)element.angles.sweepAngle / (float)element.valRange.max);
//	PointF sp, ep;
//	PointF nc((REAL)element.needle.needleCenter.x, (REAL)element.needle.needleCenter.y);	// needle center
//	Graphics* g =Graphics::FromImage(this->m_dashImg);
//	Pen* pen =new Pen(Color::Red, 4.0f);
//	SolidBrush* br =new SolidBrush(Color::Black);
//
//	pen->SetEndCap(LineCapTriangle);
//		
//	// TODO: still need to check if kph or mph should be used for calculations (kph is default)
//	speed = speed * 3.6f;	// m/s to km/h conversion
//	float rad =M_PI / 180;
//	float rot =(speed * angle_inc) + element.angles.startAngle;
//	float a =rot;
//	if (a >=360.0f)
//		a -=360.0f;
//
//	ep =PointF((float)element.radius * sin(a * rad), 
//			  (float)element.radius * cos(a * rad) );
//
//	g->DrawLine(pen, nc.X, nc.Y,
//					 nc.X + ep.X, nc.Y - ep.Y);
//	g->FillEllipse(br, RectF((float)nc.X - 6, (float)nc.Y - 6,
//					(float)12.0f, (float)12.0f));
//
//	g->SetSmoothingMode(SmoothingModeAntiAlias);
//
//	delete br;
//	delete pen;
//	delete g;
//}
//
//void COverlayRender::updateRpm(float rpm, DASH_ELEMENT_GAUGE& element)
//{
//	float angle_inc =(float)((float)element.angles.sweepAngle / (float)element.valRange.max);
//	PointF sp, ep;
//	PointF nc((REAL)element.needle.needleCenter.x, (REAL)element.needle.needleCenter.y);	// needle center
//	Graphics* g =Graphics::FromImage(this->m_dashImg);
//	Pen* pen =new Pen(Color::Red, 4.0f);
//	SolidBrush* br =new SolidBrush(Color::Black);
//
//	pen->SetEndCap(LineCapTriangle);
//		
//	float rad =M_PI / 180;
//	float rot =(rpm * angle_inc) + element.angles.startAngle;
//	float a =rot;
//	if (a >=360.0f)
//		a -=360.0f;
//
//	ep =PointF((float)element.radius * sin(a * rad), 
//			  (float)element.radius * cos(a * rad) );
//
//	g->DrawLine(pen, nc.X, nc.Y,
//					 nc.X + ep.X, nc.Y - ep.Y);
//	g->FillEllipse(br, RectF((float)nc.X - 6, (float)nc.Y - 6,
//					(float)12.0f, (float)12.0f));
//
//	g->SetSmoothingMode(SmoothingModeAntiAlias);
//
//	delete br;
//	delete pen;
//	delete g;
//}
//
//void COverlayRender::updateSlider(float val, DASH_ELEMENT_SLIDER& element)
//{
//	int zp =0; // slider zero point (left, center or right)
//	float inc =(float)((float)element.valRange.max / (float)(element.size.width / 2));
//	Color cl(element.color.r, element.color.g, element.color.b);
//	SolidBrush* brush =new SolidBrush(cl);
//	float x, y, w, h;
//	RectF bar((REAL)element.pos.x, (REAL)element.pos.y, (REAL)element.size.width, (REAL)element.size.height);
//	
//	// convert val into correct unit (m/s^2 -> g)
//	val =val / 9.81f;
//
//	// check if bar is drawn horizontal or vertical
//	if (element.barType =='h') // horizontal
//	{
//		// check type of bar
//		if (element.type ==SliderType_min0max) {
//			if (val > 0) {
//				zp =element.size.width / 2;
//				x =(REAL)bar.X + zp;
//				y =(REAL)bar.Y;
//				w =(REAL)(val / inc);
//				h =(REAL)bar.Height;
//			}
//			else {
//				w =(REAL)(abs((val / inc)));
//				zp =element.size.width / 2;
//				x =(REAL)bar.X + (zp - w);
//				y =(REAL)bar.Y;
//				h =(REAL)bar.Height;
//			}
//
//			RectF rc(x,y,w,h);
//			Graphics* gfx;
//			if (this->m_dashImg) {
//				gfx =Graphics::FromImage(this->m_dashImg);
//				if (gfx) {
//					gfx->FillRectangle(brush, rc);
//
//					delete gfx;
//					goto exit;
//				}
//			}
//		}
//	}
//	else if (element.barType =='v') // vertical
//	{
//	}
//
//exit:
//	delete brush;
//}
//
//void COverlayRender::updateTextBox(float val, DASH_ELEMENT_TEXTBOX& element)
//{
//	int v =(int)val;
//	Font* f =new Font(element.font, (float)element.fontSize);
//	Graphics* gfx =Graphics::FromImage(this->m_dashImg);
//	Color cl(element.color.r, element.color.g, element.color.b);
//	SolidBrush* brush =new SolidBrush(cl);
//	wchar_t buf[4];
//
//	// TODO: convert val into the right units (kph/mph)
//	val = val * 3.6f;
//	v =(int)val;
//
//	_snwprintf((wchar_t*)&buf[0], 4, L"%d", (int)v);
//	buf[4] ='\x00\x00';
//
//	PointF pt((REAL)element.pos.x+2, (REAL)element.pos.y+3);
//
//	gfx->DrawString(buf, 3, f, pt, brush);
//
//	delete brush;
//	delete gfx;
//	delete f;
//}
//
//
//
////void COverlayRender::updateOverlay()
////{
////	PDATASAMPLE sample =this->m_dataLog->GetSample(this->m_sampleBufIdx);
////	PDASHELEMENT* dashElem =(PDASHELEMENT*)this->m_dashboard->pDashElementBuf;
////
////	if (sample != NULL) {
////		// draw other elements
////		for (int i=0; i < this->m_dashboard->DashElementBufCnt; i++) {
////			if (dashElem[i]->type ==elementType_Gauge && (wcscmp(dashElem[i]->id, L"Speedo") ==0) ) {
////				updateSpeed(sample->groundSpeed, dashElem[i]->gauge);
////			}
////			if (dashElem[i]->type ==elementType_Gauge && (wcscmp(dashElem[i]->id, L"Rpm") ==0) ) {
////				updateRpm(sample->engineRpm, dashElem[i]->gauge);
////			}
////			if (dashElem[i]->type ==elementType_Slider) {
////				updateSlider(sample->latG, dashElem[i]->slider);
////			}
////
////			if (dashElem[i]->type ==elementType_TextBox && (wcscmp(dashElem[i]->id, L"Speed") ==0) ) {
////				updateTextBox(sample->groundSpeed, dashElem[i]->txtBox);
////			}
////		}
////	}
////}
//
//////////////////////////////////////////////////////////////////////////////////////////
////int SetOverlayType(int overlayType)
////{
////	if (m_overlayType ==overlayType)
////		return 0;
////
////	m_overlayType =overlayType;
////
////	lastErr =0;
////
////	// default overlay
////	if (overlayType ==0) {
////
////		SpeedGauge.bmp =new Gdiplus::Bitmap(L"C:\\Dokumente und Einstellungen\\Thorsten\\Eigene Dateien\\Visual Studio 2008\\Projects\\TVO\\images\\speed01_330kph.png");
////		if (SpeedGauge.bmp ==NULL) {
////			lastErr =-1;
////		}
////		SpeedGauge.gfx =Gdiplus::Graphics::FromImage(SpeedGauge.bmp);
////		if (SpeedGauge.gfx ==NULL)
////			lastErr =-2;
////		SpeedGauge.ptCenter =Gdiplus::PointF((float)SpeedGauge.bmp->GetWidth() / 2, (float)SpeedGauge.bmp->GetHeight() / 2);
////		SpeedGauge.r1 =SpeedGauge.ptCenter.X * 0.75f;
////		SpeedGauge.StartAngle =210.0f;
////		SpeedGauge.SweepAngle =275.0f;
////		SpeedGauge.angle_inc =SpeedGauge.SweepAngle / 330.0f;
////
////		RpmGauge.bmp =new Gdiplus::Bitmap(L"C:\\Dokumente und Einstellungen\\Thorsten\\Eigene Dateien\\Visual Studio 2008\\Projects\\TVO\\images\\rpm01_10k.png");
////		if (RpmGauge.bmp ==NULL) {
////			lastErr =-1;
////		}
////		RpmGauge.gfx =Gdiplus::Graphics::FromImage(RpmGauge.bmp);
////		if (RpmGauge.gfx ==NULL)
////			lastErr =-2;
////		RpmGauge.ptCenter =Gdiplus::PointF((float)RpmGauge.bmp->GetWidth() / 2, (float)RpmGauge.bmp->GetWidth() / 2);
////		RpmGauge.r1 =RpmGauge.ptCenter.X * 0.75f;
////		RpmGauge.StartAngle =210.0f;
////		RpmGauge.SweepAngle =220.0f;
////		RpmGauge.angle_inc =RpmGauge.SweepAngle / 10000.0f;
////
////		ThrottleBar.bmp =new Bitmap(80, 25, SpeedGauge.bmp->GetPixelFormat());
////		ThrottleBar.gfx =Graphics::FromImage(ThrottleBar.bmp);
////
////		BrakeBar.bmp =new Bitmap(80, 25, SpeedGauge.bmp->GetPixelFormat());
////		BrakeBar.gfx =Graphics::FromImage(BrakeBar.bmp);
////
////		GCircle.bmp =new Bitmap(100, 100, SpeedGauge.bmp->GetPixelFormat());
////		GCircle.gfx =Graphics::FromImage(GCircle.bmp);
////		GCircle.ptCenter.X =GCircle.bmp->GetWidth() / 2;
////		GCircle.ptCenter.Y =GCircle.bmp->GetHeight() / 2;
////
////		Dashboard.bmp =new Bitmap(SpeedGauge.bmp->GetWidth() + RpmGauge.bmp->GetWidth() + GCircle.bmp->GetWidth(),
////									SpeedGauge.bmp->GetHeight() + 20, SpeedGauge.bmp->GetPixelFormat());
////		Dashboard.gfx =Graphics::FromImage(Dashboard.bmp);
////	}
////
////	CreateBasicOverlay();
////
////	if (lastErr !=0)
////		return -1;
////
////	return 0;
////}
//
////void COverlayRender::CreateBasicOverlay()
////{
////	Pen* pen =new Pen(Color::White, (float)2.0f);
////	SolidBrush* br =new SolidBrush(Color::Black);
////	Font* f =new Font(L"Arial", 15.0f, FontStyleBold);
////
////	Dashboard.gfx->FillRectangle(br, RectF(0, 0, Dashboard.bmp->GetWidth(), Dashboard.bmp->GetHeight()));
////
//////	GCircle.gfx->DrawEllipse(pen, RectF((float)pen->GetWidth(), (float)pen->GetWidth(),
//////							(float)GCircle.bmp->GetWidth() - (pen->GetWidth() * 2), (float)GCircle.bmp->GetWidth() - (pen->GetWidth() * 2)));
////
////	GCircle.gfx->FillRectangle(br, RectF(0, 0, GCircle.bmp->GetWidth(), GCircle.bmp->GetHeight()));
////
////	float w =pen->GetWidth();
////	pen->SetWidth((float)1.0f);
////	pen->SetDashStyle(DashStyleSolid);
////
////	GCircle.gfx->DrawLine(pen, (float)0,
////								(float)GCircle.ptCenter.Y,
////								(float)GCircle.bmp->GetWidth(),
////								(float)GCircle.ptCenter.Y);
////
////	GCircle.gfx->DrawLine(pen, (float)GCircle.ptCenter.X,
////								(float)0,
////								(float)GCircle.ptCenter.X,
////								(float)GCircle.bmp->GetHeight());
////
////	pen->SetWidth(w);
////	pen->SetDashStyle(DashStyleDash);
////
////	GCircle.gfx->DrawEllipse(pen, (float)GCircle.ptCenter.X - 15, (float)GCircle.ptCenter.Y - 15,
////								  (float)30, (float)30);
////	GCircle.gfx->DrawEllipse(pen, (float)GCircle.ptCenter.X - 30, (float)GCircle.ptCenter.Y - 30,
////								  (float)60, (float)60);
////	GCircle.gfx->DrawEllipse(pen, (float)GCircle.ptCenter.X - 45, (float)GCircle.ptCenter.Y - 45,
////								  (float)90, (float)90);
////
////	ThrottleBar.gfx->FillRectangle(br, RectF(0, 0, ThrottleBar.bmp->GetWidth(), ThrottleBar.bmp->GetHeight()));
////	BrakeBar.gfx->FillRectangle(br, RectF(0, 0, BrakeBar.bmp->GetWidth(), BrakeBar.bmp->GetHeight()));
////
////	PointF origin;
////	RectF rc;
////	wchar_t* lbl =L"G";
////	br->SetColor(Color::White);
////	GCircle.gfx->MeasureString(lbl, 1, f, origin, &rc);
////	GCircle.gfx->DrawString(lbl, 1, f, PointF((float)GCircle.ptCenter.X - (rc.Width / 2),
////											  (float)GCircle.ptCenter.Y - (rc.Height / 2)), br);
////
////	delete f;
////	delete br;
////	delete pen;
////}
//
////void COverlayRender::DrawDashboard(Gdiplus::Graphics* gfx, SizeF vidSize, PTELEMETRY_TABLE_ENTRY entry)
////{
////	if (entry !=NULL) {
////
////		DrawSpeedGauge(entry->corrSpeed);
////		DrawRpmGauge(entry->engineRpm);
////		DrawGCircle(entry->latG, entry->longG);
////		DrawThrottleBar(entry->throttlePos);
////		DrawBrakeBar(entry->brakePos);
////		DrawGearDisplay(entry->gear);
////
////		PointF pos =PointF((float)10, vidSize.Height - Dashboard.bmp->GetHeight());
////
////		gfx->DrawImage(Dashboard.bmp, RectF(pos.X, pos.Y, (float)Dashboard.bmp->GetWidth() * 0.85f, (float)Dashboard.bmp->GetHeight() * 0.85f));
////	}
////}
//
////void COverlayRender::DrawSpeedGauge(float speed)
////{
////	if (SpeedGauge.bmp !=NULL) {
////
////		Bitmap* bmp =new Bitmap(SpeedGauge.bmp->GetWidth(), SpeedGauge.bmp->GetHeight(), SpeedGauge.bmp->GetPixelFormat());
////		Graphics* g =Graphics::FromImage(bmp);
////		Pen* pen =new Pen(Color::Red, 4.0f);
////		SolidBrush* br =new SolidBrush(Color::Black);
////
////		pen->SetEndCap(LineCapTriangle);
////		g->DrawImage(SpeedGauge.bmp, 0, 0);
////		
////		float rad =M_PI / 180;
////		float rot =(speed * SpeedGauge.angle_inc) + SpeedGauge.StartAngle;
////		float a =rot;
////		if (a >=360.0f)
////			a -=360.0f;
////
////		PointF ep =PointF((float)SpeedGauge.r1 * sin(a * rad), 
////						  (float)SpeedGauge.r1 * cos(a * rad) );
////
////		g->DrawLine(pen, SpeedGauge.ptCenter.X, SpeedGauge.ptCenter.Y,
////						 SpeedGauge.ptCenter.X + ep.X, SpeedGauge.ptCenter.Y - ep.Y);
////		g->FillEllipse(br, RectF((float)SpeedGauge.ptCenter.X - 6, (float)SpeedGauge.ptCenter.Y - 6,
////								 (float)12.0f, (float)12.0f));
////
////		g->SetSmoothingMode(SmoothingModeAntiAlias);
////
////		Dashboard.gfx->DrawImage(bmp, PointF((float)0, (float)0));
////
////		delete br;
////		delete pen;
////		delete g;
////		delete bmp;
////	}
////}
////
////void COverlayRender::DrawRpmGauge(int rpm)
////{
////	if (RpmGauge.bmp !=NULL) {
////
////		Bitmap* bmp =new Bitmap(RpmGauge.bmp->GetWidth(), RpmGauge.bmp->GetHeight(), RpmGauge.bmp->GetPixelFormat());
////		Graphics* g =Graphics::FromImage(bmp);
////		Pen* pen =new Pen(Color::Red, 4.0f);
////		SolidBrush* br =new SolidBrush(Color::Black);
////
////		pen->SetEndCap(LineCapTriangle);
////		g->DrawImage(RpmGauge.bmp, 0, 0);
////		
////		float rad =M_PI / 180;
////		float rot =(float)(rpm * RpmGauge.angle_inc) + RpmGauge.StartAngle;
////		float a =rot;
////		if (a >=360.0f)
////			a -=360.0f;
////
////		PointF ep =PointF((float)RpmGauge.r1 * sin(a * rad), 
////						  (float)RpmGauge.r1 * cos(a * rad) );
////
////		g->DrawLine(pen, RpmGauge.ptCenter.X, RpmGauge.ptCenter.Y,
////						 RpmGauge.ptCenter.X + ep.X, RpmGauge.ptCenter.Y - ep.Y);
////		g->FillEllipse(br, RectF((float)RpmGauge.ptCenter.X - 6, (float)RpmGauge.ptCenter.Y - 6,
////								 (float)12.0f, (float)12.0f));
////
////		g->SetSmoothingMode(SmoothingModeAntiAlias);
////
////		Dashboard.gfx->DrawImage(bmp, PointF((float)Dashboard.bmp->GetWidth() - RpmGauge.bmp->GetWidth(), (float)0));
////
////		delete br;
////		delete pen;
////		delete g;
////		delete bmp;
////	}
////}
////
////void COverlayRender::DrawThrottleBar(float pos)
////{
////	if (ThrottleBar.bmp !=NULL) {
////
////		Bitmap* bmp =new Bitmap(ThrottleBar.bmp->GetWidth(), ThrottleBar.bmp->GetHeight(), ThrottleBar.bmp->GetPixelFormat());
////		Graphics* g =Graphics::FromImage(bmp);
////		SolidBrush* br =new SolidBrush(Color::Green);
////		SolidBrush* textBr =new SolidBrush(Color::LightGray);
////		Font* f =new Font(L"Arial", (float)10.0f, FontStyleBold);
////
////		g->DrawImage(ThrottleBar.bmp, 0, 0);
////
////		float x =(float)((bmp->GetWidth() / 100.0f) * pos);
////
////		g->FillRectangle(br, RectF(0, 0, x, bmp->GetHeight()));
////		
////		PointF origin(0.0f, 0.0f);
////		RectF rc;
////		wchar_t* lbl =L"Throttle";
////		g->MeasureString(lbl, wcslen(lbl), f, origin, &rc);
/////*		g->DrawRectangle(&Pen(Color::White, 1.0f), RectF((float)(bmp->GetWidth() / 2) - (rc.Width / 2),
////														 (float)(bmp->GetHeight() / 2) - (rc.Height / 2),
////														 (float)rc.Width, rc.Height));
////*/
////		g->DrawString(lbl, wcslen(lbl), f, PointF((float)(bmp->GetWidth() / 2) - (rc.Width / 2),
////												  (float)(bmp->GetHeight() / 2) - (rc.Height / 2)), textBr);
////		
////		Dashboard.gfx->DrawImage(bmp, PointF((float)(Dashboard.bmp->GetWidth() / 2) - (bmp->GetWidth() / 2),
////											 (float)(Dashboard.bmp->GetHeight() - BrakeBar.bmp->GetHeight() - bmp->GetHeight() - 10)));
////
////		delete f;
////		delete textBr;
////		delete br;
////		delete g;
////		delete bmp;
////	}
////}
////
////void COverlayRender::DrawBrakeBar(float pos)
////{
////	if (BrakeBar.bmp !=NULL) {
////
////		Bitmap* bmp =new Bitmap(BrakeBar.bmp->GetWidth(), BrakeBar.bmp->GetHeight(), BrakeBar.bmp->GetPixelFormat());
////		Graphics* g =Graphics::FromImage(bmp);
////		SolidBrush* br =new SolidBrush(Color::Red);
////		SolidBrush* textBr =new SolidBrush(Color::LightGray);
////		Font* f =new Font(L"Arial", (float)10.0f, FontStyleBold);
////
////		g->DrawImage(BrakeBar.bmp, 0, 0);
////
////		float x =(float)((bmp->GetWidth() / 100.0f) * pos);
////
////		g->FillRectangle(br, RectF(0, 0, x, bmp->GetHeight()));
////		
////		PointF origin(0.0f, 0.0f);
////		RectF rc;
////		wchar_t* lbl =L"Brake";
////		g->MeasureString(lbl, wcslen(lbl), f, origin, &rc);
/////*		g->DrawRectangle(&Pen(Color::White, 1.0f), RectF((float)(bmp->GetWidth() / 2) - (rc.Width / 2),
////														 (float)(bmp->GetHeight() / 2) - (rc.Height / 2),
////														 (float)rc.Width, rc.Height));
////*/
////		g->DrawString(lbl, wcslen(lbl), f, PointF((float)(bmp->GetWidth() / 2) - (rc.Width / 2),
////												  (float)(bmp->GetHeight() / 2) - (rc.Height / 2)), textBr);
////		
////		Dashboard.gfx->DrawImage(bmp, PointF((float)(Dashboard.bmp->GetWidth() / 2) - (bmp->GetWidth() / 2),
////											 (float)(Dashboard.bmp->GetHeight() - bmp->GetHeight() - 5)) );
////
////		delete f;
////		delete textBr;
////		delete br;
////		delete g;
////		delete bmp;
////	}
////}
////
////void COverlayRender::DrawGCircle(float gLat, float gLong)
////{
////	if (GCircle.bmp !=NULL) {
////
////		Bitmap* bmp =new Bitmap(GCircle.bmp->GetWidth(), GCircle.bmp->GetHeight(), GCircle.bmp->GetPixelFormat());
////		Graphics* g =Graphics::FromImage(bmp);
////		SolidBrush* br =new SolidBrush(Color::Red);
////
////		g->DrawImage(GCircle.bmp, RectF(0, 0, (float)GCircle.bmp->GetWidth(), (float)GCircle.bmp->GetHeight()));
////
////		float inc =(float)(1.0f / 15.0f);
////		float latPos =GCircle.ptCenter.X + (gLat / inc);
////		float longPos =GCircle.ptCenter.Y + (gLong / inc);
////
////		g->FillEllipse(br, (float)latPos - 4, (float)longPos - 4, (float)8, (float)8);
////
////		Dashboard.gfx->DrawImage(bmp, PointF((float)SpeedGauge.bmp->GetWidth(), (float)0));
////
////		delete br;
////		delete g;
////		delete bmp;
////	}
////
////}
////
////void COverlayRender::DrawGearDisplay(int gear)
////{
////	Point displaySize(30, 40);
////	Bitmap* bmp =new Bitmap(displaySize.X, displaySize.Y, SpeedGauge.bmp->GetPixelFormat());
////	Graphics* g =Graphics::FromImage(bmp);
////	SolidBrush* bkg =new SolidBrush(Color::Black);
////	Pen* digitBkg =new Pen(Color(70, 255, 0, 0), 3.0f);
////	Pen* digit =new Pen(Color::Red, 3.0f);
////	float margin =8.0f;
////
////	digitBkg->SetStartCap(LineCapTriangle);
////	digitBkg->SetEndCap(LineCapTriangle);
////	digit->SetStartCap(LineCapTriangle);
////	digit->SetEndCap(LineCapTriangle);
////
////	g->FillRectangle(bkg, (float)0, (float)0, (float)bmp->GetWidth(), (float)bmp->GetHeight());
////
////
////		// 1
////		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
////										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
////		// 2
////		g->DrawLine(digitBkg, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
////										 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////		// 3
////		g->DrawLine(digitBkg, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////										 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////		// 4
////		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
////										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
////		// 5
////		g->DrawLine(digitBkg, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////										 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////		// 6
////		g->DrawLine(digitBkg, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
////										 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////		// 7
////		g->DrawLine(digitBkg, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////										 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////
////		if (gear ==1) {
////			// 2
////			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////			// 3
////			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////		}
////		else if (gear ==2) {
////			// 1
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
////			// 2
////			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////			// 4
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
////			// 5
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////			// 7
////			g->DrawLine(digit, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////		}
////		else if (gear ==3) {
////			// 1
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
////			// 2
////			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////			// 3
////			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////			// 4
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
////			// 5
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////		}
////		else if (gear ==4) {
////			// 2
////			g->DrawLine(digit, PointF(displaySize.X - margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////			// 3
////			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////			// 5
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////			// 6
////			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////		}
////		else if (gear ==5) {
////			// 1
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
////			// 3
////			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////			// 4
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
////			// 5
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////			// 6
////			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////		}
////		else if (gear ==6) {
////			// 1
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, margin));
////			// 3
////			g->DrawLine(digit, PointF(displaySize.X - margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(displaySize.X - margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////			// 4
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y - margin),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y - margin));
////			// 5
////			g->DrawLine(digit, PointF(margin + (digitBkg->GetWidth() / 2) + 1, displaySize.Y/2),
////											 PointF(displaySize.X - margin - (digitBkg->GetWidth() / 2) - 1, displaySize.Y/2));
////			// 6
////			g->DrawLine(digit, PointF(margin, margin + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(margin, displaySize.Y - (displaySize.Y/2) - (digitBkg->GetWidth() / 2) - 1));
////			// 7
////			g->DrawLine(digit, PointF(margin, displaySize.Y - (displaySize.Y/2) + (digitBkg->GetWidth() / 2) + 1),
////											 PointF(margin, displaySize.Y - margin - (digitBkg->GetWidth() / 2) -1));
////		}
////
////
////		Dashboard.gfx->DrawImage(bmp, PointF(Dashboard.bmp->GetWidth() - RpmGauge.ptCenter.X + 10,
////											 RpmGauge.ptCenter.Y + 20));
////
////	delete digit;
////	delete digitBkg;
////	delete bkg;
////	delete g;
////	delete bmp;
////}
////
////void COverlayRender::DrawLapTime(float time)
////{
////}
