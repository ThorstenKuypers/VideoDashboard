#include "Sweeper.h"


using namespace libLDF;


CSweeper::CSweeper()
{
	type = DashboardElementType::sweeper;

	_path = pathType_sweeper;
	_range = tuple<int, int>(0, 0);
	_sweepAngle = tuple<int, int>(0, 0);
	_sweep = 0;
	_divisions = 0;
	_rect = Rect(-1, -1, -1, -1);
	_lineEnd = std::tuple<int, int>(0, 0);
	_lineStart = std::tuple<int, int>(0, 0);
	_shape = _shapeType_Sweeper::shapeType_line;
	_extend = 0;
	_thickness = 0;
	_taper = 0;
	_divisions = 0;
	_rgbOff = Color::White;
	_rgbOn = Color::Black;
	_threshold = std::vector<int>();
	_rgbThresholdOn = std::vector<Color>();
	_renderDir = RotationType::cw;
	_sweeperInfo = SWEEPER_INFO();
	_origRect = Rect(-1, -1, -1, -1);

}


CSweeper::~CSweeper()
{
}

void CSweeper::SetRuler(void* r)
{
	_ruler = r;

	((CRuler*)_ruler)->SetRectangle(_rect);
	((CRuler*)_ruler)->SetPathType(_path);
	((CRuler*)_ruler)->SetPathRange(std::get<0>(_range), std::get<1>(_range));
	((CRuler*)_ruler)->SetArcSweep((int)round(std::get<0>(_sweepAngle)), (int)(round(std::get<1>(_sweepAngle))));

	if (_scale != 1)
		((CRuler*)_ruler)->SetScalingFactor(_scale);

}

Gdiplus::Bitmap* CSweeper::Render(int sampleIndex)
{
	int w = _sweeperInfo.width;
	int h = _sweeperInfo.height + 2;

	//_rectangle = Rect(_rect.X, _rect.Y, w, h);
	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	if (bmp != nullptr) {

		Graphics* gfx = Graphics::FromImage(bmp);
		if (gfx != nullptr) {

			gfx->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			gfx->SetSmoothingMode(SmoothingModeAntiAlias);

			// TODO: FixMe***
			// check for correct data type!
			float val = 0;
			if (_dataLoggerInst != NULL)
				_dataLoggerInst->GetChannelData(_channel, sampleIndex, (void*)&val);

			if (_scale != 0)
				val *= (float)_scale;
			float minVal = (std::get<0>(_range) * (float)_scale);

			if (val < minVal)
				val = 0;
			else
				val -= minVal;

			size_t activeDivs = (size_t)round(val / _sweeperInfo.divStep); // number of active indicators for current value

			Pen* pen = new Pen(_rgbOn, (float)_thickness);
			PointF sp, ep;
			SizeF sz;
			float x = 0, y = 0;

			for (size_t i = 0; i < activeDivs && i < _sweeperInfo.indicators.size(); i++) {

				switch (_shape)
				{
				case shapeType_line:
					sp = PointF(_sweeperInfo.indicators[i].X, _sweeperInfo.indicators[i].Y);
					ep = PointF(_sweeperInfo.indicators[i].Width, _sweeperInfo.indicators[i].Height);

					// draw line indicator
					gfx->DrawLine(pen, sp, ep);

					break;

				case shapeType_ellipse:

					// calculate center point of circle indicator 
					// (center of inner/outer radius from indicator line)
					x = ((_sweeperInfo.indicators[i].X + _sweeperInfo.indicators[i].Width) / 2);
					y = ((_sweeperInfo.indicators[i].Y + _sweeperInfo.indicators[i].Height) / 2);
					sp = PointF(x - ((float)_extend / 2), y - ((float)_extend / 2));
					sz = SizeF((float)_extend, (float)_extend);

					gfx->FillEllipse(&SolidBrush(_rgbOn), RectF(sp.X, sp.Y, sz.Width, sz.Height));

					break;

				case shapeType_rectangle:

					// calculate center point of rectangle indicator 
					// (center of inner/outer radius from indicator line)
					x = ((_sweeperInfo.indicators[i].X + _sweeperInfo.indicators[i].Width) / 2);
					y = ((_sweeperInfo.indicators[i].Y + _sweeperInfo.indicators[i].Height) / 2);
					sp = PointF(x - ((float)_extend / 2), y - ((float)_extend / 2));
					sz = SizeF((float)_extend, (float)_extend);

					gfx->FillRectangle(&SolidBrush(_rgbOn), RectF(sp.X, sp.Y, sz.Width, sz.Height));

					break;
				}
			}
			// NEW
			// ruler element is now drawn in its own image and rendered to
			// the dashboard in the dashboards render method just like all 
			// other elements!

			//if (_ruler != NULL) {

			//	Bitmap* rulerImg = _ruler->Render(bmp, _layer);
			//	if (rulerImg != nullptr) {

			//		delete gfx;
			//		delete bmp;
			//		bmp = rulerImg;
			//		gfx = Graphics::FromImage(bmp);
			//	}
			//}

			delete pen;

			delete gfx;
		}
	}

	return bmp;
}

