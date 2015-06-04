
#include <windows.h>
#include "VideoCompressorEnumerator.h"

using namespace DShow;

wchar_t* comparer[] = {/*L"Intel IYUV",*/
	L"Huffyuv",
	L"Lagarith" };

CVideoCompressorEnumerator::CVideoCompressorEnumerator(void)
{
	m_lastConfiguredFilter = NULL;
}

CVideoCompressorEnumerator::~CVideoCompressorEnumerator(void)
{
	for (auto x : _vidCompressors)
	{
		if (x.inst != nullptr)
		{
			IBaseFilter* p = static_cast<IBaseFilter*>(x.inst);
			p->Release();
			p = nullptr;
		}
	}
}

std::vector<VideoCompressor>& CVideoCompressorEnumerator::GetCompressorList()
{
	return _vidCompressors;
}

void CVideoCompressorEnumerator::EnumerateCompressors()
{
	int idx = 0;
	bool found = false;
	int cnt = 2;

	try {
		// Create the System Device Enumerator.
		HRESULT hr;
		ICreateDevEnum *pSysDevEnum = NULL;
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
			IID_ICreateDevEnum, (void **)&pSysDevEnum);
		if (FAILED(hr))
		{
			throw hr;
		}

		// Obtain a class enumerator for the video compressor category.
		IEnumMoniker *pEnumCat = NULL;
		hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEnumCat, 0);

		if (hr == S_OK)
		{
			// Enumerate the monikers.
			IMoniker *pMoniker = NULL;
			ULONG cFetched;
			while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
			{
				IPropertyBag *pPropBag;
				hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
					(void **)&pPropBag);
				if (SUCCEEDED(hr))
				{
					// To retrieve the filter's friendly name, do the following:
					VARIANT varName;
					VariantInit(&varName);
					hr = pPropBag->Read(L"FriendlyName", &varName, 0);
					if (SUCCEEDED(hr))
					{
						// compare filter name to comparer array and check which of the wanted 
						// compressors are installed
						for (int i = 0; i < 2; i++) {
							if (wcsstr(varName.bstrVal, comparer[i])) {
								found = true;
								break;
							}
						}
						if (found) {
							VideoCompressor vcp = { 0 };
							vcp.index = idx;
							vcp.name = std::wstring(varName.bstrVal);

							IBaseFilter* flt = NULL;
							HRESULT hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&flt);
							if (SUCCEEDED(hr)) {

								vcp.inst = static_cast<void*>(flt);
							}

							_vidCompressors.push_back(vcp);
						}

					}
					VariantClear(&varName);
					pPropBag->Release();
				}
				//if (!found)
				//	pMoniker->Release();
				//else
				//	idx++;
				if (found) {
					idx++;
				}

				pMoniker->Release();
				found = false;
			}
			pEnumCat->Release();
		}
		pSysDevEnum->Release();

	}
	catch (...) {
		int err = -1;
	}
}

void CVideoCompressorEnumerator::ShowConfigDialog(int idx, HWND parent)
{
	if (idx < 0 || idx >= (int)_vidCompressors.size())
		return;

	IAMVfwCompressDialogs* p;
	IBaseFilter* filter = (IBaseFilter*)_vidCompressors[idx].inst;

	HRESULT hr = S_OK;
	if (filter != NULL) {
		hr = filter->QueryInterface(IID_IAMVfwCompressDialogs, (void**)&p);
		if (SUCCEEDED(hr))
		{
			hr = p->ShowDialog(VfwCompressDialog_Config, parent);
			if (SUCCEEDED(hr)) {
				p->Release();
				p = NULL;
			}
			else {
				p->Release();
				p = NULL;

				//CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, hr, true);
				//throw ex;
			}
		}
		else {
			CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, hr, true);
			throw ex;
		}

	}
}

void* CVideoCompressorEnumerator::GetCompressionFilter(int idx)
{
	if (idx < 0 || idx >= (int)_vidCompressors.size())
		return NULL;

	IBaseFilter* filter = (IBaseFilter*)_vidCompressors[idx].inst;

	if (filter != NULL) {
		filter->AddRef();	// remember to release ref in RawAviEncoder class, after encoding is done
		return (void*)filter;
	}
	return NULL;
}
