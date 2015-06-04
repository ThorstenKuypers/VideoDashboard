
#include "SrcVideoEditor.h"

using namespace DShow;

CSrcVideoEditor::CSrcVideoEditor(void)
{
	int ln =0;
	m_hr =0;

	m_TL =NULL;
	m_audioGroup =NULL;
	m_videoGroup =NULL;
	m_audioTrack =NULL;
	m_videoTrack =NULL;
	m_audioGroupObj =NULL;
	m_videoGroupObj =NULL;
	m_audioTrackObj =NULL;
	m_videoTrackObj =NULL;

	m_curTimelinePos =0;
	m_srcFileCnt =0;

	// create empty timeline object
	m_hr =CoCreateInstance(CLSID_AMTimeline, NULL, CLSCTX_INPROC_SERVER, IID_IAMTimeline, (void**)&m_TL);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	// GROUPS: add audio and video group to the timeline 
	m_hr =m_TL->CreateEmptyNode(&m_audioGroupObj, TIMELINE_MAJOR_TYPE_GROUP);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =m_audioGroupObj->QueryInterface(IID_IAMTimelineGroup, (void**)&m_audioGroup);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =m_TL->CreateEmptyNode(&m_videoGroupObj, TIMELINE_MAJOR_TYPE_GROUP);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =m_videoGroupObj->QueryInterface(IID_IAMTimelineGroup, (void**)&m_videoGroup);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	return;

fail:

	if (m_videoGroup !=NULL)
	{
		m_videoGroup->Release();
		m_videoGroup =NULL;
	}
	if (m_videoGroupObj !=NULL)
	{
		m_videoGroupObj->Release();
		m_videoGroupObj =NULL;
	}
	if (m_audioGroup !=NULL)
	{
		m_audioGroup->Release();
		m_audioGroup =NULL;
	}
	if (m_audioGroupObj !=NULL)
	{
		m_audioGroupObj->Release();
		m_audioGroupObj =NULL;
	}

	if (m_TL !=NULL)
	{
		m_TL->Release();
		m_TL =NULL;
	}

	throw new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, m_hr, true);
}

CSrcVideoEditor::~CSrcVideoEditor(void)
{
	if (m_videoGroup !=NULL)
	{
		m_videoGroup->Release();
		m_videoGroup =NULL;
	}
	if (m_videoGroupObj !=NULL)
	{
		m_videoGroupObj->Release();
		m_videoGroupObj =NULL;
	}
	if (m_audioGroup !=NULL)
	{
		m_audioGroup->Release();
		m_audioGroup =NULL;
	}
	if (m_audioGroupObj !=NULL)
	{
		m_audioGroupObj->Release();
		m_audioGroupObj =NULL;
	}
	if (m_videoTrackObj !=NULL)
	{
		m_videoTrackObj->Release();
		m_videoTrackObj =NULL;
	}
	if (m_audioTrackObj !=NULL)
	{
		m_audioTrackObj->Release();
		m_audioTrackObj =NULL;
	}
	if (m_videoTrack !=NULL)
	{
		m_videoTrack->Release();
		m_videoTrack =NULL;
	}
	if (m_audioTrack !=NULL)
	{
		m_audioTrack->Release();
		m_audioTrack =NULL;
	}

	if (m_TL !=NULL)
	{
		m_TL->Release();
		m_TL =NULL;
	}
}

