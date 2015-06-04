#pragma once

#include "DirectShow.h"
#include "WMProfiler.h"

namespace DShow {
	namespace VideoEncoding {

		class CWMEncoder : CBaseEncoderGraph,IVideoEncoder
		{
		private:
			IBaseFilter* m_pAsfWriter;
			IConfigAsfWriter2* m_pAsfWriterConfig;
			IFileSinkFilter2* m_pFileSink;

			DShow::CWMProfiler* m_pProfiler;

			HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

		protected:
			virtual void buildGraph();
			virtual void configGraph();
			virtual void connectGraph();
			virtual void releaseGraph();

		public:
			CWMEncoder(void);
			~CWMEncoder(void);

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