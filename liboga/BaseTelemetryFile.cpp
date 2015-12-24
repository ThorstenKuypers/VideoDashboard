
#include "BaseTelemetryFile.h"
#include "exception.h"

using namespace libOGA;
using namespace libOGA::FileLogs;

CBaseTelemetryFile::CBaseTelemetryFile(const std::wstring& logfile) :
m_file(NULL),
m_lapCnt(0),
m_lastLap(0),
m_curLapIdx(0),
m_lapInfo(0),
m_fastLapInfo({ 0 }),
_lapsInfo(0),
_fastestLap(0, 0.0f),
m_sampleCnt(0),
channels(),
mathChannelsMap(),
mathChannels(),
sectors(),
m_carDataInfo()
{
	m_file = CreateFileW(logfile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_file == INVALID_HANDLE_VALUE)
	{
		CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, GetLastError(), true);
		throw ex;
	}
}

CBaseTelemetryFile::~CBaseTelemetryFile()
{
	CloseFile();
}

void CBaseTelemetryFile::CloseFile()
{
	if (m_file != INVALID_HANDLE_VALUE)
		CloseHandle(m_file);
}

int CBaseTelemetryFile::GetSampleCount()
{
	return m_sampleCnt;
}

LAPINFO& CBaseTelemetryFile::GetLapInfo(int lap)
{
	return this->m_lapInfo[lap];
}

int CBaseTelemetryFile::GetSessionLapCount()
{
	return this->m_lapCnt;
}

FASTEST_LAP_INFO& CBaseTelemetryFile::GetFastLapInfo()
{
	return this->m_fastLapInfo;
}

void CBaseTelemetryFile::GetFastestLapInSession()
{
	// TODO: parse SessionInfoString and extract the fastest lap number and time from it
	// ...
}

void CBaseTelemetryFile::GetFastestLapInStint()
{
	size_t idx = 0; // lap index of fastest lap in stint data
	float lapTime = 0; // current time
	float fastLap = 0;	// fastest lap

	for (size_t i = 0; i < this->m_lapInfo.size(); i++)
	{
		// exclude first and last lap from calculation, because it's unlikely that in-/out-lap is fastest
		if ((i > 0) && (i < (this->m_lapInfo.size() - 1))) {
			lapTime = (float)this->m_lapInfo[i].lapTime;
			if (fastLap == 0) {
				fastLap = lapTime;
				idx = i;
			}

			if (lapTime > 0 && lapTime < fastLap) {
				fastLap = lapTime;
				idx = i;
			}
		}
	}

	this->m_fastLapInfo.fastestLapStint = this->m_lapInfo[idx].lapNo;
	this->m_fastLapInfo.fastestLapTimeInStint = (float)this->m_lapInfo[idx].lapTime;
}

int CBaseTelemetryFile::ShiftRpm()
{
	return m_carDataInfo.shiftLightRpm;
}

int CBaseTelemetryFile::RedlineRpm()
{
	return m_carDataInfo.redlineRpm;
}

CDataChannel& CBaseTelemetryFile::GetChannel(std::string& name)
{
	try {

		// check if name is one of the math channels
		if (name == CH_SP_SPEED_KPH || name == CH_SP_SPEED_MPH || name == CH_SP_LATG || name == CH_SP_LONGG || name == CH_SP_LAP_TIME ||
			name == CH_SP_LAP || name == CH_SP_BEST_LAP_TIME || name == CH_SP_BEST_LAP || name == CH_SP_CURRENT_SECTOR ||
			name == CH_SP_SECTOR_TIME)
			return mathChannels[name];
		else
			return channels[name];
	}
	catch (std::exception)
	{
		throw;
	}

}

vector<std::string> CBaseTelemetryFile::GetExportedChannels()
{
	vector<std::string> v;

	for (auto c : channels) {
		v.emplace_back(c.second.get_Name());
	}

	return std::move(v);
}
vector<std::string> CBaseTelemetryFile::GetSpecialChannelNames()
{
	vector<std::string> v;

	for (auto c : mathChannels) {
		v.emplace_back(c.second.get_Name());
	}

	return std::move(v);
}

