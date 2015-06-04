#pragma once
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

#include <math.h>
#include "DirectShow.h"
#include <Wmsdk.h>
#include <Wmdrmsdk.h>

// WindowsMedia Profiler, handles everything related to WindowsMedia (Profiles, codecs, etc)
namespace DShow {
#pragma unmanaged

#define SAFE_RELEASE(p) if(p){ delete p; p =NULL;}

	typedef struct _videoStreamInfo
	{
		WM_MEDIA_TYPE* pmt;
		DWORD quality;
		LONGLONG keyframesIntv;

	}VIDEO_STREAM_INFO, *PVIDEO_STREAM_INFO;

	typedef struct _WM_Profile
	{
		wchar_t* name;
		wchar_t* path;

		WMVIDEOINFOHEADER wmv_hdr;
		WAVEFORMATEX wav_hdr;

		DWORD quality;
		LONGLONG keyframes;
	}WM9PROFILE, *PWM9PROFILE;

	struct IProfiler {
		protected:
			void* wm_profiler;
	};


// *** NOTE: only 1 Audio and 1 Video Streams are supported! ***
class CWMProfiler : IProfiler
{
public:
	CWMProfiler();
	~CWMProfiler(void);
	WM_MEDIA_TYPE* m_pMediaType; // obsolete

	WM9PROFILE* LoadProfile(wchar_t* profileName);
	void CloseProfile();
	void configureVideoStream();
	IWMProfile3* GetRawProfileHandle();	// of current active profile

private:

	IWMProfileManager* m_profileMgr;	// profile manager object
	IWMProfile3* m_profile;	// profile object
	IWMStreamConfig* m_videoStream; // video stream object
	IWMStreamConfig* m_audioStream; // audio stream object
	WM_MEDIA_TYPE* m_mtAudio;
	WM_MEDIA_TYPE* m_mtVideo;

	IWMCodecInfo3* m_codecInfo;	// codec information

	WM9PROFILE* m_wm9Profile; // current active profile

	DWORD m_codecIdx;
	DWORD m_codecCnt;

	DWORD m_codexFmtIdx;
	DWORD m_codecFmtCnt;
	
	PDWORD* m_profileStreams;
	DWORD m_profileStreamCnt;

	bool loadProfileFromFile(wchar_t* profileName);
	void enumCodecs();
};


}