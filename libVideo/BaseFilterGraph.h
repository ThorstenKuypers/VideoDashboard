#ifndef _BASE_FILTER_GRAPH_H_
#define _BASE_FILTER_GRAPH_H_

#include <Windows.h>
#include <atlbase.h>
#include <dshow.h>

#include "libVideo.h"

namespace libVideo
{

	class CBaseFilterGraph
	{
	public:
		CBaseFilterGraph();
		~CBaseFilterGraph();

		int Play();
		int Pause();
		int Stop();

		long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
		void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);

		int GetVideoFrameRate();
		LONGLONG GetAvgFrameTime();
		
		BYTE* GetCurrentVideoImage();
		bool CheckCanSeek();
		LONGLONG GetVideoDuration();
		LONGLONG GetVideoPos();
		int SetVideoPos(__int64 frame);
		int GetVideoSize(RECT* rc);

		void RepaintVideo(HDC hdc);

		DWORD GetLastError(HRESULT* lastComErr);

		VideoStatus GetPlaybackStatus();

	protected:
		CComPtr<IGraphBuilder> _graph;
		CComPtr<IMediaControl> _control;
		CComPtr<IMediaEventEx> _event;
		CComPtr<IMediaSeeking> _seek;

		CComPtr<IBaseFilter> _vmr;
		//IVMRWindowlessControl9* m_pWindowless;

		virtual void BuildFilterGraph() = 0;
		virtual void ReleaseFilterGraph(void) = 0;
		virtual void ConfigureFilterGraph() = 0;

	private:
	};

}

#endif // _BASE_FILTER_GRAPH_H_