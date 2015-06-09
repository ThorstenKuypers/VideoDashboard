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

#include "DashboardLayout.h"


using namespace libLDF;

CDashboardLayout::CDashboardLayout()
{
	GdiplusStartupInput gdiStartup;

	GdiplusStartup(&_gdiplus_tok, &gdiStartup, NULL);
	//_dashboards = std::map<string, CDashboard*>();
	_curDashboard = nullptr;
	_dataLogger = nullptr;
}


CDashboardLayout::~CDashboardLayout()
{
	if (_curDashboard != nullptr) {

		delete _curDashboard;
		_curDashboard = nullptr;
	}

	GdiplusShutdown(_gdiplus_tok);
}

void CDashboardLayout::ShowBoundingBoxes(bool show)
{
	if (_curDashboard != nullptr)
		_curDashboard->ShowBoundingBoxes(show);
}

CDashboard* CDashboardLayout::GetDashboard(string& name)
{
	// OLD
	CDashboard* ptr = nullptr;

	if (_curDashboard != nullptr)
		ptr = _curDashboard;

	return ptr;
}

/*Bitmap**/ _GDI_BMP CDashboardLayout::RenderDashboard(string& name, int sampleIndex)
{
	// NEW
	CDashboard* dash = nullptr;
	if (_curDashboard != nullptr)
		dash = _curDashboard;

	if (dash != nullptr) {

		dash->SetDataLogger(_dataLogger);

		Gdiplus::Bitmap* bmp = dash->RenderToImage(sampleIndex);

		return (_GDI_BMP)bmp;
	}

	return nullptr;
}

std::string CDashboardLayout::GetDashboardPrettyName(std::string& filename)
{
	// NEW
	if (_curDashboard != nullptr)
		return _curDashboard->GetPrettyName();

	return string("");
}

std::string CDashboardLayout::GetDashboardShortName(std::string& filename)
{
	//NEW
	if (_curDashboard != nullptr)
		return _curDashboard->GetShortName();

	return string("");
}

void CDashboardLayout::SetDataLogger(IDataLogger* logger)
{
	_dataLogger = logger;

	//NEW 
	if (_curDashboard != nullptr)
		_curDashboard->SetDataLogger(_dataLogger);
}

void CDashboardLayout::extractDashboardFilePath()
{
	if (!_curDashboardFileName.empty())
	{
		size_t pos = 0;
		std::string path;

		pos = _curDashboardFileName.rfind("\\");
		if (pos != std::string::npos)
		{
			_curDashboardFilePath = _curDashboardFileName.substr(0, pos + 1);
		}

	}
}

