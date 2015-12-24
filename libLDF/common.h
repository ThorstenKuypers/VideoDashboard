#pragma once
#ifndef _LIBLDF_COMMON_H_
#define _LIBLDF_COMMON_H_

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
// common header file for all classes

#include <string>
#include <vector>
#include <algorithm>

#include "libLDF.h"
#include "errex.h"

namespace libLDF
{

#define PI 3.141592653f
#define RAD(x) ((PI / 180.0f) * x)
#define DEG(x) (x * (180.0f / PI))

	enum _elementType
	{
		unknown =-1,
		gauge = 1,
		textbox,
		gcircle,
		sweeper,
		indicator,
		slider,
		ringgauge,
		ruler
	};
	typedef enum libLDF::_elementType DashboardElementType;

	enum _pathPosition
	{
		PathPosition_inside = 1,
		PathPosition_outside,
		PathPosition_center,
		PathPosition_left,
		PathPosition_right,
		PathPosition_above,
		PathPosition_below
	};
	typedef enum libLDF::_pathPosition PathPositions;

	// fontstyles
	enum _fontstyles
	{
		normal = 1,
		bold,
		italic,
		bold_italic
	};
	typedef libLDF::_fontstyles FontStyle;

	enum _labelFormatType
	{
		LabelFormatType_integer = 1,
		LabelFormatType_real,
		LabelFormatType_time,
		LabelFormatType_laptime
	};
	typedef libLDF::_labelFormatType LabelFormatType;

	enum _tickShape
	{
		line = 1,
		circle
	};
	typedef enum libLDF::_tickShape TickShape;

	// justification for text/fonts
	enum _justify
	{
		TextJustify_left = 10,
		TextJustify_right,
		TextJustify_center
	};
	typedef libLDF::_justify TextJustify;

	// path types
	enum  _pathType
	{
		pathType_sweeper = 1,
		pathType_arc,
		pathType_line
	};
	typedef libLDF::_pathType PathType;

	enum _rotation_type
	{
		cw = 0,
		ccw,
		left_right,
		right_left,
		top_bottom,
		bottom_top
	};
	typedef libLDF::_rotation_type RotationType;

	enum _slider_orientation
	{
		vertical = 0,
		horizontal
	};
	typedef libLDF::_slider_orientation SliderOrientation;

	std::string trim(std::string& str, const std::string& trimChars);
	void splitTuple(std::string& tpl, std::vector<std::string>& toks, parsing_error& ec);
	void splitTuple2(std::string& tpl, std::vector<std::string>& tuples);
	void removeChar(std::string& str, const char c);
	void getColorFromString(std::string& s, Gdiplus::Color& color);

	void str_tolower(std::string str);

	bool isAbsoluteFilePath(std::string path);
}

#endif // _LIBLDF_COMMON_H_