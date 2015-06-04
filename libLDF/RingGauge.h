#pragma once

#include "common.h"
#include "DashboardElement.h"
#include "Ruler.h"

namespace libLDF
{

	class CRingGauge : public CDashboardElement
	{
	public:
		CRingGauge();
		~CRingGauge();

		virtual Gdiplus::Bitmap* Render(int sampleIndex);
		virtual void Init();
		virtual void SetRuler(void* r);

		void SetID(std::string& id) { _id = id; }
		std::string GetID() { return _id; }

		//void SetRectangle(string& s);
		virtual Rect GetRectangle() { return _rectangle; }

		void SetRange(string& s);
		std::tuple<int, int>& GetRange() { return _range; }

		void SetRingWidth(string& s);

		void SetSweep(string& s);
		std::tuple<int, int>& GetSweep() { return _sweep; }

		void SetRotation(string& s);

		//void SetForegroundColor(string& s);

		void SetShaded(bool b) { _shaded = b; }

		void SetMarkerWidth(string &s);

		void SetMarkerColor(string& s);

		void SetMarkerHoldTime(string& s);

		void SetMarkerDecay(string& s);


	private:

		// The minimum and maximum value of the variable.
		// Ranges can be in the form <o, x> or <x1, x2> for a positive value
		// display, or <-x, +x> for a center zero ringgauge.
		std::tuple<int, int> _range;

		// Width of the ring in pixels
		int _width;

		// The angles are measure clockwise from the horizontal.
		// Negative values are counter clockwise, but you can use positive
		// values and 'rotation' to control direction.
		std::tuple<int, int> _sweep;

		// Direction of rotation
		int _rotation;

		// Apply shading to the ring. Default is no.
		bool _shaded;

		// Pixel width of the optional max value marker
		int _markerWidth;

		// Color of the max value marker. Default is foreground color.
		Color _markerColor;

		// Number of seconds before the marker falls back to current value. 
		// Default is 0 no marker
		int _markerHoldTime;

		// The rate that the markers drops to the current value after
		// marker_hold_time. Default = 1
		// Higher values produce higher decay rate
		int _markerDecay;


	};

}