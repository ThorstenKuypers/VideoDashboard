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

#include "liboga.h"

namespace libLDF
{
	struct _IDashboardLayout
	{
	public:
		//virtual void ParseLayoutFile(const std::string& filename) = 0;
		virtual void SetActiveDashboard(const std::string& filename) = 0;
		virtual std::string& GetDashboardPrettyName(std::string& filename) = 0;
		virtual std::string& GetDashboardShortName(std::string& filename) = 0;
		virtual Gdiplus::Bitmap* RenderDashboard(libOGA::DataSample& sample, libOGA::IGenericLogger& logger, bool renderBlank) = 0;
		virtual Gdiplus::Bitmap* RenderDashboard(libOGA::IGenericLogger& logger, int sampleIndex, bool renderBlank) = 0;

		//virtual void SetDataLogger(libDataLogging::DataLogger::IDataLogger* logger) = 0;
	};
	typedef struct libLDF::_IDashboardLayout IDashboardLayout;

}

extern "C" LIBLDF_API libLDF::IDashboardLayout* get_LDF();
extern "C" LIBLDF_API void release_LDF(libLDF::IDashboardLayout* inst);

#endif // _LIBLDF_H_