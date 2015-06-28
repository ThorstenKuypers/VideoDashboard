///////////////////////////////////////////////////////////////////////////////
//
//	VideoDashboard
//	----------------------
//	Project: libLDF - layout definition format library
//
//	Copyright 2014-2015 Thorsten Kuypers
//  All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

#include "DashboardLayout.h"


using namespace libLDF;
using namespace Gdiplus;

CDashboardLayout::CDashboardLayout() :
_curDashboard(),
_curDashboardFileName(),
_curDashboardFilePath(),
_gdiplus_tok(0)
{
	GdiplusStartupInput gdiStartup;

	GdiplusStartup(&_gdiplus_tok, &gdiStartup, NULL);
}


CDashboardLayout::~CDashboardLayout()
{
	GdiplusShutdown(_gdiplus_tok);
}

void CDashboardLayout::ShowBoundingBoxes(bool show)
{
	_curDashboard.ShowBoundingBoxes(show);
}

Gdiplus::Bitmap* CDashboardLayout::RenderDashboard(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank)
{
	return _curDashboard.RenderToImage(sample, logger, renderBlank);
}

Gdiplus::Bitmap* CDashboardLayout::RenderDashboard(IGenericLogger& logger, int sampleIndex, bool renderBlank)
{
	DataSample s;
	if (!renderBlank) {
		logger.GetSample(s, sampleIndex);
	}

	return _curDashboard.RenderToImage(s, logger, renderBlank);
}

std::string& CDashboardLayout::GetDashboardPrettyName(std::string& filename)
{
	return _curDashboard.GetPrettyName();
}

std::string& CDashboardLayout::GetDashboardShortName(std::string& filename)
{
	return _curDashboard.GetShortName();
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

void CDashboardLayout::SetActiveDashboard(const std::string& filename)
{
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

	// clear previous dashboard element
	_curDashboard.clear();

	// 3. step -> split sections and extract raw information from it
	//for (unsigned int i = 0; i < sections.size(); i++) {
	for (auto sec : sections) {
		//readSection(lines, sections[i]);
		parseSections(sec);

		// 4. step -> parse section information and create actual element objects 
		if (sec.name == std::string("dashboard")) {

			parseDashboardSection(sec);
		}
		else if (sec.name == string("textbox")) {

			parseTextBoxSection(sec);
		}
		else if (sec.name == string("gauge")) {

			parseGaugeSection(sec);
		}
		else if (sec.name == string("indicator")) {

			parseIndicatorSection(sec);
		}
		else if (sec.name == string("slider")) {

			parseSliderSection(sec);
		}
		else if (sec.name == string("gcircle")) {

			parseGCircleSection(sec);
		}
		else if (sec.name == string("ringgauge")) {

			parseRingGaugeElement(sec);
		}
		else if (sec.name == string("sweeper")) {

			parseSweeperElement(sec);
		}
		else if (sec.name == string("ruler")) {

			parseRulerElement(sec);

		}
	}

	// iterate through all elements and find the elements that have a ruler element assigned
	std::vector<CDashboardElement> elements = _curDashboard.GetDashboardElements();

	for (size_t i = 0; i < elements.size(); i++) {
		CDashboardElement* e = &elements[i];

		// iterate through the ruler lookup table
		for (auto r : rulerTable) {
			if (e->GetID() == r.second.GetUsingElement()) {

				// initialize ruler element
				e->SetRuler(r.second);
			}
			r.second.Init();
		}
	}

#ifdef _DEBUG
	int x = 0;
#endif
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

void CDashboardLayout::checkElementLocalSettingsOverwrite(CDashboardElement& e)
{
	if (e.UseGlobalBackgroundColor())
		e.SetBackgroundColor(_curDashboard.GetBackgroundColor());
	if (e.UseGlobalFontName())
		e.SetFontName(_curDashboard.GetFontName());
	if (e.UseGlobalFontStyle())
		e.SetFontStyle(_curDashboard.GetFontStyle());
	if (e.UseGlobalForegroundColor())
		e.SetForegroundColor(_curDashboard.GetForegroundColor());
	if (e.UseGlobalTextJustify())
		e.SetTextJustification(_curDashboard.GetTextJustification());

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


void CDashboardLayout::parseDashboardSection(elementSection& section)
{
	_curDashboard.SetDashboardFilePath(_curDashboardFilePath);
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

				_curDashboard.SetShortName(lp.second);
			}
			else if (lp.first == std::string("prettyname")) {

				_curDashboard.SetPrettyName(lp.second);
			}
			else if (lp.first == std::string("imagefile")) {

				_curDashboard.SetImageFile(lp.second);
			}
			else if (lp.first == std::string("targetwidth")) {

				_curDashboard.SetTargetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("width")) {

				_curDashboard.SetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("height")) {

				_curDashboard.SetHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("foreground")) {
				_curDashboard.SetForegroundColor(lp.second);
			}
			else if (lp.first == std::string("background")) {
				_curDashboard.SetBackgroundColor(lp.second);
			}
			else if (lp.first == std::string("fontname")) {

				_curDashboard.SetFontName(lp.second);
			}
			else if (lp.first == std::string("fontstyle")) {

				_curDashboard.SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				_curDashboard.SetFontHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("justify")) {

				_curDashboard.SetTextJustification(lp.second);
			}
		}
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, section.name.c_str(), _curDashboardFileName.c_str());
	}
}

