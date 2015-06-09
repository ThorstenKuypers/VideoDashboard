#pragma once

#include "DashboardLayout.h"


namespace libLDFWrapper {

#pragma managed

	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;

	using namespace libLDF;

	public ref class DashboardLayout {

	public:
		DashboardLayout()
		{
			_ldf = new CDashboardLayout();
		}

		~DashboardLayout()
		{
			if (_ldf != nullptr) {
				delete _ldf;
				_ldf = nullptr;
			}
		}

		void ShowBoundingBoxes(bool show)
		{
			if (_ldf !=nullptr)
				_ldf->ShowBoundingBoxes(show);
		}

		void ParseLayoutFile(String^ file)
		{
			try {
				if (file != nullptr) {

					if (_ldf != nullptr) {

						char* ptr = (char*)Marshal::StringToHGlobalAnsi(file).ToPointer();
						string fname = string(ptr);

						_ldf->ParseLayoutFile(fname);

						Marshal::FreeHGlobal(IntPtr(ptr));
					}
				}
			}
			catch (ParsingException& e)
			{
				//Exception^ ex = gcnew Exception(gcnew String("syntax error"), gcnew Exception(gcnew String(e.what())));
				Exception^ ex = gcnew Exception(gcnew String(e.what()));

				//if (_ldf != nullptr)
				//{
				//	delete _ldf;
				//	_ldf = nullptr;
				//}
				throw ex;
			}
		}

		String^ GetDashboardPrettyName(String^ file)
		{
			if (file != nullptr && file != "") {

				char* ptr = (char*)Marshal::StringToHGlobalAnsi(file).ToPointer();
				std::string str = string(ptr);

				std::string name = _ldf->GetDashboardPrettyName(str);
				String^ ret = gcnew String(name.c_str());

				Marshal::FreeHGlobal(IntPtr(ptr));
				return ret;
			}

			return nullptr;
		}

		String^ GetDashboardShortName(String^ file)
		{
			if (file != nullptr && file != "") {

				char* ptr = (char*)Marshal::StringToHGlobalAnsi(file).ToPointer();
				std::string str = string(ptr);

				std::string name = _ldf->GetDashboardShortName(str);
				String^ ret = gcnew String(name.c_str());

				Marshal::FreeHGlobal(IntPtr(ptr));
				return ret;
			}

			return nullptr;
		}

		IntPtr GetInstancePtr()
		{
			if (_ldf != NULL)
				return IntPtr((void*)_ldf);

			return IntPtr(0);
		}

		System::Drawing::Bitmap^ RenderDashboard(String^ name, int sampleIndex)
		{
			//BYTE* pix = nullptr;
			//int stride = 0;
			System::Drawing::Bitmap^ bmp = nullptr;
			Gdiplus::Bitmap* dashImg = nullptr;

			if (name != nullptr && sampleIndex >= 0) {

				char *ptr = (char*)Marshal::StringToHGlobalAnsi(name).ToPointer();
				std::string str = string(ptr);

				try {
					dashImg = reinterpret_cast<Gdiplus::Bitmap*>(_ldf->RenderDashboard(str, sampleIndex));
					if (dashImg != nullptr) {

						Gdiplus::BitmapData bd = { 0 };
						Gdiplus::Rect rc(0, 0, dashImg->GetWidth(), dashImg->GetHeight());
						if (dashImg->LockBits(&rc, ImageLockModeRead, dashImg->GetPixelFormat(), &bd) == Gdiplus::Status::Ok) {

							//stride = bd.Stride;
							//int buflen = bd.Stride * dashImg->GetHeight();
							//pix = new BYTE[buflen];
							//memcpy(pix, bd.Scan0, buflen);

							cli::array<Byte>^ pixbuf = gcnew cli::array<Byte>(bd.Stride * bd.Height);
							Marshal::Copy(IntPtr(bd.Scan0), pixbuf, 0, pixbuf->Length);

							IntPtr pix = Marshal::UnsafeAddrOfPinnedArrayElement(pixbuf, 0);

							bmp = gcnew System::Drawing::Bitmap(dashImg->GetWidth(),
								dashImg->GetHeight(),
								bd.Stride,
								System::Drawing::Imaging::PixelFormat::Format32bppArgb,
								pix);

							dashImg->UnlockBits(&bd);

							delete pixbuf;
							pixbuf = nullptr;
						}

					}
				}
				catch (std::exception& ex)
				{
					System::Exception^ e = gcnew System::Exception(gcnew System::String(ex.what()));

					throw e;
				}
				finally
				{
					if (dashImg != nullptr) {
						delete dashImg;
						dashImg = nullptr;
					}
					Marshal::FreeHGlobal(IntPtr(ptr));
				}
			}

			return bmp;
		}

		void SetDataLogger(IntPtr logger)
		{
			if (logger != IntPtr(0)) {

				IDataLogger* dl = (IDataLogger*)logger.ToPointer();

				_ldf->SetDataLogger(dl);
			}
		}

	private:

		CDashboardLayout* _ldf;
	};
}