void CDashboardLayout::ParseLayoutFile(std::string filename)
{
	CDashboard* dash = nullptr;
	std::ifstream fs(filename, ios_base::in);
	std::vector<std::string> lines;
	std::vector<ldf_line> _lines;

	_curDashboardFileName = filename;

	extractDashboardFilePath();

	std::string line;
	int lineNo = 0;
	do {

		std::getline(fs, line);
		lineNo++;

		size_t pos = line.find_first_not_of("\t ");
		// skip empty lines
		if (pos == std::string::npos)
			continue;
		if (pos != 0)
		{
			//pos -= 1;
			line.erase(0, pos);
		}

		// skip pure comment lines
		if (line[0] == '/')
			continue;
		else if (line[0] == '/' && line[1] == '/')
			continue;
		//else if (line[0] == '#')
		//	continue;

		pos = line.find_last_not_of("\t: ");
		pos += 1;
		if (pos != std::string::npos && pos != line.length()) {

			line.erase(pos, line.length() - pos);
		}

		ldf_line ln = make_pair(lineNo, line);
		_lines.push_back(ln);

		///////////////////////////////////////
		lines.push_back(line);

	} while (!fs.eof());
	fs.close();

	// 2. step -> iterate through all lines and find elements and positions for 
	// start and end of elements
	std::vector<elementSection> sections; // sections vector (name, startLine, endLine)
	int secStart = 0, secStop = 0;
	std::string sec;
	size_t pos = 0, pos2 = 0;
	elementSection s;
	bool sectionOpen = false;

	for (size_t i = 0; i < _lines.size(); i++) {

		if ((pos = _lines[i].second.rfind("{")) != std::string::npos) {

			if (sectionOpen) {

				// throw error since section close bracket is missing
				throw ParsingException(parsing_error::sectionNotClosed, _lines[i].first, sec.c_str(), filename.c_str());
			}
			// check for characters in front of '{'
			pos2 = _lines[i].second.find_last_not_of("\t{ ", pos);
			if (pos2 < pos)
			{
				// characters before '{' found so it is at end of line
				// opening bracket in same line
				sec = std::string(_lines[i].second, 0, pos2 + 1);
			}
			else {
				// no characters before '{' so begin of line
				// opening bracket at begin of line
				sec = _lines[i - 1].second;
			}
			secStart = 1;
			sectionOpen = true;
		}

		else if (_lines[i].second.find('}') != std::string::npos) {

			if (!sectionOpen) {

				// throw error since section open bracket is missing
				throw ParsingException(parsing_error::sectionNotOpened, _lines[i].first, sec.c_str(), filename.c_str());
			}

			size_t _s = sec.find_first_not_of("\t ");
			size_t _e = sec.find_last_not_of("\t ", _s);
			if (_e > 0)
				sec = std::string(sec, _s, _e + 1);
			s.name = sec;
			sections.push_back(s);

			s.lines = std::vector < ldf_line >();
			secStart = 0;
			secStop = 0;
			sectionOpen = false;
		}

		else {
			if (!secStart)
				continue;

			if (!sectionOpen) {

				// throw error since section was not opened
				throw ParsingException(parsing_error::sectionNotOpened, _lines[i].first, sec.c_str(), filename.c_str());
			}

			s.lines.push_back(_lines[i]);
		}
	}

	string str;

	// 3. step -> split sections and extract raw information from it
	//for (unsigned int i = 0; i < sections.size(); i++) {
	for (auto sec : sections) {
		//readSection(lines, sections[i]);
		parseSections(sec);

		CDashboardElement* ptr = nullptr;

		// 4. step -> parse section information and create actual element objects 
		if (sec.name == std::string("dashboard")) {

			dash = parseDashboardSection(sec);
		}
		else if (sec.name == string("textbox")) {

			ptr = (CDashboardElement*)parseTextBoxSection(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);
		}
		else if (sec.name == string("gauge")) {

			ptr = (CDashboardElement*)parseGaugeSection(sec);
			//			((CGauge*)ptr)->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);

			ptr->Init();
		}
		else if (sec.name == string("indicator")) {

			ptr = (CDashboardElement*)parseIndicatorSection(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);

			ptr->Init();
		}
		else if (sec.name == string("slider")) {

			ptr = (CDashboardElement*)parseSliderSection(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);

			ptr->Init();
		}
		else if (sec.name == string("gcircle")) {

			ptr = (CDashboardElement*)parseGCircleSection(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);
		}
		else if (sec.name == string("ringgauge")) {

			ptr = (CDashboardElement*)parseRingGaugeElement(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			//checkElementLocalSettingsOverwrite(ptr, dash);			
		}
		else if (sec.name == string("sweeper")) {

			ptr = (CDashboardElement*)parseSweeperElement(sec);
			//			ptr->SetDashboardFilePath(_curDashboardFilePath);
			checkElementLocalSettingsOverwrite(ptr, dash);

			((CSweeper*)ptr)->Precalculate();
		}
		else if (sec.name == string("ruler")) {

			CRuler* p = parseRulerElement(sec);

			// check if this ruler element belongs to another element (by using property) or not
			if (p->GetUsingElement() == string(""))
			{
				// this ruler element is a standalone element just like all the other elements
				// so initialize it and add it to the elements list
				p->Init();

				// add this ruler element to the elements list here 
				dash->AddDashboardElement((CDashboardElement*)p);
			}
			else {
				// iterate through all elements and find the ID to which this ruler element is assigned to
				std::vector<CDashboardElement*> elements = dash->GetDashboardElements();

				for (size_t j = 0; j < elements.size(); j++) {

					if (elements[j]->GetID() == p->GetUsingElement()) {

						elements[j]->SetRuler(p);
						//Rect rc = elements[j]->GetRectangle();
						//p->SetRectangle(rc);
						p->Init();

					}
				}
			}
		}
		if (sec.name != string("dashboard") && sec.name != string("ruler"))
			dash->AddDashboardElement(ptr);
	}

	// NEW (test)
	if (_curDashboard != nullptr) {

		delete _curDashboard;
		_curDashboard = nullptr;
	}
	_curDashboard = dash;

	//fs.close();
}

void CDashboardLayout::parseSections(elementSection& sec)
{
	size_t end = 0;
	int j = 0;

	for (auto ln : sec.lines) {

		j = 0;
		end = ln.second.find(';');
		if (end == std::string::npos) {

			throw ParsingException(parsing_error::missingSemicolon, ln.first, sec.name.c_str(), _curDashboardFileName.c_str());
		}

		for (auto it = ln.second.cbegin(); it != ln.second.cend(); ++it, j++) {

			if (*it == '=') {
				string k(ln.second, 0, j);
				// remove whitespaces from key string
				//int s = 0, e = 0;
				//s = k.find_first_of(" \t");
				//if (s == 0)
				//	s += 1;
				//else if (s == std::string::npos)
				//	s = 0;
				//e = k.find_last_of(" \t");
				//if (s == e)
				//	s = 0;
				//k = std::string(k, s, e - s);

				string v(ln.second, j + 1, end - j - 1);
				trim(k, " \t");
				trim(v, " \t");
				std::transform(k.begin(), k.end(), k.begin(), tolower);

				if (v == "")
					throw ParsingException(parsing_error::emptyKeyValue, ln.first, sec.name.c_str(), _curDashboardFileName.c_str());

				// NEW 
				ldf_pair lp;
				lp.first = k;
				lp.second = v;
				sec.keyPairs[ln.first] = lp;
			}
		}
	}
}

void CDashboardLayout::checkElementLocalSettingsOverwrite(CDashboardElement* e, CDashboard* d)
{
	if (e->UseGlobalBackgroundColor())
		e->SetBackgroundColor(d->GetBackgroundColor());
	if (e->UseGlobalFontName())
		e->SetFontName(d->GetFontName());
	if (e->UseGlobalFontStyle())
		e->SetFontStyle(d->GetFontStyle());
	if (e->UseGlobalForegroundColor())
		e->SetForegroundColor(d->GetForegroundColor());
	if (e->UseGlobalTextJustify())
		e->SetTextJustification(d->GetTextJustification());

}

//void CDashboardLayout::readSection(std::vector<std::string>& lines, elementSection& section)
//{
//	int end = 0;
//
//	for (int i = section.begin; i < section.end; i++) {
//		for (unsigned int j = 0; j < lines[i].size(); j++) {
//			if (lines[i][j] == '=') {
//				if ((end = lines[i].find_first_of(';')) != std::string::npos) {
//
//					string k(lines[i], 0, j);
//					// remove whitespaces from key string
//					int s = 0, e = 0;
//					s = k.find_first_of(" \t");
//					if (s == 0)
//						s += 1;
//					else if (s == std::string::npos)
//						s = 0;
//					e = k.find_last_of(" \t");
//					if (s == e)
//						s = 0;
//					k = std::string(k, s, e - s);
//
//					string v(lines[i], j + 1, end - j - 1);
//
//					std::transform(k.begin(), k.end(), k.begin(), tolower);
//					section.pairs[k] = v;
//				}
//			}
//		}
//	}
//
//}


CDashboard* CDashboardLayout::parseDashboardSection(elementSection& section)
{
	CDashboard* dash = new CDashboard();
	dash->SetDashboardFilePath(_curDashboardFilePath);
	int _line = 0;

	try
	{
		for (auto kp : section.keyPairs) {

			_line = kp.first;

			// remove whitespaces
			ldf_pair lp = kp.second;
			lp.second = trim(lp.second, " \t\"");

			std::string key = lp.first;
			std::transform(key.begin(), key.end(), key.begin(), tolower);

			if (key == string("shortname")) {

				dash->SetShortName(lp.second);
			}
			else if (lp.first == std::string("prettyname")) {

				dash->SetPrettyName(lp.second);
			}
			else if (lp.first == std::string("imagefile")) {

				dash->SetImageFile(lp.second);
			}
			else if (lp.first == std::string("targetwidth")) {

				dash->SetTargetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("width")) {

				dash->SetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("height")) {

				dash->SetHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("foreground")) {
				if (dash != nullptr) {
					dash->SetForegroundColor(lp.second);
				}
			}
			else if (lp.first == std::string("background")) {
				if (dash != nullptr) {
					dash->SetBackgroundColor(lp.second);
				}
			}
			else if (lp.first == std::string("fontname")) {

				dash->SetFontName(lp.second);
			}
			else if (lp.first == std::string("fontstyle")) {

				dash->SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				dash->SetFontHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("justify")) {

				dash->SetTextJustification(lp.second);
			}
		}
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, section.name.c_str(), _curDashboardFileName.c_str());
	}
	return dash;
}