void CDashboardLayout::parseCommonKeys(CDashboardElement& e, elementSection& sect)
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

				e.SetBackgroundColor(lp.second);
			}
			else if (lp.first == string("foreground")) {

				e.SetForegroundColor(lp.second);
			}
			else if (lp.first == string("fontname")) {

				e.SetFontName(lp.second);
			}
			else if (lp.first == string("fontstyle")) {

				e.SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				e.SetFontHeight(stoi(lp.second));
			}
			else if (lp.first == string("justify")) {

				e.SetTextJustification(lp.second);
			}
			else if (lp.first == string("scale")) {

				double f = stod(lp.second);
				e.SetScale(f);
			}
			else if (lp.first == string("channel") || lp.first == string("variable")) {

				e.SetChannel(lp.second);
			}
			else if (lp.first == string("layer")) {

				e.SetLayer(stoi(lp.second));
			}
			else if (lp.first == string("rectangle")) {

				e.SetRectangle(lp.second);
			}
			else if (lp.first == string("id")) {

				e.SetID(lp.second);
			}
			else if (lp.first == string("position")) {

				e.SetPosition(lp.second);
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

void CDashboardLayout::parseTextBoxSection(elementSection& section)
{
	CTextBox tb;
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(tb, section);

		for (auto kp : section.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("format")) {

				tb.SetFormatType(lp.second);
			}
			else if (lp.first == string("label")) {

				tb.SetLabel(lp.second);
			}
			else if (lp.first == string("draw_zero")) {

				tb.SetDrawZero(lp.second);
			}
			else if (lp.first == string("signed")) {

				tb.SetSigned(lp.second);
			}
			else if (lp.first == string("precision")) {

				tb.SetPrecision(stoi(lp.second));
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

	checkElementLocalSettingsOverwrite(tb);
	tb.Init();
	_curDashboard.AddDashboardElement(tb);
}

void CDashboardLayout::parseGaugeSection(elementSection& sect)
{
	CGauge g;
	int _line = 0;
	ldf_pair lp;

	try {
		g.SetDashboardFilePath(_curDashboardFilePath);
		parseCommonKeys(g, sect);
		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("range")) {

				g.SetRange(lp.second);
			}
			else if (lp.first == string("divisions")) {

				g.SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("radius")) {

				g.SetRadius(stoi(lp.second));
			}
			else if (lp.first == string("face_image")) {

				g.SetImageFile(lp.second);
			}
			else if (lp.first == string("needle_image")) {

				g.SetNeedleImageFile(lp.second);
			}
			else if (lp.first == string("sweep")) {

				g.SetSweepAngle(lp.second);
			}
			//else if (lp.first == string("rotation")) {

			//	
			//}
			else if (lp.first == string("position")) {

				g.SetPosition(lp.second);
			}
			else if (lp.first == string("needle_center")) {

				g.SetNeedleCenter(lp.second);
			}
			else if (lp.first == string("needle_offset")) {

				g.SetNeedleOffset(lp.second);
			}
			else if (lp.first == string("draw_face")) {

				if (lp.second == string("yes") || lp.second == string("true")) {
					g.SetUseImage(true);
				}
				else{
					g.SetUseImage(false);
				}
			}
			else if (lp.first == string("precision")) {

				g.SetPrecision(stoi(lp.second));
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

	checkElementLocalSettingsOverwrite(g);
	g.Init();
	_curDashboard.AddDashboardElement(g);
}

void CDashboardLayout::parseIndicatorSection(elementSection& sect)
{
	CIndicator p;
	int _line = 0;
	ldf_pair lp;

	try {
		p.SetDashboardFilePath(_curDashboardFilePath);

		parseCommonKeys(p, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("rgbon")) {

				p.SetColorON(lp.second);
			}
			else if (lp.first == string("rgboff")) {

				p.SetColorOff(lp.second);
			}
			else if (lp.first == string("threshold")) {

				p.SetThresholdValue(stof(lp.second));
			}
			else if (lp.first == string("shape")) {

				p.SetShape(lp.second);
			}
			else if (lp.first == string("image_on")) {

				p.SetOnImage(lp.second);
			}
			else if (lp.first == string("image_off")) {

				p.SetOffImage(lp.second);
			}
			else if (lp.first == string("thickness")) {

				p.SetThickness(stoi(lp.second));
			}
			else if (lp.first == string("filled")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p.SetFilled(true);
				else
					p.SetFilled(false);
			}
			else if (lp.first == string("outlined")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p.SetOutlined(true);
				else
					p.SetOutlined(false);
			}
			else if (lp.first == string("shaded")) {

				if (lp.second == string("true") || lp.second == string("yes"))
					p.SetShaded(true);
				else
					p.SetShaded(false);
			}
			else if (lp.first == string("unshaded")) {

				p.SetUnshadedValue(stof(lp.second));
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

	checkElementLocalSettingsOverwrite(p);
	p.Init();
	_curDashboard.AddDashboardElement(p);
}

void CDashboardLayout::parseSliderSection(elementSection& sect)
{
	CSlider s;
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(s, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("range")) {

				s.SetRange(lp.second);
			}
			else if (lp.first == string("rgbpositive")) {

				s.SetColorPositive(lp.second);
			}
			else if (lp.first == string("rgbnegative")) {

				s.SetColorNegative(lp.second);
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

	checkElementLocalSettingsOverwrite(s);
	s.Init();
	_curDashboard.AddDashboardElement(s);
}

void CDashboardLayout::parseGCircleSection(elementSection& sect)
{
	CGCircle g;
	int _line = 0;
	ldf_pair lp;

	try {

		parseCommonKeys(g, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("divisions")) {

				g.SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("radius")) {

				g.SetRadius(stoi(lp.second));
			}
			else if (lp.first == string("line_width")) {

				g.SetLineWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer")) {

				g.SetPointerColor(lp.second);
			}
			else if (lp.first == string("pointer_width")) {

				g.SetPointerWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer_size")) {

				g.SetPointerSize(stoi(lp.second));
			}
			else if (lp.first == string("scale_lat_accel")) {

				g.SetLateralScaleFactor(stof(lp.second));
			}
			else if (lp.first == string("scale_lon_accel")) {

				g.SetLongitudinalScaleFactor(stof(lp.second));
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

	checkElementLocalSettingsOverwrite(g);
	g.Init();
	_curDashboard.AddDashboardElement(g);
}

void CDashboardLayout::parseSweeperElement(elementSection& sect)
{
	CSweeper s;
	int _line = 0;
	ldf_pair lp;

	try {
		parseCommonKeys(s, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("path")) {

				s.SetPathType(lp.second);
			}
			else if (lp.first == string("range")) {

				s.SetRange(lp.second);
			}
			else if (lp.first == string("sweep")) {

				s.SetSweepAngle(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				s.SetRectangle(lp.second);
			}
			else if (lp.first == string("line_start")) {

				s.SetLineStart(lp.second);
			}
			else if (lp.first == string("line_end")) {

				s.SetLineEnd(lp.second);
			}
			else if (lp.first == string("shape")) {

				s.SetShapeType(lp.second);
			}
			else if (lp.first == string("extend")) {

				s.SetExtend(stoi(lp.second));
			}
			else if (lp.first == string("thickness")) {

				s.SetThickness(stoi(lp.second));
			}
			else if (lp.first == string("taper")) {

				s.SetTaper(stoi(lp.second));
			}
			else if (lp.first == string("divisions")) {

				s.SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("rgboff")) {

				s.SetColorOFF(lp.second);
			}
			else if (lp.first == string("rgbon")) {

				removeChar(lp.second, ' ');
				if (lp.second[0] == '<' && lp.second[1] == '<')
					s.SetThresholdColors(lp.second);
				else
					s.SetColorON(lp.second);
			}
			else if (lp.first == string("threshold")) {

				s.SetThreshold(lp.second);
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

	checkElementLocalSettingsOverwrite(s);
	s.Init();
	s.Precalculate();
	_curDashboard.AddDashboardElement(s);
}

void CDashboardLayout::parseRulerElement(elementSection& sect)
{
	CRuler p;
	int _line = 0;
	ldf_pair lp;

	try {
		//parseCommonKeys(p, sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("using")) {

				p.SetUsingElement(lp.second);
			}
			else if (lp.first == string("path")) {

				p.SetPathType(lp.second);
			}
			else if (lp.first == string("draw_path")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p.SetDrawPath(b);
			}
			else if (lp.first == string("path_color")) {

				p.SetPathColor(lp.second);
			}
			else if (lp.first == string("path_offset")) {

				p.SetPathOffset(stoi(lp.second));
			}
			else if (lp.first == string("path_width")) {

				p.SetPathWidth(stoi(lp.second));
			}
			else if (lp.first == string("path_range")) {

				p.SetPathRange(lp.second);
			}
			else if (lp.first == string("path_position")) {

				p.SetPathPosition(lp.second);
			}
			else if (lp.first == string("draw_label")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p.SetDrawLabel(b);
			}
			else if (lp.first == string("label_margin")) {

				p.SetLabelMargin(stoi(lp.second));
			}
			else if (lp.first == string("label_format")) {

				//p-(lp.second);
			}
			else if (lp.first == string("line_start")) {

				p.SetLineStart(lp.second);
			}
			else if (lp.first == string("line_end")) {

				p.SetLineEnd(lp.second);
			}
			//else if (lp.first == string("arc_start")) {

			//	
			//}
			//else if (lp.first == string("arc_end")) {

			//}
			else if (lp.first == string("arc_sweep")) {

				p.SetArcSweep(lp.second);
			}
			else if (lp.first == string("draw_major")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p.SetDrawMajor(b);
			}
			else if (lp.first == string("major_divisions")) {

				p.SetMajorDivisions(stoi(lp.second));
			}
			else if (lp.first == string("major_units")) {

				p.SetMajorUnits(stof(lp.second));
			}
			else if (lp.first == string("major_margin")) {

				p.SetMajorMargin(stoi(lp.second));
			}
			else if (lp.first == string("major_color")) {

				p.SetMajorColor(lp.second);
			}
			else if (lp.first == string("major_shape")) {

				p.SetMajorShape(lp.second);
			}
			else if (lp.first == string("major_length")) {

				p.SetMajorLength(stoi(lp.second));
			}
			else if (lp.first == string("major_width")) {

				p.SetMajorWidth(stoi(lp.second));
			}
			else if (lp.first == string("major_radius")) {

				p.SetMajorRadius(stoi(lp.second));
			}
			else if (lp.first == string("draw_minor")) {

				bool b = false;

				if (lp.second == string("yes") || lp.second == string("true"))
					b = true;

				p.SetDrawMinor(b);
			}
			else if (lp.first == string("minor_divisions")) {

				p.SetMinorDivisions(stoi(lp.second));
			}
			else if (lp.first == string("minor_units")) {

				p.SetMinorUnits(stof(lp.second));
			}
			else if (lp.first == string("minor_margin")) {

				p.SetMinorMargin(stoi(lp.second));
			}
			else if (lp.first == string("minor_color")) {

				p.SetMinorColor(lp.second);
			}
			else if (lp.first == string("minor_shape")) {

				p.SetMinorShape(lp.second);
			}
			else if (lp.first == string("minor_length")) {

				p.SetMinorLength(stoi(lp.second));
			}
			else if (lp.first == string("minor_width")) {

				p.SetMinorWidth(stoi(lp.second));
			}
			else if (lp.first == string("minor_radius")) {

				p.SetMinorRadius(stoi(lp.second));
			}
			else if (lp.first == string("foreground")) {

				p.SetForegroundColor(lp.second);
			}
			else if (lp.first == string("background")) {

				p.SetBackgroundColor(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				p.SetFontHeight(stoi(lp.second));
			}
			else if (lp.first == string("fontstyle")) {

				p.SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontname")) {

				p.SetFontName(lp.second);
			}
			else if (lp.first == string("layer")) {

				p.SetLayer(stoi(lp.second));
			}
			else if (lp.first == string("transparency")) {

				p.SetTransparency(stof(lp.second));
			}
			else if (lp.first == string("scale")) {

				p.SetScalingFactor(stod(lp.second));
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

	//checkElementLocalSettingsOverwrite(p);

	// check if this ruler element belongs to another element (by using property) or not
	//if (p.GetUsingElement() == string(""))
	//{
	//	// this ruler element is a standalone element just like all the other elements
	//	// so initialize it and add it to the elements list
	//	p.Init();

	//	// add this ruler element to the elements list here 
	//	_curDashboard.AddDashboardElement(p);
	//}
	//else {
	// asign this ruler element to a temporary lookup table; this is needed in case the ruler element is not
	// the last element in the layout file. Thus it is possible that the ruler will be parsed before the
	// element it belongs to!
	rulerTable[p.GetUsingElement()] = p;
	//}
}


void CDashboardLayout::parseRingGaugeElement(elementSection& sect)
{
	CRingGauge g;
	int _line = 0;
	ldf_pair lp;

	try {

		parseCommonKeys(g, sect);

		for (auto kp : sect.keyPairs)
		{
			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("id")) {

				g.SetID(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				g.SetRectangle(lp.second);
			}
			else if (lp.first == string("range")) {

				g.SetRange(lp.second);
			}
			else if (lp.first == string("width")) {

				g.SetRingWidth(lp.second);
			}
			else if (lp.first == string("sweep")) {

				g.SetSweep(lp.second);
			}
			else if (lp.first == string("rotation")) {

				g.SetRotation(lp.second);
			}
			else if (lp.first == string("foreground")) {

				g.SetForegroundColor(lp.second);
			}
			else if (lp.first == string("shaded")) {

				if ((lp.second == "yes") || (lp.second == "true"))
					g.SetShaded(true);
				else
					g.SetShaded(false);
			}
			else if (lp.first == string("marker_width")) {

				g.SetMarkerWidth(lp.second);
			}
			else if (lp.first == string("marker_color")) {

				g.SetMarkerColor(lp.second);
			}
			else if (lp.first == string("marker_hold_time")) {

				g.SetMarkerHoldTime(lp.second);
			}
			else if (lp.first == string("marker_decay")) {

				g.SetMarkerDecay(lp.second);
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

	checkElementLocalSettingsOverwrite(g);
	g.Init();
	_curDashboard.AddDashboardElement(g);
}