void CSrcVideoEditor::AddSrcFile(wchar_t *filename)
{
	int ln =0;
	IAMTimelineComp* vidComp =NULL;
	IAMTimelineComp* audComp =NULL;
	CSrcVideoFileInfo* vfi =NULL;
	SrcVideoInfoData* srcFileInfo =NULL;

	if (filename ==NULL)
	{
		m_hr =E_INVALIDARG;
		ln =__LINE__;

		goto fail;
	}

	if (m_srcFileCnt ==0)
	{
		// set media type info for audio and video group of the first source file
		m_hr =setGroupsMediaType(filename);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_TL->AddGroup(m_audioGroupObj);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_TL->AddGroup(m_videoGroupObj);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_TL->CreateEmptyNode(&m_videoTrackObj, TIMELINE_MAJOR_TYPE_TRACK);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_TL->CreateEmptyNode(&m_audioTrackObj, TIMELINE_MAJOR_TYPE_TRACK);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_videoGroup->QueryInterface(IID_IAMTimelineComp, (void**)&vidComp);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_audioGroup->QueryInterface(IID_IAMTimelineComp, (void**)&audComp);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =vidComp->VTrackInsBefore(m_videoTrackObj, 0);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =audComp->VTrackInsBefore(m_audioTrackObj, 0);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_videoTrackObj->QueryInterface(IID_IAMTimelineTrack, (void**)&m_videoTrack);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		m_hr =m_audioTrackObj->QueryInterface(IID_IAMTimelineTrack, (void**)&m_audioTrack);
		if (FAILED(m_hr))
		{
			ln =__LINE__;

			goto fail;
		}

		audComp->Release();
		audComp =NULL;
		vidComp->Release();
		vidComp =NULL;

	}

	try
	{
		vfi =new CSrcVideoFileInfo(filename);
		srcFileInfo =vfi->getSrcVideoInfo();
	}
	catch(CErrEx* ex)
	{
		throw ex;
	}

	// SOURCE: add source to the track
	IAMTimelineSrc* src =NULL;
	IAMTimelineObj* srcObj =NULL;

	m_hr =m_TL->CreateEmptyNode(&srcObj, TIMELINE_MAJOR_TYPE_SOURCE);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =srcObj->QueryInterface(IID_IAMTimelineSrc, (void**)&src);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	REFERENCE_TIME tlStart =m_curTimelinePos;
	REFERENCE_TIME tlStop =m_curTimelinePos + (REFERENCE_TIME)(srcFileInfo->streamInfo[0].duration * (10^7));

	m_hr =srcObj->SetStartStop(tlStart, tlStop);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}
	
	m_hr =src->SetMediaName(filename);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =src->SetMediaTimes(0, (REFERENCE_TIME)(srcFileInfo->streamInfo[0].duration * (10^7)));
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =m_audioTrack->SrcAdd(srcObj);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_hr =m_videoTrack->SrcAdd(srcObj);
	if (FAILED(m_hr))
	{
		ln =__LINE__;

		goto fail;
	}

	m_curTimelinePos =tlStop;

	srcObj->Release();
	srcObj =NULL;
	src->Release();
	src =NULL;

	delete vfi;
	vfi =NULL;

	m_srcFileCnt++;

	return;

fail:
	throw new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, m_hr, true);

}

HRESULT CSrcVideoEditor::setGroupsMediaType(wchar_t* filename)
{
	HRESULT hr =0;
	CSrcVideoFileInfo* srcInfo =NULL;
	SrcVideoInfoData* srcInfoData =NULL;
	
	if (filename ==NULL)
	{
		return E_INVALIDARG;
	}

	try 
	{
		srcInfo =new CSrcVideoFileInfo(filename);
		srcInfoData =srcInfo->getSrcVideoInfo();
	}
	catch(CErrEx* ex)
	{
		throw ex;
	}

	if (srcInfoData !=NULL)
	{
		for (int i=0; i < srcInfoData->streamCnt; i++)
		{
			if (srcInfoData->streamInfo[i].mt.majortype ==MEDIATYPE_Video)
			{
				// set video group media type
				//AM_MEDIA_TYPE mtVideo;
				//memset(&mtVideo, 0, sizeof(AM_MEDIA_TYPE));

				//mtVideo.majortype =srcInfoData->streamInfo[i].mt.majortype;
				hr =m_videoGroup->SetMediaType(&srcInfoData->streamInfo[i].mt);				
				if (FAILED(hr))
					return hr;
			}
			if (srcInfoData->streamInfo[i].mt.majortype ==MEDIATYPE_Audio)
			{
				hr =m_audioGroup->SetMediaType(&srcInfoData->streamInfo[i].mt);
				if (FAILED(hr))
					return hr;
			}
		}

		if (srcInfo !=NULL)
		{
			delete srcInfo;
			srcInfo =NULL;
			srcInfoData =NULL;
		}
	}

	return hr;
}