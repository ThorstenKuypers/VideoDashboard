///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011 Thorsten Kuypers
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You should have obtained a copy of the License with this Software. If not,
//	you may obtain a copy of the License at
//
//				http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
// * version history:
//   -----------------
//
//	0.4 - first public release
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataLogger.h"


using namespace libDataLogging::DataLogger;


#pragma region // class CDataLogger implementation
CDataLogger::CDataLogger(wchar_t* logfile) : IDataLogging()
{
	wchar_t* ext = NULL;

	ext = wcsrchr(logfile, L'.');
	if (ext) {
		ext++;
		if (wcscmp(ext, L"ibt") == 0) {
			//m_ibtLogger = new CibtDataLogger(logfile);
			m_dataLogger = (CBaseDataLogger*)new CibtDataLogger(logfile);//(CBaseDataLogger*)m_ibtLogger;
			m_LoggerType = Ibt;
			if (!m_dataLogger) {
				CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)-1, true);
				throw ex;
			}
		}
		else if (wcscmp(ext, L"csv") == 0) {
			//m_csvLogger =new CcsvDataLogger(logfile);
			m_dataLogger = (CBaseDataLogger*)new CcsvDataLogger(logfile);
			m_LoggerType = Csv;
			if (!m_dataLogger) {
				CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)-1, true);
				throw ex;
			}
		}
	}
}

CDataLogger::~CDataLogger(void)
{
	if (this->m_dataLogger) {
		delete this->m_dataLogger;
		this->m_dataLogger = NULL;
	}
}

int CDataLogger::GetSampleCount()
{
	return this->m_dataLogger->GetSampleCount();
}

int CDataLogger::GetSampleTickRate()
{
	return this->m_dataLogger->GetSampleTickRate();
}

int CDataLogger::GetSessionLapCount()
{
	return this->m_dataLogger->GetSessionLapCount();
}

PLAPINFO CDataLogger::GetLapInfo(int lap)
{
	return this->m_dataLogger->GetLapInfo(lap);
}

PFAST_LAP_INFO CDataLogger::GetFastLapInfo()
{
	return this->m_dataLogger->GetFastLapInfo();
}

int CDataLogger::RedlineRPM()
{
	return m_dataLogger->RedlineRpm();
}

int CDataLogger::ShiftLightRPM()
{
	return m_dataLogger->ShiftRpm();
}

std::vector<std::string>& CDataLogger::GetExportedChannelNames()
{
	return m_dataLogger->exportedChannelNames;
}

void CDataLogger::GetSpecialChannelNames(std::vector<std::string>* vc)
{
	if (vc != nullptr) {

		for (auto c : m_dataLogger->specialChannelsMap) {

			vc->push_back(c.first);
		}
	}
}

std::vector<CBaseChannel*>& CDataLogger::GetExportedChannels()
{
	return m_dataLogger->exportedChannels;
}

CBaseChannel& CDataLogger::GetChannel(std::string channelName)
{
	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end())
		throw ChannelNotFoundException(channelName);

	CBaseChannel* ch = m_dataLogger->exportedChannels[it->second];

	return (*ch);
}

