#include "Ruler.h"


using namespace libLDF;

CRuler::CRuler() :
_path(pathType_arc),
_drawPath(false),
_pathColor(Color::Black),
_pathOffset(0),
_pathWidth(1),
_pathPos(PathPosition_inside),
_drawLabel(true),
_labelMargin(0),
_labelFormat(LabelFormatType_integer),
_drawMajor(false),
_mjDiv(0),
_mjUnits(0),
_mjMargin(0),
_mjColor(Color::Black),
_mjShape(TickShape::line),
_mjLength(0),
_mjWidth(1),
_mjRadius(0),
_drawMinor(false),
_mnDiv(0),
_mnUnits(0),
_mnMargin(0),
_mnColor(Color::Black),
_mnShape(TickShape::line),
_mnLength(0),
_mnWidth(1),
_mnRadius(0),
_fontname(string("Arial")),
_fontheight(10),
_fontstyle(FontStyleRegular),
_foreground(Color::Black),
_layer(0),
_transparency(0),
_arcRadius(0),
_rulerImg(nullptr),
_rulerGfx(nullptr),
_lblFont(nullptr),
textR(0),
_scale(1),
_using(string(""))
{
}


CRuler::~CRuler()
{
	if (_lblFont != nullptr) {

		delete _lblFont;
		_lblFont = nullptr;
	}
	if (_rulerGfx != nullptr) {

		delete _rulerGfx;
		_rulerGfx = nullptr;
	}
	if (_rulerImg != nullptr) {

		delete _rulerImg;
		_rulerImg = nullptr;
	}
}

void CRuler::Init()
{
	if (_rulerGfx != nullptr) {

		delete _rulerGfx;
		_rulerGfx = nullptr;
	}
	if (_rulerImg != nullptr) {

		delete _rulerImg;
		_rulerImg = nullptr;
	}

	WCHAR* fntname;
	wstring fn;

	fn = wstring(_fontname.begin(), _fontname.end());
	fntname = (WCHAR*)fn.c_str();
	_lblFont = new Font(fntname, (REAL)_fontheight, _fontstyle);
	float sweep = 0;

	_range = (int)((std::get<1>(_pathRange)*_scale) - (std::get<0>(_pathRange)*_scale));

	if (_mjDiv != 0) {

		_stepAngle = (float)(_sweep / (float)_mjDiv);
		_valStep = (float)_range / (float)_mjDiv;
		// TODO: throw error message to user
		//if (valStep < 1.0f)
		//	throw - 1;
		_steps = _mjDiv;
	}
	if (_mjUnits != 0) {

		_stepAngle = (float)((_sweep / (float)_range) * (float)(_mjUnits*_scale));
		_valStep = (float)((double)_mjUnits*_scale);
		_steps = (int)round(((float)_range / (float)(_mjUnits*_scale)));
	}

	if (_mnDiv != 0) {

		_subSteps = _mnDiv; // +1;
		_subStepAngle = (float)(_stepAngle / (float)_subSteps);
	}
	if (_mnUnits != 0) {

		_subSteps = (int)round(((float)_valStep / (float)(_mnUnits*_scale)));
		_subStepAngle = (float)(_stepAngle / (float)_subSteps);
	}

	calculatePathBounds();

	// NEW
	//_rulerImg = new Bitmap(pathBounds.Width, pathBounds.Height, PixelFormat32bppARGB);
	_rulerImg = new Bitmap((int)_size.Width, (int)_size.Height, PixelFormat32bppARGB);
	if (_rulerImg != nullptr) {

		_rulerGfx = Graphics::FromImage(_rulerImg);
		if (_rulerGfx != nullptr) {

			sweep = _sweep;
			sweep *= -1;


			switch (_path) {

			case PathType::pathType_arc:

				if (_drawPath)
					_rulerGfx->DrawArc(&Pen(_pathColor, (float)_pathWidth), pathBounds.X + (float)((float)_pathWidth / 2.0f), pathBounds.Y + (float)((float)_pathWidth / 2.0f),
					pathBounds.Width - (float)_pathWidth, pathBounds.Height - (float)_pathWidth,
					(float)_arcStart, sweep);

				calculateArcPath();

				break;

			case PathType::pathType_sweeper:

				if (_drawPath)
					_rulerGfx->DrawArc(&Pen(_pathColor, (float)_pathWidth), pathBounds.X + (float)((float)_pathWidth / 2.0f), pathBounds.Y + (float)((float)_pathWidth / 2.0f),
					pathBounds.Width - (float)_pathWidth, pathBounds.Height - (float)_pathWidth,
					(float)_arcStart, sweep);

				calculateSweeperPath();

				break;

			case PathType::pathType_line:

				if (_drawPath)
					_rulerGfx->DrawLine(&Pen(_pathColor, (float)_pathWidth), pathBounds.X, pathBounds.Y, pathBounds.Width, pathBounds.Height);

				calculateLinePath();

				break;
			}

			if (_drawMajor && _drawMinor) {
				drawMinorTicks();
			}

			if (_drawMajor) {
				drawMajorTicks();
			}

			if (_drawLabel)
				drawTicksLabels();

		}
	}
}

void CRuler::SetBackgroundColor(std::string& s)
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

void CRuler::SetForegroundColor(std::string& s)
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


void CRuler::drawMinorTicks()
{
	if (_rulerImg != nullptr) {

		if (_rulerGfx != nullptr) {

			for (size_t i = 0; i < minorTicksPt.size(); i++) {

				if (_mnShape == TickShape::line) {

					_rulerGfx->DrawLine(&Pen(_mnColor, (REAL)_mnWidth), minorTicksPt[i].X, minorTicksPt[i].Y, minorTicksPt[i].Width, minorTicksPt[i].Height);
				}
				if (_mnShape == TickShape::circle) {

					_rulerGfx->FillEllipse(&SolidBrush(_mnColor), minorTicksPt[i]);
				}
			}
		}
	}
}

void CRuler::drawMajorTicks()
{
	if (_rulerImg != nullptr) {

		if (_rulerGfx != nullptr) {

			for (size_t i = 0; i < majorTicksPt.size(); i++) {

				if (_mjShape == TickShape::line) {

					_rulerGfx->DrawLine(&Pen(_mjColor, (REAL)_mjWidth), majorTicksPt[i].X, majorTicksPt[i].Y, majorTicksPt[i].Width, majorTicksPt[i].Height);
				}
				if (_mjShape == TickShape::circle) {

					_rulerGfx->FillEllipse(&SolidBrush(_mjColor), majorTicksPt[i]);
				}
			}
		}
	}
}

void CRuler::drawTicksLabels()
{
	if (_rulerImg != nullptr) {

		if (_rulerGfx != nullptr) {

			for (size_t i = 0; i < ticksLabels.size(); i++) {

				WCHAR* str = (WCHAR*)ticksLabels[i].label.c_str();

				StringFormat sf;
				sf.SetLineAlignment(StringAlignmentCenter);
				sf.SetAlignment(StringAlignmentCenter);

				_rulerGfx->DrawString(str, (int)wcslen(str), _lblFont, ticksLabels[i].textRect, &sf, &SolidBrush(_foreground));
			}
		}
	}
}

