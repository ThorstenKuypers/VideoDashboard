#pragma once

#include "common.h"
#include "libDataLogging.h"
#include "BaseDataLogger.h"
#include "ErrEx.h"

#include "irsdk_defines.h"
#include "yaml_parser.h"

using namespace iR_sdk;

namespace libDataLogging {
	namespace DataLogger {


class CibtDataLogger : libDataLogging::DataLogger::CBaseDataLogger
{
public:
	CibtDataLogger(wchar_t* logfile);
	virtual ~CibtDataLogger();

	int GetSampleTickRate();

protected:

private:
	irsdk_header m_header;
	irsdk_varHeader *m_varHeaders;
	irsdk_diskSubHeader m_subHeader;

	char* m_sessionInfo;
				
	void processDataFile();

	void ParseSessionInfo(const char* sessionInfoStr);

	void readIBTHeader();
	void readSubHeader();
	void readVarHeaders();
	void readSessInfo();
	void readDataBuf();
	void extractChannelData(char* buf);

};

	}
}