
#include "WMEncoder.h"

using namespace DShow;
using namespace DShow::VideoEncoding;

CWMEncoder::CWMEncoder(void) : CBaseEncoderGraph()
{
	this->m_hr =S_OK;
	m_outFileName =NULL;
	m_filterCfg =new TVOFILTERCONFIG;
	memset(m_filterCfg, 0, sizeof(TVOFILTERCONFIG));

	m_pProfiler =NULL;
	m_pEvent =NULL;
	m_pControl =NULL;
	m_pSeek =NULL;
	m_pFileSink =NULL;
	m_pProfiler =NULL;
	m_pAsfWriterConfig =NULL;
	m_pAsfWriter =NULL;
	m_pTvoFilterCfg =NULL;
	m_pTvoFilter =NULL;
	m_pGraph =NULL;

}

CWMEncoder::~CWMEncoder(void)
{
	releaseGraph();

	if (m_filterCfg) {
		delete m_filterCfg;
		m_filterCfg =NULL;
	}
}

void CWMEncoder::buildGraph()
{
	int ln =0;

	// get capture graph builder instance
	//m_hr =CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (LPVOID*)&this->m_pGraphBuilder);
	//if (FAILED(m_hr)) goto fail;

	// create FilterGraph instance
	m_hr =CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&this->m_pGraph);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	// assign filter graph to capture graph
//	this->m_pGraphBuilder->SetFiltergraph(this->m_pGraph);

	// create ASF Writer instance, add filter to graph, query for FileSinker interface and query for config interface
	m_hr =CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&this->m_pAsfWriter);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pGraph->AddFilter(this->m_pAsfWriter, L"ASF-Writer");
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pAsfWriter->QueryInterface(IID_IFileSinkFilter2, (LPVOID*)&this->m_pFileSink);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pFileSink->SetFileName(this->m_outFileName, (AM_MEDIA_TYPE*)this->m_pProfiler->m_pMediaType);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pAsfWriter->QueryInterface(IID_IConfigAsfWriter2, (LPVOID*)&this->m_pAsfWriterConfig);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	// create TVOFilter instance, get config interface and add filter to graph
	m_hr =CoCreateInstance(CLSID_TVOFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&this->m_pTvoFilter);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pTvoFilter->QueryInterface(IID_ITVOFilterConfig, (LPVOID*)&this->m_pTvoFilterCfg);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =this->m_pGraph->AddFilter(this->m_pTvoFilter, L"TVOFilter");
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	// configure TVOFilter
	this->m_pTvoFilterCfg->SetFilterConfig((void*)this->m_filterCfg);

	m_hr =m_pGraph->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pControl);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	m_hr =m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID*)&m_pEvent);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}

	this->m_graphId =GRAPHID_CAPTURE_GRAPH;
	m_hr =this->m_pEvent->SetNotifyWindow((OAHWND)this->m_parentWnd, WM_DS_CAPTURE_GRAPH_NOTIFY, this->m_graphId);
	if (FAILED(m_hr)) {
		ln =__LINE__;
		goto fail;
	}
	
	return;

fail:
	releaseGraph();

	CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)m_hr, true);
	throw ex;
}

void CWMEncoder::configGraph()
{
}

void CWMEncoder::connectGraph()
{
	int _ln =0;

	try {
		m_hr =this->m_pAsfWriterConfig->ConfigureFilterUsingProfile(this->m_pProfiler->GetRawProfileHandle());
		if (FAILED(m_hr)) {
			_ln =__LINE__;
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
			throw ex;
		}

		this->m_pProfiler->configureVideoStream();
	}
	catch (...) {
		throw;
	}

	Sleep(500);

	IMediaSeeking* seek =NULL;
	m_hr =this->m_pGraph->QueryInterface(IID_IMediaSeeking, (LPVOID*)&seek);
	if (FAILED(m_hr)) {
		_ln =__LINE__;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
		throw ex;
	}

	m_hr =this->m_pGraph->RenderFile(this->m_filterCfg->videoFileName, NULL);
	if (FAILED(m_hr)) {
		_ln =__LINE__;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
		throw ex;
	}

	DWORD caps =0;
	LONGLONG cpos =0;
	LONGLONG endPos =0;
	LONGLONG startPos =0;
	LONGLONG dur =0;

	char canSeek =0;

	//****
	int len =wcslen(m_filterCfg->appPath) + wcslen(L"\\WMEncoderGraph.grf") + 1;
	wchar_t* gf =new wchar_t[len];
	memset(gf, 0, len);
	StringCchCat(gf, len, m_filterCfg->appPath);
	StringCchCat(gf, len, L"\\WMEncoderGraph.grf");
	//m_hr =SaveGraphFile(m_pGraph, L"C:\\projects\\mygraph.grf");
	m_hr =SaveGraphFile(m_pGraph, gf);
	if (FAILED(m_hr)) {
		_ln =__LINE__;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
		throw ex;
	}
	delete[] gf;
	gf =NULL;

	//****

	GetSourceVideoInfo();

	m_hr =seek->GetCapabilities(&caps);
	if (SUCCEEDED(m_hr)) {
		if (caps & AM_SEEKING_CanSeekAbsolute) {
			canSeek =1;
		}
		if (caps & AM_SEEKING_CanGetCurrentPos) {
			cpos =this->m_filterCfg->syncData.startFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame;
			m_hr =seek->GetPositions(&startPos, &endPos);
		}
		if (caps & AM_SEEKING_CanGetStopPos) {
			endPos =this->m_filterCfg->syncData.endFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame;
			m_hr =seek->GetPositions(&startPos, &endPos);
		}
		if (caps & AM_SEEKING_CanGetDuration) {
			m_hr =seek->GetDuration(&dur);
		}
	}
	if (FAILED(m_hr)) {
		_ln =__LINE__;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
		throw ex;
	}

	//if (canSeek) {
		startPos =(LONGLONG)((LONGLONG)this->m_filterCfg->syncData.startFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame);
		endPos =(LONGLONG)((LONGLONG)this->m_filterCfg->syncData.endFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame);

		m_hr =seek->SetPositions(&startPos, AM_SEEKING_AbsolutePositioning, &endPos, AM_SEEKING_AbsolutePositioning);
		if (FAILED(m_hr)) {
			seek->Release();
			_ln =__LINE__;
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, _ln, (DWORD)m_hr, true);
			throw ex;
		}
	//}

	seek->Release();
	seek =NULL;
}

