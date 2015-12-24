#include "Slider.h"


using namespace libLDF;


CSlider::CSlider() :CDashboardElement(),
_orientation(SliderOrientation::vertical),
_rgbNegative(Color::Red),
_rgbPositive(Color::Blue),
_cp(PointF(0, 0)),
_valStep(0)
{
	type = DashboardElementType::slider;
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

	imgInfo.width = _rectangle.Width;
	imgInfo.height = _rectangle.Height;
	imgInfo.stride = imgInfo.width * sizeof(int);

	_pixBufLen = imgInfo.stride * imgInfo.height;
	_pixBuf = std::shared_ptr<BYTE>(new BYTE[_pixBufLen]);
	imgInfo.pixbuf = _pixBuf.get();
}

ImageInfo CSlider::Render(DataSample& sample, IGenericLogger* logger, bool renderBlank)
{
	Graphics* gfx = nullptr;
	int min = std::get<0>(_range), max = std::get<1>(_range);
	float w = 0, h = 0;
	RectF rc = RectF(0, 0, 0, 0);
	Brush* br = nullptr;
	float val = 0;

	memset(_pixBuf.get(), 0, _pixBufLen);

	Bitmap bmp(imgInfo.width, imgInfo.height, imgInfo.stride, imgInfo.pixelFormat, static_cast<BYTE*>(imgInfo.pixbuf));
	gfx = Graphics::FromImage(&bmp);
	if (gfx != NULL)
	{
		if (!renderBlank) {
			try {
				CDataChannel& ch = std::move(logger->GetChannel(_channel));
				SampleValue sv = CDataChannel::GetSampleData(*logger, sample, ch);

				switch (sv.type())
				{
				case irsdk_float:
					val = sv.get_value<float>();
					break;
				case irsdk_double:
					val = static_cast<float>(sv.get_value<double>());
					break;
				case irsdk_int:
					val = static_cast<float>(sv.get_value<int>());
					break;
				case irsdk_char:
					val = static_cast<float>(sv.get_value<char>());
					break;
				default:
					val = 0;
					break;
				}
			}
			catch (std::exception)
			{
				throw;
			}
		}

#pragma region "Center 0 Slider rendering (min->0->max)"

		if (min != 0 && max != 0)
		{
			if (_orientation == SliderOrientation::horizontal) {

				w = val / _valStep;

				// render from min to max with 0 in center
				if (val < 0) {

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

				h = val / _valStep;

				if (val < 0) {

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
				w = val / _valStep;

				if (val < 0) {

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
				h = val / _valStep;

				if (val < 0) {

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

		gfx->FillRectangle(br, rc);

		delete br;
		br = nullptr;

		delete gfx;
		gfx = NULL;

	}

	return ImageInfo{ imgInfo.width, imgInfo.height, imgInfo.stride, imgInfo.pixelFormat, _pixBuf.get() };
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
	Color c;

	try {
		getColorFromString(s, c);
		//if (c.size() == 1)
		//	_rgbPositive = Color((255) | c[0]);
		//if (c.size() == 3)
		//	_rgbPositive = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
		_rgbPositive = c;
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CSlider::SetColorNegative(string& s)
{
	Color c;

	try {
		getColorFromString(s, c);
		//if (c.size() == 1)
		//	_rgbNegative = Color((255) | c[0]);
		//if (c.size() == 3)
		//	_rgbNegative = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
		_rgbNegative = c;
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CSlider::SetRuler(CRuler& r)
{
	r.SetPathRange(std::get<0>(_range), std::get<1>(_range));
	r.SetLineOrientation(_orientation);
	r.SetPathType(pathType_line);
	r.SetRectangle(_rectangle);

	if (_scale != 1)
		r.SetScalingFactor(_scale);
}