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
#include "DirectShow.h"

#pragma region // implementation of Preview render graph (including TVO-filter)

using namespace DShow;

CPreviewPlayerGraph::CPreviewPlayerGraph(HWND parentWindow, DShow::TVOFILTERCONFIG* filterCfg, LONGLONG avgTimePerFrame)
{
	m_pGraph =NULL;
	m_pControl =NULL;
	m_pEvent =NULL;
	m_pSeek =NULL;
	m_pTvoFilter =NULL;
	m_pVmr =NULL;
	m_pWindowless =NULL;
	m_pConfig =NULL;

	m_parentWnd =parentWindow;
	this->m_pRenderWnd =NULL;
	this->m_pTvoFilterCfg =NULL;

	hr =E_FAIL;

	status =VideoStopped;

	m_seekCaps =0;
	m_canSeek =0;
	m_duration =0;
	this->m_avgTimePerFrame =avgTimePerFrame;

	m_filterCfg =filterCfg;
	
	try {
		BuildFilterGraph();
		connectFilterGraph();
	}
	catch (...) {
		throw;
	}

	Sleep(500);
	this->Play();
}

CPreviewPlayerGraph::~CPreviewPlayerGraph()
{
	ReleaseFilterGraph();
}

/******************************************************************************
** private:
** Method for deleting the Filter Graph
** - done everytime a new video is opened
**
******************************************************************************/
void CPreviewPlayerGraph::ReleaseFilterGraph(void)
{

	if (m_pSeek !=NULL) {
		m_pSeek->Release();
		m_pSeek =NULL;
	}
	if (m_pWindowless !=NULL) {
		m_pWindowless->Release();
		m_pWindowless =NULL;
	}
	if (m_pConfig !=NULL) {
		m_pConfig->Release();
		m_pConfig =NULL;
	}
	if (this->m_pRenderWnd !=NULL) {
		this->m_pRenderWnd->put_Owner((OAHWND)NULL);
		this->m_pRenderWnd->Release();
		this->m_pRenderWnd =NULL;
	}
	if (m_pVmr !=NULL) {
		m_pVmr->Release();
		m_pVmr =NULL;
	}
	if (this->m_pTvoFilterCfg !=NULL) {
		this->m_pTvoFilterCfg->Release();
		this->m_pTvoFilterCfg =NULL;
	}
	if (m_pTvoFilter !=NULL) {
		m_pTvoFilter->Release();
		m_pTvoFilter =NULL;
	}
	if (m_pEvent !=NULL) {
		m_pEvent->Release();
		m_pEvent =NULL;
	}
	if (m_pControl !=NULL) {
		m_pControl->Release();
		m_pControl =NULL;
	}
	if (m_pGraph !=NULL) {
		m_pGraph->Release();
		m_pGraph =NULL;
	}
}

/******************************************************************************
** private:
** Method to pre-build the filter graph (allocates all needed resources
** - I/P pins are not connected now 
**
******************************************************************************/
void CPreviewPlayerGraph::BuildFilterGraph(void)
{
	int _ln =0;

	HRESULT hr =CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGraph);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pControl);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID*)&m_pEvent);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*)&m_pVmr);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->AddFilter(m_pVmr, L"VMR-9");
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =CoCreateInstance(CLSID_TVOFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*)&m_pTvoFilter);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =this->m_pTvoFilter->QueryInterface(IID_ITVOFilterConfig, (LPVOID*)&this->m_pTvoFilterCfg);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =this->m_pTvoFilterCfg->SetFilterConfig(this->m_filterCfg);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->AddFilter(m_pTvoFilter, L"TVOFilter");
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =this->m_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID*)&this->m_pRenderWnd);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =this->m_pRenderWnd->put_Owner((OAHWND)this->m_parentWnd);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =this->m_pRenderWnd->put_Caption(L"TVO Preview of ...");
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->QueryInterface(IID_IMediaSeeking, (LPVOID*)&m_pSeek);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	m_graphId =GRAPHID_OVERLAY_GRAPH;
	hr =m_pEvent->SetNotifyWindow((OAHWND)m_parentWnd, WM_DS_OVERLAY_GRAPH_NOTIFY, m_graphId);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

