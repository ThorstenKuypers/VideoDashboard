#include "RingGauge.h"

using namespace libLDF;

CRingGauge::CRingGauge()
{
	type = DashboardElementType::ringgauge;

	_range = std::tuple<int, int>(0, 0);
	_width = 0;
	_sweep = std::tuple<int, int>(0, 0);
	_rotation = RotationType::cw;
	_shaded = false;
	_markerWidth = 1;
	_markerColor = Color(255, 255, 255);
	_markerDecay = 0;
	_markerHoldTime = 0;
}


CRingGauge::~CRingGauge()
{
}


Gdiplus::Bitmap* CRingGauge::Render(int sampleIndex)
{
	Bitmap* bmp = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	if (bmp != nullptr) {

		Graphics* gfx = Graphics::FromImage(bmp);
		if (gfx != nullptr) {
			gfx->SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
			gfx->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
			gfx->SetTextRenderingHint(TextRenderingHint::TextRenderingHintClearTypeGridFit);

			float sweep = (float)(std::get<1>(_sweep) - std::get<0>(_sweep));
			//if (sweep < 0)
			if (_rotation == RotationType::cw) {
				sweep *= -1;
				sweep = 360.0f - sweep;
			}
			else
				sweep -= 360.0f;

			if (sweep >= 360.0f)
				sweep -= 360.0f;
			if (sweep <= -360.0f)
				sweep += 360.0f;

			float maxVal = (float)std::get<1>(_range) * (float)_scale;
			float minVal = (float)std::get<0>(_range) * (float)_scale;
			float stepping = sweep / (maxVal - (float)std::get<0>(_range)); // degrees per channel value step

			float val = 0;

			if (_dataLoggerInst != NULL)
				_dataLoggerInst->GetChannelData(_channel, sampleIndex, &val);

			// scale channel value
			val *= (float)_scale;

			// is current value lower than minumum value defined by range attribute?
			if (val < minVal)
				val = 0;
			else
				val -= minVal;

			float rot = (val*stepping); // +std::get<0>(_sweep); // needle rotation angle

			Gdiplus::Pen* pen = new Gdiplus::Pen(_foreground, (float)_width);
			gfx->DrawArc(pen, (int)((float)_width / 2), (int)((float)_width / 2), _rectangle.Width - _width, _rectangle.Height - _width, (float)std::get<0>(_sweep), rot);

			delete pen;
			pen = nullptr;

			//if (_ruler != nullptr) {

			//	Bitmap* rulerImg = ((CRuler*)_ruler)->Render(bmp, _layer);
			//	if (rulerImg != nullptr) {

			//		delete gfx;
			//		delete bmp;
			//		bmp = rulerImg;
			//		gfx = Graphics::FromImage(bmp);
			//	}
			//}

			delete gfx;
		}
	}
	return bmp;
}

void CRingGauge::Init()
{

}

void CRingGauge::SetRange(std::string& s)
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

void CRingGauge::SetRingWidth(string& s)
{
	_width = stoi(s);
}

void CRingGauge::SetSweep(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_sweep) = stoi(toks[0]);
	std::get<1>(_sweep) = stoi(toks[1]);
}

void CRingGauge::SetRotation(string& s)
{
	if (s == "cw")
	{
		_rotation = RotationType::cw;
	}
	else if (s == "ccw")
		_rotation = RotationType::ccw;
	else
		_rotation = RotationType::cw; //default
}

void CRingGauge::SetMarkerWidth(string& s)
{
	_markerWidth = stoi(s);
}

void CRingGauge::SetMarkerColor(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_markerColor = Color((255) | c[0]);
		if (c.size() == 3)
			_markerColor = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CRingGauge::SetMarkerHoldTime(string& s)
{
	_markerHoldTime = stoi(s);
}

void CRingGauge::SetMarkerDecay(string& s)
{
	_markerDecay = stoi(s);
}

void CRingGauge::SetRuler(void* ruler)
{
	_ruler = ruler;

	if (_ruler != nullptr) {

		((CRuler*)_ruler)->SetPathRange(std::get<0>(_range), std::get<1>(_range));
		((CRuler*)_ruler)->SetArcRadius(_rectangle.Width / 2);
		((CRuler*)_ruler)->SetRectangle(_rectangle);

		if (_rectangle.Width == _rectangle.Height)
			((CRuler*)_ruler)->SetPathType(std::string("arc"));
		else
			((CRuler*)_ruler)->SetPathType(std::string("sweeper"));

		if (_scale != 1)
			((CRuler*)_ruler)->SetScalingFactor(_scale);

		int s = std::get<0>(_sweep), e = std::get<1>(_sweep);
		if (_rotation == RotationType::cw) {

			if (s > 0)
				s = 360 - s;
			else
				s *= -1;

			if (e < 0)
				e = -360 - e;
			else
				e *= -1;

		}
		if (_rotation == RotationType::ccw) {

			if (s > 0)
				s *= -1;
			else
				s = -360 - s;

			if (e < 0)
				e *= -1;
			else
				e = 360 - e;

		}

		((CRuler*)_ruler)->SetArcSweep(s, e);

	}
}