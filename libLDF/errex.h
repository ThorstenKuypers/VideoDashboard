#pragma once
#ifndef _LIBLDF_ERREX_H_
#define _LIBLDF_ERREX_H_
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

#include <exception>
#include <system_error>
#include <string>
#include <sstream>

using namespace std;

namespace libLDF {

	enum class parsing_error {

		ok = 0,
		sectionNotOpened = -1,
		sectionNotClosed = -2,
		missingSemicolon = -3,
		emptyKeyValue = -4,
		missingArgument = -5,
		tupleNotOpened = -6,
		tupleNotClosed = -7
	};

	class ParsingException : public std::exception
	{
	public:
		ParsingException(parsing_error ec, int line, const char* section, const char* file)
		{
			_e = static_cast<int>(ec);
			_lineNo = line;

			std::stringstream ss;
			switch ((parsing_error)_e)
			{
			case parsing_error::sectionNotOpened:

				ss << "Section: " << section << " not opened" << endl;
				ss << "  -> Missing \"{\" " << endl;
				ss << " File: " << file << endl;
				ss << "Line: " << _lineNo;
				break;

			case parsing_error::sectionNotClosed:

				ss << "Section: " << section << " not closed" << endl;
				ss << "  -> Missing \"}\" " << endl;
				ss << " File: " << file << endl;
				ss << "Line: " << _lineNo;
				break;

			case parsing_error::missingSemicolon:

				ss << "Missing \";\" " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;

			case parsing_error::emptyKeyValue:

				ss << "Empty key value " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;

			case parsing_error::missingArgument:

				ss << "Missing or wrong argument value " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;

			case parsing_error::tupleNotClosed:

				ss << "Value tuple is not closed - Missing \">\" " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;

			case parsing_error::tupleNotOpened:

				ss << "Value tuple is not opened - Missing \"<\" " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;

			default:

				ss << "Unknown error in line: " << endl;
				ss << "File: " << file << endl;
				ss << "Line: " << line;
				break;
			}

			_msg = ss.str();
		}

		const char* what() const override
		{

			return _msg.c_str();
		}

	private:
		std::string _msg;
		int _lineNo;
		int _e;
	};
	
}

#endif // _LIBLDF_ERREX_H_