int CDataLogger::GetChannelDataType(std::string channelName)
{

	// filter special channel names
	if (channelName == std::string(CH_SP_LAP_TIME))
		return irsdk_float;
	else if (channelName == std::string(CH_SP_BEST_LAP_TIME))
		return irsdk_float;
	else if (channelName == std::string(CH_SP_SECTOR_SPLIT))
		return irsdk_float;
	else if (channelName == std::string(CH_SP_SECTOR_TIME))
		return irsdk_float;
	else if (channelName == std::string(CH_SP_CURRENT_SECTOR))
		return irsdk_int;
	else if (channelName == std::string(CH_SP_LAP))
		return irsdk_int;
	else if (channelName == std::string(CH_SP_BEST_LAP))
		return irsdk_int;

	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end()) {

		// check for special names (math channels)
		specials_map::iterator spit = m_dataLogger->specialChannelsMap.find(channelName);
		if (spit == m_dataLogger->specialChannelsMap.end())
			throw ChannelNotFoundException(channelName);

		return irsdk_float;
	}

	return m_dataLogger->exportedChannels[it->second]->GetType();
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, double* val)
{
	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end())
		throw ChannelNotFoundException(channelName);

	if (m_dataLogger->exportedChannels[it->second]->GetType() == irsdk_double) {

		double v = ((CChannel<double>*)m_dataLogger->exportedChannels[it->second])->GetChannelData(sampleIndex);
		memcpy(val, &v, sizeof(double));
	}
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, int* val)
{
	channel_map::iterator it;

	if (channelName == std::string(CH_SP_LAP))
		it = m_dataLogger->channelsMap.find("Lap");
	else if (channelName == std::string(CH_SP_BEST_LAP)) {

		*val = m_dataLogger->GetFastLapInfo()->fastestLapStint;
		return;
	}
	else if (channelName == std::string(CH_SP_CURRENT_SECTOR))
	{
		int lap = 0;
		float lapDist = 0;
		int sectorNum = 0;

		lap = ((CChannel<int>*)m_dataLogger->exportedChannels[m_dataLogger->channelsMap["Lap"]])->GetChannelData(sampleIndex);
		lapDist = ((CChannel<float>*)m_dataLogger->exportedChannels[m_dataLogger->channelsMap["LapDistPct"]])->GetChannelData(sampleIndex);

		PLAPINFO pli = NULL;
		for (int i = 0; i < m_dataLogger->GetSessionLapCount(); i++) {

			pli = m_dataLogger->GetLapInfo(i);
			if (pli->lapNo == lap)
				break;
		}



		// iterate through all the sectors and find closest match to lapDistPct
		for (size_t k = pli->splitTimeInfo.size(); k > 0; k--)
		{
			if (lapDist > pli->splitTimeInfo[k - 1].sectorStartPct) {

				sectorNum = pli->splitTimeInfo[k - 1].sectorNum;

				if (val)
					*val = sectorNum;

				return;
			}
		}
		return;
	}
	else
		it = m_dataLogger->channelsMap.find(channelName);

	if (it == m_dataLogger->channelsMap.end())
		throw ChannelNotFoundException(channelName);

	if (m_dataLogger->exportedChannels[it->second]->GetType() == irsdk_int) {

		int v = ((CChannel<int>*)m_dataLogger->exportedChannels[it->second])->GetChannelData(sampleIndex);
		memcpy(val, &v, sizeof(int));
	}
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, float* val)
{
	float v = 0;

#pragma region "handling of lap time channel"
	// check if channel is one of the other special channels
	if (channelName == std::string(CH_SP_LAP_TIME)) {

		// calculate and return current laptime
		double curTime = 0;
		int lap = 0;

		channel_map::iterator it_lap = m_dataLogger->channelsMap.find("Lap");
		channel_map::iterator it_time = m_dataLogger->channelsMap.find("SessionTime");
		if (it_lap != m_dataLogger->channelsMap.end() && it_time != m_dataLogger->channelsMap.end()) {
			lap = ((CChannel<int>*)m_dataLogger->exportedChannels[it_lap->second])->GetChannelData(sampleIndex);
			curTime = ((CChannel<double>*)m_dataLogger->exportedChannels[it_time->second])->GetChannelData(sampleIndex);

			PLAPINFO pli = NULL;
			for (int i = 0; i < m_dataLogger->GetSessionLapCount(); i++) {

				pli = m_dataLogger->GetLapInfo(i);
				if (pli->lapNo == lap)
					break;
			}

			float time = (float)curTime - (float)pli->SessTimeIdx;

			if (val != NULL)
				*val = time;

			return;
		}
	}
	else if (channelName == std::string(CH_SP_BEST_LAP_TIME)) {

		if (val != NULL)
			*val = m_dataLogger->GetFastLapInfo()->fastestLapTimeInStint;

		return;
	}
#pragma endregion

#pragma region "handling of sectors"

	if (channelName == std::string(CH_SP_SECTOR_TIME))
	{
		int lap = 0;
		float lapDist = 0;
		float sectorTime = 0;

		lap = ((CChannel<int>*)m_dataLogger->exportedChannels[m_dataLogger->channelsMap["Lap"]])->GetChannelData(sampleIndex);
		lapDist = ((CChannel<float>*)m_dataLogger->exportedChannels[m_dataLogger->channelsMap["LapDistPct"]])->GetChannelData(sampleIndex);

		PLAPINFO pli = NULL;
		for (int i = 0; i < m_dataLogger->GetSessionLapCount(); i++) {

			pli = m_dataLogger->GetLapInfo(i);
			if (pli->lapNo == lap)
				break;
		}



		// iterate through all the sectors and find closest match to lapDistPct
		for (size_t k = pli->splitTimeInfo.size(); k > 0; k--)
		{
			if (lapDist > pli->splitTimeInfo[k - 1].sectorStartPct) {

				sectorTime = pli->splitTimeInfo[k - 1].sectorTime;

				if (val)
					*val = sectorTime;

				return;
			}
		}
		return;
	}
#pragma endregion


	// 1. check if channel is a standart channel name
	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end())
	{

		// 2. check if requested channel is a special channel name (math channel)
		specials_map::iterator spit = m_dataLogger->specialChannelsMap.find(channelName);
		if (spit == m_dataLogger->specialChannelsMap.end()) {

			// requested channel name was also not found in special channels,
			// so throw an exception
			throw ChannelNotFoundException(channelName);
		}
		else {

			// return value from special channel

			for (size_t i = 0; m_dataLogger->specialChannels.size(); i++) {
				if (m_dataLogger->specialChannels[i]->GetName() == channelName) {

					v = ((CChannel<float>*)m_dataLogger->specialChannels[i])->GetChannelData(sampleIndex);
					memcpy(val, &v, sizeof(float));

					return;
				}
			}
		}
	}

	if (m_dataLogger->exportedChannels[it->second]->GetType() == irsdk_float) { // iterate all channels with data type double

		v = ((CChannel<float>*)m_dataLogger->exportedChannels[it->second])->GetChannelData(sampleIndex);
		memcpy(val, &v, sizeof(float));
	}
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, bool* val)
{
	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end())
		throw ChannelNotFoundException(channelName);

	if (m_dataLogger->exportedChannels[it->second]->GetType() == irsdk_bool) { // iterate all channels with data type double

		bool v = ((CChannel<bool>*)m_dataLogger->exportedChannels[it->second])->GetChannelData(sampleIndex);
		memcpy(val, &v, sizeof(bool));
	}
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, char* val)
{
	channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
	if (it == m_dataLogger->channelsMap.end())
		throw ChannelNotFoundException(channelName);

	if (m_dataLogger->exportedChannels[it->second]->GetType() == irsdk_char) { // iterate all channels with data type double

		char v = ((CChannel<char>*)m_dataLogger->exportedChannels[it->second])->GetChannelData(sampleIndex);
		memcpy(val, &v, sizeof(char));
	}
}

