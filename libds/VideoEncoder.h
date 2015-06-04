#pragma once

#include "libds.h"
#include "WM9_Profile.h"

//#include "WMEncoder.h"
//#include "RawAviEncoder.h"

namespace libds {

using namespace System;
using namespace System::ComponentModel;
using namespace DShow::VideoEncoding;

#pragma managed

public enum VideoEncoderType
{
	VideoEncoder_WMV =1,
	VideoEncoder_RAW =2
};

public enum GraphId
{
	Playback =GRAPHID_PLAYBACK_GRAPH,
	Renderer =GRAPHID_CAPTURE_GRAPH
};

enum DsNotifyWindowsMessageId
{
	DsNotifyMessage_Playback =WM_DS_PLAYBACK_GRAPH_NOTIFY,
	DsNotifyMessage_Renderer =WM_DS_CAPTURE_GRAPH_NOTIFY
};

public ref class ProgressEventArgs
{
public:
	ProgressEventArgs(int frame)
	{
		curFrame =frame;
	}

	property int CurrentProgress
	{
		int get(void)
		{
			return curFrame;
		}
	}

private:
	int curFrame;
};

public delegate void ProgressUpdateHandler(ProgressEventArgs^ e);

private delegate void InternalProgressUpdateHandler(int progress);


public ref struct OverlayConfig
{
	int overlayCfg_posX;
	int overlayCfg_posY;
	float overlayCfg_transp;
	float overlayCfg_scale;
};

public ref struct SyncConfig
{
	int syncCfg_dataSampleInc;
	int syncCfg_startFrame;
	int syncCfg_startSample;
	int syncCfg_endFrame;
	int syncCfg_endSample;
};

public ref struct VideoEncoderConfig
{
	int encoderType;
	IntPtr parentWnd;
	IntPtr wm9_profilerObj;
	IntPtr videoCompressor;
	bool preview;

	String^ appPath;
	String^ srcVideoFileName;
	String^ outVideoFileName;

	int videoWidth;
	int videoHeight;
	float aspectRatio;
	int framerate;

	IntPtr dashboardlayoutObj;	// instance of DahboardLayout parser/rendering class
	int dataLogTickRate;
	//String^ overlayFileName;

	int overlayCfg_posX;
	int overlayCfg_posY;
	float overlayCfg_transp;
	float overlayCfg_scale;

	int syncCfg_dataSampleInc;
	int syncCfg_startFrame;
	int syncCfg_startSample;
	int syncCfg_endFrame;
	int syncCfg_endSample;

};


public ref class CVideoEncoder
{
public:

	event ProgressUpdateHandler^ ProgressUpdate {

		void add(ProgressUpdateHandler^ d) {h +=d;}
		void remove(ProgressUpdateHandler^ d){h -=d;}

		void raise(ProgressEventArgs^ e)
		{
			if (h !=nullptr) {
				if (h !=nullptr) {
					h->Invoke(e);
				}
			}
		}
	}

	property int DsNotifyWindowsMessageId_Playback
	{
		int get(void)
		{
			return DsNotifyMessage_Playback;
		}
	}

	property int DsNotifyWindowsMessageId_Renderer
	{
		int get(void)
		{
			return DsNotifyMessage_Renderer;
		}
	}

	property int GraphId_Playback
	{
		int get(void)
		{
			return Playback;
		}
	}

	property int GraphId_Renderer
	{
		int get(void)
		{
			return Renderer;
		}
	}

	CVideoEncoder()
	{
		//m_encoder =init_WindowsMediaEncoder9();

		m_encoderCfg =new VIDEO_ENCODER_CONFIG;
		memset(m_encoderCfg, 0, sizeof(VIDEO_ENCODER_CONFIG));

		m_progressHandler =gcnew InternalProgressUpdateHandler(this, &CVideoEncoder::OnProgress);
	}

	~CVideoEncoder()
	{
		this->!CVideoEncoder();
	}

	!CVideoEncoder()
	{
		int type =m_encoderCfg->encoderType;

		if (m_encoderCfg) {
			if (m_encoderCfg->outVideoFileName) {			
				Marshal::FreeHGlobal(IntPtr(m_encoderCfg->outVideoFileName));
			}
			if (m_encoderCfg->srcVideoFileName) {
				Marshal::FreeHGlobal(IntPtr(m_encoderCfg->srcVideoFileName));
			}

			if (m_encoderCfg->appPath) {
				Marshal::FreeHGlobal(IntPtr(m_encoderCfg->appPath));
			}

			delete m_encoderCfg;
			m_encoderCfg =NULL;
		}

		if (m_encoder !=nullptr) {
			release_instance(m_encoder, type);
			m_encoder =nullptr;
		}

		delete m_progressHandler;
		m_progressHandler =nullptr;
	}

	void Start()
	{
		try {
			m_encoder->EncodeStart();
		} 
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);		

			delete ex;

			throw x;
		}
	}

	void Cancel()
	{
		try {
			m_encoder->EncodeCancel();
		}
		catch (CErrEx* ex) {
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);		

			delete ex;

			throw x;
		}
	}

	void SetConfig(VideoEncoderConfig^ cfg)
	{
		if (cfg) {

			IntPtr p =Marshal::StringToHGlobalUni(cfg->outVideoFileName);
			m_encoderCfg->outVideoFileName =(wchar_t*)p.ToPointer();
			p =Marshal::StringToHGlobalUni(cfg->srcVideoFileName);
			m_encoderCfg->srcVideoFileName =(wchar_t*)p.ToPointer();
			p =Marshal::StringToHGlobalUni(cfg->appPath);
			m_encoderCfg->appPath =(wchar_t*)p.ToPointer();

			m_encoderCfg->preview =cfg->preview;

			m_encoderCfg->encoderType =cfg->encoderType;
			m_encoderCfg->parentWnd =(HWND)cfg->parentWnd.ToPointer();

			m_encoderCfg->dataLogTickRate = cfg->dataLogTickRate;

			m_encoderCfg->overlayCfg.posX =cfg->overlayCfg_posX;
			m_encoderCfg->overlayCfg.posY =cfg->overlayCfg_posY;
			m_encoderCfg->overlayCfg.scale =cfg->overlayCfg_scale;
			m_encoderCfg->overlayCfg.transp =cfg->overlayCfg_transp;
			m_encoderCfg->dashboardLayoutObj =cfg->dashboardlayoutObj.ToPointer();

			m_encoderCfg->syncCfg.endFrame =cfg->syncCfg_endFrame;
			m_encoderCfg->syncCfg.endSample =cfg->syncCfg_endSample;
			m_encoderCfg->syncCfg.startFrame =cfg->syncCfg_startFrame;
			m_encoderCfg->syncCfg.startSample =cfg->syncCfg_startSample;

			m_encoderCfg->videoHeight =cfg->videoHeight;
			m_encoderCfg->videoWidth =cfg->videoWidth;
			m_encoderCfg->aspectRatio =cfg->aspectRatio;
			m_encoderCfg->framerate =cfg->framerate;

			IntPtr ptr =Marshal::GetFunctionPointerForDelegate(m_progressHandler);

			if (m_encoderCfg->encoderType ==1) {
				m_encoder =init_WindowsMediaEncoder9();

				CWMProfiler* prf =(CWMProfiler*)cfg->wm9_profilerObj.ToPointer();
				m_encoderCfg->wm9_profilerObj =(void*)prf;
			}
			if (cfg->encoderType ==2) {
				m_encoder =init_RawVideoEncoder();
				m_encoderCfg->videoCompressor =cfg->videoCompressor.ToPointer();
			}

			if (m_encoder) {
				m_encoder->SetEncoderConfig(m_encoderCfg);
				m_encoder->SetProgressCallback(ptr.ToPointer());
			}
		}
	}

	property long DShowEventCode
	{
		long get()
		{
			if (!this->m_encoder)
				return -1;

			long ret =-1;
			LONG_PTR p1, p2;

			try {
				ret =this->m_encoder->GetDShowEventCode(&p1, &p2);
			}
			catch (CErrEx* ex) {
				Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
				Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

				delete ex;
				throw x;
			}

			this->m_encoder->FreeDShowEventParams((long)ret, p1, p2);

			return ret;
		}
	}

private:
	DShow::IVideoEncoder* m_encoder;
	int m_encoderType;
	VIDEO_ENCODER_CONFIG* m_encoderCfg;
	InternalProgressUpdateHandler^ m_progressHandler;
	ProgressUpdateHandler^ h;

	void OnProgress(int progress)
	{
		ProgressEventArgs^ e =gcnew ProgressEventArgs(progress);		

		ProgressUpdate(e);

		delete e;
		e =nullptr;
	}

};

}