/******************************************************************************
*** Method: calculateEllipsePath()
*** args: none
*** returns: nothing
***
*** Calculates the elliptical path, along which the indicator elements are positioned
*** and drawn.
***
******************************************************************************/
void CSweeper::calculateEllipsePath()
{
	float a = (float)_rect.Width; // X-axis of ellipse
	float b = (float)_rect.Height; // Y-axis of ellipse
	PointF cp = PointF((float)(_rect.Width + _extend), (float)(_rect.Height + _extend));

	//_sweeperInfo.width = (int)round(cp.X * 2);
	//_sweeperInfo.height = (int)round(cp.Y * 2);
	_sweeperInfo.width = (int)round((2 * a) + (2 * _extend));
	_sweeperInfo.height = (int)round((2 * b) + (2 * _extend));

	float phi = 0;
	float sweep = (float)(std::get<1>(_sweepAngle)) - std::get<0>(_sweepAngle); // sweep angle

	// TODO: check for direction of sweeper movement as declared by layout file property "rotation"
	if (sweep < 0) {
		//	sweep *= -1;
		_renderDir = cw;
	}
	else
		_renderDir = ccw;

	float range = ((std::get<1>(_range) * (float)_scale) - (std::get<0>(_range) * (float)_scale)); // range
	float valStepAngle = (float)(sweep / range);

	if (_divisions != 0)
		_sweeperInfo.divStep = (float)(range / _divisions); // division step

	float divAngle = valStepAngle * _sweeperInfo.divStep; // angle step for each indicator

	for (int i = 0; i < _divisions; i++) {

		phi = std::get<0>(_sweepAngle) +(i*divAngle);
		if (phi < 0)
			phi += 360.0f;
		if (phi >= 360.0f)
			phi -= 360.0f;

		float _t = tan(RAD(phi)) / (b / a);
		float t = DEG(atan(_t));

		_t = tan(RAD(phi)) / ((b * b) / (a * a));
		float beta = 0;

		if (_t < 0)
			beta = DEG(atan(_t)) * -1;
		else
			beta = 180.0f - DEG(atan(_t));

		float alpha = beta - 90.0f;

		float y, x;
		PointF ep, sp;

		if (phi < 90 || phi > 270.0f) {
			y = cp.Y - (b * sin(RAD(t)));
			x = cp.X + (a * cos(RAD(t)));
			ep = PointF(x, y);
		}
		else {
			y = cp.Y + (b * sin(RAD(t)));
			x = cp.X - (a * cos(RAD(t)));
			ep = PointF(x, y);
		}

		if (phi < 180.0f)
		{
			if (_taper != 0) {
				float l = ((float)i * (float)_taperStep) + _taper;

				x = ep.X + (l * sin(RAD(alpha)));
				y = ep.Y - (l * cos(RAD(alpha)));
				sp = PointF(x, y);
			}
			else {
				x = ep.X + (_extend * sin(RAD(alpha)));
				y = ep.Y - (_extend * cos(RAD(alpha)));
				sp = PointF(x, y);
			}
		}
		else
		{
			if (_taper != 0) {
				float l = ((float)i * (float)_taperStep) + _taper;

				x = ep.X - (l * sin(RAD(alpha)));
				y = ep.Y + (l * cos(RAD(alpha)));
				sp = PointF(x, y);
			}
			else {
				x = ep.X - (_extend * sin(RAD(alpha)));
				y = ep.Y + (_extend * cos(RAD(alpha)));
				sp = PointF(x, y);
			}
		}
		_sweeperInfo.indicators.push_back(RectF(sp.X, sp.Y, ep.X, ep.Y));

	}
}

