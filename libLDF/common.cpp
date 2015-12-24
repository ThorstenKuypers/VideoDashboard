
#include "common.h"

using namespace libLDF;

bool libLDF::isAbsoluteFilePath(std::string path)
{
	if (!path.empty())
	{
		if (path.find(":") != std::string::npos)
			return true;
	}

	return false;
}

std::string libLDF::trim(std::string& str, const std::string& trimChars)
{
	size_t last = str.find_last_not_of(trimChars) + 1;
	size_t first = str.find_first_not_of(trimChars);

	str.erase(last);
	return str.erase(0, first);
}

void libLDF::splitTuple(std::string& tpl, std::vector<std::string>& toks, parsing_error& ec)
{
	std::string str = trim(tpl, " \t");
	size_t p = 0, i = 0;
	bool open = false;

	// check if tuple is opened and closed (values enclosed with <>)
	for (char c : tpl) {
		if (c == '<')
			open = true;
		if (c == '>') {
			if (!open) {
				ec = parsing_error::tupleNotOpened;
				return;
			}
			open = false;
		}
	}
	if (open) {
		ec = parsing_error::tupleNotClosed;
		return;
	}

	str = trim(tpl, "<>");
	while (p != str.size()) {
		p = str.find(',', i);
		if (p == std::string::npos)
			p = str.size();

		string s = std::string(str, i, p - i);
		s = trim(s, " \t");
		toks.push_back(s);
		i = p + 1;
	}

	ec = parsing_error::ok;
	return;
}

void libLDF::splitTuple2(std::string& tpl, std::vector<std::string>& tuples)
{
	std::string str = tpl;
	size_t p = 0, i = 0;
	size_t ts = -1, te = -1;
	std::string s;
	bool open = false;

	removeChar(str, ' ');
	removeChar(str, '\t');

	for (size_t i = 0; i < str.size(); i++) {

		if (str[i] == '<') {
			if (i > 0) // start of tuple inside tuple
				ts = i;
			else
				p = i;
			open = true;
		}
		if (str[i] == '>') {
			te = i + 1;
		}


		if (ts != -1 && te != -1) {
			// extract tuple from tuple
			s = std::string(str, ts, te - ts);
			tuples.push_back(s);
			ts = te = -1;
		}
	}
}

void libLDF::removeChar(string& s, const char c)
{
	size_t p = 0, i = 0;

	while (p != s.size()) {

		p = s.find(c, i);
		if (p == std::string::npos)
			p = s.size();
		s = s.erase(p, 1);
		i = p;
	}
}

void libLDF::getColorFromString(string& s, Gdiplus::Color& color)
{
	std::vector<string> toks;
	//int* v = new int[3];
	parsing_error ec;

	// check if hexadecimal (HTML) color code is used
	if (s[0] == '#') {

		// split HEX string into RGB values
		for (int i = 0; i < 3; i++)
		{
			string c = s.substr((i * 2) + 1, 2);
			//color->push_back(stoi(c, nullptr, 16));
			color = Gdiplus::Color(stoi(c, nullptr, 16));
		}
	}
	else if (s[0] == '<') { // or if RGB tuple

		splitTuple(s, toks, ec);
		if (ec != parsing_error::ok)
			throw ec;

		color = Gdiplus::Color(stoi(toks[0]), stoi(toks[1]), stoi(toks[2]));

		//color->push_back(stoi(toks[0]));
		//color->push_back(stoi(toks[1]));
		//color->push_back(stoi(toks[2]));
	}
	else { // otherwise assume X11 color name

		if (!s.empty())
		{
			X11Colors::Get(s, color);
		}
	}
}

void libLDF::str_tolower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

