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

#include "Dashboard.h"

using namespace libLDF;

void CDashboard::Parse()
{
	if (_fileName.empty())
		throw std::exception("dashboard file name is empty!");

	std::ifstream fs(_fileName, ios_base::in);
	std::vector<std::string> lines;
	std::vector<ldf_line> _lines;

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
				throw ParsingException(parsing_error::sectionNotClosed, _lines[i].first, sec.c_str(), _fileName.c_str());
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
				throw ParsingException(parsing_error::sectionNotOpened, _lines[i].first, sec.c_str(), _fileName.c_str());
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
				throw ParsingException(parsing_error::sectionNotOpened, _lines[i].first, sec.c_str(), _fileName.c_str());
			}

			s.lines.push_back(_lines[i]);
		}
	}

	// clear previous dashboard element
	clear();

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
	for (size_t i = 0; i < _elements.size(); i++) {

		// iterate through the ruler lookup table
		for (RulerLookupTable::iterator it = rulerTable.begin(); it != rulerTable.end(); ++it) {
			if (_elements[i]->GetID() == it->first) {

				// initialize ruler element
				CRuler& re = *it->second.get();
				_elements[i]->SetRuler(re);
				it->second->Init();
			}
		}
	}

#ifdef _DEBUG
	int x = 0;
#endif
}

void CDashboard::extractDashboardFilePath()
{
	if (!_fileName.empty())
	{
		size_t pos = 0;
		std::string path;

		pos = _fileName.rfind("\\");
		if (pos != std::string::npos)
		{
			_dashboardFilePath = _fileName.substr(0, pos + 1);
		}
	}
}

Bitmap* CDashboard::RenderToImage(libOGA::DataSample& sample, IGenericLogger& logger, bool renderBlank)
{
	Gdiplus::Bitmap* img = nullptr;

	WCHAR dir[256] = { 0 };
	GetCurrentDirectoryW(256, dir);


	if (!_imagefile.empty()){
		std::string fn = _dashboardFilePath + _imagefile;
		wstring ws = wstring(fn.begin(), fn.end());
#ifdef _DEBUG
		WCHAR* f = (WCHAR*)ws.c_str();
		img = new Bitmap(f);
#else
		img = new Bitmap((WCHAR*)ws.c_str());
#endif
		int w = img->GetWidth(), h = img->GetHeight();

		int x = 0;
	}
	else if (_width != 0 && _height != 0) {

		img = new Bitmap(_width, _height, PixelFormat32bppARGB);
	}
	else {
		throw std::exception("error creating base bitmap!");
	}

	if (img == nullptr)
		throw - 1;

	Gdiplus::Graphics* gfx = Gdiplus::Graphics::FromImage(img);
	if (gfx == nullptr) {

		if (img != nullptr)
			delete img;

		throw std::exception("can't acquire GDI+ graphics object!");
	}

	Bitmap* rulerImg = nullptr;
	Gdiplus::Rect rulerRect{ 0, 0, 0, 0 };
	Bitmap* bmp = nullptr;
	Gdiplus::Rect rc{ 0, 0, 0, 0 };
	int rulerLayer = -1;

	/***********************************************************************************************/
	// experimental code
	for (int layer = 0; layer < MAX_LAYERS; layer++){
		for (unsigned int i = 0; i < _elements.size(); i++) {

			if (layer == _elements[i]->GetLayer()) {
				bmp = _elements[i]->Render(sample, logger, renderBlank);
				rc = _elements[i]->GetRectangle();
				if (bmp != nullptr) {

					// check if current element has a ruler assigned to it
					for (RulerLookupTable::iterator it = rulerTable.begin(); it != rulerTable.end(); ++it) {
						if (_elements[i]->GetID() == it->first) {
							CRuler& ruler = (CRuler&)*it->second.get();
							//rulerImg = ruler.Render(bmp, layer);
							rulerImg = ruler.Render(0);
							rulerRect = ruler.GetRectangle();
							rulerLayer = ruler.GetLayer();
						}
					}

					// TODO: add layering
					if (rulerImg != nullptr && rulerLayer <= layer) {

						// draw ruler image first if same or lower layer
						gfx->DrawImage(rulerImg, rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());

						if (_showBoundingBoxes)
							gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
					}


					gfx->DrawImage(bmp, rc.X, rc.Y, bmp->GetWidth(), bmp->GetHeight());
					if (_showBoundingBoxes)
						gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rc.X, rc.Y, bmp->GetWidth(), bmp->GetHeight());

					if (rulerImg != nullptr && rulerLayer > layer) {

						// draw ruler image afetr corresponding element if ruler is on higher layer
						gfx->DrawImage(rulerImg, rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
						if (_showBoundingBoxes)
							gfx->DrawRectangle(&Pen(&SolidBrush(Color::Red)), rulerRect.X, rulerRect.Y, rulerImg->GetWidth(), rulerImg->GetHeight());
					}

					// NO NOT free the returned image if the rendered element is a ruler because
					// the ruler element is the only static element. The image reference is held 
					// in its class and freed when the ruler class instance is destroyed!
					if (_elements[i]->GetElementType() != DashboardElementType::ruler) {
						delete bmp;
						bmp = nullptr;
					}
				}
			}
		}
	}

	/*****************************************************************************************************/

	delete gfx;
	gfx = nullptr;

	return img;

	//return nullptr;
}

