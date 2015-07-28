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
		DashboardLayout() :
			_logger(nullptr),
			_ldf(nullptr)
		{
			_ldf = new CDashboard();
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

						_ldf->SetActiveDashboard(fname);
						_ldf->Parse();
						Marshal::FreeHGlobal(IntPtr(ptr));
					}
				}
			}
			catch (ParsingException& e)
			{
				Exception^ ex = gcnew Exception(gcnew String(e.what()));
				throw ex;
			}
		}

		String^ GetDashboardPrettyName(String^ file)
		{
			if (file != nullptr && file != "") {

				std::string& name = _ldf->GetPrettyName();
				String^ ret = gcnew String(name.c_str());

				return ret;
			}

			return nullptr;
		}

		String^ GetDashboardShortName(String^ file)
		{
			if (file != nullptr && file != "") {

				std::string& name = _ldf->GetShortName();
				String^ ret = gcnew String(name.c_str());

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
			System::Drawing::Bitmap^ bmp = nullptr;
			ImageInfo imgInfo;

			if (name != nullptr && sampleIndex >= 0) {

				char *ptr = (char*)Marshal::StringToHGlobalAnsi(name).ToPointer();
				std::string str = string(ptr);
				
				try {
					imgInfo = _ldf->RenderDashboard(_logger, sampleIndex, false);
					if (imgInfo.pixbuf != nullptr) {

							cli::array<Byte>^ pixbuf = gcnew cli::array<Byte>(imgInfo.stride);
							Marshal::Copy(IntPtr(imgInfo.pixbuf), pixbuf, 0, pixbuf->Length);

							IntPtr pix = Marshal::UnsafeAddrOfPinnedArrayElement(pixbuf, 0);

							bmp = gcnew System::Drawing::Bitmap(imgInfo.width,
								imgInfo.height,
								imgInfo.stride,
								System::Drawing::Imaging::PixelFormat::Format32bppArgb,
								pix);

							delete pixbuf;
							pixbuf = nullptr;

					}
				}
				catch (std::exception& ex)
				{
					System::Exception^ e = gcnew System::Exception(gcnew System::String(ex.what()));

					throw e;
				}
				finally
				{
					Marshal::FreeHGlobal(IntPtr(ptr));
				}
			}

			return bmp;
		}

		void SetDataLogger(IntPtr logger)
		{
			if (logger != IntPtr(0)) {

				_logger = (IGenericLogger*)logger.ToPointer();
			}
		}

	private:

		libOGA::IGenericLogger* _logger;
		CDashboard* _ldf;
	};
}

