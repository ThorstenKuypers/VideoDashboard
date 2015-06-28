#pragma once
#ifndef _DASHBOARD_H_
#define _DASHBOARD_H_

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

#include <fstream>

#include "common.h"
#include "libLDF.h"
#include "DashboardElement.h"
#include "Gauge.h"
#include "GCircle.h"
#include "Indicator.h"
#include "Slider.h"
#include "Sweeper.h"
#include "TextBox.h"
#include "Ruler.h"
#include "RingGauge.h"

#define MAX_LAYERS	16 // maximum number of supported layers


namespace libLDF
{

	// This class defines the actual dashboard element as a whole
	class CDashboard
	{
	public:

		CDashboard(std::string file) :
			_targetwidth(0),
			_height(0),
			_width(0),
			_fontHeight(12),
			_fontName(string("Arial")),
			_fontStyle(FontStyleRegular),
			_foreground(Color::White),
			_background(Color::Transparent),
			_showBoundingBoxes(false)
		{
			_fileName = file;
		}

		CDashboard(std::string name, int targetWidth = 1024) :
			_targetwidth(targetWidth),
			_height(0),
			_width(0),
			_shortName(name),
			_showBoundingBoxes(false)
		{
		}

		~CDashboard() {}

		// prses the dashboard object from file
		void Parse();

		Bitmap* RenderToImage(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank);

		void SetDashboardFilePath(std::string s) { _dashFilePath = s; }

		void SetShortName(string& name) { _shortName = name; }
		string& GetShortName() { return _shortName; }

		void SetPrettyName(std::string& s) { _prettyName = s; }
		std::string& GetPrettyName() { return _prettyName; }

		void SetTargetWidth(int w) { _targetwidth = w; }
		int GetTargetWidth() { return _targetwidth; }

		void SetImageFile(std::string& s); // { _imagefile = s; }
		std::string& GetImageFile() { return _imagefile; }

		void SetHeight(int h) { _height = h; }
		int GetHeight() { return _height; }

		void SetWidth(int w) { _width = w; }
		int GetWidth() { return _width; }

		void SetBackgroundColor(std::string& s);
		void SetBackgroundColor(int r, int g, int b) { _background = Color((BYTE)r, (BYTE)g, (BYTE)b); }
		Color GetBackgroundColor() { return _background; }

		void SetForegroundColor(std::string& s);
		void SetForegroundColor(int r, int g, int b) { _foreground = Color((BYTE)r, (BYTE)g, (BYTE)b); }
		Color GetForegroundColor() { return _foreground; }

		void SetFontName(std::string& s) { _fontName = s; }
		std::string& GetFontName() { return _fontName; }

		void SetTextJustification(string& s);
		TextJustify GetTextJustification() { return _justify; }

		void SetFontStyle(string& s);
		Gdiplus::FontStyle GetFontStyle() { return _fontStyle; }

		void SetFontHeight(int h) { _fontHeight = h; }
		int GetFontHeight() { return _fontHeight; }

		void AddDashboardElement(CDashboardElement& element) { _elements.emplace_back(std::move(element)); }

		std::vector<CDashboardElement>& GetDashboardElements() { return _elements; }

		void SetRulerTable(std::map<std::string, CRuler>& table) { rulerTable = std::move(table); }
		std::map<std::string, CRuler>& GetRulerTable() { return rulerTable; }


		void ShowBoundingBoxes(bool show)
		{
			_showBoundingBoxes = show;
		}

		void clear()
		{
			_elements.clear();
			_showBoundingBoxes = false;
			_dashFilePath = "";
			_shortName = "";
			_prettyName = "";
			_targetwidth = 0;
			_imagefile = "";
			_height = 0;
			_width = 0;
			_background = 0;
			_foreground = 0;
			_fontHeight = 0;
			_fontName = "";
			_fontStyle = FontStyleRegular;
			_justify = TextJustify_left;
		}

	private:

		bool _showBoundingBoxes;

		std::string _dashFilePath;

		// file name of this dahboard object
		std::string _fileName;

		// the name that appears in the dashboard selection list
		std::string _shortName;
		// is not important. It's not actually used anywhere! (except as internal ID)
		std::string _prettyName;
		// is the pixel width of the video frame the this dashboard was designed for.
		// VideoDashboard will scale the dash when used with a smaller frame width.
		int _targetwidth;
		// the name of the background image file for the dashboard
		std::string _imagefile;
		// the height of the dashboard in pixels if no image file is used
		int _height;
		// the width of the dashboard in pixels if no image file is used
		int _width;
		// the default background color [typically if no imagefile is used]
		Color _background;
		// the default color for foreground elements, including text
		//std::tuple<int, int, int> _foreground;
		Color _foreground;
		// the default font for all text elements
		std::string _fontName;
		// the default font style
		Gdiplus::FontStyle _fontStyle;
		// the default text justification [left/center/right]
		TextJustify _justify;
		// The height of the font, in pixels.The font height defaults to the height of the element's bounding
		int _fontHeight;

		std::map<std::string, CRuler> rulerTable;

		// vector containig all elements found in definition file
		std::vector<std::unique_ptr<CDashboardElement>> _elements;
	};

}

#endif // _DASHBOARD_H_