void CDashboardLayout::parseCommonKeys(CDashboardElement* e, elementSection& sect)
{
	int _line = 0;
	ldf_pair lp;

	try
	{
		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;
			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("background")) {

				e->SetBackgroundColor(lp.second);
			}
			else if (lp.first == string("foreground")) {

				e->SetForegroundColor(lp.second);
			}
			else if (lp.first == string("fontname")) {

				e->SetFontName(lp.second);
			}
			else if (lp.first == string("fontstyle")) {

				e->SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				e->SetFontHeight(stoi(lp.second));
			}
			else if (lp.first == string("justify")) {

				e->SetTextJustification(lp.second);
			}
			else if (lp.first == string("scale")) {

				double f = stod(lp.second);
				e->SetScale(f);
			}
			else if (lp.first == string("channel") || lp.first == string("variable")) {

				e->SetChannel(lp.second);
			}
			else if (lp.first == string("layer")) {

				e->SetLayer(stoi(lp.second));
			}
			else if (lp.first == string("rectangle")) {

				e->SetRectangle(lp.second);
			}
			else if (lp.first == string("id")) {

				e->SetID(lp.second);
			}
			else if (lp.first == string("position")) {

				e->SetPosition(lp.second);
			}
			//else if (lp.first == string("using")) {

			//}
		}
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}
}