void CDataLogger::GetChannelData(string channelName, int sampleIndex, void* data)
{
	try {

		double vd = 0;
		float vf = 0;
		int vi = 0;
		char vc = 0;
		bool vb = false;

		if (data == nullptr)
			return;

		channel_map::iterator it;

		if (channelName == std::string(CH_SP_LAP))
			it = m_dataLogger->channelsMap.find("Lap");
		else if (channelName == std::string(CH_SP_BEST_LAP))
		{

			GetChannelData(channelName, sampleIndex, &vi);
			if (data)
				memcpy(data, &vi, sizeof(int));

			return;
		}
		else if (channelName == std::string(CH_SP_CURRENT_SECTOR))
		{
			GetChannelData(channelName, sampleIndex, &vi);

			if (data)
				memcpy(data, &vi, sizeof(int));

			return;
		}
		else
			it = m_dataLogger->channelsMap.find(channelName);

		//channel_map::iterator it = m_dataLogger->channelsMap.find(channelName);
		if (it == m_dataLogger->channelsMap.end())
		{

			// check if requested channel is a special channel name
			specials_map::iterator spit = m_dataLogger->specialChannelsMap.find(channelName);
			if (spit == m_dataLogger->specialChannelsMap.end()) {

				// requested channel name was also not found in special channels,
				// so throw an exception
				throw ChannelNotFoundException(channelName);
			}
			else {

				GetChannelData(channelName, sampleIndex, (float*)&vf);
				if (data)
					memcpy(data, &vf, sizeof(float));

				return;
			}
		}

		switch (m_dataLogger->exportedChannels[it->second]->GetType())
		{
		case irsdk_double:
			GetChannelData(channelName, sampleIndex, (double*)&vd);
			memcpy(data, &vd, sizeof(double));
			break;
		case irsdk_float:
			GetChannelData(channelName, sampleIndex, (float*)&vf);
			memcpy(data, &vf, sizeof(float));
			break;
		case irsdk_int:
			GetChannelData(channelName, sampleIndex, (int*)&vi);
			memcpy(data, &vi, sizeof(int));
			break;
		case irsdk_char:
			GetChannelData(channelName, sampleIndex, (char*)&vc);
			memcpy(data, &vc, sizeof(char));
			break;
		case irsdk_bool:
			GetChannelData(channelName, sampleIndex, (bool*)&vb);
			memcpy(data, &vb, sizeof(bool));
			break;
		}
	}
	catch (ChannelDataTypeMismatchException& typeMismatch)
	{
		throw typeMismatch;
	}
	catch (ChannelNotFoundException& notFound)
	{
		throw notFound;
	}
}

#pragma endregion // CDataLogger
