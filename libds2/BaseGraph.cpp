#include "BaseGraph.h"

using namespace DShow;
using namespace DShow::Graph;

CBaseGraph::CBaseGraph()
{
	HRESULT res = _graph.CreateInstance(CLSID_FilgraphManager);
	if (SUCCEEDED(res))
	{
		res = _graph.QueryInterface(IID_IMediaEvent, &_event);

	}
}


CBaseGraph::~CBaseGraph()
{
}
