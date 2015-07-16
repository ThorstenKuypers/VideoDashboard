#include "LiveDashImageSource.h"


CLiveDashImageSource::CLiveDashImageSource(CLiveDashboard* dash) : ImageSource(),
_ld(dash),
_renderPlane(nullptr),
_staticTexture(nullptr),
_currentTexture(nullptr),
_imgInfo()
{
	VBData *points = new VBData;
	points->VertList.SetSize(4);              //Reserve space for the vertices
	_renderPlane = CreateVertexBuffer(points, FALSE);  //Store the vertex list in a vertex buffer

	if (_ld != nullptr) {
		LIVEDASH_CONFIG& cfg = _ld->GetConfig();

		if (cfg.dash != nullptr && !cfg.selectedDashboard.empty())
		{
			std::string f = std::string(cfg.selectedDashboard.begin(), cfg.selectedDashboard.end());
			cfg.dash->SetActiveDashboard(f);
			cfg.dash->Parse();

			Bitmap* bmp = cfg.dash->Render(*cfg.logger, 0, true);
			if (bmp != nullptr)
			{
				_imgInfo.width = (int)bmp->GetWidth();
				_imgInfo.height = (int)bmp->GetHeight();

				_staticTexture = CreateGDITexture(bmp->GetWidth(), bmp->GetHeight());

				if (_staticTexture) {
					HDC hdc;
					if (_staticTexture->GetDC(hdc)) {

						Graphics* g = Graphics::FromHDC(hdc);
						if (g) {

							g->DrawImage(bmp, 0, 0);

							delete g;
						}

						_staticTexture->ReleaseDC();
					}

					LoadTexture(_staticTexture, 0);
				}

				delete bmp;
				bmp = nullptr;

				//_imgInfo.width = bmp->GetWidth();
				//_imgInfo.height = bmp->GetHeight();

				//BitmapData bd{ 0 };
				//Rect rc{ 0, 0, (int)_imgInfo.width, (int)_imgInfo.height };
				//if (bmp->LockBits(&rc, 0, PixelFormat32bppARGB, &bd) == Status::Ok) {

				//	_imgInfo.pixelFmt = bd.PixelFormat;
				//	_imgInfo.stride = bd.Stride;
				//	_imgInfo.scan0 = buf;

				//	DWORD len = bd.Height * bd.Stride;

				//	buf = new BYTE[len];
				//	memcpy(buf, bd.Scan0, len);

				//	bmp->UnlockBits(&bd);

				//	delete bmp;
				//	bmp = nullptr;
				//}
			}
		}
	}
}


CLiveDashImageSource::~CLiveDashImageSource()
{
	if (_staticTexture != nullptr) {

		delete _staticTexture;
		_staticTexture = nullptr;
	}
	_ld = nullptr;
}

void CLiveDashImageSource::Render(const Vect2 &pos, const Vect2 &size)
{
	LIVEDASH_CONFIG& cfg = _ld->GetConfig();

	VBData *data = _renderPlane->GetData();               //Fill our vertex buffer object with a square that
	data->VertList[0].Set(pos.x, pos.y, 0);      //occupies our entire drawing region
	data->VertList[1].Set(pos.x, pos.y + size.y, 0);
	data->VertList[2].Set(pos.x + size.x, pos.y, 0);
	data->VertList[3].Set(pos.x + size.x, pos.y + size.y, 0);
	_renderPlane->FlushBuffers();         //Flush the software representation of the vertices to the GPU
	LoadVertexBuffer(_renderPlane);     //Prepare the vertex buffer we just created for drawing

	if (_currentTexture)
		_currentTexture->Unmap();

	Bitmap* bmp = nullptr;
	if (!_ld->IsSimConnected())
	{
		//bmp = cfg.dash->Render(*cfg.logger, 0, true);
		LoadTexture(_staticTexture, 0);
		return;
	}
	else
	{
		if (cfg.logger != nullptr)
		{
			libOGA::DataSample& s = _ld->GetLatestSample();
			if (s.size() > 0)
				bmp = cfg.dash->Render(*cfg.logger, s, false);
			else
				bmp = cfg.dash->Render(*cfg.logger, 0, true);

		}
	}

	if (bmp != nullptr)
	{
		//Texture* tex = CreateGDITexture((UINT)_imgInfo.width, (UINT)_imgInfo.height);
		Texture* tex = CreateGDITexture((UINT)size.x, (UINT)size.y);
		if (tex) {
			HDC hdc;
			tex->GetDC(hdc);

			Graphics* g = Graphics::FromHDC(hdc);

			g->DrawImage(bmp, 0.0f, 0.0f, size.x, size.y);

			delete g;
			tex->ReleaseDC();

			delete bmp;
			bmp = nullptr;
			LoadTexture(tex, 0);

			delete _currentTexture;
			_currentTexture = tex;

			LoadTexture(_currentTexture, 0);
		}
		Draw(GS_TRIANGLESTRIP);    //Draw our vertices as a triangle strip.
	}
}

Vect2 CLiveDashImageSource::GetSize() const
{
	Vect2 ret;                         //anything you want. This just determines the
	ret.x = (float)_imgInfo.width;                       //standard size of your plugin image.
	ret.y = (float)_imgInfo.height;                       //You can also change this as much as you want
	return ret;                        //during runtime.
}

