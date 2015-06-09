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

//#include <vld.h>
#include "TVOfilter.h"

#include <stdlib.h>
#include <crtdbg.h>


using namespace TVOFilter;

static inline HRESULT GetPixleFormat(int bpp, PixelFormat* pixFmt)
{
	switch (bpp)
	{
	case 15:
		*pixFmt = PixelFormat16bppRGB555;
		return S_OK;
	case 16:
		*pixFmt = PixelFormat16bppRGB565;
		return S_OK;
	case 24:
		*pixFmt = PixelFormat24bppRGB;
		return S_OK;
	case 32:
		*pixFmt = PixelFormat32bppRGB;
		return S_OK;
	default:
		return E_FAIL;
	}
}

IPin* pPin;

#pragma region // TVOFilter Object implementation
CTVOFilter::CTVOFilter(LPUNKNOWN pUnk, HRESULT *phr) :
CTransformFilter(FILTER_NAME, pUnk, CLSID_TVOFilter), ITVOFilterConfig()
{
	GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	memset(&this->m_vihIn, 0, sizeof(VIDEOINFOHEADER));
	memset(&this->m_vihOut, 0, sizeof(VIDEOINFOHEADER));
	this->m_pixFmt = 0;
	this->m_stride = 0;

	this->m_filterCfg = NULL;

	this->m_progressCnt = 0;
	this->m_progressNotify = NULL;

	m_sampleBufIdx = 0;
	m_dashOverlay = NULL;

	m_curFrame = 0;
	pPin = this->m_pInput;
}

CTVOFilter::~CTVOFilter()
{

	GdiplusShutdown(m_gdiplusToken);
	//	_CrtDumpMemoryLeaks();
}

int CTVOFilter::SetFilterConfig(void* cfg)
{
	if (cfg != NULL)
	{
		this->m_filterCfg = (TVOFILTERCONFIG*)cfg;

		m_dashOverlay = (IDashboardLayout*)m_filterCfg->DashboardLayoutObj;

		m_curFrame = m_refFrame = m_filterCfg->syncData.startFrame;
		m_refSample = m_filterCfg->syncData.startSample;

		m_progressCnt = 0;

		return 0;
	}

	return -1;
}

void CTVOFilter::SetNotifyCallback(IProgressCallback* notify)
{
	if (notify != NULL)
	{
		this->m_progressNotify = notify;
	}
}

int CTVOFilter::SetPreviewConfig(__int64 startFrame, int startSample, void* DataLogger)
{
	if (DataLogger != NULL)
	{
		m_curFrame = m_refFrame = startFrame;
		m_refSample = startSample;

		return 0;
	}

	return -1;
}

int CTVOFilter::SetOverlay(void* overlay, int cnt)
{

	return -1;
}


CUnknown* WINAPI CTVOFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT* phr)
{
	CTVOFilter *tvo = new CTVOFilter(pUnk, phr);
	if (!tvo) {
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return tvo;
}

STDMETHODIMP CTVOFilter::NonDelegatingQueryInterface(REFIID iid, void **ppv)
{
	if (iid == IID_ITVOFilterConfig) {
		return GetInterface((LPUNKNOWN)static_cast<ITVOFilterConfig*>(this), ppv);
	}
	else if (iid == IID_IMediaPosition || iid == IID_IMediaSeeking){

		return this->m_pOutput->NonDelegatingQueryInterface(iid, ppv);
	}

	return CBaseFilter::NonDelegatingQueryInterface(iid, ppv);
}

HRESULT CTVOFilter::CheckInputType(const CMediaType *mtIn)
{

	if (mtIn->majortype != MEDIATYPE_Video) {
		return VFW_E_TYPE_NOT_ACCEPTED;
	}
	if (mtIn->subtype != MEDIASUBTYPE_RGB32)
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	if ((mtIn->formattype == FORMAT_VideoInfo) &&
		(mtIn->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
		(mtIn->pbFormat != NULL))
	{
		VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*)mtIn->pbFormat;

		if (vih->bmiHeader.biBitCount != 32)
			return VFW_E_TYPE_NOT_ACCEPTED;

		return S_OK;
	}

	return VFW_E_TYPE_NOT_ACCEPTED;

}

HRESULT CTVOFilter::SetMediaType(PIN_DIRECTION direction, const CMediaType *pmt)
{
	if (direction == PINDIR_INPUT) {

		VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pmt->pbFormat;
		memcpy(&m_vihIn, pvih, pmt->cbFormat);

		HRESULT hr = GetPixleFormat(m_vihIn.bmiHeader.biBitCount, &m_pixFmt);
		if (FAILED(hr))
		{
			return hr;
		}

		BITMAPINFOHEADER bih = m_vihIn.bmiHeader;
		m_stride = bih.biBitCount / 8 * bih.biWidth;

	}

	if (direction == PINDIR_OUTPUT) {

		memset(&this->m_vihOut, 0, sizeof(VIDEOINFOHEADER));
		VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pmt->pbFormat;
		memcpy(&m_vihOut, pvih, pmt->cbFormat);

		this->m_framerate = (int)((__int64)ONE_SECOND / (__int64)pvih->AvgTimePerFrame);

	}


	return S_OK;
}

HRESULT CTVOFilter::CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut)
{
	HRESULT hr = S_OK;

	if (mtOut->formattype != FORMAT_VideoInfo)
		return VFW_E_TYPE_NOT_ACCEPTED;

	VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)mtOut->pbFormat;
	if (mtOut->subtype != MEDIASUBTYPE_RGB32 && vih->bmiHeader.biBitCount != 32)
		return VFW_E_TYPE_NOT_ACCEPTED;

	return hr;
}

