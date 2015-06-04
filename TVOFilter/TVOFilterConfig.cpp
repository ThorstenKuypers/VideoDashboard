
#include "ITVOFilterConfig.h"

/*
STDMETHODIMP CTVOFilterConfig::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	if (riid ==IID_ITVOFilterConfig)
	{
		return GetInterface((ITVOFilterConfig*)this, ppv);
	}

	return E_NOINTERFACE; //CUnknown::NonDelegatingQueryInterface(riid, ppv);
}
*/
//HRESULT CTVOFilterConfig::SetFilterConfig(TVOFILTER_CONFIG* pConfig)
_declspec(dllexport) int CTVOFilterConfig::SetFilterConfig(TVOFILTER_CONFIG* pConfig)
{

	return 0;
}
