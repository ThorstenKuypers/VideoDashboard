
#include "dshow.h"

using namespace DShow;


//
//void CBaseFilterGraph::Start()
//{
//	if (this->m_pControl) this->m_pControl->Run();
//}
//
//void CBaseFilterGraph::Pause()
//{
//	if (this->m_pControl) this->m_pControl->Pause();
//}
//
//void CBaseFilterGraph::Stop()
//{
//	if (this->m_pControl) this->m_pControl->Stop();
//}
//
//long CBaseFilterGraph::GetDShowEventCode(LONG_PTR* param1, LONG_PTR* param2)
//{
//	HRESULT hr =S_OK;
//	long code;
//	
//	hr =m_pEvent->GetEvent(&code, param1, param2, 10);
//	if (FAILED(hr))
//		return -1;
//
//	return code;
//}
//
//void CBaseFilterGraph::FreeDShowEventParams(long evCode, LONG_PTR param1,
//											  LONG_PTR param2)
//{
//	m_pEvent->FreeEventParams(evCode, param1, param2);
//}
//
//
//
//CWMEncoderGraph::CWMEncoderGraph(wchar_t* filename, TVOFILTERCONFIG* cfg, HWND parent)
//{
//	this->m_outFileName =filename;
//	this->m_filterCfg =cfg;
//	this->m_parentWnd =parent;
//}
//
//CWMEncoderGraph::~CWMEncoderGraph()
//{
//	releaseGraph();
//}
//
//void CWMEncoderGraph::buildGraph()
//{
//	// create FilterGraph instance
//	HRESULT hr =CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
//								(LPVOID*)&this->m_pGraph);
//	if (FAILED(hr))
//		goto fail;
//
//	// create ASF Writer instance, add filter to graph, query for FileSinker interface and query for config interface
//	hr =CoCreateInstance(CLSID_WMAsfWriter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter,
//						(LPVOID*)&this->m_pAsfWriter);
//	if (FAILED(hr))
//		goto fail;
//	this->m_pGraph->AddFilter(this->m_pAsfWriter, L"ASF-Writer");
//
//	hr =this->m_pAsfWriter->QueryInterface(IID_IFileSinkFilter2, (LPVOID*)&this->m_pFileSink);
//	if (FAILED(hr)) {
//		goto fail;
//	}
//	hr =this->m_pFileSink->SetFileName(this->m_outFileName, (AM_MEDIA_TYPE*)this->m_pProfiler->m_pMediaType);
//	if (FAILED(hr)) {
//		goto fail;
//	}
//
//	hr =this->m_pAsfWriter->QueryInterface(IID_IConfigAsfWriter2, (LPVOID*)&this->m_pAsfWriterConfig);
//	if (FAILED(hr))
//		goto fail;
//
//	// create TVOFilter instance, get config interface and add filter to graph
//	hr =CoCreateInstance(CLSID_TVOFilter, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&this->m_pTvoFilter);
//	if (FAILED(hr))
//		goto fail;
//	hr =this->m_pTvoFilter->QueryInterface(IID_ITVOFilterConfig, (LPVOID*)&this->m_pTvoFilterCfg);
//	if (FAILED(hr))
//		goto fail;
//	this->m_pGraph->AddFilter(this->m_pTvoFilter, L"TVOFilter");
//
//	// configure TVOFilter
//	this->m_pTvoFilterCfg->SetFilterConfig((void*)this->m_filterCfg);
//
//	hr =m_pGraph->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pControl);
//	if (FAILED(hr)) {
//		// release the GraphBuilder component
//		goto fail;
//	}
//	hr =m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID*)&m_pEvent);
//	if (FAILED(hr)) {
//		goto fail;
//	}
//	this->m_graphId =GRAPHID_CAPTURE_GRAPH;
//	hr =this->m_pEvent->SetNotifyWindow((OAHWND)this->m_parentWnd, WM_DS_CAPTURE_GRAPH_NOTIFY, this->m_graphId);
//	if (FAILED(hr))
//		goto fail;
//
//	hr =this->m_pAsfWriterConfig->ConfigureFilterUsingProfile(this->m_pProfiler->m_profile);
//	if (FAILED(hr))
//		throw hr;
//
//	return;
//
//fail:
//	releaseFilterGraph();
//	throw hr;
//}
//
//void CWMEncoderGraph::configGraph()
//{
//}
//
//void CWMEncoderGraph::releaseGraph()
//{
//	if (this->m_pEvent) {
//		this->m_pEvent->Release();
//		this->m_pEvent =NULL;
//	}
//	if (this->m_pControl) {
//		this->m_pControl->Release();
//		this->m_pControl =NULL;
//	}
//	if (this->m_pAsfWriterConfig) {
//		this->m_pAsfWriterConfig->Release();
//		this->m_pAsfWriterConfig =NULL;
//	}
//	if (this->m_pFileSink) {
//		this->m_pFileSink->Release();
//		this->m_pFileSink =NULL;
//	}
//	if (this->m_pAsfWriter) {
//		this->m_pAsfWriter->Release();
//		this->m_pAsfWriter =NULL;
//	}
//	if (this->m_pTvoFilterCfg) {
//		this->m_pTvoFilterCfg->Release();
//		this->m_pTvoFilterCfg =NULL;
//	}
//	if (this->m_pTvoFilter) {
//		this->m_pTvoFilter->Release();
//		this->m_pTvoFilter =NULL;
//	}
//	if (this->m_pGraph) {
//		this->m_pGraph->Release();
//		this->m_pGraph =NULL;
//	}
//}
//
//bool CWMEncoderGraph::CheckCanSeek()
//{
//	return false;
//}
//
//__int64 CWMEncoderGraph::GetCurrentVideoPos()
//{
//	LONGLONG pos =0;
//	this->m_pSeek->GetCurrentPosition(&pos);
//	return pos;
//}
//
//int CWMEncoderGraph::GetVideoDuration()
//{
//	__int64 dur =0;
//	this->m_pSeek->GetDuration(&dur);
//	return dur;
//}
//
//int CWMEncoderGraph::SetCurrentVideoPos(__int64 pos)
//{
//	return -1;
//}
