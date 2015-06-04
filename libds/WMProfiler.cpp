
#include "WMProfiler.h"
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011-2012 Thorsten Kuypers
//	All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////


using namespace DShow;

CWMProfiler::CWMProfiler()
{
	HRESULT hr;

	m_pMediaType =NULL;
	m_profile =NULL;
	m_profileMgr =NULL;	// profile manager object
	m_profile =NULL;	// profile object
	m_videoStream =NULL; // video stream object
	m_audioStream =NULL; // audio stream object
	m_mtAudio =NULL;
	m_mtVideo =NULL;

	m_codecInfo =NULL;	// codec information
	m_wm9Profile =NULL; // current active profile
	m_codecIdx =NULL;
	m_codecCnt =NULL;
	m_codexFmtIdx =NULL;
	m_codecFmtCnt =NULL;
	m_profileStreams =NULL;
	m_profileStreamCnt =NULL;

	wm_profiler =(void*)this;

	// create profile object
	hr =WMCreateProfileManager(&this->m_profileMgr);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	hr =this->m_profileMgr->QueryInterface(IID_IWMCodecInfo3, (LPVOID*)&this->m_codecInfo);
	if (FAILED(hr)) {
		SAFE_RELEASE(this->m_profileMgr);

		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	hr =this->m_codecInfo->GetCodecInfoCount(WMMEDIATYPE_Video, &this->m_codecCnt);
	if (SUCCEEDED(hr)) {
		hr =this->m_codecInfo->GetCodecFormatCount(WMMEDIATYPE_Video, 0, &this->m_codecFmtCnt);
		if (FAILED(hr)) {
			SAFE_RELEASE(this->m_codecInfo);
			SAFE_RELEASE(this->m_profileMgr);

			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
			throw ex;
		}
	}
	else {
		SAFE_RELEASE(this->m_codecInfo);
		SAFE_RELEASE(this->m_profileMgr);

		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}
}

CWMProfiler::~CWMProfiler(void)
{
	if (this->m_pMediaType) {
		delete[] this->m_pMediaType;
		this->m_pMediaType =NULL;
	}
	if (this->m_mtAudio) {
		delete[] this->m_mtAudio;
		this->m_mtAudio =NULL;
	}
	if (this->m_mtVideo) {
		delete[] this->m_mtVideo;
		this->m_mtVideo =NULL;
	}
	if (this->m_wm9Profile) {
		delete this->m_wm9Profile;
		this->m_wm9Profile =NULL;
	}

	if (this->m_videoStream) {
		this->m_videoStream->Release();
		this->m_videoStream =NULL;
	}
	if (this->m_audioStream) {
		this->m_audioStream->Release();
		this->m_audioStream =NULL;
	}
	if (this->m_codecInfo) {
		this->m_codecInfo->Release();
		this->m_codecInfo =NULL;
	}
	if (this->m_profile) {
		this->m_profile->Release();
		this->m_profile =NULL;
	}
	if (this->m_profileMgr) {
		this->m_profileMgr->Release();
		this->m_profileMgr =NULL;
	}
}

bool CWMProfiler::loadProfileFromFile(wchar_t* profileName)
{
	wchar_t* buf;
	DWORD len, ret;
	HANDLE hf;	// file handle
	HRESULT hr =S_OK;

	if (m_profile !=NULL)
		CloseProfile();

	hf =CreateFile(profileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hf ==INVALID_HANDLE_VALUE) {
		//delete file;
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)GetLastError(), true);
		throw ex;
	}

	len =GetFileSize(hf, NULL);
	buf =new wchar_t[len];
	if (!buf) {
		CloseHandle(hf);

		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)GetLastError(), true);
		throw ex;
	}
	if (!ReadFile(hf, buf, len, &ret, NULL)) {
		if (buf)
			delete buf;
		CloseHandle(hf);

		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)GetLastError(), true);
		throw ex;
	}
	CloseHandle(hf);

	hr =this->m_profileMgr->LoadProfileByData(buf, (IWMProfile**)&this->m_profile);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	delete buf;

	return true;
}

void CWMProfiler::CloseProfile()
{
	if (this->m_mtAudio) {
		delete[] this->m_mtAudio;
		this->m_mtAudio =NULL;
	}
	if (this->m_mtVideo) {
		delete[] this->m_mtVideo;
		this->m_mtVideo =NULL;
	}
	if (this->m_wm9Profile) {
		delete this->m_wm9Profile;
		this->m_wm9Profile =NULL;
	}

	if (this->m_videoStream) {
		this->m_videoStream->Release();
		this->m_videoStream =NULL;
	}
	if (this->m_audioStream) {
		this->m_audioStream->Release();
		this->m_audioStream =NULL;
	}
	if (this->m_codecInfo) {
		this->m_codecInfo->Release();
		this->m_codecInfo =NULL;
	}
	if (this->m_profile) {
		this->m_profile->Release();
		this->m_profile =NULL;
	}
}

