#include "Slider.h"


using namespace libLDF;


CSlider::CSlider()
{
	type = DashboardElementType::slider;

	_orientation = SliderOrientation::vertical;
	_rgbNegative = Color::Red;
	_rgbPositive = Color::Blue;

	_cp = PointF(0, 0);
	_valStep = 0;
}


CSlider::~CSlider()
{
}

void CSlider::Init()
{
	if (_rectangle.Width > _rectangle.Height) {

		_orientation = SliderOrientation::horizontal;

		float v = (float)abs(std::get<0>(_range)) + abs(std::get<1>(_range));
		_valStep = v / (float)_rectangle.Width;

		_cp.X = std::get<0>(_range) / _valStep;
		if (_cp.X < 0)
			_cp.X *= -1;
	}
	if (_rectangle.Height > _rectangle.Width) {

		_orientation = SliderOrientation::vertical;

		float v = (float)abs(std::get<0>(_range)) + abs(std::get<1>(_range));
		_valStep = v / _rectangle.Height;

		_cp.Y = _rectangle.Height - abs((std::get<0>(_range) / _valStep));
		if (_cp.Y < 0)
			_cp.Y *= -1;
	}


}

Gdiplus::Bitmap* CSlider::Render(int sampleIndex)
{
	Graphics* gfx = NULL;
	int min = std::get<0>(_range), max = std::get<1>(_range);
	float w = 0, h = 0;
	RectF rc = RectF(0, 0, 0, 0);
	Brush* br =nullptr;

	Bitmap* bmp = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	if (bmp != NULL)
	{
		gfx = Graphics::FromImage(bmp);
		if (gfx != NULL)
		{

			float v = 0;
			if (_dataLoggerInst != NULL)
				_dataLoggerInst->GetChannelData(_channel, sampleIndex, &v);
			//v *= 100.0f;

			//v = v / 9.81;

#pragma region "Center 0 Slider rendering (min->0->max)"

			if (min != 0 && max != 0)
			{
				if (_orientation == SliderOrientation::horizontal) {

					w = v / _valStep;

					// render from min to max with 0 in center
					if (v < 0) {

						br = new SolidBrush(_rgbNegative);

						if (min < 0) {
							rc.X = _cp.X + w;
							rc.Y = 0;
							rc.Height = (float)_rectangle.Height;
							if (w < 0)
								w *= -1;
							rc.Width = w;
						}
						else {
							rc.X = _cp.X;
							rc.Y = 0;
							rc.Height = (float)_rectangle.Height;
							if (w < 0)
								w *= -1;
							rc.Width = w;

						}
					}
					else {

						br = new SolidBrush(_rgbPositive);

						if (max > 0) {
							rc.X = _cp.X;
							rc.Y = 0;
							rc.Height = (float)_rectangle.Height;
							rc.Width = w;
						}
						else {
							rc.X = _cp.X - w;
							rc.Y = 0;
							rc.Height = (float)_rectangle.Height;
							rc.Width = w;
						}
					}
				}

				if (_orientation == SliderOrientation::vertical) {

					h = v / _valStep;

					if (v < 0) {

						br = new SolidBrush(_rgbNegative);

						if (min < 0) {

							rc.X = 0;
							rc.Y = _cp.Y;
							rc.Width = (float)_rectangle.Width;
							if (h < 0)
								h *= -1;
							rc.Height = h;
						}
						else {

							rc.X = 0;
							rc.Y = _cp.Y + h;
							rc.Width = (float)_rectangle.Width;
							if (h < 0)
								h *= -1;
							rc.Height = h;
						}
					}
					else {

						br = new SolidBrush(_rgbPositive);

						if (max > 0) {

							rc.X = 0;
							rc.Y = _cp.Y - h;
							rc.Width = (float)_rectangle.Width;
							if (h < 0)
								h *= -1;
							rc.Height = h;
						}
						else {

							rc.X = 0;
							rc.Y = _cp.Y;
							rc.Width = (float)_rectangle.Width;
							if (h < 0)
								h *= -1;
							rc.Height = h;

						}
					}
				}
			}
#pragma endregion

#pragma region "<0,+x> ; <0,-x> ; <+x,0> ; <-x,0>"

			else if (min == 0 || max == 0)
			{
				if (_orientation == SliderOrientation::horizontal)
				{
					w = v / _valStep;

					if (v < 0) {

						br = new SolidBrush(_rgbNegative);
						if (w < 0)
							w *= -1;

						if (max < 0)
						{
							rc.X = 0;
							rc.Y = 0;
							rc.Width = w;
							rc.Height = (float)_rectangle.Height;

						}
						else if (min < 0) {

							rc.X = (float)_rectangle.Width - w;
							rc.Y = 0;
							rc.Width = w;
							rc.Height = (float)_rectangle.Height;

						}
					}
					else {

						br = new SolidBrush(_rgbPositive);

						if (max > 0)
						{
							rc.X = 0;
							rc.Y = 0;
							rc.Width = w;
							rc.Height = (float)_rectangle.Height;

						}
						else if (min > 0) {

							rc.X = (float)_rectangle.Width - w;
							rc.Y = 0;
							rc.Width = w;
							rc.Height = (float)_rectangle.Height;

						}
					}
				}

				if (_orientation == SliderOrientation::vertical)
				{
					h = v / _valStep;

					if (v < 0) {

						br = new SolidBrush(_rgbNegative);

						if (h < 0)
							h *= -1;

						if (min < 0)
						{
							rc.X = 0;
							rc.Y = 0;
							rc.Width = (float)_rectangle.Height;
							rc.Height = h;
						}
						else if (max < 0) {

							rc.X = 0;
							rc.Y = (float)_rectangle.Height - h;
							rc.Width = (float)_rectangle.Width;
							rc.Height = h;
						}
					}
					else {

						br = new SolidBrush(_rgbPositive);

						if (min > 0)
						{

							rc.X = 0;
							rc.Y = 0;
							rc.Width = (float)_rectangle.Height;
							rc.Height = h;
						}
						else if (max > 0) {

							rc.X = 0;
							rc.Y = (float)_rectangle.Height - h;
							rc.Width = (float)_rectangle.Width;
							rc.Height = h;

						}
					}
				}
			}
#pragma endregion

			//Pen& pen = Pen(Color(Color::Black), 1.0f);
			gfx->FillRectangle(br, rc);
			//gfx->DrawRectangle(&pen, 0, 0, _rectangle.Width, _rectangle.Height);

			//if (_ruler != nullptr) {

			//	Bitmap* rulerImg = ((CRuler*)_ruler)->Render(bmp, _layer);
			//	if (rulerImg != nullptr) {

			//		delete gfx;
			//		delete bmp;
			//		bmp = rulerImg;
			//		gfx = Graphics::FromImage(bmp);
			//	}
			//}

			delete br;
			br = nullptr;

			delete gfx;
			gfx = NULL;

		}
	}

	return bmp;
}

void CSlider::SetRange(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_range) = stoi(toks[0]);
	std::get<1>(_range) = stoi(toks[1]);
}

void CSlider::SetColorPositive(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_rgbPositive = Color((255) | c[0]);
		if (c.size() == 3)
			_rgbPositive = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CSlider::SetColorNegative(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_rgbNegative = Color((255) | c[0]);
		if (c.size() == 3)
			_rgbNegative = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CSlider::SetRuler(void* r)
{
	if (r != nullptr) {

		_ruler = r;

		((CRuler*)_ruler)->SetPathRange(std::get<0>(_range), std::get<1>(_range));
		((CRuler*)_ruler)->SetLineOrientation(_orientation);
		((CRuler*)_ruler)->SetPathType(pathType_line);
		((CRuler*)_ruler)->SetRectangle(_rectangle);

		if (_scale != 1)
			((CRuler*)_ruler)->SetScalingFactor(_scale);
	}
}