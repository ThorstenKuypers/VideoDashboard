#pragma once

#include "common.h"
//#include "Ruler.h"

namespace libLDF
{
	// abstract baseclass for all elements. Defines all attributes and operators common for all
	// layout elements
	class CDashboardElement
	{
	public:
		CDashboardElement();
		virtual ~CDashboardElement();

		virtual Gdiplus::Bitmap* Render(int sampleIndex) =0;
		// used to initialize the element after it is parsed
		// create all static/cached bitmaps that only change after re-parsing of the layout file;
		// precalculate values needed in each subsequent rendering run
		virtual void Init() {}

		void SetDashboardFilePath(std::string path) { _dashFilePath = path; }

		void SetBackgroundColor(std::string& s);
		void SetBackgroundColor(Color c) { _background = c; _useGlobalBackColor = false; }
		void SetBackgroundColor(int r, int g, int b) { _background = Color((BYTE)r, (BYTE)g, (BYTE)b); _useGlobalBackColor = false; }
		std::tuple<int, int, int> GetBackgroundColor() { return std::tuple<int, int, int>(_background.GetR(), _background.GetG(), _background.GetB()); }

		void SetForegroundColor(std::string& s);
		void SetForegroundColor(Color c) { _foreground = c; _useGlobalForeColor = false; }
		void SetForegroundColor(int r, int g, int b) { _foreground = Color((BYTE)r, (BYTE)g, (BYTE)b); _useGlobalForeColor = false; }
		std::tuple<int, int, int> GetForegroundColor() { return std::tuple<int, int, int>(_foreground.GetR(), _foreground.GetG(), _foreground.GetB()); }

		void SetFontName(std::string& s) { _fontname = s; _useGlobalFontname = false; }
		std::string& GetFontName() { return _fontname; }

		void SetTextJustification(string& s);
		void SetTextJustification(TextJustify t) { _justify = t; }
		TextJustify GetTextJustification() { return _justify; }

		void SetFontStyle(string& s);
		void SetFontStyle(Gdiplus::FontStyle s) { _fontstyle = s; }
		Gdiplus::FontStyle GetFontStyle() { return _fontstyle; }

		void SetFontHeight(int h) { _fontheight = h; }
		int GetFontHeight() { return _fontheight; }

		void SetScale(double f) { _scale = f; }
		double GetScale() { return _scale; }

		void SetLayer(int l) { _layer = l; }
		int GetLayer() { return _layer; }

		void SetChannel(string& s) { _channel = s; }
		string& GetChannel() { return _channel; }

		void SetRectangle(string& s);
		void SetRectangle(Gdiplus::Rect rc) { _rectangle = rc; }
		virtual Gdiplus::Rect GetRectangle() { return _rectangle; }

		void SetPosition(Point pos) { _position = pos; }
		void SetPosition(std::string& s);

		void SetID(string& s) { _id = s; }
		string& GetID() { return _id; }

		void SetTransparency(float f) { _transparency = f; }
		float GetTransparency() { return _transparency; }

		DashboardElementType GetElementType(){ return type; }

		virtual void SetRuler(void* ruler) {}
		virtual void* GetRuler() { return _ruler; }


		bool UseGlobalForegroundColor() { return _useGlobalForeColor; }
		bool UseGlobalBackgroundColor() { return _useGlobalBackColor; }
		bool UseGlobalFontName() { return _useGlobalFontname; }
		bool UseGlobalFontStyle() { return _useGlobalFontstyle; }
		bool UseGlobalTextJustify() { return _useGlobalJustify; }

		void SetDataLoggerInstance(libDataLogging::DataLogger::IDataLogger* inst) { _dataLoggerInst = inst; }

	protected:

		void getLabelBounds(Font* fnt, int minVal, int maxVal, StringFormat* strFmt, RectF* bb);
		void getLabelBounds(Font* fnt, int val, RectF* bb);

		DashboardElementType type;

		// path of the dashboard file. 
		std::string _dashFilePath;

		// switches to check if global settings are used or locally overwritten ones
		bool _useGlobalForeColor;
		bool _useGlobalBackColor;
		bool _useGlobalFontname;
		bool _useGlobalFontstyle;
		bool _useGlobalJustify;

		// element background color
		Gdiplus::Color _background;
		// element foreground color
		Gdiplus::Color _foreground;
		// element fontname
		std::string _fontname;
		// element fontstyle
		Gdiplus::FontStyle _fontstyle;
		// 
		int _fontheight;
		//
		TextJustify _justify;
		// A factor used to scale variable values. The value is multiplied by the scale 
		// factor before it is rendered by the element. The default is 1.0
		double _scale;
		// channel name from which the data samples are used to render the element
		std::string _channel;
		// element layer
		int _layer;
		// bounding rectangle of the element describing X-Y position and width/height of the element
		Gdiplus::Rect _rectangle;
		// identifier of the element (should be unique in dashboard layout for each element)
		std::string _id;
		// transparency value added to colors; defined separate from color values
		float _transparency;
		// 
		//std::string _include; 
		// ruler element assigned to this element; based on _id
		void* _ruler;

		// how much every line gets longer when tapering is used
		float _taperStep;

		// x,y position of element in case rectangle is not used
		// eg when an image is used and the size is taken from that image
		Gdiplus::Point _position;

		libDataLogging::DataLogger::IDataLogger* _dataLoggerInst;
	};

}