WM9PROFILE* CWMProfiler::LoadProfile(wchar_t *profileName)
{
	HRESULT hr =S_OK;
	DWORD streamCnt =0;
	bool vidFound =false;
	bool audFound =false;
	IWMStreamConfig* tmp =NULL;
	GUID streamT;
	IWMMediaProps* mp =NULL;
	DWORD len =0;

	if (!profileName)
		return NULL;

	if (!loadProfileFromFile(profileName))
		return NULL;

	if (!m_profile)
		return NULL;

	hr =m_profile->GetStreamCount(&streamCnt);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	// find the first audio and video stream
	for (DWORD i =0; i < streamCnt; i++)
	{
		hr =m_profile->GetStream(i, (IWMStreamConfig**)&tmp);
		if (FAILED(hr))
			break;
		hr =tmp->GetStreamType(&streamT);
		if (FAILED(hr)) 
		{
			tmp->Release();
			tmp =NULL;

			CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
			throw ex;
		}
		if (streamT ==WMMEDIATYPE_Audio && !audFound) {
			this->m_audioStream =tmp;
			audFound =true;
		}
		else if (streamT ==WMMEDIATYPE_Video && !vidFound) {
			this->m_videoStream =tmp;
			vidFound =true;
		}
		else {
			tmp->Release();
			tmp =NULL;
		}

		if (audFound && vidFound)
			break;
	}

	// get media types for audio and video
	hr =m_videoStream->QueryInterface(IID_IWMMediaProps, (PVOID*)&mp);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	hr =mp->GetMediaType(NULL, &len);
	if (FAILED(hr)) {
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}

	this->m_mtVideo =(WM_MEDIA_TYPE*)new BYTE[len];
	hr =mp->GetMediaType(m_mtVideo, &len);
	if (FAILED(hr)) {
		delete m_mtVideo;
		m_mtVideo =NULL;

		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)hr, true);
		throw ex;
	}
	mp->Release(); mp =NULL;

	hr =m_audioStream->QueryInterface(IID_IWMMediaProps, (PVOID*)&mp);
	if (FAILED(hr)) 
		return NULL;
	hr =mp->GetMediaType(NULL, &len);
	if (FAILED(hr))
		return NULL;
	this->m_mtAudio =(WM_MEDIA_TYPE*)new BYTE[len];
	hr =mp->GetMediaType(m_mtAudio, &len);
	if (FAILED(hr)) {
		delete m_mtAudio;
		m_mtAudio =NULL;
		return NULL;
	}
	mp->Release(); mp =NULL;

	IWMVideoMediaProps* vmp =NULL;
	WMVIDEOINFOHEADER* wmvih =NULL;
	WAVEFORMATEX* wfe =NULL;
	m_wm9Profile =new WM9PROFILE;
	memset(m_wm9Profile, 0, sizeof(WM9PROFILE));
	
	if (m_mtVideo->formattype ==WMFORMAT_VideoInfo && m_mtVideo->cbFormat ==sizeof(WMVIDEOINFOHEADER)) {
		wmvih =(WMVIDEOINFOHEADER*)m_mtVideo->pbFormat;
		memcpy(&m_wm9Profile->wmv_hdr, wmvih, sizeof(WMVIDEOINFOHEADER));
	}
	if (m_mtAudio->formattype ==WMFORMAT_WaveFormatEx) {
		wfe =(WAVEFORMATEX*)m_mtAudio->pbFormat;
		memcpy(&m_wm9Profile->wav_hdr, wfe, sizeof(WAVEFORMATEX));
	}
	hr =m_videoStream->QueryInterface(IID_IWMVideoMediaProps, (PVOID*)&vmp);
	if (FAILED(hr))
		return NULL;

	hr =vmp->GetQuality(&m_wm9Profile->quality);
	if (FAILED(hr)) {
		vmp->Release();
		vmp =NULL;
		return NULL;
	}
	hr =vmp->GetMaxKeyFrameSpacing(&m_wm9Profile->keyframes);
	if (FAILED(hr)) {
		vmp->Release();
		vmp =NULL;
		return NULL;
	}
	vmp->Release();
	vmp =NULL;

	return m_wm9Profile;
}

IWMProfile3* CWMProfiler::GetRawProfileHandle()
{
	return this->m_profile;
}

void CWMProfiler::configureVideoStream()
{
	HRESULT hr =S_OK;

	hr =this->m_profile->ReconfigStream(this->m_audioStream);
	if (FAILED(hr))
		throw hr;
	hr =this->m_profile->ReconfigStream(this->m_videoStream);
	if (FAILED(hr))
		throw hr;
}

void CWMProfiler::enumCodecs()
{
}
