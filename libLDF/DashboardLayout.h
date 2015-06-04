#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011 Thorsten Kuypers
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You should have obtained a copy of the License with this Software. If not,
//	you may obtain a copy of the License at
//
//				http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
// * version history:
//   -----------------
//
//	0.4 - first public release
//
///////////////////////////////////////////////////////////////////////////////



#include <map>

#include "common.h"
#include "libLDF.h"
#include "Dashboard.h"
#include "errex.h"

//#include "ldf.h"

namespace libLDF
{
#pragma unmanaged

	//typedef std::tuple<std::string, int, int> section;
	typedef std::map<string, CDashboard*> dashboard_map;
	typedef std::pair<int, std::string> ldf_line;
	typedef std::pair<std::string, std::string> ldf_pair;

	struct _section
	{
		std::string name;
		map<int, ldf_pair> keyPairs;
		std::vector<ldf_line> lines;
	};
	typedef struct _section elementSection;


	class CDashboardLayout : public IDashboardLayout
	{
	public:
		CDashboardLayout();
		~CDashboardLayout();

		void ParseLayoutFile(std::string filename);

		CDashboard* GetDashboard(string& name);
		std::string GetDashboardPrettyName(std::string& filename);
		std::string GetDashboardShortName(std::string& filename);

		void ShowBoundingBoxes(bool show);

		Bitmap* RenderDashboard(string& name, int sampleIndex);

		void SetDataLogger(IDataLogging* logger);

	private:
		//std::vector<CDashboard*> _dashboards;
		//std::map<string, CDashboard*> _dashboards;

		std::string _curDashboardFileName;
		std::string _curDashboardFilePath;

		CDashboard* _curDashboard; // current opened and parsed dashboard (single instance)

		void extractDashboardFilePath();

		//void readSection(std::vector<std::string>& lines, elementSection& section);
		void parseSections(elementSection& sec);
		void parseCommonKeys(CDashboardElement* e, elementSection& sect);

		CDashboard* parseDashboardSection(elementSection& section);
		CTextBox* parseTextBoxSection(elementSection& section);
		CGauge* parseGaugeSection(elementSection& sect);
		CIndicator* parseIndicatorSection(elementSection& sect);
		CSlider* parseSliderSection(elementSection& sect);
		CGCircle* parseGCircleSection(elementSection& sect);
		CSweeper* parseSweeperElement(elementSection& sect);
		CRuler* parseRulerElement(elementSection& sect);
		CRingGauge* parseRingGaugeElement(elementSection& sect);

		void checkElementLocalSettingsOverwrite(CDashboardElement* e, CDashboard* d);

		IDataLogging* _dataLogger;

		ULONG_PTR _gdiplus_tok;

		//bool _useLocalForeColor;
		//bool _useLocalBackColor;
		//bool _useLocalFontname;
		//bool _useLocalFontstyle;
		//bool _useLocalJustify;

	};

}

//using namespace libDataLogging::DataLogWrapper;




















#pragma region // LayoutDefinitionFormat namespace

