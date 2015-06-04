#include "Indicator.h"


using namespace libLDF;


CIndicator::CIndicator()
{
	_rgbOn = Color::Transparent;
	_rgbOff = Color::Transparent;
	_thresholdVal = 0;
	_shape = Indicator_ShapeType_image;
	_imgOff = string();
	_imgOn = string();
	_thickness = 1;
	_outlined = true;
	_filled = true;
	_shaded = false;
	_unshaded = 0.1f;

	_position = Point(0, 0);
	_rectangle = Rect(-1, -1, -1, -1);

	_bmpOn = nullptr;
	_bmpOff = nullptr;
}


CIndicator::~CIndicator()
{
	if (_bmpOn != nullptr)
	{
		delete _bmpOn;
		_bmpOn = nullptr;
	}
	if (_bmpOff != nullptr)
	{
		delete _bmpOff;
		_bmpOff = nullptr;
	}
}

void CIndicator::Init()
{
	if (!_imgOn.empty())
	{
		wstring ws = wstring(_imgOn.begin(), _imgOn.end());
		_bmpOn = new Bitmap(ws.c_str());
	}

	if (!_imgOff.empty())
	{
		wstring ws = wstring(_imgOff.begin(), _imgOff.end());
		_bmpOff = new Bitmap(ws.c_str());
	}

}

void CIndicator::SetOnImage(string& s)
{
	if (isAbsoluteFilePath(s))
		_imgOn = s;
	else
		_imgOn = _dashFilePath + s;
}

void CIndicator::SetOffImage(string& s)
{
	if (isAbsoluteFilePath(s))
		_imgOff = s;
	else
		_imgOff = _dashFilePath + s;
}

Gdiplus::Bitmap* CIndicator::Render(int sampleIndex)
{
	Gdiplus::Bitmap* img = nullptr;

	float val = 0;
	try {
		if (_dataLoggerInst != nullptr)
			_dataLoggerInst->GetChannelData(_channel, sampleIndex, &val);
	}
	catch (exception e)
	{
		throw;
	}

	switch (_shape) {
	case Indicator_ShapeType_ellipse:
		img = drawEllipseShape(val);
		break;
	case Indicator_ShapeType_rectangle:
		img = drawRectangleShape(val);
		break;
	case Indicator_ShapeType_line:
		img = drawLineShape(val);
		break;
	case Indicator_ShapeType_image:
		img = drawImage(val);
		break;
	default:
		break;
	}

	return img;
}

Gdiplus::Bitmap* CIndicator::drawEllipseShape(float val)
{
	Bitmap* img = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	if (img != nullptr)
	{
		Graphics* g = Graphics::FromImage(img);
		if (g != nullptr)
		{
			if (val > _thresholdVal &&_thresholdVal > 0)
			{
				if (_filled)
					g->FillEllipse(&SolidBrush(_rgbOn), 0, 0, _rectangle.Width, _rectangle.Height);
				if (_outlined)
					g->DrawEllipse(&Pen(&SolidBrush(_foreground), (float)_thickness), 0, 0, _rectangle.Width, _rectangle.Height);
			}
			else
			{
				if (_filled)
					g->FillEllipse(&SolidBrush(_rgbOff), 0, 0, _rectangle.Width, _rectangle.Height);
				if (_outlined)
					g->DrawEllipse(&Pen(&SolidBrush(_foreground), (float)_thickness), 0, 0, _rectangle.Width, _rectangle.Height);
			}

			delete g;
			g = nullptr;
		}
	}
	return img;
}

Gdiplus::Bitmap* CIndicator::drawRectangleShape(float val)
{
	Bitmap* img = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	if (img != nullptr)
	{
		Graphics* g = Graphics::FromImage(img);
		if (g != nullptr)
		{
			if (val > _thresholdVal &&_thresholdVal > 0)
			{
				if (_filled)
					g->FillRectangle(&SolidBrush(_rgbOn), 0, 0, _rectangle.Width, _rectangle.Height);
				if (_outlined)
					g->DrawRectangle(&Pen(&SolidBrush(_foreground), (float)_thickness), 0, 0, _rectangle.Width, _rectangle.Height);
			}
			else
			{
				if (_filled)
					g->FillRectangle(&SolidBrush(_rgbOff), 0, 0, _rectangle.Width, _rectangle.Height);
				if (_outlined)
					g->DrawRectangle(&Pen(&SolidBrush(_foreground), (float)_thickness), 0, 0, _rectangle.Width, _rectangle.Height);
			}

			delete g;
			g = nullptr;
		}
	}
	return img;
}

