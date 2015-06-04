#pragma once

#include "DirectShow.h"

namespace DShow 
{

	// this structure contains all information related to the source stream including the AM_MEDIA_TYPE data
	typedef struct SrcStreamInfoData
	{
		double duration;	// duration of the stream in seconds
		double frameRate;	// frame rate of the stream in frames per second
		AM_MEDIA_TYPE mt;	// the AM_MEDIA_TYPE buffer for the stream
		VIDEOINFOHEADER* m_vidInfoHdr;
		WAVEFORMATEX* m_wavInfoHdr;

	}SRC_STREAM_INFO_DATA, *PSRC_STREAM_INFO_DATA;

	// this structure contains the STREAM_INFO data of all the streams within a source video file
	typedef struct SrcVideoInfoData
	{
		int streamCnt;	// stream count
		SRC_STREAM_INFO_DATA* streamInfo;	// array of AM_MEDIA_TYPE structures
	}SRC_VIDEO_INFO_DATA, *PSRC_VIDEO_INFO_DATA;


class CSrcVideoFileInfo
{
public:
	CSrcVideoFileInfo(wchar_t* filename);
	~CSrcVideoFileInfo(void);

	SrcVideoInfoData* getSrcVideoInfo();

private:

	HRESULT m_hr;
	IMediaDet* m_mediaDet;

	SRC_VIDEO_INFO_DATA* m_srcVidInfo;
};

}