///
/// --> !!! ep = tangent point / sp = normal point
///

PointF CRuler::calculateEllipseTangent(float phi)
{
	/*PointF cp = PointF(_a, _b);*/
	PointF cp = PointF((float)((float)_size.Width / 2.0f), (float)((float)_size.Height / 2.0f));
	float y, x;
	PointF tp;

	float _t = tan(RAD(phi)) / (_b / _a);
	float t = DEG(atan(_t));

	if (phi < 90.0f || phi  > 270.0f) {
		//if (_pathPos == PathPosition_inside) {
		y = cp.Y - (_b * sin(RAD(t)));
		x = cp.X + (_a * cos(RAD(t)));
		//}
		//if (_pathPos == PathPosition_outside) {
		//	//y = cp.Y - ((_b + pathBounds.Y) * sin(RAD(t)));
		//	//x = cp.X + ((_a + pathBounds.X) * cos(RAD(t)));
		//	y = cp.Y - ((_b) * sin(RAD(t)));
		//	x = cp.X + ((_a) * cos(RAD(t)));
		//}
		tp = PointF(x, y);
	}
	else {
		//if (_pathPos == PathPosition_inside) {
		y = cp.Y + (_b * sin(RAD(t)));
		x = cp.X - (_a * cos(RAD(t)));
		//}
		//if (_pathPos == PathPosition_outside) {
		//	y = cp.Y + ((_b - pathBounds.Y) * sin(RAD(t)));
		//	x = cp.X - ((_a - pathBounds.X) * cos(RAD(t)));
		//}
		tp = PointF(x, y);
	}

	return tp;
}

float CRuler::calculateEllipseNormal(float phi)
{
	//float _t = tan(RAD(phi)) / ((_b * _b) / (_a * _a));
	//float bt = DEG(atan(_t));
	//float beta = 0;
	//if (bt < 0)
	//	beta = 180 + bt;
	//else if (phi == 180.0f)
	//	beta = phi;
	//else
	//	beta = bt;
	//float alpha = 90.0f - beta;
	//if (alpha < 0)
	//	alpha += 360.0f;
	//if (alpha > 90)
	//	alpha = 360.0f - alpha;

	float _t = tan(RAD(phi)) / ((_b * _b) / (_a * _a));
	float beta = 0;

	if (_t < 0)
		beta = DEG(atan(_t)) * -1;
	else
		beta = 180.0f - DEG(atan(_t));

	float alpha = beta - 90.0f;

	return alpha;
}

void CRuler::calculateSweeperPath()
{
	majorTicksPt.clear();
	minorTicksPt.clear();
	ticksLabels.clear();

	_a = (float)(_rectangle.Width - _pathOffset); // X-axis of ellipse
	_b = (float)(_rectangle.Height - _pathOffset); // Y-axis of ellipse

	float sweep = _sweep;
	float stepAngle = _stepAngle;
	float subStepAngle = _subStepAngle;
	float valStep = _valStep;
	int steps = _steps;
	int subSteps = _subSteps;

	// calculate ticks marks and labels along elliptic path
	//for (int i = 0, j = (int)(std::get<0>(_pathRange)*_scale); i < (_mjDiv + 1); i++, j += (int)valStep)
	for (int i = 0, j = (int)(std::get<0>(_pathRange)*_scale); i < (steps + 1); i++, j += (int)valStep)
	{
		float phi = (float)_arcStart - (i * stepAngle); // ((float)std::get<0>(_arcSweep) +(i * stepAngle));
		if (phi >= 360.0f)
			phi -= 360.0f;

		if (phi > 0)
			phi = 360.0f - phi;
		else
			phi *= -1;


		// calculate major tick mark
		PointF tp = calculateEllipseTangent(phi);
		calculateSweeperMajorTicks(phi, tp);

		int val = j;

		calculateSweeperLabels(j, phi, tp);

		if (i < _steps) {
			for (int k = 1; k < subSteps; k++)
			{
				phi += subStepAngle;

				tp = calculateEllipseTangent(phi);
				calculateSweeperMinorTicks(phi, tp);
			}
		}
		//}
	}
}

