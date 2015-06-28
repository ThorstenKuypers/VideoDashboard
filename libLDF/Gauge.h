#pragma once
#ifndef _GAUGE_H_
#define _GAUGE_H_
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

	// This is a complex element capable of rendering all manner of gauge styles.
	class CGauge :
		public CDashboardElement
	{

		//friend class CRuler;

	public:
		CGauge();
		virtual ~CGauge();

		void Init() override;

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual Gdiplus::Bitmap* Render(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank);
		virtual void SetRuler(CRuler& ruler);

		void SetRange(string& s);
		std::tuple<int, int>& GetRange() { return _range; }

		void SetDivisions(int d) { _divisions = d; }
		int GetDivisions() { return _divisions; }

		void SetRadius(int r) { _radius = r; }
		int GetRadius() { return _radius; }

		void SetImageFile(string& s); // { _imgFile = s; }
		string& GetImageFile() { return _imgFile; }

		void SetNeedleImageFile(string& s); // { _needleImgFile = s; }
		string& GetNeedleImageFile() { return _needleImgFile; }

		void SetNeedleOffset(string& s);
		tuple<int, int>& GetNeedleOffset() { return _needleOffset; }

		void SetSweepAngle(string& s);
		tuple<float, float>& GetSweepAngle() { return _sweepAngle; }

		void SetPosition(string& s);
		tuple<int, int>& GetPosition() { return _pos; }

		void SetNeedleCenter(string& s);
		tuple<int, int>& GetNeedleCenter() { return _needleCenter; }

		void SetUseImage(bool b) { _useImg = b; }
		bool GetUseImage() { return _useImg; }

		void SetPrecision(int p) { _precision = p; }
		int GetPrecision() { return _precision; }

		void SetID(string& s) { _id = s; }
		string& GetID() { return _id; }

	private:

		// pre-cached gauge background image
		Bitmap* _bgImg;

		// pre-cached needle image
		Bitmap* _needleImg;

		// size of final element image
		Size _imgSize;

		// fix offset value for simple gauge/scale rendering (if no ruler element is active)
		int _margin;

		// Used to associate gauge with its ruler element
		std::string _id;
		// The minimum and maximum value of the variable. Ranges can be in the 
		// form <o, x> for an RPM gauge, or <-x, +x> which defines a center zero gauge.
		std::tuple<int, int> _range; // min, max value (former valRange)
		// The number of subdivisions on the gauge
		int _divisions;
		// The length of the gauge needle
		int _radius;
		// Pathname of the image file for the gauge face.
		std::string _imgFile;
		// Pathname of the image file for the gauge needle. Needle images are horizontal, pointing to the left.
		std::string _needleImgFile;
		// Pixel offset of the pivot point of the needle relative to <0,0> [top left] of the needle image.
		std::tuple<int, int> _needleOffset;
		// The angle of the needle's travel (in degrees), from minimum to maximum variable value.
		// 0 is along the x-axis and increases counter-clockwise. Positive and negative values are allowed
		std::tuple<float, float> _sweepAngle;
		float _sweep; // sweep angle

		float _renderStartAngle; // start angle for rendering (DrawEllipse)

		// 
		// _rotation ?
		// The position of the gauge. Useful when using a face image without specifying the bounding rectangle
		std::tuple<int, int> _pos;
		// The coordinate of the pivot point of the needle, relative to the top-left position of the gauge
		std::tuple<int, int> _needleCenter;
		// When yes or true, the image declared with face_image is drawn.
		// Otherwise, the default gauge face is rendered. Default is yes
		bool _useImg;
		// Number of decimal places displayed in division labels. Default is 0
		int _precision;

	};

}

#endif // _GAUGE_H_