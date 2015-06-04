
#include "RawAviEncoder.h"

using namespace DShow;
using namespace DShow::VideoEncoding;

CRawAviEncoder::CRawAviEncoder(void) : CBaseEncoderGraph()
{
	m_outFileName =NULL;
	m_filterCfg =new TVOFILTERCONFIG;
	memset(m_filterCfg, 0, sizeof(TVOFILTERCONFIG));

	m_pGraphBuilder =NULL;
	m_pCsc =NULL;
	m_pAviDec =NULL;

	m_pAviMux =NULL;
	m_pAviWriter =NULL;
	m_pSrc =NULL;

	m_pComp =NULL;
}

CRawAviEncoder::~CRawAviEncoder(void)
{
	releaseGraph();

	if (m_filterCfg) {
		delete m_filterCfg;
		m_filterCfg =NULL;
	}

	if (this->m_progressCb !=NULL) {
		delete this->m_progressCb;
		this->m_progressCb =NULL;
	}
}

void CRawAviEncoder::buildGraph()
{
	int ln =__LINE__;
	HRESULT hr =S_OK;
	//REFERENCE_TIME startPos =0;
	//REFERENCE_TIME endPos =0;

	hr =CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (LPVOID*)&this->m_pGraphBuilder);
	if (FAILED(hr)) {
		int ln =__LINE__;
		goto _fail;
	}

	// create TVOFilter instance, get config interface and add filter to graph
	hr =CoCreateInstance(CLSID_TVOFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&this->m_pTvoFilter);
	if (FAILED(hr)) {
		int ln =__LINE__;
		goto _fail;
	}
	hr =this->m_pTvoFilter->QueryInterface(IID_ITVOFilterConfig, (LPVOID*)&this->m_pTvoFilterCfg);
	if (FAILED(hr)) {
		int ln =__LINE__;
		goto _fail;
	}

	// configure TVOFilter
	this->m_pTvoFilterCfg->SetFilterConfig((void*)this->m_filterCfg);

	hr =this->m_pGraphBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, this->m_outFileName, (IBaseFilter**)&this->m_pAviMux, &this->m_pAviWriter);
	if (SUCCEEDED(hr)) {
		hr =this->m_pGraphBuilder->GetFiltergraph(&this->m_pGraph);
		if (SUCCEEDED(hr)) {
			hr =m_pGraph->AddSourceFilter(this->m_filterCfg->videoFileName, L"Source Filter", &this->m_pSrc);
			if (SUCCEEDED(hr)) {
				hr =this->m_pGraph->AddFilter(this->m_pTvoFilter, L"TVOFilter");
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}

				if (m_pComp !=NULL) {
					hr =m_pGraph->AddFilter(m_pComp, L"VideoCompressor");
					if (FAILED(hr)) {
						int ln =__LINE__;
						goto _fail;
					}

					// connect video stream to compressor over TVOFilter
					hr =this->m_pGraphBuilder->RenderStream(NULL, NULL, this->m_pSrc, this->m_pTvoFilter, m_pComp);
					if (FAILED(hr)) {
						int ln =__LINE__;
						goto _fail;
					}
					// connect output pin of compressor filter to AVI multiplexer
					hr =this->m_pGraphBuilder->RenderStream(NULL, &MEDIATYPE_Video, this->m_pComp, NULL, this->m_pAviMux);
					if (FAILED(hr)) {
						int ln =__LINE__;
						goto _fail;
					}
					// connect audio stream to AVI multiplexer
					hr =this->m_pGraphBuilder->RenderStream(NULL, NULL, this->m_pSrc, NULL, this->m_pAviMux);
					if (FAILED(hr)) {
						int ln =__LINE__;
						goto _fail;
					}
				}
				else {
					hr =this->m_pGraphBuilder->RenderStream(NULL, NULL, this->m_pSrc, this->m_pTvoFilter, this->m_pAviMux);
					if (SUCCEEDED(hr)) {
						hr =this->m_pGraphBuilder->RenderStream(NULL, NULL, this->m_pSrc, NULL, this->m_pAviMux);
						if (FAILED(hr)) {
							int ln =__LINE__;
							goto _fail;
						}
					}
					else {
						goto _fail;
					}
				}
//				SaveGraphFile(m_pGraph, L"C:\\projects\\mygraph.grf");
				int len =wcslen(m_filterCfg->appPath) + wcslen(L"\\RawEncoderGraph.grf") + 1;
				wchar_t* gf =new wchar_t[len];
				memset(gf, 0, len);
				StringCchCat(gf, len, m_filterCfg->appPath);
				StringCchCat(gf, len, L"\\RawEncoderGraph.grf");
				//m_hr =SaveGraphFile(m_pGraph, L"C:\\projects\\mygraph.grf");
				m_hr =SaveGraphFile(m_pGraph, gf);
				if (FAILED(m_hr)) {
					ln =__LINE__;
					CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)m_hr, true);
					throw ex;
				}
				delete[] gf;
				gf =NULL;

				hr =this->m_pAviMux->QueryInterface(IID_IMediaSeeking, (void**)&this->m_pSeek);
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}
				hr =this->m_pGraph->QueryInterface(IID_IMediaControl, (void**)&this->m_pControl);
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}
				hr =this->m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&this->m_pEvent);
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}

				this->m_graphId =GRAPHID_CAPTURE_GRAPH;
				m_hr =this->m_pEvent->SetNotifyWindow((OAHWND)this->m_parentWnd, WM_DS_CAPTURE_GRAPH_NOTIFY, this->m_graphId);
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}

				hr =this->m_pSeek->GetDuration(&this->m_duration);
				if (FAILED(hr)) {
					int ln =__LINE__;
					goto _fail;
				}
			}
		}
	}
	if (FAILED(hr)) {
		int ln =__LINE__;
		goto _fail;
	}

	return;

