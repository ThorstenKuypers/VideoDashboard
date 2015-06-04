#pragma once

#include "DirectShow.h"

namespace DShow {
	namespace VideoEncoding {

		class CRawAviEncoder : CBaseEncoderGraph,IVideoEncoder
		{
		private:
			ICaptureGraphBuilder2* m_pGraphBuilder;
			IBaseFilter* m_pCsc;	// color-space-converter
			IBaseFilter* m_pAviDec;

			IBaseFilter* m_pAviMux;
			IFileSinkFilter* m_pAviWriter;
			IBaseFilter *m_pSrc;	// source filter

			IBaseFilter* m_pComp;	// compression filter (if used)

			HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

		protected:
			virtual void buildGraph();
			virtual void configGraph();
			virtual void connectGraph();
			virtual void releaseGraph();

		public:
			CRawAviEncoder(void);
			~CRawAviEncoder(void);

			// IVideoEncoder interface implementation
			virtual void SetFileName(wchar_t* fileName);
			virtual void SetEncoderConfig(PVIDEO_ENCODER_CONFIG cfg);

			virtual int EncodeStart();
			virtual void EncodeCancel();

			virtual void SetProgressCallback(void* cb);

			virtual long GetDShowEventCode(LONG_PTR*, LONG_PTR*);
			virtual void FreeDShowEventParams(long, LONG_PTR, LONG_PTR);
		};

	}
}