void CDashboard::SetBackgroundColor(std::string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_background = Color((255) | c[0]);
		if (c.size() == 3)
			_background = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CDashboard::SetForegroundColor(std::string& s)
{
	std::vector<int> c;

	try {
		getColorFromString(s, &c);
		if (c.size() == 1)
			_foreground = Color((255) | c[0]);
		if (c.size() == 3)
			_foreground = Color((BYTE)c[0], (BYTE)c[1], (BYTE)c[2]);
	}
	catch (parsing_error& ec)
	{
		throw ec;
	}
}

void CDashboard::SetFontStyle(string& s)
{

	if (s == string("bold")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleBold;
	}
	else if (s == string("italic")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleItalic;
	}
	else if (s == string("normal")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	}
	else if (s == string("bold_italic")) {

		_fontStyle = Gdiplus::FontStyle::FontStyleBoldItalic;
	}
	else {

		_fontStyle = Gdiplus::FontStyle::FontStyleRegular;
	}
}

void CDashboard::SetTextJustification(string& s)
{
	if (s == string("center")) {

		_justify = TextJustify_center;
	}
	else if (s == string("right")) {

		_justify = TextJustify_right;
	}
	else if (s == string("left")) {

		_justify = TextJustify_left;
	}
	else {

		_justify = TextJustify_left;
	}
}

void CDashboard::SetImageFile(std::string& s)
{
	if (isAbsoluteFilePath(s))
		_imagefile = s;
	else
		_imagefile = _dashFilePath + s;
}

void CDashboard::parseSections(elementSection& sect)
{
	size_t end = 0;
	int j = 0;

	for (auto ln : sect.lines) {

		j = 0;
		end = ln.second.find(';');
		if (end == std::string::npos) {

			throw ParsingException(parsing_error::missingSemicolon, ln.first, sect.name.c_str(), _fileName.c_str());
		}

		for (auto it = ln.second.cbegin(); it != ln.second.cend(); ++it, j++) {

			if (*it == '=') {
				string k(ln.second, 0, j);
				string v(ln.second, j + 1, end - j - 1);
				trim(k, " \t");
				trim(v, " \t");
				std::transform(k.begin(), k.end(), k.begin(), tolower);

				if (v == "")
					throw ParsingException(parsing_error::emptyKeyValue, ln.first, sect.name.c_str(), _fileName.c_str());

				// NEW 
				ldf_pair lp;
				lp.first = k;
				lp.second = v;
				sect.keyPairs[ln.first] = lp;
			}
		}
	}
}

void CDashboard::parseCommonKeys(CDashboardElement* e, elementSection& sect)
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
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}
}

void CDashboard::parseDashboardSection(elementSection& sect)
{
	int _line = 0;

	try
	{
		for (auto kp : sect.keyPairs) {

			_line = kp.first;

			// remove whitespaces
			ldf_pair lp = kp.second;
			lp.second = trim(lp.second, " \t\"");

			std::string key = lp.first;
			std::transform(key.begin(), key.end(), key.begin(), tolower);

			if (key == string("shortname")) {

				SetShortName(lp.second);
			}
			else if (lp.first == std::string("prettyname")) {

				SetPrettyName(lp.second);
			}
			else if (lp.first == std::string("imagefile")) {

				SetImageFile(lp.second);
			}
			else if (lp.first == std::string("targetwidth")) {

				SetTargetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("width")) {

				SetWidth(std::stoi(lp.second));
			}
			else if (lp.first == std::string("height")) {

				SetHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("foreground")) {
				SetForegroundColor(lp.second);
			}
			else if (lp.first == std::string("background")) {
				SetBackgroundColor(lp.second);
			}
			else if (lp.first == std::string("fontname")) {

				SetFontName(lp.second);
			}
			else if (lp.first == std::string("fontstyle")) {

				SetFontStyle(lp.second);
			}
			else if (lp.first == string("fontheight")) {

				SetFontHeight(std::stoi(lp.second));
			}
			else if (lp.first == std::string("justify")) {

				SetTextJustification(lp.second);
			}
		}
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}
}

