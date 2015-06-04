#pragma once

#include <string>
#include <Windows.h>
#include <gdiplus.h>

#include "libDataLogging.h"

#ifdef LIBLDF_EXPORTS
#define LIBLDF_API __declspec(dllexport)
#else
#define LIBLDF_API __declspec(dllimport)
#endif


namespace libLDF
{
	struct _IDashboardLayout
	{
	public:
		virtual void ParseLayoutFile(std::string filename) =0;

		//virtual CDashboard* GetDashboard(string& name) =0;
		virtual std::string GetDashboardPrettyName(std::string& filename) = 0;
		virtual std::string GetDashboardShortName(std::string& filename) = 0;

		virtual Gdiplus::Bitmap* RenderDashboard(string& name, int sampleIndex) = 0;

		virtual void SetDataLogger(libDataLogging::DataLogger::IDataLogging* logger) = 0;
	};
	typedef struct libLDF::_IDashboardLayout IDashboardLayout;

	extern "C" LIBLDF_API libLDF::IDashboardLayout* init_instance();
	extern "C" LIBLDF_API void release_instance(libLDF::IDashboardLayout* inst);
}

