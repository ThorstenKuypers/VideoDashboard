#pragma once
#ifndef _LIBLDF_H_
#define _LIBLDF_H_
///////////////////////////////////////////////////////////////////////////////
//
//	VideoDashboard
//	----------------------
//	Project: libLDF - layout definition format library
//
//	Copyright 2014-2015 Thorsten Kuypers
//  All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

#ifdef LIBLDF_EXPORTS
#define LIBLDF_API __declspec(dllexport)
#else
#define LIBLDF_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <gdiplus.h>
#include <string>

#include "../../../Projects/VideoDashboard/liboga/liboga.h"

namespace libLDF
{
	struct ImageInfo
	{
		ImageInfo() : 
			width(0),
			height(0),
			stride(0),
			pixelFormat(),
			pixbuf(nullptr)
		{}

		ImageInfo(int w, int h, int s, Gdiplus::PixelFormat fmt, void* buf) :
			width(w),
			height(h),
			stride(s),
			pixelFormat(fmt),
			pixbuf(buf)
		{}

		ImageInfo(ImageInfo&) = delete;
		ImageInfo(ImageInfo&& o)
		{
			this->height = o.height;
			o.height = 0;

			this->pixbuf = o.pixbuf;
			o.pixbuf = nullptr;

			this->pixelFormat = o.pixelFormat;
			o.pixelFormat = 0;

			this->stride = o.stride;
			o.stride = 0;

			this->width = o.width;
			o.width = 0;
		}

		ImageInfo& operator=(ImageInfo&) = delete;
		ImageInfo& operator=(ImageInfo&& o)
		{
			this->height = o.height;
			o.height = 0;

			this->pixbuf = o.pixbuf;
			o.pixbuf = nullptr;

			this->pixelFormat = o.pixelFormat;
			o.pixelFormat = 0;

			this->stride = o.stride;
			o.stride = 0;

			this->width = o.width;
			o.width = 0;

			return *this;
		}

		int width;
		int height;
		int stride;
		Gdiplus::PixelFormat pixelFormat;
		void* pixbuf; // pixel buffer

	};

	struct LIBLDF_API _IDashboardLayout
	{
	public:
		virtual void SetActiveDashboard(const std::string& filename) = 0;
		virtual std::string& GetPrettyName() = 0;
		virtual std::string& GetShortName() = 0;
		virtual ImageInfo RenderDashboard(libOGA::IGenericLogger* logger, libOGA::DataSample& sample, bool renderBlank) = 0;
		virtual ImageInfo RenderDashboard(libOGA::IGenericLogger* logger, int sampleIndex, bool renderBlank) = 0;
		virtual void Parse() = 0;
		virtual void ShowBoundingBoxes(bool show) = 0;

		//virtual void SetDataLogger(libDataLogging::DataLogger::IDataLogger* logger) = 0;
	};
	typedef struct libLDF::_IDashboardLayout IDashboardLayout;

	// This class provides static methods to create and return instance(s) from within
	// the DLL and release those instance(s) if no longer needed
	// NOTE: This replaces the "old" C-style interface export functions
	struct LIBLDF_API LayoutInstanceFactory
	{
		static IDashboardLayout* GetDashboardLayoutInst();
		static void ReleaseDashboardLayoutInstance(IDashboardLayout* inst);
	};
}

#endif // _LIBLDF_H_