_fail:
	releaseGraph();

	CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)hr, true);
	throw ex;
}

void CRawAviEncoder::configGraph()
{
}

void CRawAviEncoder::connectGraph()
{
	int ln =__LINE__;
	HRESULT hr =S_OK;
	REFERENCE_TIME startPos =0;
	REFERENCE_TIME endPos =0;
	IMediaSeeking* seek =NULL;

	hr =this->m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&seek);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)hr, true);
		throw ex;
	}

	GetSourceVideoInfo();
	// set start and stop positions
	
	startPos =(LONGLONG)((LONGLONG)this->m_filterCfg->syncData.startFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame);
	endPos =(LONGLONG)((LONGLONG)this->m_filterCfg->syncData.endFrame * (REFERENCE_TIME)m_srcVideoInfo->AvgTimePerFrame);

	hr =seek->SetPositions(&startPos, AM_SEEKING_AbsolutePositioning, &endPos, AM_SEEKING_AbsolutePositioning);
	if (FAILED(hr)) {
		seek->Release();
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)hr, true);
		throw ex;
	}

	seek->Release();
}

void CRawAviEncoder::releaseGraph()
{
	if (this->m_pEvent !=NULL) {
		this->m_pEvent->Release();
		this->m_pEvent =NULL;
	}
	if (this->m_pControl !=NULL) {
		this->m_pControl->Release();
		this->m_pControl =NULL;
	}
	if (this->m_pSeek !=NULL) {
		this->m_pSeek->Release();
		this->m_pSeek =NULL;
	}
	if (m_pAviWriter !=NULL) {
		m_pAviWriter->Release();
		m_pAviWriter =NULL;
	}
	if (m_pAviMux !=NULL) {
		m_pAviMux->Release();
		m_pAviMux =NULL;
	}
	if (m_pComp !=NULL) {
		m_pComp->Release();
		m_pComp =NULL;
	}
	if (this->m_pTvoFilterCfg) {
		this->m_pTvoFilterCfg->Release();
		this->m_pTvoFilterCfg =NULL;
	}
	if (this->m_pTvoFilter) {
		this->m_pTvoFilter->Release();
		this->m_pTvoFilter =NULL;
	}
	if (m_pSrc !=NULL) {
		m_pSrc->Release();
		m_pSrc =NULL;
	}	
	if (m_pGraph !=NULL) {
		m_pGraph->Release();
		m_pGraph =NULL;
	}
	if (m_pGraphBuilder !=NULL) {
		m_pGraphBuilder->Release();
		m_pGraphBuilder =NULL;
	}

	//***
	if (m_pCsc !=NULL) {
		m_pCsc->Release();
		m_pCsc =NULL;
	}
	if (m_pAviDec !=NULL) {
		m_pAviDec->Release();
		m_pAviDec =NULL;
	}
}

void CRawAviEncoder::EncodeCancel()
{
	if (m_pControl !=NULL) {
		m_hr =m_pControl->Stop();
		if (FAILED(m_hr)) {
			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)m_hr, true);
			throw ex;
		}		
	}
}

int CRawAviEncoder::EncodeStart()
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

void CRawAviEncoder::SetEncoderConfig(PVIDEO_ENCODER_CONFIG cfg)
{
	if (cfg) {
		m_outFileName= cfg->outVideoFileName;
		m_filterCfg->videoFileName =cfg->srcVideoFileName;
		m_filterCfg->appPath =cfg->appPath;
		//m_filterCfg->imgPath =cfg->imgPath;

		m_filterCfg->DashboardLayoutObj =cfg->dashboardLayoutObj;
		m_filterCfg->dataLogTickRate = cfg->dataLogTickRate;

		m_filterCfg->renderCfg.width =cfg->videoWidth;
		m_filterCfg->renderCfg.height =cfg->videoHeight;

		memcpy(&m_filterCfg->syncData, &cfg->syncCfg, sizeof(VIDEO_SYNC_OPTIONS));
		memcpy(&m_filterCfg->overlayCfg, &cfg->overlayCfg, sizeof(OVERLAY_CONFIG_DATA));

		// TODO: replace with selected compression filter
		//m_pProfiler =(CWMProfiler*)cfg->wm9_profilerObj;
		m_pComp =(IBaseFilter*)cfg->videoCompressor;
		m_parentWnd =(HWND)cfg->parentWnd;

		m_preview =cfg->preview;
		//if (cfg->preview) {
		//	m_previewLength =30;	// 30 seconds of preview video rendering
		//}
	}
}

void CRawAviEncoder::SetFileName(wchar_t *fileName)
{
}

long CRawAviEncoder::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
{
	return DShow::CBaseEncoderGraph::GetDShowEventCode(param1, param2);
}

void CRawAviEncoder::FreeDShowEventParams(long evCode, LONG_PTR param1,
											  LONG_PTR param2)
{
	DShow::CBaseEncoderGraph::FreeDShowEventParams(evCode, param1, param2);
}

void CRawAviEncoder::SetProgressCallback(void* cb)
{
	this->m_progressCb =new CProgressCallback();
	m_progressCb->SetCallbackRoutine(cb);
}

HRESULT CRawAviEncoder::SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
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
