#pragma once
#ifndef _SLIDER_H_
#define _SLIDER_H_
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

#include "common.h"
#include "DashboardElement.h"
#include "Ruler.h"

namespace libLDF
{
	// The slider element displays a horizontal or vertical sliding value. Slider can be defined as horizontal or vertical,
	// and can display most data ranges including center zero, 0 to + or – values, + or – values to zero.
	// [New features added in V2.2 include shading, color controlled by value, and calibration using the Ruler element.]
	class CSlider :
		public CDashboardElement
	{
		friend class CRuler;

	public:
		CSlider();
		virtual ~CSlider();

		virtual void Init();

		virtual void SetRuler(CRuler& r);

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual ImageInfo Render(DataSample& sample, IGenericLogger* logger, bool renderBlank);

		void SetRange(string& s);
		tuple<int, int> GetRange() { return _range; }

		void SetColorPositive(string& s);
		Gdiplus::Color GetColorPositive() { return _rgbPositive; }

		void SetColorNegative(string& s);
		Gdiplus::Color GetColorNegative() { return _rgbNegative; }

	private:

		SliderOrientation _orientation;	// orientation vertical or horizontal
		float _valStep; 

		PointF _cp; // center point of slider for center 0 mode

		// The minimum and maximum values of the variable.
		// May be in the form <-x, +x>, <-x1, x2> or <0, +x>, or <0, -x>, or <+x, 0>, or <-x, 0>
		std::tuple<int, int> _range; // ( former valRange )
		// The color to use when the variable value is > zero
		Gdiplus::Color _rgbPositive;
		// The color to use when the variable value is < zero
		Gdiplus::Color _rgbNegative;
		// 

	};

}

#endif // _SLIDER_H_