//namespace LayoutDefinitionFormat {
//
//#pragma managed
//	using namespace cli;
//	using namespace System;
//	using namespace System::Collections::Generic;
//	using namespace System::IO;
//	using namespace System::Drawing;
//	using namespace System::Runtime::InteropServices;
//
//	using namespace LayoutDefinitionFormat::Native;
//
//	public enum struct eTest {
//		test1,
//		test2
//	};
//
//
//	public enum DashboardElementType {
//		elementType_Gauge = 1,
//		elementType_Slider,
//		elementType_Bar,
//		elementType_Label,
//		elementType_TextBox,
//		elementType_Display,
//		elementType_Display_var,	// variable text display
//		elementType_gCircle,
//		elementType_Sweeper,
//		elementType_Led
//	};
//
//	public enum DashboardElementUnits {
//		elementUnits_Kph = 1,
//		elementUnits_Mph
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct Dashboard {
//		String^ file;
//		String^ name;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct EPos {
//		int x;
//		int y;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct ESize {
//		int width;
//		int height;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct ValueRange {
//		int min;
//		int max;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct Angles {
//		int startAngle;
//		int sweepAngle;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct _Color {
//		int r;
//		int g;
//		int b;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct Needle {
//		String^ m_file; // image file name
//		EPos imgOff;	// image center offset
//		EPos center; // needle center positions
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Gauge {
//	public:
//
//		String^ file;			// element file name (if used; otherwise NULL)
//		EPos pos;				// gauge pos
//		ESize size;				// gauge size
//		bool useNeedle;
//		ValueRange valRange;
//		Angles angles;
//		int radius;
//		int units;
//		String^ channelName;	// name of telemetry data channel for this element
//
//		Needle needle;			// needle element (nested inside Gauge element
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Slider {
//		int type;		// slider type (0 = min -> max; 1 = min <- 0 -> max; 2 = max -> min)
//		int barType;	// orientation (horizontal or vertical)
//		EPos pos;
//		ESize size;
//		ValueRange valRange;
//		LayoutDefinitionFormat::_Color color;
//		String^ channelName;	// name of telemetry data channel for this element
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Label {
//		EPos pos;
//		ESize size;
//		String^ font;
//		int fontSize;
//		int fontStyle;
//		LayoutDefinitionFormat::_Color color;
//		String^ channelName;	// name of telemetry data channel for this element
//		int units;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_TextBox {
//		EPos pos;
//		ESize size;
//		String^ font;
//		int fontSize;
//		int fontStyle;
//		LayoutDefinitionFormat::_Color color;
//		String^ text; // user text
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Display {
//		int displayType;
//		EPos pos;
//		ESize size;
//		//			VALRANGE valRange;
//		LayoutDefinitionFormat::_Color color;	// text color
//		//String^ font;	// |
//		//int fontSize;	// |-> only used if type=text
//		//int fontStyle;	// |
//		String^ channelName;	// name of telemetry data channel for this element
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_gCircle {
//		EPos pos;
//		EPos center;
//		ESize size;
//		ValueRange latRange;
//		ValueRange longRange;
//		int radius;
//		LayoutDefinitionFormat::_Color GridColor;
//		int BallType;
//		int BallSize;
//		LayoutDefinitionFormat::_Color ballColor;
//	};
//
//	// new in version 0.8 -> sweeper
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Sweeper {
//		String^ channelName;	// name of telemetry data channel for this element
//		EPos pos;
//		//EPos center;
//		ESize size;
//		ValueRange values;
//		Angles angles;
//		int threshVal; // threshold value
//		float width;	// width of bar
//		LayoutDefinitionFormat::_Color threshColor;
//		LayoutDefinitionFormat::_Color color;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement_Led {
//	public:
//		String^ channelname;
//		EPos pos;
//		ESize size;
//		ValueRange onOffVals;	// min = off - max = on
//		String^ imgFileOn;
//		String^ imgFileOff;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardLayoutElement {
//	public:
//
//		int type;	// element type)
//		String^	id;	// element ID or name
//
//		LayoutDefinitionFormat::DashboardLayoutElement_Gauge gauge;
//		LayoutDefinitionFormat::DashboardLayoutElement_Slider slider;
//		LayoutDefinitionFormat::DashboardLayoutElement_Label label;
//		LayoutDefinitionFormat::DashboardLayoutElement_TextBox txtBox;
//		LayoutDefinitionFormat::DashboardLayoutElement_Display display;
//		LayoutDefinitionFormat::DashboardLayoutElement_gCircle gCircle;
//		LayoutDefinitionFormat::DashboardLayoutElement_Sweeper sweeper;
//		LayoutDefinitionFormat::DashboardLayoutElement_Led led;
//	};
//
//	[StructLayout(LayoutKind::Sequential, CharSet = CharSet::Unicode)]
//	public ref struct DashboardOverlay {
//		String^ name;		// name (id) of Overlay
//		String^ fileName;	// image file name
//
//		IntPtr elements;	// element array
//		int elementCount;
//	};
//
//
//	public ref class CDashboardLayout : IDisposable
//	{
//	public:
//		~CDashboardLayout(void)
//		{
//			this->!CDashboardLayout();
//		}
//
//		!CDashboardLayout(void)
//		{
//			m_dashElementList->Clear();
//			delete m_dashElementList;
//			m_dashElementList = nullptr;
//
//			delete m_dashboard.file;
//			m_dashboard.file = nullptr;
//
//			delete m_dashboard.name;
//			m_dashboard.name = nullptr;
//
//			cli::array<int>::Clear(m_sectionHdrs, 0, m_sectionHdrs->Length);
//			delete m_sectionHdrs;
//			m_sectionHdrs = nullptr;
//
//			if (lines != nullptr) {
//				for (int i = 0; i < lines->Count; i++) {
//					delete lines[i];
//					lines[i] = nullptr;
//				}
//				lines->Clear();
//				delete lines;
//				lines = nullptr;
//			}
//		}
//
//		CDashboardLayout(String^ layoutFile)
//		{
//			this->m_dashElementList = nullptr;
//
//			String^ line;
//			try {
//				sr = gcnew StreamReader(layoutFile);
//				lines = gcnew List<String^>();
//
//				while (!sr->EndOfStream) {
//					// skip empty and comment lines
//					line = sr->ReadLine();
//					if (line->StartsWith("#") || String::IsNullOrEmpty(line))
//						continue;
//					lines->Add(line);
//				}
//
//				getSectionHeaders();
//				parseLines();
//			}
//			catch (Exception^ e) {
//
//				sr->Close();
//				delete sr;
//				sr = nullptr;
//
//				throw e;
//			}
//
//			sr->Close();
//			delete sr;
//			sr = nullptr;
//		}
//
//		property String^ DashboardName
//		{
//			String^ get(void)
//			{
//				return this->m_dashboard.name;
//			}
//		}
//
//		property String^ DashImageFile
//		{
//			String^ get(void)
//			{
//				return this->m_dashboard.file;
//			}
//		}
//
//		property List<DashboardLayoutElement^>^ DashElements
//		{
//			List<DashboardLayoutElement^>^ get(void)
//			{
//				return m_dashElementList;
//			}
//		}
//
//		Drawing::Bitmap^ GetDashboardImage(String^ imgPath)
//		{
//			try {
//				Drawing::Bitmap^ dashBmp = gcnew Drawing::Bitmap((imgPath + this->m_dashboard.file));
//				Drawing::Graphics^ dashGfx = Drawing::Graphics::FromImage(dashBmp);
//
//				// draw each Gauge element with a valid file name
//				for each (DashboardLayoutElement^ e in this->m_dashElementList) {
//					if (e->type == LayoutDefinitionFormat::elementType_Gauge) {
//						if (e->gauge.file != nullptr) {
//							Drawing::Bitmap^ bmp = gcnew Drawing::Bitmap((imgPath + e->gauge.file));
//							dashGfx->DrawImage(bmp, e->gauge.pos.x, e->gauge.pos.y, e->gauge.size.width, e->gauge.size.height);
//							delete bmp;
//						}
//					}
//					if (e->type == LayoutDefinitionFormat::elementType_Led) {
//						if (e->led.imgFileOff != nullptr) {
//							Drawing::Bitmap^ bmp = gcnew Drawing::Bitmap((imgPath + e->led.imgFileOff));
//							Drawing::Rectangle rc(e->led.pos.x - (bmp->Width / 2), e->led.pos.y - (bmp->Height / 2),
//								bmp->Width, bmp->Height);
//							dashGfx->DrawImage(bmp, rc);
//							delete bmp;
//						}
//					}
//				}
//
//				delete dashGfx;
//				return dashBmp;
//			}
//			catch (Exception^ ex)
//			{
//				throw ex;
//			}
//		}
//
//		IntPtr GetDashboardOverlayObj()
//		{
//			try {
//				IntPtr p(0);
//				DashboardOverlay^ dash = gcnew DashboardOverlay();
//				dash->fileName = this->m_dashboard.file;
//				dash->name = this->m_dashboard.name;
//				dash->elementCount = GetElementCount();
//				dash->elements = GetElementBufPtr();
//
//				DASHBOARD_OVERLAY* overlay = new DASHBOARD_OVERLAY;
//				p = IntPtr((void*)overlay);
//				Marshal::StructureToPtr(dash, p, false);
//
//				return p;
//			}
//			catch (Exception^ ex)
//			{
//				throw ex;
//			}
//		}
//		void ReleaseDashboardOverlayObj(IntPtr obj)
//		{
//			if (obj != IntPtr(0)) {
//				// get ptr to overlay structure
//				DASHBOARD_OVERLAY* p = (DASHBOARD_OVERLAY*)obj.ToPointer();
//				int cnt = p->DashElementBufCnt;
//				// free each element in element buffer ...
//				PDASHELEMENT* pp = (PDASHELEMENT*)p->pDashElementBuf;
//				for (int i = 0; i < cnt; i++) {
//					if (pp[i] != 0) {
//						delete pp[i];
//						pp[i] = 0;
//					}
//				}
//				// .. and buffer itself
//				delete pp;
//				pp = 0;
//				delete p;
//				p = 0;
//				obj = IntPtr(0);
//			}
//		}
//
//		IntPtr GetElementBufPtr()
//		{
//			try {
//				int cnt = this->m_dashElementList->Count;
//				PDASHELEMENT* ptr = new PDASHELEMENT[cnt];
//				IntPtr p;
//
//				for (int i = 0; i < cnt; i++) {
//					DashboardLayoutElement^ e = this->m_dashElementList[i];
//					DASHELEMENT* el = new DASHELEMENT;
//					for (int j = 0; j < sizeof(DASHELEMENT); j++)
//						((unsigned char*)el)[j] = 0;
//
//					p = IntPtr((void*)el);
//					Marshal::StructureToPtr(e, p, false);
//					ptr[i] = el;
//				}
//
//				p = IntPtr((void*)ptr);
//				return p;
//			}
//			catch (Exception^ ex)
//			{
//				throw ex;
//			}
//		}
//
//		int GetElementCount()
//		{
//			return this->m_dashElementList->Count;
//		}
//
//	private:
//		List<String^>^ lines;
//		StreamReader^ sr;
//
//		int m_lineIdx;
//		cli::array<int>^ m_sectionHdrs;
//
//		List<DashboardLayoutElement^>^ m_dashElementList;
//		Dashboard m_dashboard;
//
//		void getSectionHeaders()
//		{
//			List<int>^ ls = gcnew List<int>();
//
//			for (int i = 0; i < this->lines->Count; i++) {
//				if (this->lines[i]->StartsWith("[") && this->lines[i]->EndsWith("]")) {
//					ls->Add(i);
//#ifdef _DEBUG
//					int dbg = 0;
//#endif
//				}
//			}
//
//			this->m_sectionHdrs = ls->ToArray();
//
//			delete ls;
//		}
//
//		void parseLines()
//		{
//			this->m_dashElementList = gcnew List<DashboardLayoutElement^>();
//			cli::array<wchar_t>^ sep = { '.' };
//			cli::array<wchar_t>^ sep2 = { '=' };
//			int j = 0;
//
//			try {
//				for (int i = 0; i < this->m_sectionHdrs->Length; i++) {
//					j = this->m_sectionHdrs[i];
//
//					if (this->lines[j]->Contains("Dashboard")) {
//						for (int k = j + 1; k < (j + 3); k++) {
//							cli::array<String^>^ toks = this->lines[k]->Split(sep2);
//							if (toks[0]->ToUpper() == "FILE")
//								this->m_dashboard.file = toks[1];
//							if (toks[0]->ToUpper() == "NAME")
//								this->m_dashboard.name = toks[1];
//						}
//					}
//
//					// get type and name of section (-> Element)
//					if (this->lines[j]->Contains("Gauge")) {
//						// type: Gauge
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Gauge, s, j);
//					}
//					if (this->lines[j]->Contains("Slider")) {
//						// type: Gauge
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Slider, s, j);
//					}
//					if (this->lines[j]->Contains("Label")) {
//						// type: Gauge
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Label, s, j);
//					}
//					if (this->lines[j]->Contains("TextBox")) {
//						// type: Gauge
//						//				array<String^>^ toks =this->lines[j]->Split(sep);
//						//				String^ s =toks[1]->Remove(toks[1]->Length-1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_TextBox, "TextBox", j);
//					}
//					if (this->lines[j]->Contains("Display")) {
//						// type: Gauge
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Display, s, j);
//					}
//					if (this->lines[j]->Contains("GCircle")) {
//						// type: Gauge
//						//				array<String^>^ toks =this->lines[j]->Split(sep);
//						//				String^ s =toks[1]->Remove(toks[1]->Length-1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_gCircle, "gCircle", j);
//					}
//					if (this->lines[j]->Contains("Sweeper")) {
//						// type: Sweeper
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Sweeper, s, j);
//					}
//					if (this->lines[j]->Contains("LED")) {
//						cli::array<String^>^ toks = this->lines[j]->Split(sep);
//						String^ s = toks[1]->Remove(toks[1]->Length - 1, 1);
//						parseElement(LayoutDefinitionFormat::elementType_Led, s, j);
//					}
//				}
//			}
//			catch (Exception^ ex)
//			{
//				throw ex;
//			}
//		}
//
//		void parseElement(DashboardElementType type, String^ channel, int startIdx)
//		{
//			DashboardLayoutElement^ e = gcnew DashboardLayoutElement();
//			int i = startIdx + 1;
//			int x, y;
//			cli::array<wchar_t>^ sep = { '=', ',' };
//
//			e->type = type;
//			e->id = channel;
//
//			while ((this->lines[i]->StartsWith("[") == false) && (i < this->lines->Count)){
//
//#pragma region // parse Gauge Element
//				if (type == LayoutDefinitionFormat::elementType_Gauge) {
//					// Gauge (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//					if (toks[0]->ToUpper() == "FILE") {
//						if (!lines[i]->EndsWith("=")) {
//							e->gauge.file = toks[1];
//						}
//						else {
//							e->gauge.file = nullptr;
//						}
//					}
//
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.pos.x = x;
//						e->gauge.pos.y = y;
//					}
//
//					if (toks[0]->ToUpper() == "SIZE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.size.width = x;
//						e->gauge.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "VALUERANGE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.valRange.min = x;
//						e->gauge.valRange.max = y;
//					}
//					if (toks[0]->ToUpper() == "ANGLES") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.angles.startAngle = x;
//						e->gauge.angles.sweepAngle = y;
//					}
//					if (toks[0]->ToUpper() == "NEEDLE") {
//
//						if (toks[1] != "") {
//							e->gauge.needle.m_file = toks[1];
//						}
//					}
//					if (toks[0]->ToUpper() == "NEEDLECENTER") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.needle.center.x = x;
//						e->gauge.needle.center.y = y;
//					}
//					if (toks[0]->ToUpper() == "NEEDLEOFFSET") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gauge.needle.imgOff.x = x;
//						e->gauge.needle.imgOff.y = y;
//					}
//					if (toks[0]->ToUpper() == "RADIUS") {
//
//						int::TryParse(toks[1], x);
//						e->gauge.radius = x;
//					}
//					if (toks[0]->ToUpper() == "UNITS") {
//
//						if (toks[1]->ToLower() == "kph")
//							e->gauge.units = elementUnits_Kph;
//						if (toks[1]->ToLower() == "mph")
//							e->gauge.units = elementUnits_Mph;
//					}
//				}
//
//#pragma endregion
//
//#pragma region // parse Slider Element Section
//				if (type == LayoutDefinitionFormat::elementType_Slider) {
//					// Gauge (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//					if (toks[0]->ToUpper() == "TYPE") {
//						int::TryParse(toks[1], x);
//						e->slider.type = x;
//					}
//					if (toks[0]->ToUpper() == "BARTYPE") {
//						wchar_t c = toks[1][0];
//						e->slider.barType = (int)c;
//					}
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->slider.pos.x = x;
//						e->slider.pos.y = y;
//					}
//					if (toks[0]->ToUpper() == "SIZE") {
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->slider.size.width = x;
//						e->slider.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "VALUERANGE") {
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->slider.valRange.min = x;
//						e->slider.valRange.max = y;
//					}
//					if (toks[0]->ToUpper() == "COLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->slider.color.r = r;
//						e->slider.color.g = g;
//						e->slider.color.b = b;
//					}
//
//				}
//#pragma endregion
//
//#pragma region // parse Label Element Section
//				if (type == LayoutDefinitionFormat::elementType_Label) {
//					// Label (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->label.pos.x = x;
//						e->label.pos.y = y;
//					}
//					if (toks[0]->ToUpper() == "SIZE") {
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->label.size.width = x;
//						e->label.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "FONT") {
//						if (String::IsNullOrEmpty(toks[1])) {
//							e->label.font = "Arial";
//						}
//						else {
//							e->label.font = gcnew String(toks[1]->ToCharArray());
//						}
//					}
//					if (toks[0]->ToUpper() == "FONTSIZE") {
//						int::TryParse(toks[1], x);
//						e->label.fontSize = x;
//					}
//					if (toks[0]->ToUpper() == "FONTSTYLE") {
//						int::TryParse(toks[1], x);
//						e->label.fontStyle = x;
//					}
//					if (toks[0]->ToUpper() == "COLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->label.color.r = r;
//						e->label.color.g = g;
//						e->label.color.b = b;
//					}
//					if (toks[0]->ToUpper() == "UNITS") {
//
//						if (toks[1]->ToLower() == "kph")
//							e->label.units = elementUnits_Kph;
//						if (toks[1]->ToLower() == "mph")
//							e->label.units = elementUnits_Mph;
//					}
//				}
//#pragma endregion
//
//#pragma region // parse TextBox Element Section
//				if (type == LayoutDefinitionFormat::elementType_TextBox) {
//					// TextBox (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->txtBox.pos.x = x;
//						e->txtBox.pos.y = y;
//					}
//					if (toks[0]->ToUpper() == "SIZE") {
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->txtBox.size.width = x;
//						e->txtBox.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "FONT") {
//						if (String::IsNullOrEmpty(toks[1])) {
//							e->txtBox.font = "Arial";
//						}
//						else {
//							e->txtBox.font = gcnew String(toks[1]->ToCharArray());
//						}
//					}
//					if (toks[0]->ToUpper() == "FONTSIZE") {
//						int::TryParse(toks[1], x);
//						e->txtBox.fontSize = x;
//					}
//					if (toks[0]->ToUpper() == "FONTSTYLE") {
//						int::TryParse(toks[1], x);
//						e->txtBox.fontStyle = x;
//					}
//					if (toks[0]->ToUpper() == "COLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->txtBox.color.r = r;
//						e->txtBox.color.g = g;
//						e->txtBox.color.b = b;
//					}
//					if (toks[0]->ToUpper() == "TEXT") {
//						if (toks[1]) {
//							String^ text = gcnew String(toks[1]->ToCharArray());
//							e->txtBox.text = text;
//						}
//					}
//				}
//#pragma endregion
//
//#pragma region // parse GCircle Element
//				if (type == LayoutDefinitionFormat::elementType_gCircle) {
//					// GCircle (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gCircle.pos.x = x;
//						e->gCircle.pos.y = y;
//					}
//
//					if (toks[0]->ToUpper() == "CENTER") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gCircle.center.x = x;
//						e->gCircle.center.y = y;
//					}
//					if (toks[0]->ToUpper() == "RADIUS") {
//
//						int::TryParse(toks[1], x);
//						e->gCircle.radius = x;
//					}
//					if (toks[0]->ToUpper() == "SIZE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gCircle.size.width = x;
//						e->gCircle.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "LATRANGE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gCircle.latRange.min = x;
//						e->gCircle.latRange.max = y;
//					}
//					if (toks[0]->ToUpper() == "LONGRANGE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->gCircle.longRange.min = x;
//						e->gCircle.longRange.max = y;
//					}
//					if (toks[0]->ToUpper() == "GRIDCOLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->gCircle.GridColor.r = r;
//						e->gCircle.GridColor.g = g;
//						e->gCircle.GridColor.b = b;
//					}
//					if (toks[0]->ToUpper() == "BALLTYPE") {
//
//						int::TryParse(toks[1], x);
//						e->gCircle.BallType = x;
//					}
//					if (toks[0]->ToUpper() == "BALLSIZE") {
//
//						int::TryParse(toks[1], x);
//						e->gCircle.BallSize = x;
//					}
//					if (toks[0]->ToUpper() == "BALLCOLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->gCircle.ballColor.r = r;
//						e->gCircle.ballColor.g = g;
//						e->gCircle.ballColor.b = b;
//					}
//				}
//
//#pragma endregion
//
//#pragma region // parse Display Element
//				if (type == LayoutDefinitionFormat::elementType_Display) {
//					// Display (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//
//					if (toks[0]->ToUpper() == "DISPLAYTYPE") {
//
//						int::TryParse(toks[1], x);
//						e->display.displayType = x;
//					}
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						if (toks[1] != "") {
//							int::TryParse(toks[1], x);
//							int::TryParse(toks[2], y);
//							e->display.pos.x = x;
//							e->display.pos.y = y;
//						}
//						else {
//							e->display.pos.x = 0;
//							e->display.pos.y = 0;
//						}
//					}
//					if (toks[0]->ToUpper() == "SIZE") {
//
//						if (toks[1] != "") {
//							int::TryParse(toks[1], x);
//							int::TryParse(toks[2], y);
//							e->display.size.width = x;
//							e->display.size.height = y;
//						}
//						else {
//							e->display.size.width = 0;
//							e->display.size.height = 0;
//						}
//					}
//					//if (toks[0]->ToUpper() =="FONT") {
//					//	if (String::IsNullOrEmpty(toks[1])) {
//					//		e->display.font ="Arial";
//					//	}
//					//	else {
//					//		e->display.font =gcnew String(toks[1]->ToCharArray());
//					//	}
//					//}
//					//if (toks[0]->ToUpper() =="FONTSIZE") {
//					//	int::TryParse(toks[1], x);
//					//	e->display.fontSize =x;
//					//}
//					if (toks[0]->ToUpper() == "COLOR") {
//						int r, g, b;
//
//						if (toks[1] != "") {
//							int::TryParse(toks[1], r);
//							int::TryParse(toks[2], g);
//							int::TryParse(toks[3], b);
//							e->display.color.r = r;
//							e->display.color.g = g;
//							e->display.color.b = b;
//						}
//						else {
//							e->display.size.width = 0;
//							e->display.color.r = 0;
//							e->display.color.g = 0;
//							e->display.color.b = 0;
//						}
//					}
//				}
//
//#pragma endregion
//
//#pragma region // parse Sweeper Element
//				if (type == LayoutDefinitionFormat::elementType_Sweeper) {
//					// Display (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						if (toks[1] != "") {
//							int::TryParse(toks[1], x);
//							int::TryParse(toks[2], y);
//							e->sweeper.pos.x = x;
//							e->sweeper.pos.y = y;
//						}
//						else {
//							e->sweeper.pos.x = 0;
//							e->sweeper.pos.y = 0;
//						}
//					}
//
//					if (toks[0]->ToUpper() == "SIZE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->sweeper.size.width = x;
//						e->sweeper.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "VALUERANGE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->sweeper.values.min = x;
//						e->sweeper.values.max = y;
//					}
//					if (toks[0]->ToUpper() == "ANGLES") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->sweeper.angles.startAngle = x;
//						e->sweeper.angles.sweepAngle = y;
//					}
//					if (toks[0]->ToUpper() == "THRESHOLDVALUE") {
//
//						int::TryParse(toks[1], x);
//						e->sweeper.threshVal = x;
//					}
//					if (toks[0]->ToUpper() == "COLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->sweeper.color.r = r;
//						e->sweeper.color.g = g;
//						e->sweeper.color.b = b;
//					}
//					if (toks[0]->ToUpper() == "THRESHOLDCOLOR") {
//						int r, g, b;
//
//						int::TryParse(toks[1], r);
//						int::TryParse(toks[2], g);
//						int::TryParse(toks[3], b);
//						e->sweeper.threshColor.r = r;
//						e->sweeper.threshColor.g = g;
//						e->sweeper.threshColor.b = b;
//					}
//					if (toks[0]->ToUpper() == "WIDTH") {
//						float w = 0;
//						float::TryParse(toks[1], w);
//						e->sweeper.width = w;
//					}
//
//				}
//
//#pragma endregion
//
//#pragma region // parse LED Element
//				if (type == LayoutDefinitionFormat::elementType_Led) {
//					// LED (Element) section
//					cli::array<String^>^ toks = lines[i]->Split(sep);
//
//					if (toks[0]->ToUpper() == "POSITION") {
//
//						if (toks[1] != "") {
//							int::TryParse(toks[1], x);
//							int::TryParse(toks[2], y);
//							e->led.pos.x = x;
//							e->led.pos.y = y;
//						}
//						else {
//							e->led.pos.x = 0;
//							e->led.pos.y = 0;
//						}
//					}
//
//					if (toks[0]->ToUpper() == "SIZE") {
//
//						int::TryParse(toks[1], x);
//						int::TryParse(toks[2], y);
//						e->led.size.width = x;
//						e->led.size.height = y;
//					}
//					if (toks[0]->ToUpper() == "VALON") {
//
//						int::TryParse(toks[1], y);
//						e->led.onOffVals.max = y;
//					}
//					if (toks[0]->ToUpper() == "VALOFF") {
//
//						int::TryParse(toks[1], x);
//						e->led.onOffVals.min = x;
//					}
//					if (toks[0]->ToUpper() == "FILEON") {
//
//						e->led.imgFileOn = toks[1];
//					}
//					if (toks[0]->ToUpper() == "FILEOFF") {
//
//						int::TryParse(toks[1], x);
//						e->led.imgFileOff = toks[1];
//					}
//				}
//
//#pragma endregion
//
//
//				i++;
//				if (i == this->lines->Count)
//					break;
//			}
//			this->m_dashElementList->Add(e);
//		}
//
//	};
//

//} //libLDF
#pragma endregion
