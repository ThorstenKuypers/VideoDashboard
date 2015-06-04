#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011-2012 Thorsten Kuypers
//	All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

#include "WMProfiler.h"

//namespace DShow {
namespace libds {

#pragma managed

	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Collections;
	using namespace System::Collections::Generic;

	using namespace customControls;
	using namespace DShow;

public ref class CWM9_ProfileManager
{

	[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	ref struct BitmapInfoHeader
	{
		System::UInt32 biSize;
		System::Int32  biWidth;
		System::Int32  biHeight;
		System::UInt16  biPlanes;
		System::UInt16  biBitCount;
		System::UInt32 biCompression;
		System::UInt32 biSizeImage;
		System::Int32  biXPelsPerMeter;
		System::Int32  biYPelsPerMeter;
		System::UInt32 biClrUsed;
		System::UInt32 biClrImportant;
	};
	[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	ref struct WMVideoInfoHeader
	{
		System::Drawing::Rectangle	rcSource;
		System::Drawing::Rectangle	rcTarget;
		System::UInt32				dwBitRate;
		System::UInt32				dwBitErrorRate;
		System::Int64				AvgTimePerFrame;
		BitmapInfoHeader			bmiHeader;
	};
	[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	ref struct WaveFormatEx
	{
		System::UInt16  wFormatTag;
		System::UInt16  nChannels;
		System::UInt32 nSamplesPerSec;
		System::UInt32 nAvgBytesPerSec;
		System::UInt16  nBlockAlign;
		System::UInt16  wBitsPerSample;
		System::UInt16  cbSize;
	};
	//[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	//ref struct Samples {
	//	  System::UInt16 wValidBitsPerSample;
	//	  System::UInt16 wSamplesPerBlock;
	//	  System::UInt16 wReserved;
	//};
	//[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	//ref struct WaveFormatExten
	//{
	//	WaveFormatEx Format;
	//	Samples samples;
	//	System::UInt32 dwChannelMask;
	//	System::Guid SubFormat;
	//};

	[StructLayout(LayoutKind::Sequential, CharSet=CharSet::Unicode)]
	ref struct WM9_Profile
	{
		String^ name;
		String^ path;

		WMVideoInfoHeader wmv_hdr;
		WaveFormatEx wav_hdr;

		System::UInt32 quality;
		System::Int64 keyframes;
	};

public:

	CWM9_ProfileManager(void)
	{
		try {
			m_profiler = new CWMProfiler();
		}
		catch (CErrEx* ex)
		{
			Exception^ e = gcnew Exception(gcnew String(ex->msg), gcnew Exception(gcnew String(ex->logMsg)));

			delete ex;
			ex = nullptr;

			throw e;
		}
	}
	~CWM9_ProfileManager()
	{
		this->!CWM9_ProfileManager();
	}
	!CWM9_ProfileManager()
	{
		CloseCurrentProfile();

		if (m_activeProfile)
		{
			delete m_activeProfile;
			m_activeProfile =nullptr;
		}

		if (m_profiler) 
		{
			delete m_profiler;
			m_profiler =NULL;
		}
	}

	void LoadProfile(String^ name)
	{
		IntPtr p =Marshal::StringToHGlobalUni(name);
		wchar_t* str =(wchar_t*)p.ToPointer();

		m_profile =m_profiler->LoadProfile(str);
		if (!m_profile) {
			//MessageBox::Show(, "Error");
			Marshal::FreeHGlobal(p);
			throw gcnew Exception("Error while loading WM9 profile!");
		}
		m_activeProfile =gcnew WM9_Profile();
		IntPtr prf =IntPtr(m_profile);

		Marshal::PtrToStructure(prf, m_activeProfile);
		Marshal::FreeHGlobal(p);
	}

	void CloseCurrentProfile()
	{
		if (m_activeProfile !=nullptr) {
			delete m_activeProfile;
			m_activeProfile =nullptr;
		}

		m_profiler->CloseProfile();
	}

	IntPtr GetHandle()
	{
		return IntPtr((void*)this->m_profiler);
	}

	property int ProfileVideoWidth
	{
		int get()
		{
			return m_activeProfile->wmv_hdr.bmiHeader.biWidth;
		}
	}

	property int ProfileVideoHeight
	{
		int get()
		{
			return m_activeProfile->wmv_hdr.bmiHeader.biHeight;
		}
	}

	property System::UInt32 ProfileImageSize
	{
		System::UInt32 get()
		{
			return m_activeProfile->wmv_hdr.bmiHeader.biSizeImage;
		}
	}

	property System::UInt32 ProfileQuality
	{
		System::UInt32 get()
		{
			return m_activeProfile->quality;
		}
	}

	property System::UInt32 ProfileBitrate
	{
		System::UInt32 get()
		{
			return m_activeProfile->wmv_hdr.dwBitRate;
		}
	}

	property System::Int64 ProfileKeyframes
	{
		System::Int64 get()
		{
			return m_activeProfile->keyframes;
		}
	}

	property System::UInt32 ProfileFramerate
	{
		System::UInt32 get()
		{
			float f =(float)Math::Round((double)(10000000 / m_activeProfile->wmv_hdr.AvgTimePerFrame));
			UInt32 fr =(UInt32)f;
			return fr;
		}
	}

private:
	WM9_Profile^ m_activeProfile;

	// unmanaged
	CWMProfiler* m_profiler;
	WM9PROFILE* m_profile;

};

}
//}