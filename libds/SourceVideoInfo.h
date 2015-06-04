#pragma once

#include "DirectShow.h"
#include "SrcVideoFileInfo.h"

namespace libds 
{
	using namespace DShow;

#pragma managed

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Runtime::InteropServices;


public ref class SourceVideoFileInfo
{
public:
	SourceVideoFileInfo(String^ filename)
	{
		wchar_t *m_filename =(wchar_t*)Marshal::StringToHGlobalUni(filename).ToPointer();

		try {
			m_srcVidInfo =new CSrcVideoFileInfo(m_filename);
		}
		catch(CErrEx* ex)
		{
			Exception^ ix =gcnew Exception(gcnew String(ex->logMsg));
			Exception^ x =gcnew Exception(gcnew String(ex->msg), ix);

			delete ex;
			throw x;
		}
	}

	~SourceVideoFileInfo(void)
	{
		this->!SourceVideoFileInfo();
	}

	!SourceVideoFileInfo(void)
	{
		if (m_srcVidInfo !=nullptr)
		{
			delete m_srcVidInfo;
		}

		if (m_filename !=nullptr)
		{
			Marshal::FreeHGlobal(IntPtr((void*)m_filename));
		}
	}

private:
	wchar_t* m_filename;
	CSrcVideoFileInfo* m_srcVidInfo;
};

}