void CDashboard::parseTextBoxSection(elementSection& section)
{
	std::unique_ptr<CTextBox> tb = std::unique_ptr<CTextBox>(new CTextBox());
	int _line = 0;
	ldf_pair lp;

	tb->SetDashboardFilePath(_dashboardFilePath);
	try {
		parseCommonKeys(tb.get(), section);

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
		throw ParsingException(ec, _line, section.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(tb.get());
	tb->Init();

	// add element to list of elements
	_elements.emplace_back(std::move(tb));
}

void CDashboard::parseGaugeSection(elementSection& sect)
{
	std::unique_ptr<CGauge> g = std::unique_ptr<CGauge>(new CGauge);
	int _line = 0;
	ldf_pair lp;

	g->SetDashboardFilePath(_dashboardFilePath);

	try {
		parseCommonKeys(g.get(), sect);
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
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(g.get());
	g->Init();
	_elements.emplace_back(std::move(g));
}

void CDashboard::parseIndicatorSection(elementSection& sect)
{
	std::unique_ptr<CIndicator> p = std::unique_ptr<CIndicator>(new CIndicator);
	int _line = 0;
	ldf_pair lp;

	p->SetDashboardFilePath(_dashboardFilePath);

	try {
		parseCommonKeys(p.get(), sect);

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
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(p.get());
	p->Init();
	_elements.emplace_back(std::move(p));
}

void CDashboard::parseSliderSection(elementSection& sect)
{
	std::unique_ptr<CSlider> s = std::unique_ptr<CSlider>(new CSlider);
	int _line = 0;
	ldf_pair lp;

	s->SetDashboardFilePath(_dashboardFilePath);

	try {
		parseCommonKeys(s.get(), sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("range")) {

				s->SetRange(lp.second);
			}
			else if (lp.first == string("rgbpositive")) {

				s->SetColorPositive(lp.second);
			}
			else if (lp.first == string("rgbnegative")) {

				s->SetColorNegative(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(s.get());
	s->Init();
	_elements.emplace_back(std::move(s));
}

void CDashboard::parseGCircleSection(elementSection& sect)
{
	std::unique_ptr<CGCircle> g = std::unique_ptr<CGCircle>(new CGCircle);
	int _line = 0;
	ldf_pair lp;

	g->SetDashboardFilePath(_dashboardFilePath);

	try {

		parseCommonKeys(g.get(), sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("divisions")) {

				g->SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("radius")) {

				g->SetRadius(stoi(lp.second));
			}
			else if (lp.first == string("line_width")) {

				g->SetLineWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer")) {

				g->SetPointerColor(lp.second);
			}
			else if (lp.first == string("pointer_width")) {

				g->SetPointerWidth(stoi(lp.second));
			}
			else if (lp.first == string("pointer_size")) {

				g->SetPointerSize(stoi(lp.second));
			}
			else if (lp.first == string("scale_lat_accel")) {

				g->SetLateralScaleFactor(stof(lp.second));
			}
			else if (lp.first == string("scale_lon_accel")) {

				g->SetLongitudinalScaleFactor(stof(lp.second));
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(g.get());
	g->Init();
	_elements.emplace_back(std::move(g));
}

void CDashboard::parseSweeperElement(elementSection& sect)
{
	std::unique_ptr<CSweeper> s = std::unique_ptr<CSweeper>(new CSweeper);
	int _line = 0;
	ldf_pair lp;

	s->SetDashboardFilePath(_dashboardFilePath);

	try {
		parseCommonKeys(s.get(), sect);

		for (auto kp : sect.keyPairs) {

			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("path")) {

				s->SetPathType(lp.second);
			}
			else if (lp.first == string("range")) {

				s->SetRange(lp.second);
			}
			else if (lp.first == string("sweep")) {

				s->SetSweepAngle(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				s->SetRectangle(lp.second);
			}
			else if (lp.first == string("line_start")) {

				s->SetLineStart(lp.second);
			}
			else if (lp.first == string("line_end")) {

				s->SetLineEnd(lp.second);
			}
			else if (lp.first == string("shape")) {

				s->SetShapeType(lp.second);
			}
			else if (lp.first == string("extend")) {

				s->SetExtend(stoi(lp.second));
			}
			else if (lp.first == string("thickness")) {

				s->SetThickness(stoi(lp.second));
			}
			else if (lp.first == string("taper")) {

				s->SetTaper(stoi(lp.second));
			}
			else if (lp.first == string("divisions")) {

				s->SetDivisions(stoi(lp.second));
			}
			else if (lp.first == string("rgboff")) {

				s->SetColorOFF(lp.second);
			}
			else if (lp.first == string("rgbon")) {

				removeChar(lp.second, ' ');
				if (lp.second[0] == '<' && lp.second[1] == '<')
					s->SetThresholdColors(lp.second);
				else
					s->SetColorON(lp.second);
			}
			else if (lp.first == string("threshold")) {

				s->SetThreshold(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(s.get());
	s->Init();
	s->Precalculate();
	_elements.emplace_back(std::move(s));
}

void CDashboard::parseRulerElement(elementSection& sect)
{
	std::unique_ptr<CRuler> p = std::unique_ptr<CRuler>(new CRuler);
	int _line = 0;
	ldf_pair lp;

	p->SetDashboardFilePath(_dashboardFilePath);

	try {
		parseCommonKeys(p.get(), sect);

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
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(p.get());

	// check if this ruler element belongs to another element (by using property) or not
	if (p->GetUsingElement() == string(""))
	{
		// this ruler element is a standalone element just like all the other elements
		// so initialize it and add it to the elements list
		p->Init();

		// add this ruler element to the elements list here 
		_elements.emplace_back(std::move(p));
	}
	else {
		//	asign this ruler element to a temporary lookup table; this is needed in case the ruler element is not
		//	the last element in the layout file.Thus it is possible that the ruler will be parsed before the
		//	element it belongs to!
		rulerTable[p->GetUsingElement()] = std::move(p);
	}
}

void CDashboard::parseRingGaugeElement(elementSection& sect)
{
	std::unique_ptr<CRingGauge> g = std::unique_ptr<CRingGauge>(new CRingGauge);
	int _line = 0;
	ldf_pair lp;

	g->SetDashboardFilePath(_dashboardFilePath);

	try {

		parseCommonKeys(g.get(), sect);

		for (auto kp : sect.keyPairs)
		{
			_line = kp.first;
			lp = kp.second;

			lp.second = trim(lp.second, " \t\"");

			if (lp.first == string("id")) {

				g->SetID(lp.second);
			}
			else if (lp.first == string("rectangle")) {

				g->SetRectangle(lp.second);
			}
			else if (lp.first == string("range")) {

				g->SetRange(lp.second);
			}
			else if (lp.first == string("width")) {

				g->SetRingWidth(lp.second);
			}
			else if (lp.first == string("sweep")) {

				g->SetSweep(lp.second);
			}
			else if (lp.first == string("rotation")) {

				g->SetRotation(lp.second);
			}
			else if (lp.first == string("foreground")) {

				g->SetForegroundColor(lp.second);
			}
			else if (lp.first == string("shaded")) {

				if ((lp.second == "yes") || (lp.second == "true"))
					g->SetShaded(true);
				else
					g->SetShaded(false);
			}
			else if (lp.first == string("marker_width")) {

				g->SetMarkerWidth(lp.second);
			}
			else if (lp.first == string("marker_color")) {

				g->SetMarkerColor(lp.second);
			}
			else if (lp.first == string("marker_hold_time")) {

				g->SetMarkerHoldTime(lp.second);
			}
			else if (lp.first == string("marker_decay")) {

				g->SetMarkerDecay(lp.second);
			}
		}
	}
	catch (ParsingException& ex)
	{
		throw ex;
	}
	catch (parsing_error& ec)
	{
		throw ParsingException(ec, _line, sect.name.c_str(), _fileName.c_str());
	}

	checkElementLocalSettingsOverwrite(g.get());
	g->Init();
	_elements.emplace_back(std::move(g));
}

void CDashboard::checkElementLocalSettingsOverwrite(CDashboardElement* e)
{
	if (e->UseGlobalBackgroundColor())
		e->SetBackgroundColor(_background);
	if (e->UseGlobalFontName())
		e->SetFontName(_fontName);
	if (e->UseGlobalFontStyle())
		e->SetFontStyle(_fontStyle);
	if (e->UseGlobalForegroundColor())
		e->SetForegroundColor(_foreground);
	if (e->UseGlobalTextJustify())
		e->SetTextJustification(_justify);

}