void CSweeper::calculateArcPath()
{
	//_sweeperInfo = SWEEPER_INFO();
	PointF cp = PointF((float)_rect.Width / 2, (float)_rect.Height / 2);
	_sweeperInfo.width = _rect.Width;
	_sweeperInfo.height = _rect.Height;

	float range = (float)(std::get<1>(_range) -std::get<0>(_range)); // range
	float sa = 0, ea = 0, a = 0;
	float _s = 0, _e = 0;

	float sweep = (float)std::get<1>(_sweepAngle) -(float)std::get<0>(_sweepAngle); // sweep angle

	// TODO: check directon of movement ( CW or CCW )

	float valStepAngle = (float)(sweep / range);

	if (_divisions != 0)
		_sweeperInfo.divStep = (float)(range / _divisions); // division step

	float divAngle = valStepAngle * _sweeperInfo.divStep; // angle step for each indicator

	for (int i = 0; i < _divisions; i++) {

		float a = std::get<0>(_sweepAngle) +(i * divAngle);
		//float a = (sa + 90.0f) + (i * divAngle);

		if (a >= 360.0f)
			a -= 360.0f;

		float x = (cp.X - _extend) * cos(RAD(a));
		float y = (cp.Y - _extend) * sin(RAD(a));
		PointF ep = PointF(cp.X + x, cp.Y - y);

		x = cp.X * cos(RAD(a));
		y = cp.Y * sin(RAD(a));
		PointF sp = PointF(cp.X + x, cp.Y - y);

		// sp = outer radius
		// ep = inner radius
		_sweeperInfo.indicators.push_back(RectF(sp.X, sp.Y, ep.X, ep.Y));
	}
}

void CSweeper::calculateLinePath()
{
	//_sweeperInfo = SWEEPER_INFO();
	//_sweeperInfo.indicators.clear();

	float divStep = 0;
	bool vertical = false;

	// check if thickness is used. It's NOT used for circle and rectangle indicators
	if (_thickness == 0)
		_thickness = _extend; // thickness is equal to extend for circle and rectangle indicators

	int w = abs(std::get<0>(_lineStart) -std::get<0>(_lineEnd));
	int h = abs(std::get<1>(_lineStart) -std::get<1>(_lineEnd));
	if (w == 0) {

		// x-coordinates are equal, so we have vertical line element
		w = _extend;
		h += (_thickness * 2);

		if (_divisions != 0)
			divStep = (float)((float)h / (float)_divisions);

		vertical = true;
	}
	if (h == 0) {

		// y-coordinates are equal, so we have horizontal line element
		h = _extend;
		w += (_thickness * 2);

		if (_divisions != 0)
			divStep = (float)((float)w / (float)_divisions);

	}
	float range = (float)(std::get<1>(_range) -std::get<0>(_range)); // range
	_sweeperInfo.divStep = (float)(range / (float)_divisions);
	_sweeperInfo.width = w;
	_sweeperInfo.height = h;
	_rect = Rect(std::get<0>(_lineStart), std::get<1>(_lineStart), w, h);

	if (_taper != 0)
		_taperStep = (float)(((float)_extend - (float)_taper) / (float)_divisions);

	for (int i = 0; i < _divisions; i++) {

		float x, y;
		PointF sp, ep;

		if (vertical) {

			// calculate start point of indicator line
			x = 0;
			y = h - (i * divStep) - ((float)_thickness / 2);
			sp = PointF(x, y);

			// calculate end point of indicator line
			if (_taper != 0)
				x = (i * _taperStep) + (float)_taper;
			else
				x = (float)_extend;

			y = sp.Y;
			ep = PointF(x, y);

		}
		else {

			x = (i * divStep) + ((float)_thickness / 2);
			y = (float)h;
			sp = PointF(x, y);

			if (_taper != 0)
				y = h - (i * _taperStep) - _taper;
			else
				y = (float)(h - _extend);
			x = sp.X;
			ep = PointF(x, y);
		}

		_sweeperInfo.indicators.push_back(RectF(sp.X, sp.Y, ep.X, ep.Y));
	}
}

void CSweeper::Precalculate()
{
	_sweeperInfo = SWEEPER_INFO();
	_sweeperInfo.indicators.clear();

	if (_taper != 0)
		_taperStep = (float)(((float)_extend - (float)_taper) / (float)_divisions);

	switch (_path) {

	case pathType_sweeper:
		calculateEllipsePath();
		break;

	case pathType_arc:
		calculateArcPath();
		break;

	case pathType_line:
		calculateLinePath();
		break;
	}

}

