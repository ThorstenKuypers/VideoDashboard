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

#include "DashboardLayout.h"


using namespace libLDF;
using namespace Gdiplus;

//CDashboardLayout::CDashboardLayout()
//{
//	//GdiplusStartupInput gdiStartup;
//
//	//GdiplusStartup(&_gdiplus_tok, &gdiStartup, NULL);
//}
//
//
//CDashboardLayout::~CDashboardLayout()
//{
//	//GdiplusShutdown(_gdiplus_tok);
//}

//void CDashboardLayout::ShowBoundingBoxes(bool show)
//{
//	_curDashboard.ShowBoundingBoxes(show);
//}
//
//Gdiplus::Bitmap* CDashboardLayout::RenderDashboard(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank)
//{
//	return _curDashboard.RenderToImage(sample, logger, renderBlank);
//}
//
//Gdiplus::Bitmap* CDashboardLayout::RenderDashboard(IGenericLogger& logger, int sampleIndex, bool renderBlank)
//{
//	DataSample s;
//	if (!renderBlank) {
//		logger.GetSample(s, sampleIndex);
//	}
//
//	return _curDashboard.RenderToImage(s, logger, renderBlank);
//}
//
//std::string& CDashboardLayout::GetDashboardPrettyName(std::string& filename)
//{
//	return _curDashboard.GetPrettyName();
//}
//
//std::string& CDashboardLayout::GetDashboardShortName(std::string& filename)
//{
//	return _curDashboard.GetShortName();
//}
//
//
//void CDashboardLayout::SetActiveDashboard(const std::string& filename)
//{
//}
//
//
//
//
//
//
//
//
