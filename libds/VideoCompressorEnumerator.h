#pragma once

#include "DirectShow.h"
#include <vector>
#include <string>

namespace DShow {

#pragma unmanaged

	// this structure describes a video compressor which the enumerator has found
	typedef struct VideoCompressor {
		int index;
		std::wstring name;
		void* inst;	// compressor instance (actually pointer to a IMoniker interface)

	}VIDEO_COMPRESSOR, *PVIDEO_COMPRESSOR;

	typedef struct VideoCompressorList {
		int count;
		PVIDEO_COMPRESSOR compressor_array; // list of count VIDEO_COMPRESSOR structs

	}VIDEO_COMPRESSOR_LIST, *PVIDEO_COMPRESSOR_LIST;


	class CVideoCompressorEnumerator
	{
	public:
		CVideoCompressorEnumerator(void);
		~CVideoCompressorEnumerator(void);

		void EnumerateCompressors();
		void GetCompressorList(PVIDEO_COMPRESSOR_LIST list, int* count);
		std::vector<VideoCompressor>& GetCompressorList();

		//	void ShowConfigDialog(IMoniker* comp, HWND parent);
		void ShowConfigDialog(int idx, HWND parent);
		void* GetCompressionFilter(int idx);

	private:

		// @ v0.9.3.12 - changed compressor array allocated on heap to C++ STL vector 
		// and string objects -> easier to maintain and enumerate
		std::vector<VideoCompressor> _vidCompressors;
		IBaseFilter* m_lastConfiguredFilter;
	};

}

namespace libds {

	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Collections::Generic;

	using namespace DShow;

#pragma managed

	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
	public value struct VideoCompressor {
		int index;
		String^ name;
		//IntPtr inst;
	};

	public ref class VideoCompressorEnum
	{
	public:
		VideoCompressorEnum(void)
		{
			m_vidCompEnum = new CVideoCompressorEnumerator();
		}

		~VideoCompressorEnum(void)
		{
			this->!VideoCompressorEnum();
		}

		!VideoCompressorEnum(void)
		{
			if (m_vidCompEnum) {
				delete m_vidCompEnum;
				m_vidCompEnum = NULL;
			}
		}

		property IntPtr CompressionFilter
		{
			IntPtr get(void)
			{
				return m_compFilter;
			}
		}

		void ShowConfigDialog(int idx, IntPtr parentWnd)
		{
			try {
				m_vidCompEnum->ShowConfigDialog(idx, (HWND)parentWnd.ToPointer());
				//void* p =m_vidCompEnum->GetCompressionFilter();
				//m_compFilter =IntPtr(p);
			}
			catch (CErrEx* ex) {
				// TODO: implement and throw exception back to managed code
				Exception^ ix = gcnew Exception(gcnew String(ex->logMsg));
				Exception^ x = gcnew Exception(gcnew String(ex->msg), ix);

				delete ex;
				throw x;
			}
		}

		void Enumerate()
		{
			m_vidCompEnum->EnumerateCompressors();
		}

		List<VideoCompressor>^ GetCompressorList()
		{
			std::vector<DShow::VideoCompressor> vc = m_vidCompEnum->GetCompressorList();

			System::Collections::Generic::List<VideoCompressor>^ list = gcnew List<VideoCompressor>((int)vc.size());
			for (auto x : vc) {

				VideoCompressor c; 
				c.index = x.index;
				c.name = gcnew String(x.name.c_str());

				list->Add(c);
			}

			return list;
		}

		IntPtr GetCompressorFilter(int idx)
		{
			return IntPtr(m_vidCompEnum->GetCompressionFilter(idx));
		}

	private:

		CVideoCompressorEnumerator* m_vidCompEnum;
		IntPtr m_compFilter;
	};

}