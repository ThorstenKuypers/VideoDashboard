#include "Gauge.h"


using namespace libLDF;


CGauge::CGauge() : CDashboardElement(),
_range(std::tuple<int, int>(0, 0)),
_pos(tuple<int, int>(-1, -1)),
_sweepAngle(tuple<float, float>(0.0f, 0.0f)), // (135.0f, 270.0f);
_divisions(0),
_radius(0),
_imgFile(std::string("")),
_needleImgFile(string("")),
_needleCenter(tuple<int, int>(-1, -1)),
_needleOffset(tuple<int, int>(0, 0)),
_useImg(true),
_precision(0),
_margin(5),
_imgSize(Size(-1, -1)),
_renderStartAngle(0),
_bgImg(nullptr),
_needleImg(nullptr)
{
	type = DashboardElementType::gauge;
}


CGauge::~CGauge()
{
	if (_bgImg != nullptr) {

		delete _bgImg;
		_bgImg = nullptr;
	}

	if (_needleImg != nullptr) {

		delete _needleImg;
		_needleImg = nullptr;
	}
}

//
// void Init() - virtual
///////////////////////////////////////////////////////////////////////////////
void CGauge::Init()
{
	if (_bgImg != nullptr) {

		delete _bgImg;
		_bgImg = nullptr;
	}

	if (_needleImg != nullptr) {

		delete _needleImg;
		_needleImg = nullptr;
	}

	if (_useImg) { // pre-cache the gauge face from specified image file
		if (!_imgFile.empty()) {

			wstring ws = wstring(_imgFile.begin(), _imgFile.end());
			WCHAR* wc = (WCHAR*)ws.c_str();
			_bgImg = new Bitmap(wc);

			if (_bgImg != NULL) {

				if (_rectangle.Width == -1 && _rectangle.Height == -1) {

					_rectangle.X = std::get<0>(_pos);
					_rectangle.Y = std::get<1>(_pos);
					_rectangle.Width = _imgSize.Width = _bgImg->GetWidth();
					_rectangle.Height = _imgSize.Height = _bgImg->GetHeight();
				}
				else {

					//_imgSize.Width = _rectangle.Width;
					//_imgSize.Height = _rectangle.Height;

					_imgSize.Width = _bgImg->GetWidth();
					_imgSize.Height = _bgImg->GetHeight();

				}
			}
		}
		else
			_useImg = false;
	}
	if (!_useImg) {

		// make sure gauge has aspect ratio of 1 for first rendering
		// the actual width and height is set when the element is
		// drawn into the dashboard image. This way it's easier to
		// draw disorted element images.
		_imgSize.Width = _imgSize.Height = max(_rectangle.Width, _rectangle.Height);
	}

	if (!_needleImgFile.empty())  {

		wstring ws = wstring(_needleImgFile.begin(), _needleImgFile.end());
		WCHAR* wc = (WCHAR*)ws.c_str();
		_needleImg = new Bitmap(wc);
	}
}