Gdiplus::Bitmap* CIndicator::drawLineShape(float val)
{
	//Bitmap* img = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	//if (img != nullptr)
	//{
	//	Graphics* g = Graphics::FromImage(img);
	//	if (g != nullptr)
	//	{
	//		if (val > _thresholdVal)
	//		{
	//			if (_filled)
	//				g->DrawLine(&Pen(&SolidBrush(_rgbOn), _thickness), 0, 0, _rectangle.Width, _rectangle.Height);
	//			if (_outlined)
	//				g->DrawRectangle(&Pen(&SolidBrush(_foreground), _thickness), 0, 0, _rectangle.Width, _rectangle.Height);
	//		}
	//		else
	//		{
	//			if (_filled)
	//				g->FillRectangle(&SolidBrush(_rgbOff), 0, 0, _rectangle.Width, _rectangle.Height);
	//			if (_outlined)
	//				g->DrawRectangle(&Pen(&SolidBrush(_foreground), _thickness), 0, 0, _rectangle.Width, _rectangle.Height);
	//		}

	//		delete g;
	//		g = nullptr;
	//	}
	//}
	//return img;

	return nullptr;
}

Gdiplus::Bitmap* CIndicator::drawImage(float val)
{
	Bitmap* img = nullptr;
	int w = 0, h = 0;

	if (val > _thresholdVal &&_thresholdVal > 0)
	{
		if (_bmpOn != nullptr)
		{
			if (_rectangle.Width == -1 && _rectangle.Height == -1)
			{
				w = _bmpOn->GetWidth();
				h = _bmpOn->GetHeight();
				//_rectangle = Rect(_position.X, _position.Y, _bmpOn->GetWidth(), _bmpOn->GetHeight());
			}
			else
			{
				w = _rectangle.Width;
				h = _rectangle.Height;
			}

			img = new Bitmap(w, h, PixelFormat32bppARGB);
			if (img != nullptr)
			{
				Graphics* g = Graphics::FromImage(img);
				if (g != nullptr)
				{
					g->DrawImage(_bmpOn, 0, 0, w, h);

					delete g;
					g = nullptr;

					return img;
				}
			}
		}
	}
	else
	{
		if (_bmpOff != nullptr)
		{
			if (_rectangle.Width == -1 && _rectangle.Height == -1)
			{
				w = _bmpOff->GetWidth();
				h = _bmpOff->GetHeight();

				//_rectangle = Rect(_position.X, _position.Y, _bmpOff->GetWidth(), _bmpOff->GetHeight());
			}
			else
			{
				w = _rectangle.Width;
				h = _rectangle.Height;
			}
			img = new Bitmap(w, h, PixelFormat32bppARGB);
			if (img != nullptr)
			{
				Graphics* g = Graphics::FromImage(img);
				if (g != nullptr)
				{
					g->DrawImage(_bmpOff, 0, 0, w, h);

					delete g;
					g = nullptr;

					return img;
				}
			}
		}
	}

	return nullptr;
}

void CIndicator::SetColorON(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_rgbOn = Color((255) | c[0]);
		if (c.size() == 3)
			_rgbOn = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CIndicator::SetColorOff(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_rgbOff = Color((255) | c[0]);
		if (c.size() == 3)
			_rgbOff = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CIndicator::SetShape(string& s)
{
	if (s == string("image")) {
		_shape = Indicator_ShapeType_image;
	}
	else if (s == string("line")) {
		_shape = Indicator_ShapeType_line;
	}
	else if (s == string("rectangle")) {
		_shape = Indicator_ShapeType_rectangle;
	}
	else if (s == string("ellipse")) {
		_shape = Indicator_ShapeType_ellipse;
	}
	else{
		_shape = Indicator_ShapeType_line;
	}
}