HRESULT CTVOFilter::DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES *pProp)
{
	HRESULT hr = S_OK;
	AM_MEDIA_TYPE mt;

	hr = this->m_pInput->ConnectionMediaType(&mt);
	if (FAILED(hr))
		return hr;

	if (mt.formattype == FORMAT_VideoInfo) {
		BITMAPINFOHEADER *pbih = (BITMAPINFOHEADER*)HEADER(mt.pbFormat);
		pProp->cbBuffer = DIBSIZE(*pbih) * 2;

		if (pProp->cbAlign == 0)
			pProp->cbAlign = 1;
		if (pProp->cBuffers == 0)
			pProp->cBuffers = 1;
		FreeMediaType(mt);

		ALLOCATOR_PROPERTIES Actual;
		hr = pAlloc->SetProperties(pProp, &Actual);
		if (FAILED(hr))
			return hr;

		if (pProp->cbBuffer > Actual.cbBuffer)
			return E_FAIL;

		return S_OK;
	}

	return hr;
}


HRESULT CTVOFilter::GetMediaType(int iPos, CMediaType* pMediaType)
{
	if (!this->m_pInput->IsConnected())
		return E_FAIL;

	if (iPos < 0)
		return E_INVALIDARG;

	if (iPos == 0) {	// first (preferred) media format used by this filter
		HRESULT hr = this->m_pInput->ConnectionMediaType(pMediaType);
		if (FAILED(hr))
			return hr;

		if (pMediaType->cbFormat >= sizeof(VIDEOINFOHEADER) && pMediaType->pbFormat != NULL) {
			VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pMediaType->pbFormat;

			if (this->m_filterCfg) {
				vih->bmiHeader.biWidth = this->m_filterCfg->renderCfg.width;
				vih->bmiHeader.biHeight = this->m_filterCfg->renderCfg.height * -1;
				vih->bmiHeader.biSizeImage = DIBSIZE(vih->bmiHeader);

				return S_OK;
			}
			else {
				//vih->bmiHeader.biWidth =1680;
				//vih->bmiHeader.biHeight =-1050;
				//vih->bmiHeader.biSizeImage =DIBSIZE(vih->bmiHeader);

				return S_OK;
			}
		}
	}

	return VFW_S_NO_MORE_ITEMS;
}


HRESULT CTVOFilter::StopStreaming()
{
	int x = 0;
	return CTransformFilter::StopStreaming();
}

HRESULT CTVOFilter::Run(REFERENCE_TIME s)
{
	__int64 x = m_curFrame;
	return CTransformFilter::Run(s);
}


