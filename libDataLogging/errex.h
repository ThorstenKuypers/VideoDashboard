#pragma once

#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include <exception>
#include <string>
#include <sstream>

namespace libDataLogging {

class CErrEx : std::exception
{
public:
	CErrEx(char* mod, char* cls, char* fnc, char* file, int line, DWORD err, bool format);
	virtual ~CErrEx(void);

	char* msg;	// error Message String (from FormatMessage function)
	char* logMsg;	// extended error message for error.log file

	virtual const char* what()
	{
		return msg;
	}

protected:
	char* m_module;
	char* m_class;
	char* m_function;
	char* m_file;
	int m_line;

	DWORD m_errcode;

	char* GetInternalErrMsg();
	char* GetErrMsg();
	char* GetLogMsg();
};


class ChannelDataTypeMismatchException : public std::exception
{
public:
	ChannelDataTypeMismatchException() :std::exception("The data type specified does not fit the channels data type") {}
	virtual ~ChannelDataTypeMismatchException() {}

};

class ChannelNotFoundException : public std::exception
{
public:
	ChannelNotFoundException() : std::exception("The specified channel could not be found in the data logger.") {}
	ChannelNotFoundException(std::string channel)
	{
		std::stringstream ss;

		ss << "The specified channel - (" << channel << ") - could not be found in the data logger.";
		_s = ss.str();		
	}
	virtual ~ChannelNotFoundException() {}

	virtual const char* what() const { return _s.c_str(); }

private:
	std::string _s;
};

}