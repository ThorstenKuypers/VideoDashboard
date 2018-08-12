#ifndef _BASE_GRAPH_H
#define _BASE_GRAPH_H

#include <comdef.h>
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>
#include <Dshowasf.h>
#include <memory>


namespace DShow {

	_COM_SMARTPTR_TYPEDEF(IGraphBuilder, __uuidof(IGraphBuilder));
	_COM_SMARTPTR_TYPEDEF(IMediaControl, __uuidof(IMediaControl));
	_COM_SMARTPTR_TYPEDEF(IMediaEventEx, __uuidof(IMediaEventEx));
	_COM_SMARTPTR_TYPEDEF(IMediaSeeking, __uuidof(IMediaSeeking));
	_COM_SMARTPTR_TYPEDEF(IBaseFilter, __uuidof(IBaseFilter));
	_COM_SMARTPTR_TYPEDEF(IVMRWindowlessControl9, __uuidof(IVMRWindowlessControl9));
	_COM_SMARTPTR_TYPEDEF(IVMRFilterConfig9, __uuidof(IVMRFilterConfig9));

	namespace Graph {

		class CBaseGraph
		{
		public:
			CBaseGraph();

			~CBaseGraph();
		protected:
			IGraphBuilderPtr _graph{ nullptr };
			IMediaControlPtr _control;
			IMediaEventExPtr _event;
			IMediaSeekingPtr _seek;
			IBaseFilterPtr _pVmr;
			IVMRWindowlessControl9Ptr _pWindowless;
			IVMRFilterConfig9Ptr  _pConfig;

			//std::unique_ptr<AM_MEDIA_TYPE> _srcMediaType;
			//std::unique_ptr<VIDEOINFOHEADER> _srcVideoInfo;

		};

	}
}

#endif // _BASE_GRAPH_H