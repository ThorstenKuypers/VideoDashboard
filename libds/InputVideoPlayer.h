#pragma once

#include "DirectShow.h"

namespace libds {

	using namespace DShow;

#pragma managed

	using namespace System;
	using namespace System::Drawing;
	using namespace System::ComponentModel;
	using namespace System::Runtime::InteropServices;

	public enum VideoPlaybackStatusEnum
	{
		VideoStatus_stopped =0,
		VideoStatus_playing =1,
		VideoStatus_pause =2
	};

	public ref struct VideoFrame
	{
		IntPtr rawPtr;
		Drawing::Bitmap^ frame;
	};

public ref class InputVideoPlayer : IDisposable
{
public:
	InputVideoPlayer(IntPtr parentWnd, IntPtr DisplayWnd, System::Drawing::Rectangle DisplayRect, String^ videoFile, String^ appRootPath)
	{
		m_videoFile =(wchar_t*)(Marshal::StringToHGlobalUni(videoFile).ToPointer());
		RECT rc;
		rc.left =DisplayRect.X;
		rc.right =DisplayRect.Width;
		rc.top =DisplayRect.Y;
		rc.bottom =DisplayRect.Height;

		m_appRootPath =(wchar_t*)(Marshal::StringToHGlobalUni(appRootPath).ToPointer());

		try {
			m_player =new CInputPlayerGraph((HWND)parentWnd.ToPointer(), m_videoFile, (HWND)DisplayWnd.ToPointer(), rc, m_appRootPath);
		} 
		catch(CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}
	}

	~InputVideoPlayer(void)
	{
		this->!InputVideoPlayer();
	}

	!InputVideoPlayer()
	{
		//if (this->m_currentFrameBuf !=IntPtr(0)) {
		//	Marshal::FreeHGlobal(this->m_currentFrameBuf);
		//	this->m_currentFrameBuf =IntPtr(0);
		//}
		Marshal::FreeHGlobal(IntPtr(m_appRootPath));
		Marshal::FreeHGlobal(IntPtr(m_videoFile));

		if (m_player) {
			delete m_player;
			m_player =NULL;
		}
	}

	property bool CanSeek
	{
		bool get()
		{
			if (m_player->CheckCanSeek())
				return true;
		
			return false;
		}
	}

	//property bool CanFrameSeek
	//{
	//	bool get()
	//	{
	//		if (m_player->CheckCanFrameSeek())
	//			return true;
	//	
	//		return false;
	//	}
	//}

	property Int64 Duration
	{
		Int64 get()
		{
			Int64 ret;

			if ((ret =m_player->GetVideoDuration()) < 0)
			{
				Exception^ ex =gcnew Exception("Error while reading video duration from InputPlayer");

				throw ex;
			}
			return ret;
		}
	}

	property Int64 VideoPosition
	{
		Int64 get()
		{
			Int64 ret;

			try {
				ret =m_player->GetVideoPos();
			}
			catch (CErrEx* ex) {
				Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
				Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

				ret =-1;
				delete ex;
				throw x;
			}

			return ret;
		}

		void set(Int64 pos)
		{
			try {
				m_player->SetVideoPos(pos);
			}
			catch (CErrEx* ex) {
				Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
				Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

				delete ex;
				throw x;
			}
		}
	}

	property int VideoFrameRate
	{
		int get()
		{
			return m_player->GetVideoFrameRate();
		}
	}

	property Int64 AvgTimePerFrame
	{
		Int64 get()
		{
			Int64 t(m_player->GetAvgFrameTime());
			return t;
		}
	}

	void Play()
	{
		if (!m_player)
			return;

		if (m_videoStatus ==VideoStatus_playing)
			return;

		try {
			m_player->Play();
		}
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}

		m_videoStatus =VideoStatus_playing;
	}

	void Pause()
	{
		if (!m_player)
			return;

		if (m_videoStatus ==VideoStatus_pause)
			return;

		try {
			m_player->Pause();
		}
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}

		m_videoStatus =VideoStatus_pause;
	}

	void Stop()
	{
		if (m_player ==nullptr)
			return;

		if (m_videoStatus ==VideoStatus_stopped)
			return;

		try {
			m_player->Stop();
		}
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}

		m_videoStatus =VideoStatus_stopped;
	}

	void RepaintVideo(void)
	{
		if (m_player)
			m_player->RepaintVideo(NULL);
	}

	property long DShowEventCode
	{
		long get()
		{
			if (!m_player)
				return -1;

			long ret =-1;
			LONG_PTR p1, p2;

			try {
				ret =m_player->GetDShowEventCode(&p1, &p2);
			}
			catch (CErrEx* ex)
			{
				ret =-1;
				Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
				Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

				delete ex;
				throw x;
			}
			m_player->FreeDShowEventParams((long)ret, p1, p2);

			return ret;
		}
	}

	property int VideoPlaybackStatus
	{
		int get()
		{
			return m_videoStatus;
		}
	}

	property Drawing::Size VideoSize 
	{
		Drawing::Size get()
		{
			RECT rc;

			if (this->m_player->GetVideoSize(&rc) ==0) {
				Drawing::Size s; // =gcnew Drawing::Size(0,0);

				s.Width =rc.right;
				s.Height =rc.bottom;

				return s;
			}

			return Drawing::Size(0,0);
		}
	}

	VideoFrame^ GetCurrentVideoFrame(void)
	{
		BYTE* dib =NULL;

		try {
			dib =m_player->GetVideoImage();
		}
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}

		if (dib)
		{
			BITMAPINFOHEADER* bih =(BITMAPINFOHEADER*)dib;
			IntPtr ptr =IntPtr((void*)(dib+bih->biSize));
			Drawing::Bitmap^ bmp =gcnew Drawing::Bitmap(bih->biWidth, bih->biHeight, bih->biWidth*(bih->biBitCount/8), Drawing::Imaging::PixelFormat::Format32bppRgb, ptr);
			bmp->RotateFlip(Drawing::RotateFlipType::RotateNoneFlipY);

			VideoFrame^ frame =gcnew VideoFrame();
			frame->rawPtr =IntPtr((void*)dib);
			frame->frame =bmp;

			return frame;
		}

		return nullptr;
	}

	void ReleaseVideoframe(VideoFrame^ frame)
	{
		if (frame !=nullptr) 
		{
			delete frame->frame;
			frame->frame =nullptr;

			CoTaskMemFree(frame->rawPtr.ToPointer());
			frame->rawPtr =IntPtr(0);

			delete frame;
			frame =nullptr;
		}
	}

private:

	CInputPlayerGraph* m_player;
	wchar_t* m_videoFile;
	wchar_t* m_appRootPath;

	int m_videoStatus;

};

} // namespace libds