
#include "IracingBinaryTelemetryFile.h"
#include "exception.h"
#include "yaml_parser.h"

using namespace libOGA;
using namespace libOGA::FileLogs;


CIracingBinaryTelemetryFile::CIracingBinaryTelemetryFile(const std::wstring& logfile) : CBaseTelemetryFile(logfile),
m_header({ 0 }),
m_varHeaders(),
m_subHeader({ 0 }),
m_sessionInfo("")
{
	try {
		this->processDataFile();
	}
	catch (...) {
		throw;
	}
}

CIracingBinaryTelemetryFile::~CIracingBinaryTelemetryFile()
{
}

int CIracingBinaryTelemetryFile::GetSampleTickRate()
{
	return this->m_header.tickRate;
}

void CIracingBinaryTelemetryFile::processDataFile()
{
	try
	{
		this->readIBTHeader();
		this->readSubHeader();
		this->readVarHeaders();

		// after reading var headers initialize the channels
		m_sampleCnt = m_subHeader.sessionRecordCount; // number of samples

		for (int i = 0; i < m_header.numVars; i++) {

			//irsdk_varHeader* rec = (irsdk_varHeader*)&m_varHeaders[i];
			channels.emplace(make_pair(m_varHeaders[i].name, CDataChannel(m_varHeaders[i].name,
				m_varHeaders[i].unit,
				m_varHeaders[i].desc,
				i,
				m_varHeaders[i].type,
				m_varHeaders[i].offset, ChannelType_File)));

		}
		mathChannelsMap[CH_SP_SPEED_KPH] = "Speed";
		mathChannelsMap[CH_SP_SPEED_MPH] = "Speed";
		mathChannelsMap[CH_SP_LATG] = "LatAccel";
		mathChannelsMap[CH_SP_LONGG] = "LongAccel";
		//specialChannelsMap[CH_SP_LAP] = "Lap";

		mathChannels.emplace(make_pair(CH_SP_SPEED_KPH, CDataChannel(CH_SP_SPEED_KPH, "kph", "Ground speed in KPH", 0, irsdk_float, 0, ChannelType_Math)));
		mathChannels.emplace(make_pair(CH_SP_SPEED_MPH, CDataChannel(CH_SP_SPEED_MPH, "mph", "Ground speed in MPH", 1, irsdk_float, 1, ChannelType_Math)));
		mathChannels.emplace(make_pair(CH_SP_LATG, CDataChannel(CH_SP_LATG, "g", "Lateral G forces", 2, irsdk_float, 2, ChannelType_Math)));
		mathChannels.emplace(make_pair(CH_SP_LONGG, CDataChannel(CH_SP_LONGG, "g", "Longitudinal G forces", 3, irsdk_float, 3, ChannelType_Math)));
		mathChannels.emplace(make_pair(CH_SP_BEST_LAP, CDataChannel(CH_SP_BEST_LAP, "", "Best lap in stint", 4, irsdk_int, 4, ChannelType_Math)));
		mathChannels.emplace(make_pair(CH_SP_BEST_LAP_TIME, CDataChannel(CH_SP_BEST_LAP_TIME, "s", "Best lap time in stint", 5, irsdk_double, 5, ChannelType_Math)));
		//mathChannels.push_back(CDataChannel("LastLapTime", "s", "Last lap time", 6, irsdk_double, 6, ChannelType_Math));

		mathChannelBuf = std::vector<std::vector<SampleValue>>(mathChannels.size());

#ifdef _DEBUG
		this->readSessInfo();
#endif

		this->readDataBuf();

		this->GetFastestLapInStint();

	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

void CIracingBinaryTelemetryFile::ParseSessionInfo(const std::string& sessionInfoStr)
{
	const char* val = NULL;
	int len = 0;
	char str[128] = { 0 };
	std::stringstream ss;

	//, max_cars =64;
	//bool ret =0;
	//char key_str[512] ={0};
	//PCAR_RESULTS_INFO tmp_buf =NULL;
	//int i =0;

	if (parseYaml(sessionInfoStr.c_str(), "DriverInfo:DriverCarSLShiftRPM:", &val, &len))
	{
		strncpy_s(str, 128, val, len);
		str[len] = '\0';

		m_carDataInfo.shiftLightRpm = (int)atoi(str);
	}

	if (parseYaml(sessionInfoStr.c_str(), "DriverInfo:DriverCarRedLine:", &val, &len))
	{
		strncpy_s(str, 128, val, len);
		str[len] = '\0';

		m_carDataInfo.redlineRpm = (int)atoi(str);
	}

	for (int i = 0; i < 0xFFFF; i++) {

		ss = std::stringstream();
		ss << "SplitTimeInfo:Sectors:SectorNum:{" << i << "}SectorStartPct:";
		std::string key_str = ss.str();

		if (parseYaml(sessionInfoStr.c_str(), key_str.c_str(), &val, &len)) {

			float t = roundf(((float)atof(val) * 1000.0f));
			t /= 1000.0f;
			sectors.insert(make_pair(i + 1, t));
		}
		else // no more entries in YAML array
			break;
	}

	//PSESSION_INFO_DATA sid =new SESSION_INFO_DATA;
	//memset(sid, 0, sizeof(SESSION_INFO_DATA));

	//	// parse session info
	//	// allocate new session info
	//	PSESSION_INFO si =new SESSION_INFO;
	//	memset(si, 0, sizeof(SESSION_INFO));

	//	if (parseYaml(sessionInfoStr, "SessionInfo:Sessions:SessionNum:{0}SessionLaps", &val, &len))
	//	{
	//		strncpy(str, val, len);
	//		str[len] ='\0';
	//		if (strcmp(str, "unlimited") ==0)
	//			si->SessionLaps =-1;
	//		else 
	//			si->SessionLaps =atoi(str);
	//	}
	//	val =NULL;
	//	len =0;
	//	if (parseYaml(sessionInfoStr, (const char*)"SessionInfo:Sessions:SessionNum:{0}SessionTime:", &val, &len))
	//	{
	//		strncpy(str, val, len);
	//		str[len] ='\0';
	//		si->SessionTime =atof(str);
	//	}
	//	val =NULL;
	//	len =0;
	//	if (parseYaml(sessionInfoStr, (const char*)"SessionInfo:Sessions:SessionNum:{0}SessionType:", &val, &len))
	//	{
	//		strncpy(str, val, len);
	//		str[len] ='\0';
	//		if (strcmp(str, "Practice") ==0)
	//			si->SessionType =sessionType_practice;
	//		else
	//			si->SessionType =-1;
	//	}
	//	val =NULL;
	//	len =0;

	//	// get session timing results
	//	// first allocate temporary buffer for max 64 cars
	//	tmp_buf =new CAR_RESULTS_INFO[max_cars];
	//	memset(tmp_buf, 0, sizeof(CAR_RESULTS_INFO) * max_cars);

	//	ret =true;
	//	i =0;
	//	for (i =0; i < max_cars && ret; i++)
	//	{
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}CarIdx:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].CarIdx =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		if (!ret)
	//			break;

	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}Lap:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].Lap =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}Time:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].Time =atof(str);

	//			val =NULL;
	//			len =0;
	//		}

	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}FastestLap:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].FastestLap =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}FastestTime:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].FastestTime =atof(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}LastTime:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].LastTime =atof(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}LapsLed:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].LapsLed =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}LapsComplete:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].LapsComplete =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}LapsDriven:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].LapsDriven =atof(str);
	//
	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}Incidents:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].Incidents =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}ReasonOutId:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].ReasonOutId =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}
	//		sprintf(key_str, (const char*)"SessionInfo:Sessions:SessionNum:{0}ResultsPositions:Position:{%i}ReasonOutStr:", i+1);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			tmp_buf[i].ReasonOutStr =new char[len];
	//			strncpy(tmp_buf[i].ReasonOutStr, str, strlen(str)+1);

	//			val =NULL;
	//			len =0;
	//		}

	//		tmp_buf[i].Position =i+1;
	//	}

	//	// after we have all results info i should be the exact number of entries needed (# of cars in session)
	//	// so allocate the final array buffer
	//	int carCnt =i;
	//	PCAR_RESULTS_INFO cars =new CAR_RESULTS_INFO[carCnt];
	//	memset(cars, 0, sizeof(CAR_RESULTS_INFO) * carCnt);
	//	// fill the final array with all the info from the temporal array buffer; delete temporal buffer after this.
	//	for (i =0; i < carCnt; i++)
	//	{
	//		memcpy(&cars[i], &tmp_buf[i], sizeof(CAR_RESULTS_INFO));
	//	}
	//	delete[] tmp_buf;
	//	tmp_buf =NULL;

	//	// loop through all of the driver info and parse
	//	for (i =0; i < carCnt; i++) 
	//	{
	//		sprintf(key_str, (const char*)"DriverInfo:Drivers:CarIdx:{%i}UserName:", cars[i].CarIdx);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			cars[i].driverName =new char[len];
	//			strncpy(cars[i].driverName, str, strlen(str)+1);

	//			val =NULL;
	//			len =0;
	//		}

	//		sprintf(key_str, (const char*)"DriverInfo:Drivers:CarIdx:{%i}UserID:", cars[i].CarIdx);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			cars[i].driverID =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}

	//		sprintf(key_str, (const char*)"DriverInfo:Drivers:CarIdx:{%i}CarNumber:", cars[i].CarIdx);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			cars[i].carNumber =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}

	//		sprintf(key_str, (const char*)"DriverInfo:Drivers:CarIdx:{%i}CarClassShortName:", cars[i].CarIdx);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			cars[i].carClassShortName =new char[len];
	//			strncpy(cars[i].carClassShortName, str, strlen(str)+1);

	//			val =NULL;
	//			len =0;
	//		}

	//		sprintf(key_str, (const char*)"DriverInfo:Drivers:CarIdx:{%i}CarClassRelSpeed:", cars[i].CarIdx);
	//		if ( (ret =parseYaml(sessionInfoStr, key_str, &val, &len)) )
	//		{
	//			strncpy(str, val, len);
	//			str[len] ='\0';
	//			cars[i].carClassRelSpeed =atoi(str);

	//			val =NULL;
	//			len =0;
	//		}

	//	}

	//	si->carsInSession =carCnt;
	//	si->cars =cars;

	//	sid->sessi_data =si;

	//	delete si;
	//	delete sid;

}