void CWMEncoder::releaseGraph()
{
	if (this->m_pEvent) {
		this->m_pEvent->Release();
		this->m_pEvent =NULL;
	}
	if (this->m_pControl) {
		this->m_pControl->Release();
		this->m_pControl =NULL;
	}
	if (this->m_pSeek) {
		this->m_pSeek->Release();
		this->m_pSeek =NULL;
	}
	if (this->m_pFileSink) {
		this->m_pFileSink->Release();
		this->m_pFileSink =NULL;
	}
	//if (this->m_pProfiler) {
	//	delete m_pProfiler;
	//	this->m_pProfiler =NULL;
	//}
	if (this->m_pAsfWriterConfig) {
		this->m_pAsfWriterConfig->Release();
		this->m_pAsfWriterConfig =NULL;
	}
	if (this->m_pAsfWriter) {
		this->m_pAsfWriter->Release();
		this->m_pAsfWriter =NULL;
	}
	if (this->m_pTvoFilterCfg) {
		this->m_pTvoFilterCfg->Release();
		this->m_pTvoFilterCfg =NULL;
	}
	if (this->m_pTvoFilter) {
		this->m_pTvoFilter->Release();
		this->m_pTvoFilter =NULL;
	}
	if (this->m_pGraph) {
		this->m_pGraph->Release();
		this->m_pGraph =NULL;
	}
}

void CWMEncoder::EncodeCancel()
{
	if (m_pControl !=NULL) {
		m_hr =m_pControl->Stop();
		if (FAILED(m_hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)m_hr, true);
			throw ex;
		}
	}
}

int CWMEncoder::EncodeStart()
{
	try {
		buildGraph();
		connectGraph();

		this->m_pTvoFilterCfg->SetNotifyCallback((IProgressCallback*)m_progressCb);

		m_hr =m_pControl->Run();
		if (FAILED(m_hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)m_hr, true);
			throw ex;
		}
	}
	catch (...) {
		throw;
	}

	return 0;
}

void CWMEncoder::SetEncoderConfig(PVIDEO_ENCODER_CONFIG cfg)
{
	if (cfg) {
		m_outFileName= cfg->outVideoFileName;
		m_filterCfg->videoFileName =cfg->srcVideoFileName;
		m_filterCfg->appPath =cfg->appPath;

		m_filterCfg->DashboardLayoutObj =cfg->dashboardLayoutObj;
		m_filterCfg->dataLogTickRate = cfg->dataLogTickRate;

		m_filterCfg->renderCfg.width =cfg->videoWidth;
		m_filterCfg->renderCfg.height =cfg->videoHeight;

		memcpy(&m_filterCfg->syncData, &cfg->syncCfg, sizeof(VIDEO_SYNC_OPTIONS));
		memcpy(&m_filterCfg->overlayCfg, &cfg->overlayCfg, sizeof(OVERLAY_CONFIG_DATA));

		m_pProfiler =(CWMProfiler*)cfg->wm9_profilerObj;
		m_parentWnd =(HWND)cfg->parentWnd;

		m_preview =cfg->preview;
		if (cfg->preview) {
			m_previewLength =30;	// 30 seconds of preview video rendering
		}
	}
}

void CWMEncoder::SetFileName(wchar_t *fileName)
{
}

void CWMEncoder::SetProgressCallback(void* cb)
{
	this->m_progressCb =new CProgressCallback();
	m_progressCb->SetCallbackRoutine(cb);
}

/******************************************************************************
** This method reads and returns the DirectShow event code and params
**
******************************************************************************/
long CWMEncoder::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
{
	return DShow::CBaseEncoderGraph::GetDShowEventCode(param1, param2);
}

/******************************************************************************
** Method to free the received DirectShow event code and params
**
******************************************************************************/
void CWMEncoder::FreeDShowEventParams(long evCode, LONG_PTR param1,
											  LONG_PTR param2)
{
	DShow::CBaseEncoderGraph::FreeDShowEventParams(evCode, param1, param2);
}

HRESULT CWMEncoder::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
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