fail:
	ReleaseFilterGraph();

	CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)hr, true);
	throw ex;

}

void CPreviewPlayerGraph::connectFilterGraph()
{
	DWORD hr =S_OK;
	LONGLONG frame =(LONGLONG)this->m_filterCfg->syncData.startFrame;
	LONGLONG startPos, stopPos;

	startPos =(LONGLONG)(this->m_filterCfg->syncData.startFrame * this->m_avgTimePerFrame);
	stopPos=(LONGLONG)( this->m_filterCfg->syncData.endFrame * this->m_avgTimePerFrame);

	hr =m_pGraph->RenderFile((LPCWSTR)m_filterCfg->videoFileName, NULL);
	if (FAILED(hr)) {
		ReleaseFilterGraph();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

//	m_pControl->Pause();
	LONGLONG dur =0, start =0, stop =0;
	m_seekCaps =AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;

	hr =m_pSeek->CheckCapabilities(&m_seekCaps);
	if (FAILED(hr)) {
		ReleaseFilterGraph();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	hr =m_pSeek->GetCapabilities(&m_seekCaps);
	if (SUCCEEDED(hr)) {
		m_canSeek =1;
		if (m_seekCaps & AM_SEEKING_CanGetStopPos)
			hr =m_pSeek->GetPositions(&start, &stop);
		if (m_seekCaps & AM_SEEKING_CanGetDuration)
			hr =m_pSeek->GetDuration(&dur);
	}
	if (FAILED(hr)) {
		ReleaseFilterGraph();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	if (m_canSeek) {

		hr =m_pSeek->SetPositions(&startPos, AM_SEEKING_AbsolutePositioning, &stopPos, AM_SEEKING_AbsolutePositioning);
		if (FAILED(hr)) {
			m_pControl->Stop();
			ReleaseFilterGraph();
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
			throw ex;
		}
	}
}
 
/******************************************************************************
** This method returns a boolean value which indicates wether the open video
** is seekable or not
**
******************************************************************************/
int CPreviewPlayerGraph::CheckCanSeek()
{
	return m_canSeek;
}

/******************************************************************************
** This methods starts the video playback
**
******************************************************************************/
int CPreviewPlayerGraph::Play()
{
	hr =m_pControl->Run();
	if (FAILED(hr))
		return 0;

	status =VideoRunning;

	return 1;
}


/******************************************************************************
** This methods pauses the video playback
**
******************************************************************************/
int CPreviewPlayerGraph::Pause()
{
	hr =m_pControl->Pause();
	if (FAILED(hr))
		return 0;

	status =VideoPaused;

	return 1;
}

/******************************************************************************
** This methods stops the video playback
**
******************************************************************************/
int CPreviewPlayerGraph::Stop()
{
	hr =m_pControl->Stop();
	if (FAILED(hr))
		return 0;

	status =VideoStopped;

	return 1;
}

/******************************************************************************
** private: 
** This methods converts the Filter Graph Manager playback status into internal 
** playback status
**
******************************************************************************/
int CPreviewPlayerGraph::GetVideoStatus()
{
	OAFilterState state =0;

	hr =m_pControl->GetState(10, &state);
	if (FAILED(hr)) {
		m_pControl->Stop();
		return -1;
	}

	switch (state) 
	{
		case State_Running:
			status =VideoRunning;
			break;
		case State_Stopped:
			status =VideoStopped;
			break;
		case State_Paused:
			status =VideoPaused;
			break;
	}

	return status;
}

/******************************************************************************
** This method reads and returns the DirectShow event code and params
**
******************************************************************************/
long CPreviewPlayerGraph::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
{
	long code;
	
	hr =m_pEvent->GetEvent(&code, param1, param2, 10);
	if (FAILED(hr))
		return -1;

	return code;
}

/******************************************************************************
** Method to free the received DirectShow event code and params
**
******************************************************************************/
void CPreviewPlayerGraph::FreeDShowEventParams(long evCode, LONG_PTR param1,
											  LONG_PTR param2)
{
	m_pEvent->FreeEventParams(evCode, param1, param2);
}

/******************************************************************************
** This methods returns the current time position of the open video in percent
**
******************************************************************************/
int CPreviewPlayerGraph::GetVideoPos()
{
	OAFilterState state =0;
	int x =0;

	hr =m_pSeek->GetCurrentPosition(&m_curPos);
	if (FAILED(hr)) {
		m_pControl->Stop();
		return -1;
	}

	long pos =(long)((m_curPos * 100) / m_duration);

	return (int)pos;
}

/******************************************************************************
** Method for repainting the current video frame, if needed by application. 
** e.g. in response to WM_PAINT message
**
******************************************************************************/
int CPreviewPlayerGraph::RepaintVideo(HWND Window, RECT& windowRect)
{

	return 0;
}

/******************************************************************************
** private:
** This method returns the current video frame in form of a windows Device-
** Independent-Bitmap (DIB)
** -> BITMAPINFOHEADER followed by actual pixel data
**
******************************************************************************/
BYTE* CPreviewPlayerGraph::GetVideoImage()
{
	BYTE* dib =NULL;

	hr =m_pWindowless->GetCurrentImage(&dib);
	if (FAILED(hr)) {
		return NULL;
	}

//	BITMAPINFOHEADER *bmi =(BITMAPINFOHEADER*)dib;
//	Bitmap* bmp =new Bitmap(bmi->biWidth, bmi->biHeight, ((bmi->biBitCount / 8) * bmi->biWidth), PixelFormat32bppRGB, (BYTE*)(dib+bmi->biSize));

//	CoTaskMemFree(dib);

	return dib;
}

/******************************************************************************
** This Method returns the current application internal playback status of
** the open video
**
******************************************************************************/
VideoStatus CPreviewPlayerGraph::GetPlaybackStatus()
{
	return status;
}

#pragma endregion
/***************************************************************************************************************/

#pragma region // implementation of video rendering Filter Graph (input video) class

/******************************************************************************
** default Constructor
**
******************************************************************************/
CInputPlayerGraph::CInputPlayerGraph(HWND parentWindow, wchar_t* videoFile, HWND renderWindow, RECT& windowRect, wchar_t* appRootPath)
{
	m_pGraph =NULL;
	m_pControl =NULL;
	m_pEvent =NULL;
	m_pSeek =NULL;

	m_pVmr =NULL;
	m_pWindowless =NULL;
	m_pConfig =NULL;

	m_parentWnd =parentWindow;
	m_VideoFileName =videoFile;
	this->m_renderWnd =renderWindow;
	memcpy(&this->m_targetRect, &windowRect, sizeof(RECT));

	hr =E_FAIL;

	status =VideoStopped;

	m_seekCaps =0;
	m_canSeek =0;
	m_duration =0;
	m_framesCnt =0;

	m_isVideoOpen =0;

	m_sourceMediaType =NULL;

	m_appRootPath =appRootPath;

	this->m_fname =wcsrchr(this->m_VideoFileName, '\\')+1;

	try {
		BuildFilterGraph();
		ConfigureFilterGraph();
	}
	catch(...) {
		throw;
	}
}

/******************************************************************************
** default Destructor
**
******************************************************************************/
CInputPlayerGraph::~CInputPlayerGraph()
{
	CoTaskMemFree(m_sourceMediaType);
	this->m_videoInfo =NULL;
	this->m_sourceMediaType =NULL;

	ReleaseFilterGraph();
}

/******************************************************************************
** private:
** Method for deleting the Filter Graph
** - done everytime a new video is opened
**
******************************************************************************/
void CInputPlayerGraph::ReleaseFilterGraph(void)
{
	// first stop filter graph
	if (m_pControl !=NULL) {
		m_pControl->Stop();
	}

	if (m_pSeek !=NULL) {
		m_pSeek->Release();
		m_pSeek =NULL;
	}
	if (m_pWindowless !=NULL) {
		m_pWindowless->Release();
		m_pWindowless =NULL;
	}
	if (m_pConfig !=NULL) {
		m_pConfig->Release();
		m_pConfig =NULL;
	}
	if (m_pVmr !=NULL) {
		m_pVmr->Release();
		m_pVmr =NULL;
	}
	if (m_pEvent !=NULL) {
		m_pEvent->Release();
		m_pEvent =NULL;
	}
	if (m_pControl !=NULL) {
		m_pControl->Release();
		m_pControl =NULL;
	}
	if (m_pGraph !=NULL) {
		m_pGraph->Release();
		m_pGraph =NULL;
	}
}

/******************************************************************************
** private:
** Method to pre-build the filter graph
** - I/P pins are not connected now (done in OpenVideo method)
**
******************************************************************************/
void CInputPlayerGraph::BuildFilterGraph()
{
	int _ln =0;

	HRESULT hr =CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGraph);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID*)&m_pEvent);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*)&m_pVmr);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->AddFilter(m_pVmr, L"VMR-9");
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pVmr->QueryInterface(IID_IVMRFilterConfig9, (LPVOID*)&m_pConfig);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pConfig->SetRenderingMode(VMRMode_Windowless);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pVmr->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID*)&m_pWindowless);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	m_graphId =GRAPHID_PLAYBACK_GRAPH;
	hr =m_pEvent->SetNotifyWindow((OAHWND)m_parentWnd, WM_DS_PLAYBACK_GRAPH_NOTIFY, m_graphId);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	// set the video window for VMR9 windowless mode
	hr =m_pWindowless->SetVideoPosition(NULL, &m_targetRect);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pWindowless->SetVideoClippingWindow(m_renderWnd);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	hr =m_pGraph->RenderFile((LPCWSTR)m_VideoFileName, NULL);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	if (m_appRootPath !=NULL) {
		int len =wcslen(m_appRootPath) + wcslen(L"\\InputPlayerGraph.grf") + 1;
		wchar_t* gf =new wchar_t[len];
		memset(gf, 0, len);
		StringCchCat(gf, len, m_appRootPath);
		StringCchCat(gf, len, L"\\InputPlayerGraph.grf");
		//m_hr =SaveGraphFile(m_pGraph, L"C:\\projects\\mygraph.grf");
		hr =SaveGraphFile(m_pGraph, gf);
		if (FAILED(hr)) {
			_ln =__LINE__;
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)hr, true);
			throw ex;
		}
		delete[] gf;
		gf =NULL;
	}

	hr =m_pGraph->QueryInterface(IID_IMediaSeeking, (LPVOID*)&m_pSeek);
	if (FAILED(hr)) {
		_ln =__LINE__;
		goto fail;
	}

	return;