//
// Bitmap* Render()
///////////////////////////////////////////////////////////////////////////////
Gdiplus::Bitmap* CGauge::Render(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank)
{
	float maxVal = 0, minVal = 0;

	// the "final" element image
	Bitmap* bmp = new Bitmap(_imgSize.Width, _imgSize.Height, PixelFormat32bppARGB);
	if (bmp != nullptr) {

		Graphics* gfx = Graphics::FromImage(bmp);
		if (gfx != nullptr) {
			gfx->SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
			gfx->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
			gfx->SetTextRenderingHint(TextRenderingHint::TextRenderingHintClearTypeGridFit);

			Font* fnt = nullptr;
			WCHAR* fntName = nullptr;
			wstring ws = wstring(_fontname.begin(), _fontname.end());
			fntName = (WCHAR*)ws.c_str();

			fnt = new Font(fntName, (REAL)_fontheight, _fontstyle);
			float sweep = _sweep;// std::get<1>(_sweepAngle);

			// aplly scaling factor if specified
			maxVal = (float)(std::get<1>(_range) * _scale);
			minVal = (float)(std::get<0>(_range) * _scale);

			float stepping = sweep / (maxVal - minVal); // degrees per channel value step

			Point needleOff(std::get<0>(_needleOffset), std::get<1>(_needleOffset));

			PointF cp((float)std::get<0>(_needleCenter), (float)std::get<1>(_needleCenter)); // needle center point
			if (cp.X == -1 || cp.Y == -1) {

				// no needle center specified. So use center of image
				cp.X = (REAL)bmp->GetWidth() / 2;
				cp.Y = (REAL)bmp->GetHeight() / 2;
			}

			if (_useImg && _bgImg != nullptr) {

				gfx->DrawImage(_bgImg, 0, 0, _imgSize.Width, _imgSize.Height);
				//gfx->DrawImage(_bgImg, 0, 0, _rectangle.Width, _rectangle.Height);
			}
			else {
				// no face image given, so render gauge based on data specified in layout
				// file without face image

				if (_divisions != 0) {
					// draw a simple scale based on divisions parameter

					gfx->FillRectangle(&SolidBrush(_background), 0, 0, bmp->GetWidth(), bmp->GetHeight());

					float divStep = sweep / (float)_divisions; // angle step for each division
					//int divStepVal = (int)round((std::get<1>(_range) -std::get<0>(_range)) / _divisions);
					int divStepVal = (int)round((maxVal - minVal) / _divisions);

					// calculate radius of needle and scale based on largest divisions text
					if (fnt != nullptr) {

						StringFormat sf;
						sf.SetAlignment(StringAlignmentCenter);
						sf.SetLineAlignment(StringAlignmentCenter);

						RectF bb;
						getLabelBounds(fnt, (int)minVal, (int)maxVal, &sf, &bb);
						float lblWidth = max(bb.Width, bb.Height);

						// calculate radius of the scale independent from needle radius
						int rad = (int)round(cp.X - lblWidth - (2 * _margin));

						for (int i = 0, j = (int)minVal; i < (_divisions + 1); i++, j += divStepVal) {

							float a = std::get<0>(_sweepAngle) +(((float)i)*divStep);

							wstringstream wss;
							wss << j;
							wstring ws = wss.str();
							wchar_t* fmt = (wchar_t*)ws.c_str();
							gfx->MeasureString(fmt, (int)ws.size(), fnt, PointF(0, 0), &sf, &bb);

							// draw scale tick marks as dot
							PointF pt;
							pt.X = rad * cos(RAD(a));
							pt.Y = rad * sin(RAD(a));

							gfx->FillEllipse(&SolidBrush(_foreground), (REAL)(cp.X + pt.X - 2), (REAL)(cp.Y - pt.Y - 2), (REAL)(2 * 2), (REAL)(2 * 2));
							float textR = (float)rad + (bb.Width / 2.0f) + _margin;
							PointF tp;

							tp.X = textR * cos(RAD(a));
							tp.Y = textR * sin(RAD(a));
							float tx = (cp.X + tp.X) - (bb.Width / 2.0f);
							float ty = (cp.Y - tp.Y) - (bb.Height / 2.0f);
							RectF txtRect(tx, ty, bb.Width, bb.Height);

							gfx->DrawString(fmt, (int)wcslen(fmt), fnt, txtRect, &sf, &SolidBrush(_foreground));
						}
					}
				}
			}


			// TODO: FixMe***
			// check for correct data type and allocate space accordingly
			float vf = 0;

			if (!renderBlank) {
				try {
					CDataChannel& ch = std::move(logger.GetChannel(_channel));
					SampleValue val = CDataChannel::GetSampleData(sample, ch);
					if (val.type() == irsdk_int)
						vf = static_cast<float>(val.get_value<int>());
					else if (val.type() == irsdk_char)
						vf = static_cast<float>(val.get_value<char>());
					else if (val.type() == irsdk_double)
						vf = static_cast<float>(val.get_value<double>());
					else if (val.type() == irsdk_float)
						vf = val.get_value<float>();
					else
						vf = 0;

				}
				catch (std::exception)
				{
					throw;
				}
				// scale channel value
				vf *= (float)_scale;
			}

			// is current value lower than minumum value defined by range attribute?
			if (vf < minVal)
				vf = 0;
			else
				vf -= minVal;

			float rot = (vf*stepping);

			if (!_needleImgFile.empty())  {

				rot *= -1;
				rot += (180.0f - std::get<0>(_sweepAngle)); // needle rotation angle

				Matrix* mtx = new Matrix();
				mtx->RotateAt(rot, cp);
				mtx->Translate(0, 0);

				gfx->SetTransform(mtx);
				PointF np = PointF(cp.X - needleOff.X, cp.Y - (REAL)_needleImg->GetHeight() + needleOff.Y);
				RectF rc = RectF(np.X, np.Y, (REAL)_needleImg->GetWidth(), (REAL)_needleImg->GetHeight());
				gfx->DrawImage(_needleImg, rc);

				delete mtx;
			}
			else { // draw "simple" needle (just a line to the center point)

				rot += std::get<0>(_sweepAngle); // needle rotation angle

				PointF sp, ep;

				sp.X = _radius * cos(RAD(rot));
				sp.Y = _radius * sin(RAD(rot));
				ep.X = cp.X;
				ep.Y = cp.Y;

				gfx->DrawLine(&Pen(&SolidBrush(_foreground), 3.0f), cp.X + sp.X, cp.Y - sp.Y, cp.X, cp.Y);
			}

			if (fnt != nullptr) {
				delete fnt;
				fnt = nullptr;
			}

			delete gfx;
			gfx = nullptr;
		}
	}
	return bmp;
}

