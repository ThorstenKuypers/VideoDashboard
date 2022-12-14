#include "GCircle.h"


using namespace libLDF;


CGCircle::CGCircle() : CDashboardElement(),
_divisions(0),
_radius(0),
_lineWidth(0),
_pointerColor(Color::Black),
_pointerWidth(0),
_pointerSize(0),
_scaleLat(0),
_scaleLong(0),
_connectToCenter(true)
{
	type = DashboardElementType::gcircle;
}

CGCircle::~CGCircle()
{
}

void CGCircle::Init()
{
	imgInfo.width = _rectangle.Width;
	imgInfo.height = _rectangle.Height;
	imgInfo.stride = imgInfo.width * sizeof(int);

	_pixBufLen = imgInfo.stride * imgInfo.height;
	_pixBuf = std::shared_ptr<BYTE>(new BYTE[_pixBufLen]);
	imgInfo.pixbuf = _pixBuf.get();
}

ImageInfo CGCircle::Render(DataSample& sample, IGenericLogger* logger, bool renderBlank)
{
	memset(_pixBuf.get(), 0, _pixBufLen);

	Bitmap bmp(imgInfo.width, imgInfo.height, imgInfo.stride, imgInfo.pixelFormat, static_cast<BYTE*>(imgInfo.pixbuf));
	Graphics* gfx = Graphics::FromImage(&bmp);
	if (gfx != nullptr) {

		float longG = 0;
		float latG = 0;

		if (!renderBlank) {
			try {
				CDataChannel& chLat = std::move(logger->GetChannel(string("LatAccel")));
				CDataChannel& chLong = std::move(logger->GetChannel(string("LongAccel")));

				SampleValue val = CDataChannel::GetSampleData(*logger, sample, chLat);
				latG = val.get_value<float>();
				val = CDataChannel::GetSampleData(*logger, sample, chLong);
				longG = val.get_value<float>();
			}
			catch (std::exception)
			{
				throw;
			}

			longG /= 9.81f;
			latG /= 9.81f;

			if (_scale != 0) {
				longG *= (float)_scale;
				latG *= (float)_scale;
			}

			if (_scaleLat != 0)
				latG *= _scaleLat;
			if (_scaleLong != 0)
				longG *= _scaleLong;
		}
		else
		{
			latG = 0;
			longG = 0;
		}

		PointF cp = PointF((REAL)_rectangle.Width / 2, (REAL)_rectangle.Height / 2);
		float step = (float)((float)_rectangle.Width / 2) / (float)_radius;

		// draw center lines
		Pen* gridPen = new Pen(_foreground, (float)_lineWidth);
		gridPen->SetDashStyle(DashStyleDash);

		gfx->DrawLine(gridPen, 0.0f, cp.Y, (REAL)_rectangle.Width, cp.Y);
		gfx->DrawLine(gridPen, cp.X, 0.0f, cp.X, (REAL)_rectangle.Height);

		// draw grid circles
		for (int i = 1; i < _divisions + 1; i++) {

			float divStep = 0;
			if (_divisions != 0)
				divStep = (float)((float)_rectangle.Width / 2) / (float)_divisions;

			RectF rc = RectF(cp.X - (i*divStep), cp.Y - (i*divStep), 2 * i*divStep, 2 * i*divStep);
			gfx->DrawEllipse(gridPen, rc);
		}

		Pen* ptrPen = new Pen(_pointerColor, (float)_pointerWidth);

		// calculate pointer position based on values
		PointF vp = PointF(cp.X + (step * latG), cp.Y + (step * longG)); // vector line enpoint
		RectF ball = RectF(vp.X - ((float)_pointerSize / 2), vp.Y - ((float)_pointerSize / 2), (float)_pointerSize, (float)_pointerSize);

		gfx->DrawLine(ptrPen, cp, vp);
		gfx->FillEllipse(&SolidBrush(_pointerColor), ball);

		delete ptrPen;
		delete gridPen;
	}

	return ImageInfo{ imgInfo.width, imgInfo.height, imgInfo.stride, imgInfo.pixelFormat, _pixBuf.get() };
}

void CGCircle::SetPointerColor(string& s)
{
	Color c;

	try {

		getColorFromString(s, c);
		//if (c.size() == 1)
		//	_pointerColor = Gdiplus::Color((255) | c[0]);
		//if (c.size() == 3)
		//	_pointerColor = Gdiplus::Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
		_pointerColor = c;
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}
