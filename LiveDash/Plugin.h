#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "OBSApi.h"
//#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "libLDF.h"
#include "libDataLogging.h"
#include "LiveDataLogger.h"

#include "config.h"

extern "C" __declspec(dllexport) bool LoadPlugin();
extern "C" __declspec(dllexport) void UnloadPlugin();
extern "C" __declspec(dllexport) CTSTR GetPluginName();
extern "C" __declspec(dllexport) CTSTR GetPluginDescription();

extern "C" __declspec(dllexport) void ConfigPlugin(HWND hwnd);
extern "C" __declspec(dllexport) void OnStartStream();
extern "C" __declspec(dllexport) void OnStopStream();
extern "C" __declspec(dllexport) void OnStartStreaming();
extern "C" __declspec(dllexport) void OnStopStreaming();

extern HINSTANCE pluginInst;
extern ILiveLogger* logger;
extern IDashboardLayout* dash;


struct DashInfo
{
	UINT width;
	UINT height;
	UINT stride;
	Gdiplus::PixelFormat pixelFmt;
	BYTE* scan0;

};


class Img : public ImageSource
{
public:

	Img(LIVEDASH_CONFIG* cfg)
	{
		//_cfg = std::unique_ptr < LIVEDASH_CONFIG > {new LIVEDASH_CONFIG};

		std::string file = "D:\\Development\\Projects\\VideoDashboard\\bin\\Debug\\x86\\dashboards";
		file.append("\\RedCarbon_300KPH.layout");

		dash->ParseLayoutFile(file);
		Bitmap* bmp = (Bitmap*)dash->RenderDashboard(file, -1);
		BitmapData bd = { 0 };
		Rect rc = { 0, 0, (int)bmp->GetWidth(), (int)bmp->GetHeight() };

		if (bmp != nullptr)
		{
			bmp->LockBits(&rc, 0, PixelFormat32bppARGB, &bd);

			di.height = bd.Height;
			di.width = bd.Width;
			di.stride = bd.Stride;
			di.pixelFmt = bd.PixelFormat;
			di.scan0 = buf;

			DWORD len = bd.Stride*bd.Height;
			buf = new BYTE[len];

			memcpy(buf, bd.Scan0, len);

			bmp->UnlockBits(&bd);

			delete bmp;
			bmp = nullptr;
		}

		VBData *points = new VBData;
		points->VertList.SetSize(4);              //Reserve space for the vertices
		box = CreateVertexBuffer(points, FALSE);  //Store the vertex list in a vertex buffer
		UINT color = 0x801142FF;                  //Color is 0xAABBGGRR on little-endian machines
		//tex = CreateTexture(1, 1, GS_RGBA, &color, FALSE, TRUE);  //Create a 1x1 translucent red texture
		tex = CreateTexture(di.width, di.height, GS_BGRA, buf, 0, 0);

	}

	~Img()
	{
		if (buf)
		{
			delete buf;
			buf = nullptr;
		}
	}

	virtual void Render(const Vect2 &pos, const Vect2 &size)
	{
		VBData *data = box->GetData();               //Fill our vertex buffer object with a square that
		data->VertList[0].Set(pos.x, pos.y, 0);      //occupies our entire drawing region
		data->VertList[1].Set(pos.x, pos.y + size.y, 0);
		data->VertList[2].Set(pos.x + size.x, pos.y, 0);
		data->VertList[3].Set(pos.x + size.x, pos.y + size.y, 0);
		box->FlushBuffers();         //Flush the software representation of the vertices to the GPU
		LoadVertexBuffer(box);     //Prepare the vertex buffer we just created for drawing


		//LoadTexture(tex);          //Load our 1x1 texture for drawing
		if (tex) {

			tex->Unmap();
			if (buf) {
				delete buf;
				buf = nullptr;
			}

			std::string file = "D:\\Development\\Projects\\VideoDashboard\\bin\\Debug\\x86\\dashboards";
			file.append("\\RedCarbon_300KPH.layout");

			dash->ParseLayoutFile(file);
			Bitmap* bmp = (Bitmap*)dash->RenderDashboard(file, -1);
			BitmapData bd = { 0 };
			Rect rc = { 0, 0, (int)bmp->GetWidth(), (int)bmp->GetHeight() };
			UINT s = 0;

			if (bmp != nullptr)
			{
				//bmp->LockBits(&rc, 0, PixelFormat32bppARGB, &bd);

				//DWORD len = bd.Stride*bd.Height;
				//s = bd.Stride;
				//buf = new BYTE[len];

				//memcpy(buf, bd.Scan0, len);

				//bmp->UnlockBits(&bd);

				HDC hdc;
				tex->GetDC(hdc);

				Graphics* g = Graphics::FromHDC(hdc);

				g->DrawImage(bmp, 0, 0);

				delete g;
				tex->ReleaseDC();

				delete bmp;
				bmp = nullptr;
			}

			//tex->SetImage(buf, GS_IMAGEFORMAT_BGRA, s);
			//tex->Map(buf, s);
			LoadTexture(tex, 0);
		}
		Draw(GS_TRIANGLESTRIP);    //Draw our vertices as a triangle strip.
	}

	virtual Vect2 GetSize() const
	{
		Vect2 ret;                         //anything you want. This just determines the
		ret.x = (float)di.width;                       //standard size of your plugin image.
		ret.y = (float)di.height;                       //You can also change this as much as you want
		return ret;                        //during runtime.
	}

private:

	std::unique_ptr<LIVEDASH_CONFIG> _cfg;
	VertexBuffer* box;
	Texture* tex;

	DashInfo di;
	BYTE* buf;
};


#endif