#pragma once

#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>


namespace DShow {

class CErrEx
{
public:
	CErrEx(char* mod, char* cls, char* fnc, char* file, int line, DWORD err, bool format);
	~CErrEx(void);

	char* msg;	// error Message String (from FormatMessage function)
	char* logMsg;	// extended error message for error.log file

private:
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

}