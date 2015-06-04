#pragma once

#include "SrcVideoFileInfo.h"

// CSrcVideoEditor - This class implements the Source Video Editor. It provides methods for retrieving information about the
//					 source files and the implementation of the DirectShow Editing Services (DES). 

namespace DShow 
{

class CSrcVideoEditor
{
public:
	CSrcVideoEditor(void);
	~CSrcVideoEditor(void);

	void AddSrcFile(wchar_t* filename);

private:

	HRESULT m_hr;

	IAMTimeline* m_TL;
	IAMTimelineGroup* m_audioGroup;
	IAMTimelineGroup* m_videoGroup;

	IAMTimelineObj* m_audioGroupObj;
	IAMTimelineObj* m_videoGroupObj;

	IAMTimelineTrack* m_audioTrack;
	IAMTimelineTrack* m_videoTrack;

	IAMTimelineObj* m_audioTrackObj;
	IAMTimelineObj* m_videoTrackObj;

	int m_srcFileCnt;

	HRESULT setGroupsMediaType(wchar_t* filename);

	REFERENCE_TIME m_curTimelinePos;
};

}