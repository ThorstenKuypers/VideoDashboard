
#include "BaseDataLogger.h"
#include "ErrEx.h"

using namespace libDataLogging;
using namespace libDataLogging::DataLogger;

CBaseDataLogger::CBaseDataLogger(wchar_t* logfile)
{
	this->m_fastLapInfo =new FASTEST_LAP_INFO;
	memset(this->m_fastLapInfo, 0, sizeof(FASTEST_LAP_INFO));
	m_lapInfo = NULL;

	m_file =CreateFileW(logfile, GENERIC_READ, FILE_SHARE_READ, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_file ==INVALID_HANDLE_VALUE)
	{
		CErrEx* ex =new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, GetLastError(), true);
		throw ex;
	}

	channelsMap =std::map<string, int>();
	specialChannelsMap = std::map<string, string>();	
	sectors = std::vector<SECTOR_INFO>();
}

CBaseDataLogger::CBaseDataLogger()
{

}

CBaseDataLogger::~CBaseDataLogger()
{
	if (this->m_fastLapInfo) {
		delete this->m_fastLapInfo;
		this->m_fastLapInfo =NULL;
	}

	if (this->m_lapInfo) {
		for (int i =0; i < this->m_lapCnt; i++) {
			if (this->m_lapInfo[i] !=NULL) {
				delete this->m_lapInfo[i];
				this->m_lapInfo[i] =NULL;
			}
		}
		delete this->m_lapInfo;
		this->m_lapInfo =NULL;
	}

	CloseFile();
}

void CBaseDataLogger::CloseFile()
{
	CloseHandle(m_file);
}

int CBaseDataLogger::GetSampleCount()
{
	return m_sampleCnt;
}

PLAPINFO CBaseDataLogger::GetLapInfo(int lap)
{
	return this->m_lapInfo[lap];
}

int CBaseDataLogger::GetSessionLapCount()
{
	return this->m_lapCnt;
}

PFASTEST_LAP_INFO CBaseDataLogger::GetFastLapInfo()
{
	return this->m_fastLapInfo;
}

void CBaseDataLogger::GetFastestLapInSession()
{
	// TODO: parse SessionInfoString and extract the fastest lap number and time from it
	// ...
}

void CBaseDataLogger::GetFastestLapInStint()
{
	int fi =0;
	float ct =0; // current time
	float f =0;	// fastest lap

	for (int i =0; i < this->m_lapCnt; i++)
	{
		// exclude first and last lap from calculation, because it's unlikely that in-/out-lap is fastest
		if ( (i > 0) && (i < (this->m_lapCnt -1)) ) {
			ct =(float)this->m_lapInfo[i]->lapTime;

			if (f ==0) {
				f =ct;
				fi =i;
			}

			if (ct > 0 && ct < f) {
				f =ct;
				fi =i;
			}
		}
	}

	this->m_fastLapInfo->fastestLapStint =this->m_lapInfo[fi]->lapNo;
	this->m_fastLapInfo->fastestLapTimeInStint =this->m_lapInfo[fi]->lapTime;
}

int CBaseDataLogger::ShiftRpm()
{
	return m_carDataInfo.shiftLightRpm;
}

int CBaseDataLogger::RedlineRpm()
{
	return m_carDataInfo.redlineRpm;
}
