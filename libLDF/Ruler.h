#pragma once

#include "common.h"
#include "DashboardElement.h"
//#include "Gauge.h"

namespace libLDF {

	// A Ruler draws equally spaced marks laid out along a path.The path can be a line, an elliptical arc,
	// or a sweeper.The marks are categorized as major and minor.Major marks can be labeled.
	class CRuler : public CDashboardElement
	{

		typedef struct _tickMarkLabel {

			RectF textRect;
			std::wstring label;

		}TICK_MARK_LABEL, *PTICK_MARK_LABEL;

	public:
		CRuler();
		virtual ~CRuler();

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		//virtual Gdiplus::Bitmap* Render(Bitmap* elementImg, Graphics* elementGfx, CDashboardElement* element);
		virtual Bitmap* Render(Bitmap* elementImg, int elementLayer);

		virtual Bitmap* Render(int sampleIndex);

		// used to initialize the element after it is parsed
		// create all static/cached bitmaps that only change after re-parsing of the layout file;
		// precalculate values needed in each subsequent rendering run
		virtual void Init();

		// returns the pre-rendered and cached ruler image
		Bitmap* GetRulerImage() { return _rulerImg; }

		//void SetBackgroundColor(string& s);
		//void SetForegroundColor(string& s);
		void SetFontStyle(string& s);

		void SetFontHeight(int h) { _fontheight = h; }
		void SetFontName(string& s) { _fontname = s; }

		void SetLayer(int i) { _layer = i; }
		void SetTransparency(float f) { _transparency = f; }


		void SetUsingElement(string& s) { _using = s; }
		string& GetUsingElement() { return _using; }

		void SetPathType(string& s);
		void SetPathType(PathType type);

		void SetDrawPath(bool b) { _drawPath = b; }

		void SetPathColor(string& s);

		void SetPathOffset(int i) { _pathOffset = i; }

		void SetPathWidth(int i) { _pathWidth = i; }

		void SetPathPosition(string& s);

		void SetDrawLabel(bool b) { _drawLabel = b; }

		void SetLabelMargin(int i) { _labelMargin = i; }

		void SetDrawMajor(bool b) { _drawMajor = b; }

		void SetMajorDivisions(int i) { _mjDiv = i; }

		void SetMajorUnits(float i) { _mjUnits = i; }

		void SetMajorMargin(int i) { _mjMargin = i; }

		void SetMajorColor(Color c) { _mjColor = c; }
		void SetMajorColor(string& s);

		void SetMajorShape(string& s);

		void SetMajorLength(int i) { _mjLength = i; }

		void SetMajorWidth(int i) { _mjWidth = i; }

		void SetMajorRadius(int i) { _mjRadius = i; }

		void SetDrawMinor(bool b) { _drawMinor = b; }

		void SetMinorDivisions(int i) { _mnDiv = i; }

		void SetMinorUnits(float i) { _mnUnits = i; }

		void SetMinorMargin(int i) { _mnMargin = i; }

		void SetMinorColor(Color c) { _mnColor = c; }
		void SetMinorColor(string& s);

		void SetMinorShape(string& s);

		void SetMinorLength(int i) { _mnLength = i; }

		void SetMinorWidth(int i) { _mnWidth = i; }

		void SetMinorRadius(int i) { _mnRadius = i; }

		void SetPathRange(int min, int max);
		void SetPathRange(string& s);

		void SetLineStart(string& s);
		void SetLineEnd(string& s);
		//void SetArcStart(int i);
		//void SetArcEnd(int i);

		void SetArc(int start, int end);
		void SetArcSweep(int start, int end);
		void SetArcSweep(string& s);

		void SetArcRadius(int r) { _arcRadius = r; }
		void SetScalingFactor(double s) { _scale = s; }

		void SetLine(Point start, Point end);
		void SetLineOrientation(SliderOrientation o);

		void SetRectangle(Rect rc)
		{
			_rectangle = Rect(rc.X, rc.Y, rc.Width, rc.Height);
			//_elementRect = _rect;
		}
		virtual Gdiplus::Rect GetRectangle() { return Rect(_pos.X, _pos.Y, _size.Width, _size.Height); }

	private:

		void getLabelBounds(int minVal, int maxVal, RectF& bb);
		void getLabelBounds(int val, RectF& bb);

		void calculatePathBounds();

		void calculateArcPath();
		void calculateArcMinorTicks(float phi);
		void calculateArcMajorTicks(float phi);
		void calculateArcLabels(float phi, int val);

		void calculateSweeperPath();
		void calculateSweeperMinorTicks(float phi, PointF tp);
		void calculateSweeperMajorTicks(float phi, PointF tp);
		void calculateSweeperLabels(int val, float phi, PointF tp);
		PointF calculateEllipseTangent(float phi);
		float calculateEllipseNormal(float phi);

		void calculateLinePath();
		void calculateHorizontalLinePath();
		void calculateVerticalLinePath();
		void calculateLineMinorTicks();
		void calculateLineMajorTicks();
		void calculateLineLabels(float off, PointF sp, PointF ep, int val);

		void drawMinorTicks();
		void drawMajorTicks();
		void drawTicksLabels();

		float _a;	// a-axis of ellipse
		float _b;	// b-axis of ellipse;

		//Bitmap* renderArc(int width, int height, PixelFormat pixFmt);

