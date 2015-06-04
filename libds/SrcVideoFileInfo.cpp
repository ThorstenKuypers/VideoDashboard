
#include "SrcVideoFileInfo.h"

using namespace DShow;

CSrcVideoFileInfo::CSrcVideoFileInfo(wchar_t* filename)
{
	int ln =0;
	GUID streamType;

	m_hr =0;
	m_srcVidInfo =NULL;
	m_mediaDet =NULL;

	if (filename ==NULL) 
	{
		m_hr =E_INVALIDARG;
		ln =__LINE__;

		goto fail;
	}

	m_hr =CoCreateInstance(CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER, IID_IMediaDet, (void**)&m_mediaDet);
	if (FAILED(m_hr))
	{
		ln =__LINE__;
		goto fail;
	}
	
	m_mediaDet->put_Filename(filename);
	
	m_srcVidInfo =new SRC_VIDEO_INFO_DATA;
	if (m_srcVidInfo ==NULL) 
	{
		ln =__LINE__;
		m_hr =E_OUTOFMEMORY;

		goto fail;
	}
	memset(m_srcVidInfo, 0, sizeof(SRC_VIDEO_INFO_DATA));

	// get number of media streams inside the source file
	m_hr =m_mediaDet->get_OutputStreams((long*)&m_srcVidInfo->streamCnt);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	// allocate one AM_MEDIA_TYPE structure for each stream
	m_srcVidInfo->streamInfo =new SRC_STREAM_INFO_DATA[m_srcVidInfo->streamCnt];
	if (m_srcVidInfo->streamInfo ==NULL)
	{
		m_hr =E_OUTOFMEMORY;
		ln = __LINE__;

		goto fail;
	}
	memset(m_srcVidInfo->streamInfo, 0, sizeof(SRC_STREAM_INFO_DATA) * m_srcVidInfo->streamCnt);

	// get stream info for each stream
	for (int i =0; i < m_srcVidInfo->streamCnt; i++) 
	{
		m_hr =m_mediaDet->put_CurrentStream((long)i);
		if (FAILED(m_hr))
		{
			ln = __LINE__;

			goto fail;
		}

		m_hr =m_mediaDet->get_StreamType(&streamType);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_mediaDet->get_StreamLength(&m_srcVidInfo->streamInfo[i].duration);
		if (FAILED(m_hr))
		{
			ln = __LINE__;

			goto fail;
		}

		if (streamType ==MEDIATYPE_Video)
		{
			m_hr =m_mediaDet->get_FrameRate(&m_srcVidInfo->streamInfo[i].frameRate);
			if (FAILED(m_hr)) 
			{
				ln = __LINE__;

				goto fail;
			}
		}

		m_hr =m_mediaDet->get_StreamMediaType(&m_srcVidInfo->streamInfo[i].mt);
		if (FAILED(m_hr))
		{
			ln = __LINE__;

			goto fail;
		}

		if ( (m_srcVidInfo->streamInfo[i].mt.majortype ==MEDIATYPE_Video) &&
			 (m_srcVidInfo->streamInfo[i].mt.formattype ==FORMAT_VideoInfo) )
		{
			m_srcVidInfo->streamInfo[i].m_vidInfoHdr =(VIDEOINFOHEADER*)m_srcVidInfo->streamInfo[i].mt.pbFormat;
		}
		if ( (m_srcVidInfo->streamInfo[i].mt.majortype ==MEDIATYPE_Audio) &&
			 (m_srcVidInfo->streamInfo[i].mt.formattype ==FORMAT_WaveFormatEx) )
		{
			m_srcVidInfo->streamInfo[i].m_wavInfoHdr =(WAVEFORMATEX*)m_srcVidInfo->streamInfo[i].mt.pbFormat;
		}
	}

	return;

fail:

	if (m_srcVidInfo->streamInfo !=NULL)
	{
		delete m_srcVidInfo->streamInfo;
		m_srcVidInfo->streamInfo =NULL;
	}

	if (m_srcVidInfo !=NULL) 
	{
		delete m_srcVidInfo;
		m_srcVidInfo =NULL;
	}

	if (m_mediaDet !=NULL)
	{
		m_mediaDet->Release();
		m_mediaDet =NULL;
	}

	throw new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)m_hr, true);
}

CSrcVideoFileInfo::~CSrcVideoFileInfo(void)
{
	if (m_srcVidInfo->streamInfo !=NULL)
	{
		delete m_srcVidInfo->streamInfo;
		m_srcVidInfo->streamInfo =NULL;
	}

	if (m_srcVidInfo !=NULL) 
	{
		delete m_srcVidInfo;
		m_srcVidInfo =NULL;
	}

	if (m_mediaDet !=NULL)
	{
		m_mediaDet->Release();
		m_mediaDet =NULL;
	}
}

SrcVideoInfoData* CSrcVideoFileInfo::getSrcVideoInfo()
{
	if (m_srcVidInfo !=NULL)
		return m_srcVidInfo;

	return NULL;
}