void CSweeper::renderEllipsePath(Bitmap* bmp, Graphics* gfx, int sampleIndex)
{
	if (bmp != nullptr && gfx != nullptr) {

		//// TODO: FixMe***
		//// check for correct data type!
		//float val = 0;
		//_dataLoggerInst->GetChannelData(_channel, sampleIndex, (void*)&val);

		//size_t activeDivs = (size_t)round(val / _sweeperInfo.divStep); // number of active indicators for current value

		//Pen* pen = new Pen(_rgbOn, _thickness);

		//for (size_t i = 0; i < activeDivs && i < _sweeperInfo.indicators.size(); i++) {

		//	if (_shape == shapeType_line) {

		//		PointF sp = PointF(_sweeperInfo.indicators[i].X, _sweeperInfo.indicators[i].Y);
		//		PointF ep =PointF(_sweeperInfo.indicators[i].Width, _sweeperInfo.indicators[i].Height);

		//		// draw line indicator
		//		gfx->DrawLine(pen, sp, ep);
		//	}
		//}
		//delete pen;
	}
}

void CSweeper::renderArcPath(Bitmap* bmp, Graphics* gfx, int sampleIndex)
{

}

void CSweeper::renderLinePath(Bitmap* bmp, Graphics* gfx, int sampleIndex)
{

}


void CSweeper::SetPathType(string& s)
{
	if (s == string("sweeper"))
		_path = PathType::pathType_sweeper;
	else if (s == string("arc"))
		_path = PathType::pathType_arc;
	else if (s == string("line"))
		_path = PathType::pathType_line;
	else
		_path = PathType::pathType_sweeper;
}

void CSweeper::SetRange(string& s)
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

void CSweeper::SetRectangle(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 4)
		throw parsing_error::missingArgument;

	_rect = Gdiplus::Rect(stoi(toks[0]), stoi(toks[1]), stoi(toks[2]), stoi(toks[3]));

}

void CSweeper::SetLineStart(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_lineStart) = stoi(toks[0]);
	std::get<1>(_lineStart) = stoi(toks[1]);
}

void CSweeper::SetLineEnd(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_lineEnd) = stoi(toks[0]);
	std::get<1>(_lineEnd) = stoi(toks[1]);
}

void CSweeper::SetShapeType(string& s)
{
	if (s == string("line"))
		_shape = shapeType_Sweeper::shapeType_line;
	else if (s == string("rectangle"))
		_shape = shapeType_Sweeper::shapeType_rectangle;
	else if (s == string("ellipse"))
		_shape = shapeType_Sweeper::shapeType_ellipse;
	else
		_shape = shapeType_Sweeper::shapeType_line;
}

void CSweeper::SetColorON(string& s)
{
	if (s == string("transparent")) {

		_rgbOn = Gdiplus::Color(0, 0, 0, 0);
		return;
	}

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

void CSweeper::SetColorOFF(string& s)
{
	if (s == string("transparent")) {

		_rgbOff = Gdiplus::Color(0, 0, 0, 0);
		return;
	}

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

void CSweeper::SetThreshold(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);

	if (ec != parsing_error::ok)
		throw ec;

	_threshold = std::vector<int>();
	for (unsigned int i = 0; i < toks.size(); i++) {
		_threshold.push_back(stoi(toks[i]));
	}
}

void CSweeper::SetThresholdColors(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> tuples;
	std::vector<string> toks;

	splitTuple2(s, tuples);

	for (size_t i = 0; i < tuples.size(); i++) {

		splitTuple(tuples[i], toks, ec);
		if (ec != parsing_error::ok)
			throw ec;

		_rgbThresholdOn.push_back(Gdiplus::Color(stoi(toks[0]), stoi(toks[1]), stoi(toks[2])));
		toks = std::vector<string>();
	}
}

void CSweeper::SetSweepAngle(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_sweepAngle) = stoi(toks[0]); // start
	std::get<1>(_sweepAngle) = stoi(toks[1]); // end
}

Gdiplus::Rect CSweeper::GetRectangle()
{
	if (_path == PathType::pathType_sweeper)
		return Rect(_rect.X - _extend, _rect.Y - _extend, _rect.Width, _rect.Height);
	if (_path == PathType::pathType_arc)
		return _rectangle;

	return _rect;
}