CTextBox* CDashboardLayout::parseTextBoxSection(elementSection& section)
{
	CTextBox* tb = new CTextBox();
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(tb, section);

		for (auto kp : section.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("format")) {

				tb->SetFormatType(lp.second);
			}
			else if (lp.first == string("label")) {

				tb->SetLabel(lp.second);
			}
			else if (lp.first == string("draw_zero")) {

				tb->SetDrawZero(lp.second);
			}
			else if (lp.first == string("signed")) {

				tb->SetSigned(lp.second);
			}
			else if (lp.first == string("precision")) {

				tb->SetPrecision(stoi(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, section.name.c_str(), _curDashboardFileName.c_str());
	}
	return tb;
}

CGauge* CDashboardLayout::parseGaugeSection(elementSection& sect)
{
	CGauge* g = new CGauge();
	int _line = 0;
	ldf_pair lp;

	try {
		g->SetDashboardFilePath(_curDashboardFilePath);
		parseCommonKeys(g, sect);
		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("range")) {

				g->SetRange(lp.second);
			}
			else if (lp.first == string("divisions")) {

				g->SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("radius")) {

				g->SetRadius(stoi(lp.second));
			}
			else if (lp.first == string("face_image")) {

				g->SetImageFile(lp.second);
			}
			else if (lp.first == string("needle_image")) {

				g->SetNeedleImageFile(lp.second);
			}
			else if (lp.first == string("sweep")) {

				g->SetSweepAngle(lp.second);
			}
			//else if (lp.first == string("rotation")) {

			//	
			//}
			else if (lp.first == string("position")) {

				g->SetPosition(lp.second);
			}
			else if (lp.first == string("needle_center")) {

				g->SetNeedleCenter(lp.second);
			}
			else if (lp.first == string("needle_offset")) {

				g->SetNeedleOffset(lp.second);
			}
			else if (lp.first == string("draw_face")) {

				if (lp.second == string("yes") || lp.second == string("true")) {
					g->SetUseImage(true);
				}
				else{
					g->SetUseImage(false);
				}
			}
			else if (lp.first == string("precision")) {

				g->SetPrecision(stoi(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return g;
}

CIndicator* CDashboardLayout::parseIndicatorSection(elementSection& sect)
{
	CIndicator* p = new CIndicator();
	int _line = 0;
	ldf_pair lp;

	try {
		p->SetDashboardFilePath(_curDashboardFilePath);

		parseCommonKeys(p, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("rgbon")) {

				p->SetColorON(lp.second);
			}
			else if (lp.first == string("rgboff")) {

				p->SetColorOff(lp.second);
			}
			else if (lp.first == string("threshold")) {

				p->SetThresholdValue(stof(lp.second));
			}
			else if (lp.first == string("shape")) {

				p->SetShape(lp.second);
			}
			else if (lp.first == string("image_on")) {

				p->SetOnImage(lp.second);
			}
			else if (lp.first == string("image_off")) {

				p->SetOffImage(lp.second);
			}
			else if (lp.first == string("thickness")) {

				p->SetThickness(stoi(lp.second));
			}
			else if (lp.first == string("filled")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p->SetFilled(true);
				else
					p->SetFilled(false);
			}
			else if (lp.first == string("outlined")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p->SetOutlined(true);
				else
					p->SetOutlined(false);
			}
			else if (lp.first == string("shaded")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p->SetShaded(true);
				else
					p->SetShaded(false);
			}
			else if (lp.first == string("unshaded")) {

				p->SetUnshadedValue(stof(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return p;
}

CSlider* CDashboardLayout::parseSliderSection(elementSection& sect)
{
	CSlider* ptr = new CSlider();
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(ptr, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("range")) {

				ptr->SetRange(lp.second);
			}
			else if (lp.first == string("rgbpositive")) {

				ptr->SetColorPositive(lp.second);
			}
			else if (lp.first == string("rgbnegative")) {

				ptr->SetColorNegative(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return ptr;
}

CGCircle* CDashboardLayout::parseGCircleSection(elementSection& sect)
{
	CGCircle* ptr = new CGCircle();
	int _line = 0;
	ldf_pair lp;

	try {

		parseCommonKeys(ptr, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("divisions")) {

				ptr->SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("radius")) {

				ptr->SetRadius(stoi(lp.second));
			}
			else if (lp.first == string("line_width")) {

				ptr->SetLineWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer")) {

				ptr->SetPointerColor(lp.second);
			}
			else if (lp.first == string("pointer_width")) {

				ptr->SetPointerWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer_size")) {

				ptr->SetPointerSize(stoi(lp.second));
			}
			else if (lp.first == string("scale_lat_accel")) {

				ptr->SetLateralScaleFactor(stof(lp.second));
			}
			else if (lp.first == string("scale_lon_accel")) {

				ptr->SetLongitudinalScaleFactor(stof(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return ptr;
}

CSweeper* CDashboardLayout::parseSweeperElement(elementSection& sect)
{
	CSweeper* ptr = new CSweeper;
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(ptr, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("path")) {

				ptr->SetPathType(lp.second);
			}
			else if (lp.first == string("range")) {

				ptr->SetRange(lp.second);
			}
			else if (lp.first == string("sweep")) {

				ptr->SetSweepAngle(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				ptr->SetRectangle(lp.second);
			}
			else if (lp.first == string("line_start")) {

				ptr->SetLineStart(lp.second);
			}
			else if (lp.first == string("line_end")) {

				ptr->SetLineEnd(lp.second);
			}
			else if (lp.first == string("shape")) {

				ptr->SetShapeType(lp.second);
			}
			else if (lp.first == string("extend")) {

				ptr->SetExtend(stoi(lp.second));
			}
			else if (lp.first == string("thickness")) {

				ptr->SetThickness(stoi(lp.second));
			}
			else if (lp.first == string("taper")) {

				ptr->SetTaper(stoi(lp.second));
			}
			else if (lp.first == string("divisions")) {

				ptr->SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("rgboff")) {

				ptr->SetColorOFF(lp.second);
			}
			else if (lp.first == string("rgbon")) {

				removeChar(lp.second, ' ');
				if (lp.second[0] == '<' && lp.second[1] == '<')
					ptr->SetThresholdColors(lp.second);
				else
					ptr->SetColorON(lp.second);
			}
			else if (lp.first == string("threshold")) {

				ptr->SetThreshold(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return ptr;
}

CRuler* CDashboardLayout::parseRulerElement(elementSection& sect)
{
	CRuler* p = new CRuler();
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(p, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("using")) {

				p->SetUsingElement(lp.second);
			}
			else if (lp.first == string("path")) {

				p->SetPathType(lp.second);
			}
			else if (lp.first == string("draw_path")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p->SetDrawPath(b);
			}
			else if (lp.first == string("path_color")) {

				p->SetPathColor(lp.second);
			}
			else if (lp.first == string("path_offset")) {

				p->SetPathOffset(stoi(lp.second));
			}
			else if (lp.first == string("path_width")) {

				p->SetPathWidth(stoi(lp.second));
			}
			else if (lp.first == string("path_range")) {

				p->SetPathRange(lp.second);
			}
			else if (lp.first == string("path_position")) {

				p->SetPathPosition(lp.second);
			}
			else if (lp.first == string("draw_label")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p->SetDrawLabel(b);
			}
			else if (lp.first == string("label_margin")) {

				p->SetLabelMargin(stoi(lp.second));
			}
			else if (lp.first == string("label_format")) {

				//p-(lp.second);
			}
			else if (lp.first == string("line_start")) {

				p->SetLineStart(lp.second);
			}
			else if (lp.first == string("line_end")) {

				p->SetLineEnd(lp.second);
			}
			//else if (lp.first == string("arc_start")) {

			//	
			//}
			//else if (lp.first == string("arc_end")) {

			//}
			else if (lp.first == string("arc_sweep")) {

				p->SetArcSweep(lp.second);
			}
			else if (lp.first == string("draw_major")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p->SetDrawMajor(b);
			}
			else if (lp.first == string("major_divisions")) {

				p->SetMajorDivisions(stoi(lp.second));
			}
			else if (lp.first == string("major_units")) {

				p->SetMajorUnits(stof(lp.second));
			}
			else if (lp.first == string("major_margin")) {

				p->SetMajorMargin(stoi(lp.second));
			}
			else if (lp.first == string("major_color")) {

				p->SetMajorColor(lp.second);
			}
			else if (lp.first == string("major_shape")) {

				p->SetMajorShape(lp.second);
			}
			else if (lp.first == string("major_length")) {

				p->SetMajorLength(stoi(lp.second));
			}
			else if (lp.first == string("major_width")) {

				p->SetMajorWidth(stoi(lp.second));
			}
			else if (lp.first == string("major_radius")) {

				p->SetMajorRadius(stoi(lp.second));
			}
			else if (lp.first == string("draw_minor")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p->SetDrawMinor(b);
			}
			else if (lp.first == string("minor_divisions")) {

				p->SetMinorDivisions(stoi(lp.second));
			}
			else if (lp.first == string("minor_units")) {

				p->SetMinorUnits(stof(lp.second));
			}
			else if (lp.first == string("minor_margin")) {

				p->SetMinorMargin(stoi(lp.second));
			}
			else if (lp.first == string("minor_color")) {

				p->SetMinorColor(lp.second);
			}
			else if (lp.first == string("minor_shape")) {

				p->SetMinorShape(lp.second);
			}
			else if (lp.first == string("minor_length")) {

				p->SetMinorLength(stoi(lp.second));
			}
			else if (lp.first == string("minor_width")) {

				p->SetMinorWidth(stoi(lp.second));
			}
			else if (lp.first == string("minor_radius")) {

				p->SetMinorRadius(stoi(lp.second));
			}
			else if (lp.first == string("foreground")) {

				p->SetForegroundColor(lp.second);
			}
			else if (lp.first == string("background")) {

				p->SetBackgroundColor(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				p->SetFontHeight(stoi(lp.second));
			}
			else if (lp.first == string("fontstyle")) {

				p->SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontname")) {

				p->SetFontName(lp.second);
			}
			else if (lp.first == string("layer")) {

				p->SetLayer(stoi(lp.second));
			}
			else if (lp.first == string("transparency")) {

				p->SetTransparency(stof(lp.second));
			}
			else if (lp.first == string("scale")) {

				p->SetScalingFactor(stod(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return p;
}


CRingGauge* CDashboardLayout::parseRingGaugeElement(elementSection& sect)
{
	CRingGauge* ptr = new CRingGauge();
	int _line = 0;
	ldf_pair lp;

	try {

		parseCommonKeys(ptr, sect);

		for (auto kp : sect.keyPairs)
		{
			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("id")) {

				ptr->SetID(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				ptr->SetRectangle(lp.second);
			}
			else if (lp.first == string("range")) {

				ptr->SetRange(lp.second);
			}
			else if (lp.first == string("width")) {

				ptr->SetRingWidth(lp.second);
			}
			else if (lp.first == string("sweep")) {

				ptr->SetSweep(lp.second);
			}
			else if (lp.first == string("rotation")) {

				ptr->SetRotation(lp.second);
			}
			else if (lp.first == string("foreground")) {

				ptr->SetForegroundColor(lp.second);
			}
			else if (lp.first == string("shaded")) {

				if ((lp.second == "yes") || (lp.second == "true"))
					ptr->SetShaded(true);
				else
					ptr->SetShaded(false);
			}
			else if (lp.first == string("marker_width")) {

				ptr->SetMarkerWidth(lp.second);
			}
			else if (lp.first == string("marker_color")) {

				ptr->SetMarkerColor(lp.second);
			}
			else if (lp.first == string("marker_hold_time")) {

				ptr->SetMarkerHoldTime(lp.second);
			}
			else if (lp.first == string("marker_decay")) {

				ptr->SetMarkerDecay(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _curDashboardFileName.c_str());
	}

	return ptr;
}