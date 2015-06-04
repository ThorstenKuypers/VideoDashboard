
#include "errex.h"

using namespace DShow;

CErrEx::CErrEx(char* mod, char* cls, char* fnc, char* file, int line, DWORD err, bool format)
{
	m_module =mod;
	m_class =cls;
	m_function =fnc;
	m_file =file;
	m_line =line;
	m_errcode =err;

	if (!format)
		msg =GetInternalErrMsg();
	else 
		msg =GetErrMsg();

	logMsg =GetLogMsg();
}

CErrEx::~CErrEx(void)
{
	if (msg !=NULL) {
		LocalFree(msg);
		msg =NULL;
	}
	if (logMsg !=NULL) {
		delete[] logMsg;
		logMsg =NULL;
	}
}

char* CErrEx::GetErrMsg()
{
	char* str =NULL;
	DWORD len =0;

	len =FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, m_errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&str, 0, NULL);
	if (len)
		return str;

	return NULL;
}

char* CErrEx::GetLogMsg()
{
	int len =strlen(msg) + strlen(m_class) + strlen(m_file) + strlen(m_function) + strlen(m_module) + sizeof(m_errcode) + 1024;
	int msg_len =0;
	char* _str =new char[len];
	char* s ="%s\r\n\r\n"
			 "Module: %s\r\n"
			 //"Class: %s\r\n"
			 "Method: %s\r\n"
			 "Error: 0x%08X\r\n"
			 "File: %s\r\n"
			 "Line: %d\r\n";

	memset(_str, 0, len*sizeof(char));

	StringCchPrintfA(_str, len, s, msg, m_module, /*m_class,*/ m_function, m_errcode, m_file, m_line);
	msg_len =strlen(_str) + 1;
	char* str =new char[msg_len];
	memset(str, 0, msg_len*sizeof(char));

	StringCchCopyA(str, msg_len, _str);

	delete[] _str;

	return str;
}

char* CErrEx::GetInternalErrMsg()
{
	return NULL;
}