void CRuler::calculateSweeperMinorTicks(float phi, PointF tp)
{
	float a = calculateEllipseNormal(phi);
	float x, y;
	PointF sp, ep;
	RectF pt;

	if (phi < 180.0f)
	{
		if (_pathPos == PathPosition_inside) {

			if (_mnShape == TickShape::line) {

				x = tp.X - (float)(_mnMargin * sin(RAD(a)));
				y = tp.Y + (float)(_mnMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X - (float)((_mnLength + _mnMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mnLength + _mnMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mnShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); // ((_mnMargin + _mnRadius) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * cos(RAD(a)));
				sp = PointF(x, y);

			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mnShape == TickShape::line) {

				x = tp.X + (float)(_mnMargin * sin(RAD(a)));
				y = tp.Y - (float)(_mnMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X + (float)((_mnLength + _mnMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mnLength + _mnMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mnShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * cos(RAD(a)));
				sp = PointF(x, y);

			}
		}
	}
	else {

		if (_pathPos == PathPosition_inside) {

			if (_mnShape == TickShape::line) {

				x = tp.X + (float)(_mnMargin * sin(RAD(a)));
				y = tp.Y - (float)(_mnMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X + (float)((_mnLength + _mnMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mnLength + _mnMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mnShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * cos(RAD(a)));
				sp = PointF(x, y);

			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mnShape == TickShape::line) {

				x = tp.X - (float)(_mnMargin * sin(RAD(a)));
				y = tp.Y + (float)(_mnMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X - (float)((_mnLength + _mnMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mnLength + _mnMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mnShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); //((_mnMargin + _mnRadius) * cos(RAD(a)));
				sp = PointF(x, y);

			}
		}
	}

	if (_mnShape == TickShape::line)
		pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
	else if (_mnShape == TickShape::circle)
		pt = RectF(sp.X, sp.Y, (float)_mnRadius, (float)_mnRadius);

	minorTicksPt.push_back(pt);
}

void CRuler::calculateSweeperMajorTicks(float phi, PointF tp)
{
	float a = calculateEllipseNormal(phi);
	float x = 0, y = 0;
	PointF sp, ep;
	RectF pt;

	if (phi < 180.0f)
	{
		if (_pathPos == PathPosition_inside) {

			if (_mjShape == TickShape::line) {
				x = tp.X - (float)(_mjMargin * sin(RAD(a)));
				y = tp.Y + (float)(_mjMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X - (float)((_mjLength + _mjMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjLength + _mjMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mjShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); // (float)((_mjMargin + ((float)_mjRadius)) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * cos(RAD(a)));
				sp = PointF(x, y);
			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mjShape == TickShape::line) {
				x = tp.X + (float)(_mjMargin * sin(RAD(a)));
				y = tp.Y - (float)(_mjMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X + (float)((_mjLength + _mjMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjLength + _mjMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mjShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * cos(RAD(a)));
				sp = PointF(x, y);
			}
		}
	}
	else
	{
		if (_pathPos == PathPosition_inside) {

			if (_mjShape == TickShape::line) {
				x = tp.X + (float)(_mjMargin * sin(RAD(a)));
				y = tp.Y - (float)(_mjMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X + (float)((_mjLength + _mjMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjLength + _mjMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mjShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * cos(RAD(a)));
				sp = PointF(x, y);
			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mjShape == TickShape::line) {
				x = tp.X - (float)(_mjMargin * sin(RAD(a)));
				y = tp.Y + (float)(_mjMargin * cos(RAD(a)));
				sp = PointF(x, y);

				x = tp.X - (float)((_mjLength + _mjMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjLength + _mjMargin) * cos(RAD(a)));
				ep = PointF(x, y);
			}
			else if (_mjShape == TickShape::circle) {

				x = tp.X - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * sin(RAD(a)));
				y = tp.Y - (float)((float)_mjRadius / 2.0f); // ((_mjMargin + ((float)_mjRadius)) * cos(RAD(a)));
				sp = PointF(x, y);
			}
		}
	}

	if (_mjShape == TickShape::line)
		pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
	else if (_mjShape == TickShape::circle)
		pt = RectF(sp.X, sp.Y, (float)_mjRadius, (float)_mjRadius);

	majorTicksPt.push_back(pt);
}

void CRuler::calculateSweeperLabels(int val, float phi, PointF tp)
{
	float a = calculateEllipseNormal(phi);
	float x = 0, y = 0;

	if (phi < 180.0f)
	{
		if (_pathPos == PathPosition_inside) {

			if (_mjShape == TickShape::line) {

				x = tp.X - (float)((_mjMargin + _mjLength + _labelMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjMargin + _mjLength + _labelMargin) * cos(RAD(a)));
			}
			else if (_mjShape == TickShape::circle){

				x = tp.X - (float)((_mjMargin + _mjRadius + _labelMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjMargin + _mjRadius + _labelMargin) * cos(RAD(a)));

			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mjShape == TickShape::line) {

				x = tp.X + (float)((_mjMargin + _mjLength + _labelMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjMargin + _mjLength + _labelMargin) * cos(RAD(a)));
			}
			else if (_mjShape == TickShape::circle){

				x = tp.X + (float)((_mjMargin + _mjRadius + _labelMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjMargin + _mjRadius + _labelMargin) * cos(RAD(a)));
			}
		}
	}
	else
	{

		if (_pathPos == PathPosition_inside) {

			if (_mjShape == TickShape::line) {

				x = tp.X + (float)((_mjMargin + _mjLength + _labelMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjMargin + _mjLength + _labelMargin) * cos(RAD(a)));
			}
			else if (_mjShape == TickShape::circle){

				x = tp.X + (float)((_mjMargin + _mjRadius + _labelMargin) * sin(RAD(a)));
				y = tp.Y - (float)((_mjMargin + _mjRadius + _labelMargin) * cos(RAD(a)));
			}
		}
		if (_pathPos == PathPosition_outside) {

			if (_mjShape == TickShape::line) {

				x = tp.X - (float)((_mjMargin + _mjLength + _labelMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjMargin + _mjLength + _labelMargin) * cos(RAD(a)));
			}
			else if (_mjShape == TickShape::circle){

				x = tp.X - (float)((_mjMargin + _mjRadius + _labelMargin) * sin(RAD(a)));
				y = tp.Y + (float)((_mjMargin + _mjRadius + _labelMargin) * cos(RAD(a)));
			}
		}
	}
	std::stringstream ss;
	ss << val;

	TICK_MARK_LABEL tml = TICK_MARK_LABEL();
	string str = ss.str();
	tml.label = std::wstring(str.begin(), str.end());
	WCHAR* lbl = (WCHAR*)tml.label.c_str();

	RectF bb;
	if (_rulerGfx != NULL) {

		_rulerGfx->MeasureString(lbl, (int)wcslen(lbl), _lblFont, PointF(0, 0), &bb);
		x = x - (float)(bb.Width / 2.0f);
		y = y - (float)(bb.Height / 2.0f);

		tml.textRect = RectF(x, y, bb.Width, bb.Height);
		ticksLabels.push_back(tml);

#ifdef _DEBUG
		if (_rulerGfx != nullptr) {

			//_rulerGfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), tml.textRect);
		}
#endif
	}
}

void CRuler::calculateLineMinorTicks()
{

}

void CRuler::calculateLineMajorTicks()
{

}

void CRuler::calculateLineLabels(float off, PointF sp, PointF ep, int val)
{
	RectF bb = RectF(0, 0, 0, 0);
	PointF pt = PointF(0, 0);

	wstringstream wss;
	wss << (int)val;
	wstring ws = wss.str();

	getLabelBounds(val, bb);

	if (_lblFont != nullptr) {

		switch (_pathPos) {

		case PathPosition_above:

			pt.X = round(off - (bb.Width / 2.0f));
			pt.Y = round(ep.Y - _labelMargin - bb.Height);

			break;

		case PathPosition_below:

			//pt.X = ep.X - (bb.Width / 2.0f);
			//pt.Y = ep.Y + _labelMargin;
			pt.X = round(off - (bb.Width / 2.0f));
			pt.Y = round(ep.Y + ((float)_mjRadius) + _labelMargin);

			break;

		case PathPosition_left:

			pt.Y = round(off - (bb.Height / 2.0f));
			pt.X = round(ep.X - _labelMargin - bb.Width);

			break;

		case PathPosition_right:

			pt.Y = round(off - (bb.Height / 2.0f));
			pt.X = round(ep.X + _labelMargin);

			break;

		default:
			break;
		}

		TICK_MARK_LABEL tml = TICK_MARK_LABEL();
		tml.label = ws;
		tml.textRect = RectF(pt.X, pt.Y, bb.Width, bb.Height);
		ticksLabels.push_back(tml);
	}
}

//void CRuler::SetBackgroundColor(std::string& s)
//{
//	std::vector<std::string> toks;
//	splitTuple(s, toks);
//	_background = Color((BYTE)stoi(toks[0]), (BYTE)stoi(toks[1]), (BYTE)stoi(toks[2]));
//}

//void CRuler::SetForegroundColor(std::string& s)
//{
//	std::vector<std::string> toks;
//	splitTuple(s, toks);
//	_foreground = Color((BYTE)stoi(toks[0]), (BYTE)stoi(toks[1]), (BYTE)stoi(toks[2]));
//}

void CRuler::SetFontStyle(string& s)
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
}

void CRuler::SetPathType(string& s)
{
	if (s == string("arc"))
		_path = PathType::pathType_arc;
	else if (s == string("line"))
		_path = PathType::pathType_line;
	else if (s == string("sweeper"))
		_path = PathType::pathType_sweeper;
}

void CRuler::SetPathType(PathType type)
{
	_path = type;
}

void CRuler::SetPathColor(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_pathColor = Color((255) | c[0]);
		if (c.size() == 3)
			_pathColor = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CRuler::SetPathPosition(string& s)
{
	if (s == string("inside"))
		_pathPos = PathPositions::PathPosition_inside;
	else if (s == string("outside"))
		_pathPos = PathPositions::PathPosition_outside;
	else if (s == string("above"))
		_pathPos = PathPositions::PathPosition_above;
	else if (s == string("below"))
		_pathPos = PathPositions::PathPosition_below;
	else if (s == string("left"))
		_pathPos = PathPositions::PathPosition_left;
	else if (s == string("right"))
		_pathPos = PathPositions::PathPosition_right;
	else if (s == string("center"))
		_pathPos = PathPositions::PathPosition_center;
}

void CRuler::SetMajorColor(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_mjColor = Color((255) | c[0]);
		if (c.size() == 3)
			_mjColor = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CRuler::SetMajorShape(string& s)
{
	if (s == string("dash") || s == string("line"))
		_mjShape = TickShape::line;
	else if (s == string("circle"))
		_mjShape = TickShape::circle;
}

void CRuler::SetMinorColor(string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_mnColor = Color((255) | c[0]);
		if (c.size() == 3)
			_mnColor = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CRuler::SetMinorShape(string& s)
{
	if (s == string("dash") || s == string("line"))
		_mnShape = TickShape::line;
	else if (s == string("circle"))
		_mnShape = TickShape::circle;
}

Bitmap* CRuler::Render(Bitmap* elementImg, int elementLayer)
{
	Rect arcRect;
	float w, h;
	Bitmap* rulerImg = nullptr;
	Bitmap* bmp = nullptr;
	PointF rulerPos = PointF(0, 0), elementPos = PointF(0, 0);

	if (elementImg != nullptr) {

		w = (float)max(elementImg->GetWidth(), _rulerImg->GetWidth());
		h = (float)max(elementImg->GetHeight(), _rulerImg->GetHeight());

		if (_rulerImg != nullptr) {

			//bmp = new Bitmap(_rulerImg->GetWidth(), _rulerImg->GetHeight(), _rulerImg->GetPixelFormat());
			bmp = new Bitmap((int)round(w), (int)round(h), _rulerImg->GetPixelFormat());
			if (bmp != nullptr) {

				Graphics* gfx = Graphics::FromImage(bmp);
				if (gfx != nullptr) {

					float x, y;
					//if ((_path == pathType_arc) || (_path == pathType_sweeper) ||
					if ((_pathPos == PathPosition_outside) || (_path == pathType_sweeper) ||
						(_pathPos == PathPosition_above) || (_pathPos == PathPosition_below)) {

						x = /*(int)round*/((((float)_rulerImg->GetWidth() - (float)elementImg->GetWidth()) / 2.0f));
					}
					else if (_pathPos == PathPosition_right || _pathPos == PathPosition_inside) {

						x = 0;
					}
					else {

						x = /*(int)round*/(((float)_rulerImg->GetWidth() - (float)elementImg->GetWidth()));
					}
					if (x < 0) {

						x *= -1;
						// ruler image smaller than element
						rulerPos.X = x;
						elementPos.X = 0;
					}
					else {

						rulerPos.X = 0;
						elementPos.X = x;
					}

					//if ((_path == pathType_arc) || (_path == pathType_sweeper) ||
					if ((_pathPos == PathPosition_outside) || (_path == pathType_sweeper) ||
						(_pathPos == PathPosition_left) || (_pathPos == PathPosition_right)) {

						y = /*(int)round*/((((float)_rulerImg->GetHeight() - (float)elementImg->GetHeight()) / 2.0f));
					}
					else if (_pathPos == PathPosition_below || _pathPos == PathPosition_inside) {

						y = 0;
					}
					else {

						y = /*(int)round*/(((float)_rulerImg->GetHeight() - (float)elementImg->GetHeight()));
					}
					if (y < 0) {

						y *= -1;

						// ruler image smaller than element
						rulerPos.Y = y;
						elementPos.Y = 0;
					}
					else {

						// ruler image bigger than element
						rulerPos.Y = 0;
						elementPos.Y = y;
					}

					if (_layer > elementLayer) {

						// draw ruler over element
						gfx->DrawImage(elementImg, elementPos.X, elementPos.Y, (float)elementImg->GetWidth(), (float)elementImg->GetHeight());
						gfx->DrawImage(_rulerImg, rulerPos.X, rulerPos.Y, (float)_rulerImg->GetWidth(), (float)_rulerImg->GetHeight());
					}
					else {

						// ! default layering: first ruler element(scale) and second element image
						// -> so draw element over ruler
						gfx->DrawImage(_rulerImg, rulerPos.X, rulerPos.Y, (float)_rulerImg->GetWidth(), (float)_rulerImg->GetHeight());
						gfx->DrawImage(elementImg, elementPos.X, elementPos.Y, (float)elementImg->GetWidth(), (float)elementImg->GetHeight());
					}

					delete gfx;
				}
			}
		}
	}

	return bmp;
}


void CRuler::calculatePathBounds()
{
	int w = 0, h = 0;
	int off = 0;
	int	o = 0;
	float x = 0, y = 0;
	RectF bb;

	if (_path == PathType::pathType_sweeper) {

		// this is the ellipses bounding box as declared by the sweepers rectangle property
		// it defines the upper left quadrant only
		w = (int)round(_rectangle.Width * 2);
		h = (int)round(_rectangle.Height * 2);

	}
	else {

		w = (int)round(_rectangle.Width);
		h = (int)round(_rectangle.Height);
	}

	getLabelBounds((int)(std::get<0>(_pathRange)*_scale), (int)(std::get<1>(_pathRange)*_scale), bb);
	off = max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + max(_mjRadius, _mnRadius); // +(int)round(max(bb.Width / 2.0f, bb.Height / 2.0f));

	// general calculations needed to adjust the image size (if required)
	////////////////////////////////////////////////////////////////////////////////////
	switch (_pathPos) {

	case PathPositions::PathPosition_inside:

		// NEW 
		// calculate path bounds relative to rulers image, so the image can be positioned at the exact same location
		// like the element it belongs to
		if (_pathOffset > 0) {

			w -= (2 * _pathOffset) - max(_mjRadius, _mnRadius) * 2;
			h -= (2 * _pathOffset) - max(_mjRadius, _mnRadius) * 2;
		}
		else {
			// if negative path offset is specified INFLATE the rulers image accordingly
			w += abs(2 * _pathOffset) + max(_mjRadius, _mnRadius) * 2;
			h += abs(2 * _pathOffset) + max(_mjRadius, _mnRadius) * 2;
		}

		pathBounds = RectF((float)(max(_mjRadius, _mnRadius)), (float)(max(_mjRadius, _mnRadius)),
			(float)w - (max(_mjRadius, _mnRadius) * 2), (float)h - (max(_mjRadius, _mnRadius) * 2));

		//if (_path == pathType_sweeper)
		_pos = Point(_rectangle.X + _pathOffset - _mjRadius, _rectangle.Y + _pathOffset - _mjRadius);
		//else if (_path == pathType_arc)
		//	_pos = Point(_rectangle.X + ((_rectangle.Width - w) / 2), _rectangle.Y + ((_rectangle.Height - h) / 2));

		break;

	case PathPositions::PathPosition_outside:

		if (_pathOffset >= 0) {

			w -= (2 * _pathOffset);
			h -= (2 * _pathOffset);
		}
		else {

			w += abs(2 * _pathOffset);
			h += abs(2 * _pathOffset);
		}

		o = (int)round(max(bb.Width, bb.Height));

		w += abs(2 * off) + (int)round(o * 2);
		h += abs(2 * off) + (int)round(o * 2);
		pathBounds = RectF((float)off + o, (float)off + o, (float)w - (2 * off) - (o * 2), (float)h - (float)(2 * off) - (o * 2));
		_pos = Point(_rectangle.X - off - o + _pathOffset, _rectangle.Y - off - o + _pathOffset);

		break;

	case PathPosition_above:

		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + max(_mjRadius, _mnRadius) + _labelMargin + bb.Height);
		o = off;

		h = o;
		w += (int)round(bb.Width) * 2;

		x = (float)(w - _rectangle.Width) / 2;
		pathBounds = RectF(x, h - (max(_mjRadius, _mnRadius) / 2.0f), w - x, h - (max(_mjRadius, _mnRadius) / 2.0f));
		_pos = Point(_rectangle.X - (int)round(x), _rectangle.Y - h - _pathOffset + (max(_mjRadius, _mnRadius) / 2));

		break;

	case PathPosition_below:

		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + max(_mjRadius, _mnRadius) + _labelMargin + bb.Height);
		o = off;

		h = o;
		w += (int)round(bb.Width) * 2;

		x = (float)(w - _rectangle.Width) / 2;
		pathBounds = RectF(x, max(_mjRadius, _mnRadius) / 2.0f, w - x, max(_mjRadius, _mnRadius) / 2.0f);
		_pos = Point(_rectangle.X - (int)round(x), _rectangle.Y + _rectangle.Height + _pathOffset - (max(_mjRadius, _mnRadius) / 2));

		break;

	case PathPosition_left:

		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + max(_mjRadius, _mnRadius) + _labelMargin + bb.Width + 5);
		o = off;

		h += (int)round(bb.Height) * 2;
		w = o;

		y = (float)(h - _rectangle.Height) / 2;
		pathBounds = RectF((float)w, (float)y, (float)w, (float)_rectangle.Height + y);
		_pos = Point(_rectangle.X - w - _pathOffset, _rectangle.Y - (int)round(y));

		break;

	case PathPosition_right:

		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + max(_mjRadius, _mnRadius) + _labelMargin + bb.Width + 5);
		o = off;

		h += (int)round(bb.Height) * 2;
		w = o;

		y = (float)(h - _rectangle.Height) / 2;
		pathBounds = RectF(0, y, 0, _rectangle.Height + y);
		_pos = Point(_rectangle.X + _rectangle.Width + _pathOffset, _rectangle.Y - (int)round(y));

		break;

	default:

		break;
	}

	_size = Size(w, h);
	pathCenter = PointF((float)w / 2.0f, (float)h / 2.0f);


}
//void CRuler::calculatePathBounds()
//{
//	int w = 0, h = 0;
//	int off = 0; // (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + bb.Height);
//	int	o = 0; // _pathOffset + off;
//	float x = 0, y = 0;
//	RectF bb;
//
//	if (_path == PathType::pathType_sweeper) {
//
//		// this is the ellipses bounding box as declared by the sweepers rectangle property
//		// it defines the upper left quadrant only
//		w = (int)round(_rectangle.Width * 2);
//		h = (int)round(_rectangle.Height * 2);
//
//	}
//	else {
//
//		w = (int)round(_rectangle.Width);
//		h = (int)round(_rectangle.Height);
//	}
//
//	getLabelBounds(std::get<0>(_pathRange), std::get<1>(_pathRange), bb);
//
//	// general calculations needed to adjust the image size (if required)
//	////////////////////////////////////////////////////////////////////////////////////
//	switch (_pathPos) {
//
//	case PathPositions::PathPosition_inside:
//
//		if (_mjRadius != 0) {
//			if (_path == pathType_arc) {
//				off = _mjRadius; // max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + (int)round(max(bb.Width / 2.5f, bb.Height / 2.5f));
//			}
//			if (_path == pathType_sweeper) {
//				off = 0; // max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + (int)round(min(bb.Width / 2.0f, bb.Height / 2.0f));
//			}
//			//off = _mjRadius;
//			o = _pathOffset + off;
//
//			w = w + (2 * o);
//			h = h + (2 * o);
//			_rectangle = Rect(_rect.X, _rect.Y, w, h);
//
//			pathBounds = RectF((float)off, (float)off, (float)(w - (off * 2)), (float)(h - (off * 2)));
//			pathCenter = PointF((float)((float)w / 2.0f), (float)((float)h / 2.0f));
//		}
//		else {
//			// NEW 
//			// calculate path bounds relative to rulers image, so the image can be positioned at the exact same location
//			// like the element it belongs to
//			if (_pathOffset > 0) {
//				pathBounds = RectF(_pathOffset, _pathOffset, ((REAL)w) - _pathOffset, ((REAL)h) - _pathOffset);
//			}
//			else {
//				// if negative path offset is specified INFLATE the rulers image accordingly
//
//			}
//			_rectangle = Rect(0, 0, (int)round(pathBounds.Width), (int)round(pathBounds.Height));
//
//			pathCenter = PointF(pathBounds.Width / 2.0f, pathBounds.Height / 2.0f);
//		}
//
//		break;
//
//	case PathPositions::PathPosition_outside:
//
//		if (_path == pathType_arc) {
//			off = max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + (int)round(max(bb.Width / 2.5f, bb.Height / 2.5f));
//		}
//		if (_path == pathType_sweeper) {
//			off = max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + (int)round(max(bb.Width / 2.0f, bb.Height / 2.0f));
//		}
//		o = _pathOffset + off;
//
//		w = w + (2 * o);
//		h = h + (2 * o);
//		_rect = RectF(_rect.X, _rect.Y, (float)w, (float)h);
//
//		pathBounds = RectF((float)off, (float)off, (float)(w - (off * 2)), (float)(h - (off * 2)));
//		pathCenter = PointF((float)((float)w / 2.0f), (float)((float)h / 2.0f));
//
//		break;
//
//	case PathPosition_above:
//
//		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + bb.Height);
//		o = _pathOffset + off;
//
//		h += o;
//		w += (int)round(bb.Width);
//
//		x = ((w - (int)_elementRect.Width) / 2.0f);
//		pathBounds = RectF(x,
//			(float)off - (float)((float)_pathWidth / 2.0f),
//			_elementRect.Width + x,
//			(float)off - (float)((float)_pathWidth / 2.0f));
//		_rect = RectF(_rect.X, _rect.Y, (float)w, (float)h);
//		pathCenter = PointF((float)((float)w / 2.0f), (float)off);
//
//		break;
//
//	case PathPosition_below:
//
//		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + bb.Height);
//		o = _pathOffset + off;
//
//		h += o;
//		w += (int)round(bb.Width);
//
//		x = ((w - _elementRect.Width) / 2.0f);
//		pathBounds = RectF(x,
//			(_elementRect.Height + _pathOffset) + (float)((float)_pathWidth / 2.0f),
//			_elementRect.Width + x,
//			(_elementRect.Height + _pathOffset) + (float)((float)_pathWidth / 2.0f));
//		_rect = RectF(_rect.X, _rect.Y, (float)w, (float)h);
//		pathCenter = PointF((float)((float)w / 2.0f), (float)off);
//
//		break;
//
//	case PathPosition_left:
//
//		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + bb.Width);
//		o = _pathOffset + off;
//
//		w += o;
//		h += (int)round(bb.Height);
//
//		y = ((h - _elementRect.Height) / 2.0f);
//		pathBounds = RectF(off - (float)((float)_pathWidth / 2.0f),
//			y, off - (float)((float)_pathWidth / 2.0f),
//			_elementRect.Height + y);
//		_rect = RectF(_rect.X, _rect.Y, (float)w, (float)h);
//		pathCenter = PointF((float)((float)w / 2.0f), (float)off);
//
//		break;
//
//	case PathPosition_right:
//
//		off = (int)round(max((_mjLength + _mjMargin), (_mnLength + _mnMargin)) + _labelMargin + bb.Width);
//		o = _pathOffset + off;
//
//		w += o;
//		h += (int)round(bb.Height);
//
//		y = ((h - _elementRect.Height) / 2.0f);
//		pathBounds = RectF((_elementRect.Width + _pathOffset) + (float)((float)_pathWidth / 2.0f),
//			y, (_elementRect.Width + _pathOffset) + (float)((float)_pathWidth / 2.0f),
//			_elementRect.Height + y);
//		_rect = RectF(_rect.X, _rect.Y, (float)w, (float)h);
//		pathCenter = PointF((float)((float)w / 2.0f), (float)off);
//
//		break;
//
//	default:
//
//		pathBounds = RectF(0, 0, (float)w, (float)h);
//		_rect = RectF(0, 0, pathBounds.Width, pathBounds.Height);
//
//		pathCenter = PointF((float)((float)w / 2.0f), (float)((float)h / 2.0f));
//
//		break;
//	}
//}

void CRuler::calculateLinePath()
{

	majorTicksPt.clear();
	minorTicksPt.clear();
	ticksLabels.clear();

	int valRange = abs(std::get<0>(_pathRange)) + abs(std::get<1>(_pathRange));
	float stepWidth = 0;
	float valStep = 0;
	float steps = 0;
	float subSteps = 0;
	float subStepWidth = 0;

	float w = 0, h = 0;

	if (_orientation == SliderOrientation::horizontal) {

		calculateHorizontalLinePath();
	}
	else if (_orientation == SliderOrientation::vertical) {

		calculateVerticalLinePath();
	}
	else {
		// if element is not a slider 

		if (_path == PathType::pathType_line) {

			// check line orientation
			if (_rectangle.Width > _rectangle.Height)
				calculateHorizontalLinePath();
			else
				calculateVerticalLinePath();
		}
	}
}


void CRuler::calculateHorizontalLinePath()
{
	int valRange = 0; // abs(std::get<0>(_pathRange)) + abs(std::get<1>(_pathRange));

	//
	int minVal = std::get<0>(_pathRange);
	int maxVal = std::get<1>(_pathRange);

	if (minVal < 0 || maxVal < 0)
		valRange = abs(std::get<0>(_pathRange)) + abs(std::get<1>(_pathRange));
	else
		valRange = abs(std::get<1>(_pathRange)) - abs(std::get<0>(_pathRange));

	float stepWidth = 0;
	float valStep = 0;
	int steps = 0;
	int subSteps = 0;
	float subStepWidth = 0;

	float w;
	float x, y;
	float off;
	PointF sp, ep;

	w = (float)_rectangle.Width;

	if (_mjDiv != 0) {

		stepWidth = (float)((float)w / (float)_mjDiv);
		valStep = (float)((float)valRange / (float)_mjDiv);
		steps = _mjDiv;
	}
	if (_mjUnits != 0) {

		stepWidth = (float)((w / (float)valRange) * (float)_mjUnits);
		valStep = _mjUnits;
		steps = (int)round(((float)valRange / (float)_mjUnits));
	}
	if (_mnDiv != 0) {

		subSteps = _mnDiv;
		subStepWidth = (float)(float)stepWidth / (float)subSteps;
	}
	if (_mnUnits != 0) {

		subSteps = (int)round(valStep / (float)_mnUnits);
		subStepWidth = (float)(float)stepWidth / (float)subSteps;
	}

	int startVal = std::get<0>(_pathRange);

	for (int i = 0, j = startVal; i < (steps + 1); i++/*, j += valStep*/) {

		off = (_size.Width - _rectangle.Width) / 2.0f;
		x = off;// -(_mjWidth / 2.0f);

		sp = PointF((i*stepWidth) + x - (_mjRadius / 2.0f), pathBounds.Y);

		if (_pathPos == PathPosition_above)
		{

			if (_mjShape == TickShape::line)
				y = sp.Y - _mjLength - _mjMargin;
			else
				y = sp.Y - (float)((float)_mjRadius / 2.0f);

			ep = PointF(sp.X, y);

		}
		else if (_pathPos == PathPosition_below) {

			if (_mjShape == TickShape::line)
				y = sp.Y + _mjLength + _mjMargin;
			else
				y = sp.Y - (float)((float)_mjRadius / 2.0f);

			ep = PointF(sp.X, y);
		}

		RectF pt;
		if (_mjShape == TickShape::line)
			pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
		else
			pt = RectF(ep.X, ep.Y, (float)_mjRadius, (float)_mjRadius);

		majorTicksPt.push_back(pt);

		calculateLineLabels((i*stepWidth) + off, sp, ep, j);

		for (int k = 1; k < subSteps; k++) {

			sp = PointF((i*stepWidth) + x - (_mnRadius / 2.0f), pathBounds.Y);
			sp.X += subStepWidth;

			if (_pathPos == PathPosition_above)
			{
				if (_mnShape == TickShape::line)
					y = sp.Y - _mnLength - _mnMargin;
				else
					y = sp.Y - (float)((float)_mnRadius / 2.0f);

				ep = PointF(sp.X, y);

			}
			else if (_pathPos == PathPosition_below) {

				if (_mnShape == TickShape::line)
					y = sp.Y + _mnLength + _mnMargin;
				else
					y = sp.Y - (float)((float)_mnRadius / 2.0f);

				ep = PointF(sp.X, y);
			}

			if (i < steps) {
				if (_mnShape == TickShape::line)
					pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
				else
					pt = RectF(ep.X, ep.Y, (float)_mnRadius, (float)_mnRadius);

				minorTicksPt.push_back(pt);
			}
		}

		if (startVal == 0) {

			if (std::get<1>(_pathRange) < 0)
				j -= (int)round(valStep);
			else
				j += (int)round(valStep);
		}
		else if (startVal > 0)
			j += (int)round(valStep);
		else if (startVal < 0)
			j += (int)round(valStep);
		else
			j -= (int)round(valStep);

	}
}

void CRuler::calculateVerticalLinePath()
{
	int valRange = abs(std::get<0>(_pathRange)) + abs(std::get<1>(_pathRange));

	float stepWidth = 0;
	float valStep = 0;
	int steps = 0;
	int subSteps = 0;
	float subStepWidth = 0;

	float h;
	float x, y;
	float off;
	PointF sp, ep;

	h = (float)_rectangle.Height;

	if (_mjDiv != 0) {

		stepWidth = (float)((float)h / (float)_mjDiv);
		valStep = (float)((float)valRange / (float)_mjDiv);
		steps = _mjDiv;
	}
	if (_mjUnits != 0) {

		stepWidth = (float)((h / (float)valRange) * (float)_mjUnits);
		valStep = _mjUnits;
		steps = (int)round(((float)valRange / (float)_mjUnits));
	}
	if (_mnDiv != 0) {

		subSteps = _mnDiv;
		subStepWidth = (float)(float)stepWidth / (float)subSteps;
	}
	if (_mnUnits != 0) {

		subSteps = (int)round(valStep / (float)_mnUnits);
		subStepWidth = (float)(float)stepWidth / (float)subSteps;
	}

	int startVal = std::get<1>(_pathRange);

	for (int i = 0, j = startVal; i < (steps + 1); i++/*, j += valStep*/) {

		off = (_size.Height - _rectangle.Height) / 2.0f;
		y = off; // -(_mjWidth / 2.0f);

		sp = PointF(pathBounds.X, (i*stepWidth) + y);

		if (_pathPos == PathPosition_left)
		{

			x = sp.X - _mjLength - _mjMargin;
			ep = PointF(x, sp.Y);

		}
		else if (_pathPos == PathPosition_right) {

			x = sp.X + _mjLength + _mjMargin;
			ep = PointF(x, sp.Y);
		}

		RectF pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
		majorTicksPt.push_back(pt);

		calculateLineLabels((i*stepWidth) + off, sp, ep, j);

		for (int k = 1; k < subSteps; k++) {

			if (_pathPos == PathPosition_left) {

				sp.Y += subStepWidth;
				x = sp.X - _mnLength - _mnMargin;
				ep = PointF(x, sp.Y);
			}
			else if (_pathPos == PathPosition_right) {

				sp.Y += subStepWidth;
				x = sp.X + _mnLength + _mnMargin;
				ep = PointF(x, sp.Y);
			}

			if (i < steps) {
				pt = RectF(sp.X, sp.Y, ep.X, ep.Y);
				minorTicksPt.push_back(pt);
			}
		}

		if (startVal == 0) {

			if (std::get<0>(_pathRange) < 0)
				j -= (int)round(valStep);
			else
				j += (int)round(valStep);
		}
		else if (startVal > 0)
			j -= (int)round(valStep);
		else if (startVal < 0)
			j += (int)round(valStep);
		else
			j -= (int)round(valStep);
	}
}

void CRuler::calculateArcPath()
{
	majorTicksPt.clear();
	minorTicksPt.clear();
	ticksLabels.clear();

	float stepAngle = _stepAngle;
	float subStepAngle = _subStepAngle;
	float valStep = _valStep;
	int steps = _steps, subSteps = _subSteps;
	int range = _range;

	for (int i = 0, j = (int)(std::get<0>(_pathRange)*_scale); i < (steps + 1); i++, j += (int)valStep)
	{
		float a = std::get<0>(_arcSweep) +(i * stepAngle);
		if (a >= 360.0f)
			a -= 360.0f;

		calculateArcMajorTicks(a);
		calculateArcLabels(a, j);

		if (i < steps)
		{
			for (int k = 1; k < subSteps; k++)
			{
				if (std::get<1>(_arcSweep) > 0) {
					if (a > std::get<1>(_arcSweep))
						break;
				}
				if (std::get<1>(_arcSweep) < 0) {
					if (a < std::get<1>(_arcSweep))
						break;
				}
				a += subStepAngle;

				calculateArcMinorTicks(a);
			}
		}
	}
}


void CRuler::calculateArcMajorTicks(float phi)
{
	float arcRadius = round(pathCenter.X - pathBounds.X /*- (_pathWidth)*/);
	PointF sp, ep;
	float x = 0, y = 0;

	if (_mjShape == TickShape::line) {


		// end point of tick mark line
		if (_pathPos == PathPosition_outside) {

			// start points of tick mark line
			x = arcRadius * cos(RAD(phi));
			y = arcRadius * sin(RAD(phi));
			sp = PointF(pathCenter.X + x, pathCenter.Y - y);

			x = (arcRadius + _mjLength + _mjMargin) * cos(RAD(phi));
			y = (arcRadius + _mjLength + _mjMargin) * sin(RAD(phi));
			ep = PointF(pathCenter.X + x, pathCenter.Y - y);

		}
		else if (_pathPos == PathPosition_inside) {

			// start points of tick mark line
			x = (arcRadius - _pathWidth) * cos(RAD(phi));
			y = (arcRadius - _pathWidth) * sin(RAD(phi));
			sp = PointF(pathCenter.X + x, pathCenter.Y - y);

			x = (arcRadius - _mjLength - _mjMargin - _pathWidth) * cos(RAD(phi));
			y = (arcRadius - _mjLength - _mjMargin - _pathWidth) * sin(RAD(phi));
			ep = PointF(pathCenter.X + x, pathCenter.Y - y);

		}

		majorTicksPt.push_back(RectF(sp.X, sp.Y, ep.X, ep.Y));
	}
	else if (_mjShape == TickShape::circle) {

		if (_pathPos == PathPosition_inside) {
			// calculate tick mark "dots"
			x = (arcRadius - _mjMargin) * cos(RAD(phi));
			y = (arcRadius - _mjMargin) * sin(RAD(phi));
		}
		else if (_pathPos == PathPosition_outside) {
			// calculate tick mark "dots"
			x = (arcRadius + _mjMargin) * cos(RAD(phi));
			y = (arcRadius + _mjMargin) * sin(RAD(phi));
		}
		RectF c = RectF((pathCenter.X + x) - (float)_mjRadius, (pathCenter.Y - y) - (float)_mjRadius, (float)(_mjRadius * 2), (float)(_mjRadius * 2));

		majorTicksPt.push_back(c);
	}
}

void CRuler::calculateArcLabels(float phi, int val)
{

	// convert label value to wide char string
	// TODO: fix conversion to wide string
	////////////////////////////////////////////////////////////
	stringstream ss;
	ss << val;
	string str = ss.str();
	wstring ws = wstring(str.begin(), str.end());
	WCHAR* lbl = (WCHAR*)ws.c_str();

	float arcRadius = round(pathCenter.X - pathBounds.X - (_pathWidth / 2));

	// bounding box of label string
	RectF lblBounds;

	if (_lblFont != nullptr) {

		// radius of label text bounding box center
		_rulerGfx->MeasureString(lbl, (int)wcslen(lbl), _lblFont, PointF(0, 0), &lblBounds);
		float textR = 0;

		if (_pathPos == PathPosition_outside) {

			if (_mjShape == TickShape::line) {
				textR = arcRadius + _mjLength + _labelMargin;
				textR += lblBounds.Width / 2;
			}
			else {
				textR = arcRadius + _mjMargin + _labelMargin;
				textR += lblBounds.Width / 2;
			}
		}
		else if (_pathPos == PathPosition_inside) {

			if (_mjShape == TickShape::line) {
				textR = arcRadius - _mjLength - _labelMargin;
				textR -= lblBounds.Width / 2;
			}
			else {
				textR = arcRadius - _mjMargin - _labelMargin;
				textR -= lblBounds.Width / 2;
			}
		}

		float x = textR * cos(RAD(phi));
		float y = textR * sin(RAD(phi));
		PointF pt = PointF((pathCenter.X + x) - (lblBounds.Width / 2), (pathCenter.Y - y) - (lblBounds.Height / 2));

		TICK_MARK_LABEL tml = TICK_MARK_LABEL();
		tml.label = ws;
		tml.textRect = RectF(pt.X, pt.Y, lblBounds.Width, lblBounds.Height);

		ticksLabels.push_back(tml);
	}
}

void CRuler::calculateArcMinorTicks(float phi)
{
	float arcRadius = round(pathCenter.X - pathBounds.X /*- (_pathWidth)*/);
	float x = 0, y = 0;
	PointF sp, ep;

	if (_mnShape == TickShape::line) {

		// end point of tick mark line
		if (_pathPos == PathPosition_outside) {

			x = arcRadius  * cos(RAD(phi));
			y = arcRadius  * sin(RAD(phi));
			sp = PointF(pathCenter.X + x, pathCenter.Y - y);

			x = (arcRadius + _mnLength + _mnMargin) * cos(RAD(phi));
			y = (arcRadius + _mnLength + _mnMargin) * sin(RAD(phi));
			ep = PointF(pathCenter.X + x, pathCenter.Y - y);
		}
		else if (_pathPos == PathPosition_inside) {

			// start points of tick mark line
			x = (arcRadius - _pathWidth) * cos(RAD(phi));
			y = (arcRadius - _pathWidth) * sin(RAD(phi));
			sp = PointF(pathCenter.X + x, pathCenter.Y - y);

			x = (arcRadius - _mnLength - _mnMargin - _pathWidth) * cos(RAD(phi));
			y = (arcRadius - _mnLength - _mnMargin - _pathWidth) * sin(RAD(phi));
			ep = PointF(pathCenter.X + x, pathCenter.Y - y);
		}

		minorTicksPt.push_back(RectF(sp.X, sp.Y, ep.X, ep.Y));

	}
	else if (_mnShape == TickShape::circle) {

		// calculate tick mark "dots"
		if (_pathPos == PathPosition_inside) {
			x = (arcRadius - _mjMargin) * cos(RAD(phi));
			y = (arcRadius - _mjMargin) * sin(RAD(phi));
		}
		else if (_pathPos == PathPosition_outside) {
			x = (arcRadius + _mjMargin) * cos(RAD(phi));
			y = (arcRadius + _mjMargin) * sin(RAD(phi));
		}

		RectF c = RectF((pathCenter.X + x) - (float)_mnRadius, (pathCenter.Y - y) - (float)_mnRadius, (float)(_mnRadius * 2), (float)(_mnRadius * 2));

		minorTicksPt.push_back(c);

	}
}

void CRuler::SetPathRange(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_pathRange) = stoi(toks[0]);
	std::get<1>(_pathRange) = stoi(toks[1]);
}

void CRuler::SetPathRange(int min, int max)
{
	std::get<0>(_pathRange) = min;
	std::get<1>(_pathRange) = max;
}

void CRuler::SetLine(Point start, Point end)
{
	_lineStart = start;
	_lineEnd = end;
}

void CRuler::SetLineOrientation(SliderOrientation o)
{
	_orientation = o;
}

void CRuler::SetArc(int start, int end)
{
	/*_arcStart = start;
	_arcEnd = end;*/

	SetArcSweep(start, end);
}

void CRuler::SetLineStart(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	_lineStart = Point(stoi(toks[0]), stoi(toks[1]));
}

void CRuler::SetLineEnd(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	_lineEnd = Point(stoi(toks[0]), stoi(toks[1]));
}

void CRuler::SetArcSweep(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	SetArcSweep(stoi(toks[0]), stoi(toks[1]));
}

void CRuler::SetArcSweep(int start, int end)
{
	std::get<0>(_arcSweep) = start;
	std::get<1>(_arcSweep) = end;
	_sweep = (float)(end - start);

	if (_sweep >= 360.0f)
		_sweep -= 360.0f;
	if (_sweep <= -360.0f)
		_sweep += 360.0f;

	if (start < 0)
		_arcStart = start * -1;
	else
		_arcStart = 360 - start;

	if (end < 0)
		_arcEnd = end * -1;
	else
		_arcEnd = 360 - end;

	//_arcStart = start;
	//_arcEnd = end;
}

void CRuler::getLabelBounds(int minVal, int maxVal, RectF& bb)
{
	float bbWidth, bbHeight;
	RectF bbMinVal, bbMaxVal;

	if (_lblFont != nullptr) {

		wstringstream ss;
		ss << minVal;
		wstring s = ss.str();
		WCHAR* minValStr = (WCHAR*)s.c_str();
		ss = wstringstream();
		ss << maxVal;
		s = ss.str();
		WCHAR* maxValStr = (WCHAR*)s.c_str();

		Graphics* gfx = new Graphics((HWND)NULL);
		if (gfx != NULL) {

			RectF layoutRect = RectF(0, 0, 1000, 1000);
			CharacterRange chr[] = { CharacterRange(0, (int)wcslen(minValStr)),
				CharacterRange(0, (int)wcslen(maxValStr)) };
			StringFormat strFormat;
			Region* chRangeRegion = nullptr;
			int cnt = 0;

			strFormat.SetMeasurableCharacterRanges(2, chr);
			cnt = strFormat.GetMeasurableCharacterRangeCount();
			chRangeRegion = new Region[cnt];

			gfx->MeasureCharacterRanges(minValStr, -1, _lblFont, layoutRect, &strFormat, cnt, chRangeRegion);
			chRangeRegion[0].GetBounds(&bbMinVal, gfx);
			gfx->MeasureCharacterRanges(maxValStr, -1, _lblFont, layoutRect, &strFormat, cnt, chRangeRegion);
			chRangeRegion[0].GetBounds(&bbMaxVal, gfx);

			delete[] chRangeRegion;
			chRangeRegion = nullptr;
			delete gfx;
			gfx = nullptr;
		}
	}

	bbWidth = max(bbMinVal.Width, bbMaxVal.Width);
	bbHeight = max(bbMinVal.Height, bbMaxVal.Height);
	bb = RectF(0, 0, bbWidth, bbHeight);
}

void CRuler::getLabelBounds(int val, RectF& bb)
{
	Graphics* gfx = Graphics::FromHWND((HWND)NULL);
	if (gfx != NULL) {

		wstringstream wss;
		wss << (int)val;
		wstring ws = wss.str();

		WCHAR* wc = (WCHAR*)ws.c_str();

		gfx->MeasureString(wc, (int)ws.length(), _lblFont, PointF(0, 0), &bb);

		delete gfx;
		gfx = nullptr;

	}
}

Bitmap* CRuler::Render(int sampleIndex)
{

	return _rulerImg;
}
