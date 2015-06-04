#include "Dashboard.h"

using namespace libLDF;

CDashboard::CDashboard()
{
	_targetwidth = 0;
	_height = 0;
	_width = 0;

	_fontHeight = 12;
	_fontName = string("Arial");
	_fontStyle = FontStyleRegular;
	_foreground = Color::White;
	_background = Color::Transparent;

	_dataLoggerInst = NULL;
	_showBoundingBoxes = false;

}

CDashboard::CDashboard(std::string name, int targetWidth)
{
	_targetwidth = targetWidth;
	_height = 0;
	_width = 0;

	_shortName = name;
	_showBoundingBoxes = false;
}

CDashboard::~CDashboard()
{
	for (unsigned int i = 0; i < _elements.size(); i++) {

		if (_elements[i] != nullptr) {

			delete _elements[i];
			_elements[i] = nullptr;
		}
	}

	_dataLoggerInst = nullptr;
}

void CDashboard::Render(Gdiplus::Bitmap& videoFrame, int sampleIndex)// ,Point pos, float transparency, float scaleFactor
{
	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(&videoFrame);
	if (gfx != nullptr) {

		Gdiplus::Bitmap* dash = RenderToImage(sampleIndex);
		if (dash != nullptr) {

			gfx->DrawImage(dash, 0, 0, dash->GetWidth(), dash->GetHeight());
		}

		delete gfx;
	}
}

Bitmap* CDashboard::RenderToImage(int sampleIndex)
{
	Gdiplus::Bitmap* img = nullptr;

	WCHAR dir[256] = { 0 };
	GetCurrentDirectoryW(256, dir);

	if (!_imagefile.empty()){
		wstring ws = wstring(_imagefile.begin(), _imagefile.end());
		WCHAR* f = (WCHAR*)ws.c_str();
		img = new Bitmap(f);
		int w = img->GetWidth(), h = img->GetHeight();

		int x = 0;
	}
	else if (_width != 0 && _height != 0) {

		img = new Bitmap(_width, _height, PixelFormat32bppARGB);
	}
	else {
		throw - 1;
	}

	if (img == nullptr)
		throw - 1;

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	if (gfx == nullptr) {

		if (img != nullptr)
			delete img;

		throw - 1;
	}

	CRuler* ruler = nullptr;
	Bitmap* rulerImg = nullptr;
	Gdiplus::Rect rulerRect;
	Bitmap* bmp;
	Gdiplus::Rect rc;
	int rulerLayer = 0;

	/***********************************************************************************************/
	// experimental code
	for (int layer = 0; layer < MAX_LAYERS; layer++){
		for (unsigned int i = 0; i < _elements.size(); i++) {

			ruler = (CRuler*)_elements[i]->GetRuler();
			if (layer == _elements[i]->GetLayer()) {
				bmp = _elements[i]->Render(sampleIndex);
				rc = _elements[i]->GetRectangle();
				if (bmp != nullptr) {

					if (ruler != nullptr) {

						rulerImg = ruler->Render(sampleIndex);
						rulerRect = ruler->GetRectangle();
						rulerLayer = ruler->GetLayer();
					}

					// TODO: add layering
					if (rulerImg != nullptr && rulerLayer <= layer) {

						// draw ruler image first if same or lower layer
						gfx->DrawImage(rulerImg, rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());

						if (_showBoundingBoxes)
							gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
					}


					gfx->DrawImage(bmp, rc.X, rc.Y, bmp->GetWidth(), bmp->GetHeight());
					if (_showBoundingBoxes)
						gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rc.X, rc.Y, bmp->GetWidth(), bmp->GetHeight());

					if (rulerImg != nullptr && rulerLayer > layer) {

						// draw ruler image afetr corresponding element if ruler is on higher layer
						gfx->DrawImage(rulerImg, rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
						if (_showBoundingBoxes)
							gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
					}

					// NO NOT free the returned image if the rendered element is a ruler because
					// the ruler element is the only static element. The image reference is held 
					// in its class and freed when the ruler class instance is destroyed!
					if (_elements[i]->GetElementType() != DashboardElementType::ruler) {
						delete bmp;
						bmp = nullptr;
					}
				}
			}
		}
	}

	/*****************************************************************************************************/

	delete gfx;
	gfx = nullptr;

	return img;
}

void CDashboard::SetBackgroundColor(std::string& s)
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
}

void CDashboard::SetForegroundColor(std::string& s)
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
}

void CDashboard::SetFontStyle(string& s)
{

	if (s == string("bold")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleBold;
	}
	else if (s == string("italic")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleItalic;
	}
	else if (s == string("normal")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	}
	else if (s == string("bold_italic")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleBoldItalic;
	}
	else {

		_fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	}
}

void CDashboard::SetTextJustification(string& s)
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
}

void CDashboard::SetDataLogger(IDataLogging* logger)
{
	_dataLoggerInst = logger;

	// assign data logger instance to each element
	for (size_t i = 0; i < _elements.size(); i++) {

		_elements[i]->SetDataLoggerInstance(logger);
	}
}

void CDashboard::SetImageFile(std::string& s)
{
	if (isAbsoluteFilePath(s))
		_imagefile = s;
	else
		_imagefile = _dashFilePath + s;
}