void CIracingBinaryTelemetryFile::readIBTHeader()
{
	int len;
	DWORD ret = 0;

	len = sizeof(irsdk_header);
	if (!ReadFile(m_file, (LPVOID)&m_header, len, &ret, NULL)) {
		ret = GetLastError();
		CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
		throw ex;
	}
}

void CIracingBinaryTelemetryFile::readSubHeader()
{
	int len;
	DWORD ret = 0;

	len = sizeof(irsdk_diskSubHeader);
	if (!ReadFile(m_file, (LPVOID)&m_subHeader, len, &ret, NULL)) {
		ret = GetLastError();
		CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
		throw ex;
	}
}

void CIracingBinaryTelemetryFile::readVarHeaders()
{
	int len = sizeof(irsdk_varHeader)* m_header.numVars;
	DWORD ret = 0;
	m_varHeaders = std::vector<irsdk_varHeader>(m_header.numVars);

	SetFilePointer(m_file, m_header.varHeaderOffset, NULL, FILE_BEGIN);
	if (!ReadFile(m_file, &m_varHeaders[0], len, &ret, NULL)) {

		ret = GetLastError();
		CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
		throw ex;
	}
}

void CIracingBinaryTelemetryFile::readSessInfo()
{
	DWORD ret = 0;
	int len = m_header.sessionInfoLen;

	char* ptr = new char[len];
	if (ptr == nullptr)
		throw CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);

	SetFilePointer(m_file, m_header.sessionInfoOffset, NULL, FILE_BEGIN);
	if (!ReadFile(m_file, (LPVOID)ptr, len, &ret, NULL)) {
		ret = GetLastError();
		throw CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);;
	}
	ptr[m_header.sessionInfoLen - 1] = '\0';

	m_sessionInfo = std::string(ptr);
	ParseSessionInfo(m_sessionInfo);

	delete[] ptr;
	ptr = nullptr;
}

