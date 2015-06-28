#include "DashboardElement.h"

using namespace libLDF;

void CDashboardElement::SetRectangle(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 4)
		throw parsing_error::missingArgument;

	_rectangle = Gdiplus::Rect(stoi(toks[0]), stoi(toks[1]), stoi(toks[2]), stoi(toks[3]));
}

void CDashboardElement::SetPosition(std::string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	_position = Point(stoi(toks[0]), stoi(toks[1]));
	_rectangle.X = _position.X;
	_rectangle.Y = _position.Y;
}

void CDashboardElement::SetBackgroundColor(std::string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_background = Color((255) | c[0]);
		if (c.size() == 3)
			_background = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
	_useGlobalBackColor = false;
}

void CDashboardElement::SetForegroundColor(std::string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_foreground = Color((255) | c[0]);
		if (c.size() == 3)
			_foreground = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}

	_useGlobalForeColor = false;
}

void CDashboardElement::SetFontStyle(string& s)
{

	if (s == string("bold")) {

		_fontstyle = Gdiplus::FontStyle::FontStyleBold;
	}
	else if (s == string("italic")) {

		_fontstyle = Gdiplus::FontStyle::FontStyleItalic;
	}
	else if (s == string("normal")) {

		_fontstyle = Gdiplus::FontStyle::FontStyleRegular;
	}
	else if (s == string("bold_italic")) {

		_fontstyle = Gdiplus::FontStyle::FontStyleBoldItalic;
	}
	else {

		_fontstyle = Gdiplus::FontStyle::FontStyleRegular;
	}

	_useGlobalFontstyle = false;
}

void CDashboardElement::SetTextJustification(string& s)
{
	if (s == string("center")) {

		_justify = TextJustify_center;
	}
	else if (s == string("right")) {

		_justify = TextJustify_right;
	}
	else if (s == string("left")) {

		_justify = TextJustify_left;
	}
	else {

		_justify = TextJustify_left;
	}

	_useGlobalJustify = false;
}

void CDashboardElement::getLabelBounds(Font* fnt, int minVal, int maxVal, StringFormat* strFmt, RectF* bb)
{
	float bbWidth, bbHeight;
	RectF bbMinVal, bbMaxVal;

	if (fnt != nullptr) {

		wstringstream ss;
		ss << minVal;
		wstring s = ss.str();
		WCHAR* wc = (WCHAR*)s.c_str();

		Graphics* gfx = new Graphics((HWND)NULL);
		if (gfx != NULL) {

			if (strFmt != nullptr)
				gfx->MeasureString(wc, (int)s.size(), fnt, PointF(0, 0), strFmt, &bbMaxVal);
			else
				gfx->MeasureString(wc, (int)s.size(), fnt, PointF(0, 0), &bbMaxVal);

			// redo
			ss = wstringstream();
			ss << maxVal;
			s = ss.str();
			wc = (WCHAR*)s.c_str();

			if (strFmt != nullptr)
				gfx->MeasureString(wc, (int)s.size(), fnt, PointF(0, 0), strFmt, &bbMinVal);
			else
				gfx->MeasureString(wc, (int)s.size(), fnt, PointF(0, 0), &bbMinVal);

			delete gfx;
			gfx = nullptr;
		}
	}

	bbWidth = max(bbMinVal.Width, bbMaxVal.Width);
	bbHeight = max(bbMinVal.Height, bbMaxVal.Height);

	if (bb != nullptr)
		*bb = RectF(0, 0, bbWidth, bbHeight);
}

void CDashboardElement::getLabelBounds(Font* fnt, int val, RectF* bb)
{
	if (fnt != nullptr) {
		Graphics* gfx = Graphics::FromHWND((HWND)NULL);
		if (gfx != NULL) {

			wstringstream wss;
			wss << (int)val;
			wstring ws = wss.str();

			WCHAR* wc = (WCHAR*)ws.c_str();

			gfx->MeasureString(wc, (int)ws.length(), fnt, PointF(0, 0), bb);

			delete gfx;
			gfx = nullptr;
		}
	}
}