void CGauge::SetRange(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_range) = stoi(toks[0]);
	std::get<1>(_range) = stoi(toks[1]);
}

void CGauge::SetNeedleOffset(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_needleOffset) = stoi(toks[0]);
	std::get<1>(_needleOffset) = stoi(toks[1]);
}

void CGauge::SetSweepAngle(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	float _s = 0, _e = 0;

	_s = (float)stof(toks[0]);
	_e = (float)stof(toks[1]);

	std::get<0>(_sweepAngle) = _s;
	std::get<1>(_sweepAngle) = _e;
	_sweep = (float)(_e - _s);
	_renderStartAngle = 360.0f - _s;
	if (_renderStartAngle >= 360.0f)
		_renderStartAngle -= 360.0f;

}

void CGauge::SetPosition(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_pos) = stoi(toks[0]);
	std::get<1>(_pos) = stoi(toks[1]);
}

void CGauge::SetNeedleCenter(string& s)
{
	parsing_error ec = parsing_error::ok;

	std::vector<std::string> toks;
	splitTuple(s, toks, ec);
	if (ec != parsing_error::ok)
		throw ec;

	// check that we actually got the correct number of expected tokens from the tuple
	if (toks.size() != 2)
		throw parsing_error::missingArgument;

	std::get<0>(_needleCenter) = stoi(toks[0]);
	std::get<1>(_needleCenter) = stoi(toks[1]);
}

void CGauge::SetRuler(CRuler& ruler)
{
	ruler.SetPathRange(std::get<0>(_range), std::get<1>(_range));
	ruler.SetArcSweep((int)std::get<0>(_sweepAngle), (int)std::get<1>(_sweepAngle));
	ruler.SetArcRadius(_radius);
	ruler.SetPathType(std::string("arc"));
	ruler.SetRectangle(_rectangle);

	if (_scale != 1)
		ruler.SetScalingFactor(_scale);
}

void CGauge::SetImageFile(string& s)
{
	if (isAbsoluteFilePath(s))
		_imgFile = s;
	else
	{
		_imgFile = _dashFilePath + s;
	}
}

void CGauge::SetNeedleImageFile(string& s)
{
	if (isAbsoluteFilePath(s))
		_needleImgFile = s;
	else
		_needleImgFile = _dashFilePath + s;
}