		RectF _elementRect; // the original (un-altered) bounding box of the element this ruler belongs to

		// the position of the ruler image in the dashboard image
		Point _pos;

		// size of the ruler image
		Size _size;

		// pre-rendered and cached ruler image of last parsing run. Only changes if layout file is re-parsed
		// or any other settings of the ruler element changes.
		Bitmap* _rulerImg;

		// ruler graphics object (from ruler image)
		Graphics* _rulerGfx;

		// major tick marks Points (in form of satrt point / end point)
		std::vector<RectF> minorTicksPt;

		// minor tick marks points (in form of start point / end point)
		std::vector<RectF> majorTicksPt;

		// tick marks labels info
		std::vector<TICK_MARK_LABEL> ticksLabels;

		// bounds of the rulers path 
		RectF pathBounds;

		// the rulers path center point
		PointF pathCenter;

		// radius of labels bounding box's center 
		float textR;

		// font used for drawing labels
		Font* _lblFont;

		// orientation of slider (horiz or vert)
		SliderOrientation _orientation;

		//// common element values
		////////////////////////////////////////////////////////////////////////////////////////////////

		//// element background color
		//Gdiplus::Color _background;
		//// element foreground color
		//Gdiplus::Color _foreground;
		//// element fontname
		//std::string _fontname;
		//// element fontstyle
		//Gdiplus::FontStyle _fontstyle;
		//// 
		//int _fontheight;
		//// element layer
		//int _layer;
		//// transparency value added to colors; defined separate from color values
		//float _transparency;
		//// rectangle of element this ruler belongs to
		//RectF _rect;
		////////////////////////////////////////////////////////////////////////////////////////////////

		// The id of another element.The ruler will use attributes of the identified element.Gauge,
		// ringgauge, slider, and sweeper elements are currently supported
		std::string _using;

		// * The path type. Three are supported: arc, line and sweeper.
		PathType _path;
		//Indicates if the path should be drawn. Default = "no"
		bool _drawPath;
		// The color of the path, when drawn. Default = black
		Color _pathColor;
		// The separating distance between the ruler's bounding box 
		// and the box specified by rectangle or "using" element. 
		// See the Ruler Element Layout section for more information. Default = 0
		int _pathOffset;
		// Width of the drawn path. Default = 1
		int _pathWidth;
		// * The value range. Used to compute label values.
		std::tuple<int, int> _pathRange;
		// The position of the path relative to the rectangle or "using" element. 
		// See the Ruler Element Layout section for more information. Valid values:
		// inside, outside, center, above, below, left, right. Default = inside
		PathPositions _pathPos;
		// Indicates if the major ticks should be labeled. Default is yes.
		bool _drawLabel;
		// The distance between the end of the major tick and the center of the label bounding box.
		// The value can be positive or negative. See the Ruler Element Layout section for more information. Default = 0
		int _labelMargin;
		// Specifies formatting for the label. Similar to the "format" attribute in textbox.
		// Valid values: integer, real, time. Default = integer
		LabelFormatType _labelFormat;
		// * The start point of a line path as < x, y >
		Point _lineStart;
		// * The end point of a line path as < x, y >
		Point _lineEnd;
		// * The start angle of a arc path in degrees. Note that angles are
		// measured by the Ruler from the positive x-axis and moving clockwise.
		int _arcStart;
		// * The end angle of a arc path in degrees.
		int _arcEnd;
		// * The sweep of an arc path as < start, end > in degrees.
		std::tuple<int, int> _arcSweep;
		float _sweep; // sweep angle

		// Indicates if major tick marks are to be drawn. This must be set to yes 
		// if labels are wanted. Set the length to zero to display labels only. Default = no
		bool _drawMajor;
		// The number of major subdivisions.
		int _mjDiv;
		// The major tick units after scale is applied, eg, 1 for RPM
		float _mjUnits;
		// Distance between major margin ticks and the path line
		int _mjMargin;
		// The color of the major tick. Default = black
		Color _mjColor;
		// The tick shape. Valid values: line and circle. Default = line
		TickShape _mjShape;
		// The length of a line mark. See the Ruler Element Layout section for more information. Default = 0
		int _mjLength;
		// The width of a line mark. Default = 1
		int _mjWidth;
		// The radius of a circle mark. See the Ruler Element Layout section for more information.
		int _mjRadius;
		// Indicates if minor tick marks are drawn. Cannot be set to "yes" unless major is also "yes". Default = no
		bool _drawMinor;
		// The number of subdivisions of each major subdivision.
		int _mnDiv;
		// The minor tick units after scale is applied, eg, 0.2 for RPM, or 0.1 for Gs
		float _mnUnits;
		// Distance between major margin ticks and the path line
		int _mnMargin;
		// The color of the minor tick. Default = black
		Color _mnColor;
		// The tick shape. Valid values are line default and circle.
		TickShape _mnShape;
		// Distance between minor margin ticks and the path line
		int _mnLength;
		// The width of a line mark. Default = 1
		int _mnWidth;
		// The radius of a circle mark.
		int _mnRadius;
		// the arc radius (retrieved from associated element; if specified)
		int _arcRadius;

		// scaling factor from associated element
		double _scale;

		float _stepAngle;
		float _subStepAngle;
		float _valStep;
		int _steps;
		int _subSteps;
		int _range;

	};

}