fail:
	ReleaseFilterGraph();

	CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)hr, true);
	throw ex;
}

void CInputPlayerGraph::ConfigureFilterGraph()
{
	HRESULT hr =S_OK;

	hr =m_pGraph->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pControl);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}
 
	if (GetSourceVideoInfo() < 0) {
		hr =ERROR_INVALID_DATA;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	m_seekCaps =AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
	hr =m_pSeek->CheckCapabilities(&m_seekCaps);
	if (SUCCEEDED(hr)) {
		m_canSeek =1;

		// TODO: replace FrameSeekMode with calculation of individual frames based on reference-time
		//  -> more flexible if source or decompressor doesn't support frame-seek-mode (like some MPEG do)
		// .NET class still uses frames, but the frame index will be calculated from ref-time in this graph!
		// -> do the same in Preview- and FileRender FilterGraphs!
		hr =m_pSeek->GetDuration(&m_duration);
		if (FAILED(hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
			throw ex;
		}
	}

	// calculate duration in frames
	m_framesCnt =m_duration / m_videoInfo->AvgTimePerFrame;
}

HRESULT CInputPlayerGraph::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
{
    const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
    HRESULT hr;
    
    IStorage *pStorage = NULL;
    hr = StgCreateDocfile(
        wszPath,
        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0, &pStorage);
    if(FAILED(hr)) 
    {
        return hr;
    }

    IStream *pStream;
    hr = pStorage->CreateStream(
        wszStreamName,
        STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
        0, 0, &pStream);
    if (FAILED(hr)) 
    {
        pStorage->Release();    
        return hr;
    }

    IPersistStream *pPersist = NULL;
    pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
    hr = pPersist->Save(pStream, TRUE);
    pStream->Release();
    pPersist->Release();
    if (SUCCEEDED(hr)) 
    {
        hr = pStorage->Commit(STGC_DEFAULT);
    }
    pStorage->Release();
    return hr;
}

REFERENCE_TIME CInputPlayerGraph::frames2time(REFERENCE_TIME frame)
{
	return (REFERENCE_TIME)frame * m_videoInfo->AvgTimePerFrame;
}

REFERENCE_TIME CInputPlayerGraph::time2frames(REFERENCE_TIME time)
{
	return (REFERENCE_TIME)time / m_videoInfo->AvgTimePerFrame;
}

/******************************************************************************
** This Method checks if seeking in frame format is supported and if it is
** sets the seeking mode
**
******************************************************************************/
//bool CInputPlayerGraph::SetFrameSeekMode()
//{
//	hr =m_pSeek->IsFormatSupported(&TIME_FORMAT_FRAME);
//	if (FAILED(hr)) // seeking format is not supported or other error
//		return false;
//
//	hr =m_pSeek->SetTimeFormat(&TIME_FORMAT_FRAME);
//	if (hr ==VFW_E_WRONG_STATE) // filter graph not stopped
//		m_pControl->Stop();
//	else if (FAILED(hr)) // other error
//		return false;
//
//	m_seekModeFrame =true;
//
//	return true;
//}

/******************************************************************************
** This method returns a boolean value which indicates wether the open video
** is seekable or not
**
******************************************************************************/
bool CInputPlayerGraph::CheckCanSeek()
{
	return m_canSeek;
}

//bool CInputPlayerGraph::CheckCanFrameSeek()
//{
//	return m_seekModeFrame;
//}

/******************************************************************************
** This methods starts the video playback
**
******************************************************************************/
int CInputPlayerGraph::Play()
{
	hr =m_pControl->Run();
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	status =VideoRunning;

	return 0;
}


/******************************************************************************
** This methods pauses the video playback
**
******************************************************************************/
int CInputPlayerGraph::Pause()
{
	hr =m_pControl->Pause();
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	status =VideoPaused;

	return 0;
}

/******************************************************************************
** This methods stops the video playback
**
******************************************************************************/
int CInputPlayerGraph::Stop()
{
	hr =m_pControl->Stop();
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	status =VideoStopped;

	return 0;
}

/******************************************************************************
** private: 
** This methods converts the Filter Graph Manager playback status into internal 
** playback status
**
******************************************************************************/
int CInputPlayerGraph::GetVideoStatus()
{
	OAFilterState state =0;

	hr =m_pControl->GetState(10, &state);
	if (FAILED(hr)) {
		m_pControl->Stop();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	switch (state) 
	{
		case State_Running:
			status =VideoRunning;
			break;
		case State_Stopped:
			status =VideoStopped;
			break;
		case State_Paused:
			status =VideoPaused;
			break;
	}

	return status;
}

/******************************************************************************
** This method reads and returns the DirectShow event code and params
**
******************************************************************************/
long CInputPlayerGraph::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
{
	long code;
	
	hr =m_pEvent->GetEvent(&code, param1, param2, 10);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	return code;
}

/******************************************************************************
** Method to free the received DirectShow event code and params
**
******************************************************************************/
void CInputPlayerGraph::FreeDShowEventParams(long evCode, LONG_PTR param1,
											  LONG_PTR param2)
{
	m_pEvent->FreeEventParams(evCode, param1, param2);
}

__int64 CInputPlayerGraph::GetVideoDuration()
{
	return (__int64)m_framesCnt;
}

/******************************************************************************
** This methods returns the current time position of the open video in percent
**
******************************************************************************/
__int64 CInputPlayerGraph::GetVideoPos()
{
	OAFilterState state =0;

	hr =m_pSeek->GetCurrentPosition(&m_curPos);
	if (FAILED(hr)) {
		m_pControl->Stop();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	return (__int64)time2frames((REFERENCE_TIME)m_curPos);

}

/******************************************************************************
** This methods sets the current time position of the open video based on a
** frame number
******************************************************************************/
int CInputPlayerGraph::SetVideoPos(__int64 frame)
{
	OAFilterState state;
	REFERENCE_TIME newPos =0;
	int paused =0;

	hr =m_pControl->GetState(10, &state);
	if (FAILED(hr)) {
		m_pControl->Stop();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}
	
	if ( (state ==State_Running) || (state ==State_Stopped) ) {
		m_pControl->Pause();
		paused =1;
	}

	if (frame >= 0 && frame <= m_duration) {
		newPos =frames2time((REFERENCE_TIME)frame);
	}
	else 
		return -1;

	hr =m_pSeek->SetPositions(&newPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	if (FAILED(hr)) {
		m_pControl->Stop();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	if (paused)
		m_pControl->Run();
	if (state ==State_Stopped)
		m_pControl->Stop();

	return 0;
}

/******************************************************************************
** Method for repainting the current video frame, if needed by application. 
** e.g. in response to WM_PAINT message
**
******************************************************************************/
void CInputPlayerGraph::RepaintVideo(HDC hdc)
{
	Graphics* gfx =NULL;
	Bitmap* bmp =NULL;
	
	if (hdc !=NULL)
		gfx =Graphics::FromHDC(hdc);
	else
		gfx =Graphics::FromHWND(m_renderWnd);

	if (gfx !=NULL) {
		BYTE* dib =GetVideoImage();
		if (dib !=NULL) {
			BITMAPINFOHEADER* bih =(BITMAPINFOHEADER*)dib;
			void* p =(dib+bih->biSize);
			if (p !=NULL) {
				int stride =(bih->biBitCount / 8) * bih->biWidth;
				bmp =new Bitmap(bih->biWidth, bih->biHeight, stride, PixelFormat32bppRGB, (BYTE*)p);
				if (bmp !=NULL) {
					if (bih->biHeight > 0)
						bmp->RotateFlip(RotateNoneFlipY);

					// TODO: calculate width/height and position of image from the original video frames 
					//			AspectRatio
					float scale =(float)m_targetRect.bottom / bih->biHeight;
					if (scale > 1.0f)
						scale =1.0f;
					float w =(float)bih->biWidth * scale;
					float h =(float)bih->biHeight * scale;
					float x =(float)(m_targetRect.right - w)/2;
					float y =(float)(m_targetRect.bottom - h)/2;

					SolidBrush br(Color::Black);
					gfx->FillRectangle(&br, Rect(m_targetRect.left, m_targetRect.top, m_targetRect.right, m_targetRect.bottom));
					gfx->DrawImage(bmp, RectF(x, y, w, h));

					delete bmp;
				}
			}
			CoTaskMemFree((LPVOID)dib);
		}
		delete gfx;
	}

}

/******************************************************************************
** private:
** This method returns the current video frame in form of a windows Device-
** Independent-Bitmap (DIB)
** -> BITMAPINFOHEADER followed by actual pixel data
**
******************************************************************************/
BYTE* CInputPlayerGraph::GetVideoImage()
{
	BYTE* dib =NULL;

	hr =m_pWindowless->GetCurrentImage(&dib);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	return dib;
}

/******************************************************************************
** This Method returns the current application internal playback status of
** the open video
**
******************************************************************************/
VideoStatus CInputPlayerGraph::GetPlaybackStatus()
{
	return status;
}

int CInputPlayerGraph::GetSourceVideoInfo()
{
	HRESULT hr =S_OK;
	IPin* pin =NULL;
	ULONG ret =0;
	IBaseFilter* filter =NULL;
	AM_MEDIA_TYPE* mt =NULL;
	int bfound =0;
	int ln =__LINE__;

	hr =GetSourceFilter(&filter);
	if (SUCCEEDED(hr)) {
		hr =GetPin(filter, PINDIR_OUTPUT, &pin);
		if (FAILED(hr)) {
			ln =__LINE__;
			goto _free;
		}
		IEnumMediaTypes* mtEnum;
		hr =pin->EnumMediaTypes(&mtEnum);
		if (FAILED(hr)) {
			ln =__LINE__;
			goto _free;
		}
		while ((hr =mtEnum->Next(1, &mt, &ret)) ==S_OK) {
			if (mt->formattype ==FORMAT_VideoInfo && mt->cbFormat ==sizeof(VIDEOINFOHEADER)) {
				m_videoInfo =(VIDEOINFOHEADER*)mt->pbFormat;
				this->m_sourceMediaType =mt;

				bfound =1;
				break;
			}
			CoTaskMemFree(mt);
		}
_free:
		if (mtEnum !=NULL) {
			mtEnum->Release();
			mtEnum =NULL;
		}
		if (pin !=NULL) {
			pin->Release();
			pin =NULL;
		}
		if (filter !=NULL) {
			filter->Release();
			filter =NULL;
		}

		if (FAILED(hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)hr, true);
			throw ex;
		}
	}
	else {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	if (!bfound)
		return -1;

	return S_OK;
}

HRESULT CInputPlayerGraph::GetSourceFilter(IBaseFilter** ppFilter) 
{
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;
	int bfound =0;

    HRESULT hr = m_pGraph->EnumFilters(&pEnum);
    if (FAILED(hr)) return hr;

    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
        FILTER_INFO FilterInfo;
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if (FAILED(hr))
        {
			return hr;
        }
		if (wcscmp(L"AVI Decompressor", FilterInfo.achName) ==0) {
			*ppFilter =pFilter;
			bfound =1;
		}

		// The FILTER_INFO structure holds a pointer to the Filter Graph
		// Manager, with a reference count that must be released.
		if (FilterInfo.pGraph != NULL)
		{
			FilterInfo.pGraph->Release();
		}

		if (bfound)
			break;

        pFilter->Release();
    }

    pEnum->Release();
    return S_OK;
}



int CInputPlayerGraph::GetVideoFrameRate()
{
//	m_sourceFrameRate =m_videoInfo->AvgTimePerFrame / 10000;
	m_sourceFrameRate =(10000000 / m_videoInfo->AvgTimePerFrame);

	return (int)m_sourceFrameRate;
}

LONGLONG CInputPlayerGraph::GetAvgFrameTime()
{
	return (LONGLONG)this->m_videoInfo->AvgTimePerFrame;
}

int CInputPlayerGraph::GetVideoSize(RECT* size)
{
	if (size) {
		memset(size, 0, sizeof(RECT));
		size->right =this->m_videoInfo->bmiHeader.biWidth;
		size->bottom =this->m_videoInfo->bmiHeader.biHeight;

		return 0;
	}

	return -1;
}

HRESULT CInputPlayerGraph::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins  *pEnum = NULL;
    IPin       *pPin = NULL;
    HRESULT    hr;

    if (ppPin == NULL)
    {
        return E_POINTER;
    }

    hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
		
        hr = pPin->QueryDirection(&PinDirThis);
        if (FAILED(hr))
        {
            pPin->Release();
            pEnum->Release();
            return hr;
        }
        if (PinDir == PinDirThis)
        {
            // Found a match. Return the IPin pointer to the caller.
            *ppPin = pPin;
            pEnum->Release();
            return S_OK;
        }
        // Release the pin for the next time through the loop.
        pPin->Release();
    }
    // No more pins. We did not find a match.
    pEnum->Release();
    return E_FAIL;  
}

#pragma endregion


CBaseEncoderGraph::CBaseEncoderGraph()
{
	m_pGraph =NULL;
	m_pControl =NULL;
	m_pEvent =NULL;
	m_pSeek =NULL;

	m_pVmr =NULL;
	m_pWindowless =NULL;
	m_pConfig =NULL;
	m_pRenderWnd =NULL;

	m_pTvoFilter =NULL;
	m_pTvoFilterCfg =NULL;

	m_parentWnd =(HWND)0;	// (parent) window which receives DirectShow events/messages
	m_renderWnd =(HWND)0;	// rendering window for VMR9 windowless mode

	m_graphId =(LONG_PTR)0;

	m_duration =(REFERENCE_TIME)0;
	m_endPos =(REFERENCE_TIME)0;
	m_startPos =(REFERENCE_TIME)0;

	m_srcVideoInfo =NULL;

	m_hr =S_OK;
}

CBaseEncoderGraph::~CBaseEncoderGraph()
{
}

/******************************************************************************
** This method reads and returns the DirectShow event code and params
**
******************************************************************************/
long CBaseEncoderGraph::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
{
	long code;

	m_hr =m_pEvent->GetEvent(&code, param1, param2, 10);
	if (FAILED(m_hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)m_hr, true);
		throw ex;
	}

	return code;
}

/******************************************************************************
** Method to free the received DirectShow event code and params
**
******************************************************************************/
void CBaseEncoderGraph::FreeDShowEventParams(long evCode, LONG_PTR param1,
											  LONG_PTR param2)
{
	m_pEvent->FreeEventParams(evCode, param1, param2);
}

HRESULT CBaseEncoderGraph::GetSourceFilter(IBaseFilter** ppFilter) 
{
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;
	int bfound =0;

    HRESULT hr = m_pGraph->EnumFilters(&pEnum);
    if (FAILED(hr)) return hr;

    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
        FILTER_INFO FilterInfo;
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if (FAILED(hr))
        {
			return hr;
        }
		if (wcscmp(L"AVI Decompressor", FilterInfo.achName) ==0) {
			*ppFilter =pFilter;
			bfound =1;
		}

		// The FILTER_INFO structure holds a pointer to the Filter Graph
		// Manager, with a reference count that must be released.
		if (FilterInfo.pGraph != NULL)
		{
			FilterInfo.pGraph->Release();
		}

		if (bfound)
			break;

        pFilter->Release();
    }

    pEnum->Release();
    return S_OK;
}

