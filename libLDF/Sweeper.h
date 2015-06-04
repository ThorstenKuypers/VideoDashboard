#pragma once

#include "common.h"
#include "DashboardElement.h"
#include "Ruler.h"

namespace libLDF
{

	// shape types
	enum _shapeType_Sweeper
	{
		shapeType_line = 1,
		shapeType_rectangle,
		shapeType_ellipse
	};
	typedef libLDF::_shapeType_Sweeper shapeType_Sweeper;

	//enum _renderDir_Sweeper
	//{
	//	cw = 0,
	//	ccw,
	//	left_right,
	//	right_left,
	//	top_bottom,
	//	bottom_top
	//};

	// This structure holds all of the precalculated values which are used at render time
	typedef struct _sweeper_info {

		float divAngle; // angle increment per indicator step
		float divStep; // value step for each indicator

		int width; // width of resulting image
		int height; // height of resulting image

		// vector of indicators; 
		// *** for line indicators RectF represent the start- and endpoints
		//     RectF.X/Y -> start point
		//     RectF.Width/Height endpoint (in X/Y coordinates)
		// *** for circle and rectangle it represents the start- / endpoints and 
		// width/height or radius respectivly
		std::vector<RectF> indicators;

	}SWEEPER_INFO, *PSWEEPER_INFO;


	// The most complicated of all the elements. It creates indicator elements along a path defined by
	// the bounding rectangle. Each indicator is drawn along the path, which can be an ellipse, a circle, 
	// or a line. Indicator shapes can be line, ellipse, or rectangle. The width of the sweeper can be 
	// tapered to increase the size of the indicator elements as the displayed value increases. 
	// The color of the indicator elements can be set for certain value ranges.
	class CSweeper :
		public CDashboardElement
	{

		//friend class CRuler;

	public:
		CSweeper();
		virtual ~CSweeper();

		// used to pre-calculate values and coordinates of the indicators. This should save some
		// resources and time at render time.
		void Precalculate();

		virtual void Init() {}

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual Gdiplus::Bitmap* Render(int sampleIndex);

		void SetPathType(string& s);

		void SetRange(string& s);

		void SetSweepAngle(string& s);

		void SetRectangle(string& s);

		void SetLineStart(string& s);

		void SetLineEnd(string& s);

		void SetShapeType(string& s);

		void SetExtend(int i) { _extend = i; }

		void SetThickness(int i) { _thickness = i; }

		void SetTaper(int i) { _taper = i; }

		void SetDivisions(int i) { _divisions = i; }

		void SetColorON(string& s);

		void SetColorOFF(string& s);

		void SetThreshold(string& s);

		void SetThresholdColors(string& s);

		virtual void SetRuler(void* r);

		virtual Gdiplus::Rect GetRectangle(); // { return Rect(_rect.X - _extend, _rect.Y - _extend, _rect.Width, _rect.Height); }

	private:

		void renderEllipsePath(Bitmap* bmp, Graphics* gfx, int sampleIndex);
		void renderArcPath(Bitmap* bmp, Graphics* gfx, int sampleIndex);
		void renderLinePath(Bitmap* bmp, Graphics* gfx, int sampleIndex);

		void calculateEllipsePath();
		void calculateArcPath();
		void calculateLinePath();

		// Path can be sweeper [ellipse] arc, or line.
		PathType _path;
		// The range of variable values displayed by the sweeper.
		std::tuple<int, int> _range;
		// The angular sweep in degrees of the ellipse or arc path, from minimum to maximum variable value.
		// 0 is along the x-axis and increases counter-clockwise. Positive and negative values are allowed
		std::tuple<int, int> _sweepAngle;
		float _sweep;

		// Bounding rectangle for the ellipse or arc path. Typically the diameter of an arc path
		Gdiplus::Rect _rect;
		// Position where the line path will start
		std::tuple<int, int> _lineStart;
		// Position where the line path will end
		std::tuple<int, int> _lineEnd;
		// The shape of the indicators that form the sweeper. Default is line.
		// Rectangle is a square indicator, ellipse is a circle indicator.
		shapeType_Sweeper _shape;
		// For line shape, the length of the lines
		// For ellipse shape, the diameter of the circle
		// For rectangle shape, the size of the square
		int _extend;
		// For line shape only. Thickness of the lines.
		int _thickness;
		// The length of the first indicator line, or ellipse diameter, or rectangle side.
		// Indicators will become longer across the length of the path until their full
		// [extend] dimension is reached at max value.
		int _taper;
		// The number of indicators shapes displayed along the sweeper path.
		int _divisions;
		// The color when an indicator is in the OFF state.
		Gdiplus::Color _rgbOff;
		// The color when an indicator is in the ON state.
		Gdiplus::Color _rgbOn;
		// The values for the start of each color range.
		//std::tuple<> _threshold;
		std::vector<int> _threshold;
		// The color for each threshold range of values.
		//std::tuple<> _rgbThresholdOn;
		std::vector<Gdiplus::Color> _rgbThresholdOn;

		// rendering direction of sweeper element; clockwise or counter-clockwise for
		// elliptic and arc paths and left->right / right->left / up->down / down->up
		// for line paths. (depends on start/end angle or -position)
		RotationType _renderDir;

		SWEEPER_INFO _sweeperInfo;

		// original rectangle without ruler size modification
		Rect _origRect;

	};

}