void CIracingBinaryTelemetryFile::readDataBuf()
{
	int len = m_header.bufLen;
	DWORD ret = 0;
	int ln = 0;
	size_t currentSector; // current Sector the car is in (1 based index)
	size_t sectorIdx; // sector index counter (0 based)
	float prevSectorTime = 0; // split time of previous sector
	int sampleCnt = m_subHeader.sessionRecordCount;
	SampleBufferRow row;
	LAPINFO lapInfo{ 0 };

	this->m_lapCnt = this->m_subHeader.sessionLapCount;
	m_lapInfo = std::vector<LAPINFO>(m_lapCnt);
	sampleBuffer = std::vector<SampleBufferRow>(sampleCnt);

	SetFilePointer(m_file, m_header.varBuf[0].bufOffset, NULL, FILE_BEGIN);

	for (int i = 0; i < sampleCnt; i++) {

		row = SampleBufferRow(len);
		if (!ReadFile(m_file, (LPVOID)&row[0], len, &ret, NULL)) {
			ret = GetLastError();
			CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
			throw ex;
		}

		SampleValue lapNo = CDataChannel::GetSampleData(row, channels["Lap"]);
		SampleValue sessTime = CDataChannel::GetSampleData(row, channels["SessionTime"]);
		SampleValue dist = CDataChannel::GetSampleData(row, channels["LapDistPct"]);
		SampleValue st = CDataChannel::GetSampleData(row, channels["SessionTime"]);

		float lapDist = dist.get_value<float>();
		//double sessTime = st.get_value<double>();
		int lap = lapNo.get_value<int>();
		double time = sessTime.get_value<double>();

		try {

			if (i == 0) {
				// first (valid) entry in sample,so get first lap number

				// lap value is always an int so skip value check!
				this->m_curLapIdx = 0;
				lapInfo.lapNo = lap;
				lapInfo.sampleIdx = i;
				lapInfo.SessTimeIdx = time;
				lapInfo.splitTimeInfo = std::vector<SECTOR_TIMING_INFO>(sectors.size());

				m_lapInfo[m_curLapIdx] = lapInfo;

				// get the current sector if the data file does not start on a lap boundary
				for (size_t i = 1; i < sectors.size() + 1; i++)
				{
					if (lapDist >= sectors[i])
					{
						currentSector = i;
						sectorIdx = i - 1;
						break;
					}
				}
				//m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorNum = currentSector;
				//m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorStartPct = sectors[currentSector];

				this->m_lastLap = lapInfo.lapNo;
				ln = __LINE__;
			}

			// last lap complete; start of new lap
			if (this->m_lastLap < lap) {
				ln = __LINE__;

				// calculate sector time of last sector of previous lap
				sectorIdx = currentSector - 1;
				m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorNum = currentSector;
				m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorStartPct = sectors[currentSector];
				m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorTime = (float)((float)time - m_lapInfo[m_curLapIdx].SessTimeIdx - prevSectorTime);
				prevSectorTime = 0;
				currentSector = 1;


				this->m_curLapIdx++;

				lapInfo.lapNo = lap;
				lapInfo.sampleIdx = i;
				lapInfo.SessTimeIdx = time;
				lapInfo.splitTimeInfo = std::vector<SECTOR_TIMING_INFO>(sectors.size());

				m_lapInfo[m_curLapIdx] = lapInfo;

				// calculate laptime of last lap
				if (m_curLapIdx > 0 && (size_t)m_curLapIdx < m_lapInfo.size()) {
					LAPINFO& prevLap = m_lapInfo.at(m_curLapIdx - 1);
					prevLap.lapTime = lapInfo.SessTimeIdx - prevLap.SessTimeIdx;
					prevLap.sampleCnt = i - prevLap.sampleIdx;
				}
				this->m_lastLap = lap;

			}

			// last sample in sample buffer
			if (i == (sampleCnt - 1)) {

				ln = __LINE__;
				m_lapInfo[m_curLapIdx].sampleCnt = i - m_lapInfo[m_curLapIdx].sampleIdx;
				m_lapInfo[m_curLapIdx].SessTimeIdx = time - m_lapInfo[m_curLapIdx].SessTimeIdx;
			}

			// check if current sector is complete
			if (currentSector < sectors.size()) {
				if (lapDist >= sectors[currentSector + 1]) { // begin of next sector?

					sectorIdx = currentSector - 1;
					m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorNum = currentSector;
					m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorStartPct = sectors[currentSector];
					m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorTime = (float)((float)time - m_lapInfo[m_curLapIdx].SessTimeIdx - prevSectorTime);
					prevSectorTime += m_lapInfo[m_curLapIdx].splitTimeInfo[sectorIdx].sectorTime;
					currentSector++;
				}
			}
		}
		catch (...) {
			CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)-1, true);
			throw ex;
		}

		sampleBuffer[i] = std::move(row);

	}

#ifdef _DEBUG
	int x = 0;
#endif
}

bool CIracingBinaryTelemetryFile::GetSample(DataSample& s, size_t idx)
{
	if (idx >= 0 && idx < sampleBuffer.size())
	{
		s = sampleBuffer[idx];
		return true;
	}

	return false;
}

bool CIracingBinaryTelemetryFile::GetSample(DataSample& s)
{
	return GetSample(s, 0);
}