HRESULT CBaseEncoderGraph::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins  *pEnum = NULL;
    IPin       *pPin = NULL;
    HRESULT    hr;

    if (ppPin == NULL)
    {
        return E_POINTER;
    }

    hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
		
        hr = pPin->QueryDirection(&PinDirThis);
        if (FAILED(hr))
        {
            pPin->Release();
            pEnum->Release();
            return hr;
        }
        if (PinDir == PinDirThis)
        {
            // Found a match. Return the IPin pointer to the caller.
            *ppPin = pPin;
            pEnum->Release();
            return S_OK;
        }
        // Release the pin for the next time through the loop.
        pPin->Release();
    }
    // No more pins. We did not find a match.
    pEnum->Release();
    return E_FAIL;  
}

int CBaseEncoderGraph::GetSourceVideoInfo()
{
	HRESULT hr =S_OK;
	IPin* pin =NULL;
	ULONG ret =0;
	IBaseFilter* filter =NULL;
	AM_MEDIA_TYPE* mt =NULL;
	int bfound =0;
	int ln =__LINE__;

	hr =GetSourceFilter(&filter);
	if (SUCCEEDED(hr)) {
		hr =GetPin(filter, PINDIR_OUTPUT, &pin);
		if (FAILED(hr)) {
			ln =__LINE__;
			goto _free;
		}
		IEnumMediaTypes* mtEnum;
		hr =pin->EnumMediaTypes(&mtEnum);
		if (FAILED(hr)) {
			ln =__LINE__;
			goto _free;
		}
		while ((hr =mtEnum->Next(1, &mt, &ret)) ==S_OK) {
			if (mt->formattype ==FORMAT_VideoInfo && mt->cbFormat ==sizeof(VIDEOINFOHEADER)) {
				m_srcVideoInfo =(VIDEOINFOHEADER*)mt->pbFormat;
				CoTaskMemFree(mt);

				bfound =1;
				break;
			}
			CoTaskMemFree(mt);
		}
_free:
		if (mtEnum !=NULL) {
			mtEnum->Release();
			mtEnum =NULL;
		}
		if (pin !=NULL) {
			pin->Release();
			pin =NULL;
		}
		if (filter !=NULL) {
			filter->Release();
			filter =NULL;
		}

		if (FAILED(hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)hr, true);
			throw ex;
		}
	}
	else {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	if (!bfound)
		return -1;

	return S_OK;
}

// INTERFACE(S)
void CProgressCallback::ProgressNotify(int curProgress)
{
	((fnProgressCallback)(m_fnProgressCallback))(curProgress);
}

void CProgressCallback::SetCallbackRoutine(void* fnCallback)
{
	m_fnProgressCallback =fnCallback;
}
