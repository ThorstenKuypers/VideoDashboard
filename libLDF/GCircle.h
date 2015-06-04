#pragma once

#include "common.h"
#include "DashboardElement.h"

namespace libLDF
{

	// G-force load circle
	class CGCircle :
		public CDashboardElement
	{
	public:
		CGCircle();
		virtual ~CGCircle();

		virtual void Init() {}

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual Gdiplus::Bitmap* Render(int sampleIndex);

		void SetDivisions(int i) { _divisions = i; }
		int GetDivisions() { return _divisions; }

		void SetRadius(int r) { _radius = r; }
		int GetRadius() { return _radius; }

		void SetLineWidth(int w) { _lineWidth = w; }
		int GetLineWidth() { return _lineWidth; }

		void SetPointerColor(string& s);
		Gdiplus::Color GetPointerColor() { return _pointerColor; }

		void SetPointerWidth(int w) { _pointerWidth = w; }
		int GetPointerWidh() { return _pointerWidth; }

		void SetPointerSize(int s) { _pointerSize = s; }
		int GetPointerSize() { return _pointerSize; }

		void SetLongitudinalScaleFactor(float f) { _scaleLong = f; }
		float GetLongitudinalScaleFactor() { return _scaleLong; }

		void SetLateralScaleFactor(float f) { _scaleLat = f; }
		float GetLateralScaleFactor() { return _scaleLat; }

		void SetConnectCenter(bool b) { _connectToCenter = b; }
		bool GetConnectCenter() { return _connectToCenter; }

	private:
		// The number of concentric circles drawn
		int _divisions;
		// g value to for full scale display
		int _radius;
		// Width of outline & scale rings in pixels
		int _lineWidth;
		// The color of the vector pointer.
		Gdiplus::Color _pointerColor;
		// The width of the vector pointer in pixels
		int _pointerWidth;
		// The diameter of the pointer circle in pixels
		int _pointerSize;
		// Value x scale = displayed value
		float _scaleLat;
		// Value x scale = displayed value
		float _scaleLong;
		// connect the "ball" with the center via the vector pointer
		bool _connectToCenter;

	};

}