#pragma once
#ifndef _INDICATOR_H_
#define _INDICATOR_H_
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
	// indicator shapes
	enum _shapeType_Indicator
	{
		Indicator_ShapeType_image = 1,
		Indicator_ShapeType_line,
		Indicator_ShapeType_rectangle,
		Indicator_ShapeType_ellipse
	};
	typedef libLDF::_shapeType_Indicator Indicator_ShapeType;


	// The indicator element displays a filled or shaded shape, or an image, 
	// based on a threshold value. The indicator is in an ON or OFF state at all times.
	class CIndicator :
		public CDashboardElement
	{
	public:
		CIndicator();
		virtual ~CIndicator();

		virtual void Init();

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual Gdiplus::Bitmap* Render(DataSample& sample, IGenericLogger& logger, bool renderBlank);


		void SetColorON(string& s);
		Gdiplus::Color GetColorOn() { return _rgbOn; }

		void SetColorOff(string& s);
		Gdiplus::Color GetColorOff() { return _rgbOff; }

		void SetThresholdValue(float v) { _thresholdVal = v; }
		float GetThresholdValue() { return _thresholdVal; }

		void SetShape(string& s);
		Indicator_ShapeType GetShape() { return _shape; }

		void SetOnImage(string& s);
		string& GetOnImage() { return _imgOn; }

		void SetOffImage(string& s);
		string& GetOffImage() { return _imgOff; }

		void SetThickness(int i) { _thickness = i; }
		int GetThickness() { return _thickness; }

		void SetFilled(bool b) { _filled = b; }
		bool GetFilled() { return _filled; }

		void SetOutlined(bool b) { _outlined = b; }
		bool GetOutlined() { return _outlined; }

		void SetShaded(bool b) { _shaded = b; }
		bool GetShaded() { return _shaded; }

		void SetUnshadedValue(float v) { _unshaded = v; }
		float GetUnshadedValue() { return _unshaded; }

	private:
		// The color when the indicator is in the ON state.
		Gdiplus::Color _rgbOn;
		// The color when the indicator is in the OFF state.
		// Use Rbgoff = transparent; to show On state only.
		Gdiplus::Color _rgbOff;
		// The threshold value. The indicator is ON when the variable value > threshold, and OFF otherwise.
		float _thresholdVal;
		// The indicators shape. Use rectangle = for size and position.
		Indicator_ShapeType _shape;
		// Pathname of the image file for ON state. Use position = <x,y>; to position the images
		std::string _imgOn;
		// Pathname of the image file for OFF state
		std::string _imgOff;
		// The thickness of the line drawn when the shape is LINE, in pixels.
		int _thickness;
		// Fills the indicator shape with RBGON and RGBOFF color. Default is yes.
		bool _filled;
		// Outlines the indicator shape with foreground color. Default is yes.
		bool _outlined;
		// Select shaded fill for the image shape
		bool _shaded;
		// Special case for ellipse and rectangle shapes. Specifies the interior percentage that remains the primary color
		float _unshaded;

		Bitmap* _bmpOn;
		Bitmap* _bmpOff;

		Gdiplus::Bitmap* drawImage(float val);
		Gdiplus::Bitmap* drawEllipseShape(float val);
		Gdiplus::Bitmap* drawRectangleShape(float val);
		Gdiplus::Bitmap* drawLineShape(float val);

	};

}

#endif // _INDICATOR_H_