HRESULT CTVOFilter::Transform(IMediaSample* pIn, IMediaSample* pOut)
{
	HRESULT hr = S_OK;
	BYTE* scan0 = NULL;
	int height;
	bool seekOp = false;	// seek operation in progress (true=yes / false=no -> normal playback running)


	hr = pIn->GetPointer(&scan0);
	if (FAILED(hr))
		return hr;
	LONG inLen = pIn->GetActualDataLength();
	LONG ins = pIn->GetSize();

	int stride = 0;
	int width = 0;
	AM_MEDIA_TYPE* mt = NULL;

	hr = pOut->GetMediaType(&mt);
	if (SUCCEEDED(hr))
	{
		if (mt != NULL)
		{
			if (mt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER* v = (VIDEOINFOHEADER*)mt->pbFormat;
				if (v != NULL)
				{
					memcpy(&this->m_vihOut, v, sizeof(VIDEOINFOHEADER));
				}
			}

			DeleteMediaType(mt);
			//return hr;
		}
	}

	REFERENCE_TIME ina = 0, inb = 0;
	pIn->GetTime(&ina, &inb);
	m_curFrame = (__int64)((float)ina / (float)m_vihIn.AvgTimePerFrame);
	__int64 s = (__int64)this->m_pInput->CurrentStartTime() / this->m_vihIn.AvgTimePerFrame;
	m_curFrame += s;

	int frameInc = (int)(m_curFrame - m_refFrame);
	float n = (float)((float)frameInc * (((float)1.0f / (float)m_framerate) / ((float)1.0f / (float)m_filterCfg->dataLogTickRate)));
	m_sampleBufIdx = m_refSample + (int)round((float)n);

	// check for Bottom-Up or Top-Down DIB
	if (this->m_vihIn.bmiHeader.biHeight < 0) {
		// input frame is a Top-Down DIB, so we don't need to flip it and can use it as it is to draw the overlay
		// into that image. BUT the OUTPUT image needs to be flipped as we show the downstream filter(s) that we
		// output Bottom-Up DIBs by setting the biHeight value in bmiHeader to a positive value!!!
		height = this->m_vihIn.bmiHeader.biHeight * -1;
		this->m_flipInputImage = 0;
	}
	else {
		height = this->m_vihIn.bmiHeader.biHeight;
		this->m_flipInputImage = 1; //need to flip the image AND also need to flip it back after we're done with GDI+!!! (see above!!!)
	}

	Bitmap pInFrame(this->m_vihIn.bmiHeader.biWidth, height, this->m_stride, this->m_pixFmt, scan0);
	if (this->m_flipInputImage) {
		pInFrame.RotateFlip(RotateNoneFlipY);
	}
	Graphics* gif = Graphics::FromImage(&pInFrame);
	if (!gif)
		return E_FAIL;

	int h1 = this->m_vihOut.bmiHeader.biHeight;
	int h = abs((int)this->m_vihOut.bmiHeader.biHeight);

	// create new bitmap with output format

	Bitmap pOutFrame(this->m_vihOut.bmiHeader.biWidth, h, this->m_pixFmt);
	Graphics* gfx = Graphics::FromImage(&pOutFrame);
	if (!gfx) {
		delete gif;
		gif = NULL;
		return E_FAIL;
	}

	if (m_dashOverlay != NULL)
	{
		Bitmap* dash = static_cast<Gdiplus::Bitmap*>(m_dashOverlay->RenderDashboard(std::string(""), m_sampleBufIdx));
		if (dash != nullptr) {

			ColorMatrix m = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, (float)m_filterCfg->overlayCfg.transp, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

			RectF rc = RectF();
			rc.X = (float)this->m_filterCfg->overlayCfg.posX;;
			rc.Y = (float)this->m_filterCfg->overlayCfg.posY;;
			rc.Width = (float)((float)dash->GetWidth() * this->m_filterCfg->overlayCfg.scale);;
			rc.Height = (float)((float)dash->GetHeight() * this->m_filterCfg->overlayCfg.scale);;

			ImageAttributes ia;
			ia.SetColorMatrix(&m);
			int w = dash->GetWidth();
			int h = dash->GetHeight();

			if (gif != nullptr) {
				gif->DrawImage(dash, rc, 0, 0, (float)w, (float)h, UnitPixel, &ia);

#ifdef __BETA
				// add watermark (homepage URL) in beta version
				std::wstring wm = std::wstring(L"http://www.video-dashboard.com");

				Font fnt(L"Arial", (float)20);
				RectF bb;
				gif->MeasureString(wm.c_str(), -1, &fnt, PointF(0, 0), &bb);
				RectF loc((float)((float)pInFrame.GetWidth() - (float)bb.Width) / 2.0f, (float)20, bb.Width, bb.Height);

				SolidBrush br(Color::White);
				gif->DrawString(wm.c_str(), -1, &fnt, PointF(loc.X, loc.Y), &br);
#endif
				delete gif;
				gif = NULL;
			}

			delete dash;
			dash = nullptr;
		}
	}

	Rect in(0, 0, pInFrame.GetWidth(), pInFrame.GetHeight());
	Rect out(0, 0, pOutFrame.GetWidth(), pOutFrame.GetHeight());

	gfx->DrawImage(&pInFrame, (int)0, (int)0, (int)pOutFrame.GetWidth(), (int)pOutFrame.GetHeight());

	// flip the output frame
	if (this->m_vihOut.bmiHeader.biHeight > 0)
		pOutFrame.RotateFlip(RotateNoneFlipY);

	BYTE* buf = NULL;
	hr = pOut->GetPointer(&buf);
	if (FAILED(hr))
		return hr;
	LONG outLen = pOut->GetActualDataLength();
	LONG outs = pOut->GetSize();

	BitmapData bd = { 0 };
	Rect rc(0, 0, pOutFrame.GetWidth(), pOutFrame.GetHeight());

	pOutFrame.LockBits(&rc, 0, this->m_pixFmt, &bd);
	if (bd.Scan0 != NULL)
		memcpy(buf, bd.Scan0, this->m_vihOut.bmiHeader.biSizeImage);
	hr = pOut->SetActualDataLength(this->m_vihOut.bmiHeader.biSizeImage);
	pOutFrame.UnlockBits(&bd);

	if (m_progressNotify) {
		m_progressCnt++;
		m_progressNotify->ProgressNotify(m_progressCnt);
	}

	delete gfx;
	gfx = NULL;

	return hr;
}


#pragma endregion
