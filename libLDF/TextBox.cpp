#include "TextBox.h"


using namespace libLDF;


CTextBox::CTextBox() : CDashboardElement(),
_format(LabelFormatType_integer),
_label(string("")),
_drawZero(false),
_signed(false),
_precision(1)
{
	type = DashboardElementType::textbox;
}


CTextBox::~CTextBox()
{
}

Gdiplus::Bitmap* CTextBox::Render(DataSample& sample, IGenericLogger& logger, bool renderBlank)
{
	Bitmap* bmp = new Bitmap(_rectangle.Width, _rectangle.Height, PixelFormat32bppARGB);
	if (bmp == nullptr)
		throw - 1;

	Graphics* gfx = Graphics::FromImage(bmp);
	if (gfx == nullptr) {

		delete bmp;
		bmp = nullptr;

		throw - 1;
	}

	SolidBrush textColor(_foreground);
	SolidBrush back(_background);
	stringstream ss;

	Font* ft = nullptr;
	wstring wFontname = wstring((const wchar_t*)L"Arial");
	int fh = 0;

	if (!_fontname.empty()) {
		/*wstringstream wss;
		wss << _fontname.c_str();*/
		wFontname = std::wstring(_fontname.begin(), _fontname.end());

	}

	if (_fontheight != 0 && _fontheight != -1) {
		ft = new Font(wFontname.c_str(), (REAL)_fontheight, _fontstyle, Gdiplus::Unit::UnitPoint);
	}
	else {
		ft = new Font(wFontname.c_str(), (REAL)_rectangle.Height, _fontstyle, Gdiplus::Unit::UnitPixel);
	}

	if (!_channel.empty()) {

		try {
			double d = 0;
			float f = 0;
			int i = 0;

			double time = 0, s = 0;
			int h = 0, m = 0, sec = 0, t = 0;
			SampleValue sv;

			if (!renderBlank) {
				CDataChannel& ch = std::move(logger.GetChannel(_channel));
				sv = CDataChannel::GetSampleData(sample, ch);
			}

			switch (_format) {

			case LabelFormatType::LabelFormatType_real:
				ss.setf(ios::fixed);
				ss.precision(_precision);

				if (sv.type() == irsdk_double) {
					if (!renderBlank) {
						d = sv.get_value<double>();
						if (_scale != 0)
							d *= _scale;
					}
					ss << d;
				}
				else if (sv.type() == irsdk_float) {
					if (!renderBlank) {
						f = sv.get_value<float>();
						if (_scale != 0)
							f *= (float)_scale;
					}
					ss << f;
				}
				else if (sv.type() == irsdk_int) {
					if (!renderBlank) {
						i = sv.get_value<int>();
						f = (float)i;
						if (_scale != 0)
							f *= (float)_scale;
					}
					ss << f;
				}

				break;

			case LabelFormatType::LabelFormatType_integer:
				ss.setf(ios::boolalpha);

				if (sv.type() == irsdk_double) {
					if (!renderBlank) {
						d = sv.get_value<double>();
						i = (int)round(d);
						if (_scale != 0)
							i *= static_cast<int>(round(_scale));
					}
					ss << i;
				}
				else if (sv.type() == irsdk_float) {
					if (!renderBlank) {
						f = sv.get_value<float>();
						i = (int)round(f);
						if (_scale != 0)
							i *= static_cast<int>(round(_scale));
					}
					ss << i;
				}
				else if (sv.type() == irsdk_int) {
					if (!renderBlank) {
						i = sv.get_value<int>();
						f = (float)i;
						if (_scale != 0)
							i = (int)round(((float)i * (float)_scale));
					}
					ss << i;
				}

				break;

			case LabelFormatType::LabelFormatType_time:
				ss.setf(ios::fixed);

				if (!renderBlank) {

					if (sv.type() == irsdk_double) {
						time = sv.get_value<double>();
					}
					else if (sv.type() == irsdk_float) {
						f = sv.get_value<float>();
						time = static_cast<double>(f);
					}
				}

				ss.precision(0);
				modf(time, &s);
				t = (int)s;
				sec = t % 60;
				m = ((t - sec) / 60) % 60;
				h = (((t - sec) / 60) - m) / 60;

				ss << h << ":" << m << ":" << sec;

				break;

			case LabelFormatType::LabelFormatType_laptime:
				ss.setf(ios::fixed);
				if (_precision <= 0)
					_precision = 1;
				ss.precision(_precision);

				if (sv.type() == irsdk_float) {
					float laptime = 0;

					if (!renderBlank) {

						laptime = sv.get_value<float>();
						s = fmod(laptime, (double)60.0f);
						m = static_cast<int>(((laptime - s) / (double)60.0f));
					}
					ss << m << ":" << s;
				}
				break;
			}
		}
		catch (libOGA::ChannelDataTypeMismatchException)
		{
			ss << _label;

		}
		catch (std::exception& ex)
		{
			throw ex;
		}
		//}
	}
	else if (!_label.empty())
		ss << _label;

	StringFormat sf;

	switch (_justify) {

	case TextJustify_center:
		sf.SetAlignment(StringAlignmentCenter);
		break;
	case TextJustify_left:
		sf.SetAlignment(StringAlignmentNear);
		break;
	case TextJustify_right:
		sf.SetAlignment(StringAlignmentFar);
		break;
	}

	RectF rc((REAL)0, (REAL)0, (REAL)_rectangle.Width, (REAL)_rectangle.Height);
	string str = ss.str();
	int len = (int)str.size();
	wstring wlbl = wstring(str.begin(), str.end());
	WCHAR* wc = (WCHAR*)wlbl.c_str();

	//gfx->DrawRectangle(&Pen(Color(255, 255, 0, 0), 2.0f), rc);
	gfx->FillRectangle(&back, 0, 0, _rectangle.Width, _rectangle.Height);
	gfx->DrawString(wc, len, ft, rc, &sf, &textColor);

	delete ft;
	delete gfx;

	return bmp;
}

void CTextBox::SetSigned(string& s)
{
	if (s == string("true") || s == string("yes"))
		_signed = true;
	else
		_signed = false;
}

void CTextBox::SetFormatType(string& s)
{
	if (s == string("integer")) {

		_format = LabelFormatType::LabelFormatType_integer;
	}
	else if (s == string("real")) {

		_format = LabelFormatType::LabelFormatType_real;
	}
	else if (s == string("time")) {

		_format = LabelFormatType::LabelFormatType_time;
	}
	else if (s == string("laptime")) {

		_format = LabelFormatType::LabelFormatType_laptime;
	}
	else {

		_format = LabelFormatType::LabelFormatType_integer;
	}
}

void CTextBox::SetDrawZero(string& s)
{
	if (s == string("yes") || s == string("true")) {

		_drawZero = true;
	}
	else {

		_drawZero = false;
	}
}
