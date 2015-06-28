
#include "IracingBinaryTelemetryFile.h"

using namespace libDataLogging::DataLogger;


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
	//for (std::vector<CBaseChannel&>::iterator it = specialChannels.begin(); it != specialChannels.end(); ++it) {

	//	CBaseChannel ch = (*it);
	//	if (ch != NULL) {
	//		delete ch;
	//		ch = NULL;
	//	}
	//}
	//for (std::vector<CBaseChannel&>::iterator it = exportedChannels.begin(); it != exportedChannels.end(); ++it) {

	//	CBaseChannel* ch = (*it);
	//	if (ch != NULL) {
	//		delete ch;
	//		ch = NULL;
	//	}
	//}
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

			irsdk_varHeader* rec = (irsdk_varHeader*)&m_varHeaders[i];
			channels.emplace(make_pair(rec->name, CDataChannel(rec->name, rec->unit, rec->desc, i, rec->type, rec->offset)));

		}
		mathChannelsMap[CH_SP_SPEED_KPH] = "Speed";
		mathChannelsMap[CH_SP_SPEED_MPH] = "Speed";
		mathChannelsMap[CH_SP_LATG] = "LatAccel";
		mathChannelsMap[CH_SP_LONGG] = "LongAccel";
		//specialChannelsMap[CH_SP_LAP] = "Lap";

		mathChannels.push_back(CDataChannel("Speed_Kph", "kph", "Ground speed in KPH", 0, irsdk_float, 0));
		mathChannels.push_back(CDataChannel("Speed_Mph", "mph", "Ground speed in MPH", 1, irsdk_float, 1));
		mathChannels.push_back(CDataChannel("LatG", "g", "Lateral G forces", 2, irsdk_float, 2));
		mathChannels.push_back(CDataChannel("LongG", "g", "Longitudinal G forces", 3, irsdk_float, 3));
		//specialChannels.push_back((CBaseChannel*)new CChannel<float>("BestLap", "Best lap in stint", "", m_sampleCnt));
		//specialChannels.push_back((CBaseChannel*)new CChannel<float>("BestLapTime", "Best lap time in stint", "s", m_sampleCnt));
		//specialChannels.push_back((CBaseChannel*)new CChannel<float>("LastLapTime", "Last lap time", "s", m_sampleCnt));

#ifdef _DEBUG
		this->readSessInfo();
#endif

		this->readDataBuf();

		this->GetFastestLapInStint();

	}
	catch (...)
	{
		throw;
	}
}

void CIracingBinaryTelemetryFile::ParseSessionInfo(const char* sessionInfoStr)
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

	if (!sessionInfoStr)
		return;

	if (parseYaml(sessionInfoStr, "DriverInfo:DriverCarSLShiftRPM:", &val, &len))
	{
		strncpy_s(str, 128, val, len);
		str[len] = '\0';

		m_carDataInfo.shiftLightRpm = (int)atoi(str);
	}

	if (parseYaml(sessionInfoStr, "DriverInfo:DriverCarRedLine:", &val, &len))
	{
		strncpy_s(str, 128, val, len);
		str[len] = '\0';

		m_carDataInfo.redlineRpm = (int)atoi(str);
	}

	for (int i = 0; i < 0xFFFF; i++) {

		ss = std::stringstream();
		ss << "SplitTimeInfo:Sectors:SectorNum:{" << i << "}SectorStartPct:";
		std::string key_str = ss.str();

		if (parseYaml(sessionInfoStr, key_str.c_str(), &val, &len)) {

			//std::stringstream s = std::stringstream();
			//s.setf(ios::fixed);
			//s.precision(3);
			//s << val;

			//float t = stof(s.str().c_str());
			//t = roundf((t * 1000) / 1000);
			float t = roundf(((float)atof(val) * 1000.0f));
			t /= 1000.0f;

			//t = (t * 1000) / 1000;

			SECTOR_INFO si = SECTOR_INFO();
			si.sectorNum = (char)i;
			si.sectorStartPct = t;

			sectors.push_back(si);
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
	int len;
	DWORD ret = 0;

	len = sizeof(irsdk_varHeader)* m_header.numVars;
	irsdk_varHeader* ptr = new irsdk_varHeader[m_header.numVars];
	if (ptr == nullptr) {
		CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, (DWORD)-1, true);
		throw ex;
	}

	SetFilePointer(m_file, m_header.varHeaderOffset, NULL, FILE_BEGIN);
	if (!ReadFile(m_file, ptr, len, &ret, NULL)) {
		delete ptr;
		ptr = nullptr;

		ret = GetLastError();
		CErrEx ex = CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
		throw ex;
	}

	for (int i = 0; i < m_header.numVars; i++) {

		m_varHeaders.emplace_back(ptr[i]);
		exportedChannelNames.push_back(std::string(ptr[i].name));
	}

	delete ptr;
	ptr = nullptr;
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

	delete ptr;
	ptr = nullptr;
}

//void CIracingBinaryTelemetryFile::extractChannelData(char* buf)
//{
//
//	for (unsigned int i = 0; i < exportedChannels.size(); i++)
//	{
//		irsdk_varHeader* rec = &m_varHeaders[i];
//		assert(m_header.numVars == (int)exportedChannels.size());
//		if (std::string(rec->name) == exportedChannels[i]->GetName()) {
//
//			if (exportedChannels[i]->GetType() == irsdk_int) {
//
//				int v = 0;
//				memcpy(&v, (buf + rec->offset), sizeof(int));
//				((CChannel<int>*)exportedChannels[i])->AddSample(v);
//			}
//
//			if (exportedChannels[i]->GetType() == irsdk_float) {
//
//				float v = 0;
//				memcpy(&v, (buf + rec->offset), sizeof(float));
//
//				if (exportedChannels[i]->GetName() == "Throttle")
//					v *= 100.0f;
//				if (exportedChannels[i]->GetName() == "Brake")
//					v *= 100.0f;
//				if (exportedChannels[i]->GetName() == "Clutch")
//					v *= 100.0f;
//
//				((CChannel<float>*)exportedChannels[i])->AddSample(v);
//			}
//
//			if (exportedChannels[i]->GetType() == irsdk_double) {
//
//				double v = 0;
//				memcpy(&v, (buf + rec->offset), sizeof(double));
//				((CChannel<double>*)exportedChannels[i])->AddSample(v);
//			}
//
//			if (exportedChannels[i]->GetType() == irsdk_char) {
//
//				char v = 0;
//				memcpy(&v, (buf + rec->offset), sizeof(char));
//				((CChannel<char>*)exportedChannels[i])->AddSample(v);
//			}
//
//			if (exportedChannels[i]->GetType() == irsdk_bool) {
//
//				bool v = 0;
//				memcpy(&v, (buf + rec->offset), sizeof(bool));
//				((CChannel<bool>*)exportedChannels[i])->AddSample(v);
//			}
//		}
//	}
//}


void CIracingBinaryTelemetryFile::readDataBuf()
{
	int len = m_header.bufLen;
	DWORD ret = 0;
	int ln = 0;
	size_t k = 0;
	int prevLap = 0;

	try {
		sampleBuffer = std::shared_ptr<BYTE>((BYTE*)new char[len]);
	}
	catch (std::exception ex)
	{
		throw ex;
	}

	this->m_lapCnt = this->m_subHeader.sessionLapCount;

	SetFilePointer(m_file, m_header.varBuf[0].bufOffset, NULL, FILE_BEGIN);
	if (!ReadFile(m_file, (LPVOID)sampleBuffer.get(), len, &ret, NULL)) {
		ret = GetLastError();
		CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, __LINE__, ret, true);
		throw ex;
	}

	int sampleCnt = m_subHeader.sessionRecordCount;
	for (int i = 0; i < sampleCnt; i++) {

		try {

			if (i == 0) {
				// first (valid) entry in sample,so get first lap number
				this->m_curLapIdx = 0;
				this->m_lapInfo[this->m_curLapIdx].lapNo = channels["Lap"];
				this->m_lapInfo[this->m_curLapIdx]->sampleIdx = i;
				this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx = ((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i); /*channels.chSessionTime.GetChannelData(i);*/

				this->m_lastLap = ((CChannel<int>*)exportedChannels[channelsMap["Lap"]])->GetChannelData(i);
				ln = __LINE__;
			}
			else if (i > 1) {
				if (this->m_lastLap < ((CChannel<int>*)exportedChannels[channelsMap["Lap"]])->GetChannelData(i)) {
					ln = __LINE__;

					this->m_curLapIdx++;
					this->m_lapInfo[this->m_curLapIdx]->lapNo = ((CChannel<int>*)exportedChannels[channelsMap["Lap"]])->GetChannelData(i);
					this->m_lapInfo[this->m_curLapIdx]->sampleIdx = i;
					this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx = ((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i);
					// calculate laptime of last lap
					if (this->m_curLapIdx > 0) {
						//double t =(((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i-1) - 
						//		   this->m_lapInfo[this->m_curLapIdx-1]->SessTimeIdx);
						this->m_lapInfo[this->m_curLapIdx - 1]->lapTime = (float)((float)this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx -
							(float)this->m_lapInfo[this->m_curLapIdx - 1]->SessTimeIdx);
					}

					this->m_lastLap = ((CChannel<int>*)exportedChannels[channelsMap["Lap"]])->GetChannelData(i);
				}
				if (i == (sampleCnt - 1)) {
					if (this->m_curLapIdx != (this->m_lapCnt - 1))
					{
						ln = __LINE__;

						this->m_curLapIdx++;
						this->m_lapInfo[this->m_curLapIdx]->sampleIdx = i;
						this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx = ((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i);;
						//// calculate laptime of last lap
						if (this->m_curLapIdx > 0) {
							//double t =(((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i) - 
							//		   this->m_lapInfo[this->m_curLapIdx-1]->SessTimeIdx);
							this->m_lapInfo[this->m_curLapIdx - 1]->lapTime = (float)((float)this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx -
								(float)this->m_lapInfo[this->m_curLapIdx - 1]->SessTimeIdx);
						}
					}
					else {
						ln = __LINE__;
						double t = ((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i) - this->m_lapInfo[this->m_curLapIdx]->SessTimeIdx;
						this->m_lapInfo[this->m_curLapIdx]->lapTime = (float)t;
						this->m_lapInfo[this->m_curLapIdx]->sampleCnt = i - this->m_lapInfo[this->m_curLapIdx]->sampleIdx;

					}
				}
			}

			SECTOR_TIMING_INFO sti = SECTOR_TIMING_INFO();
			float lapDist = ((CChannel<float>*)exportedChannels[channelsMap["LapDistPct"]])->GetChannelData(i);
			double sessTime = ((CChannel<double>*)exportedChannels[channelsMap["SessionTime"]])->GetChannelData(i);

			if (sectors.size() > 0) {
				if (k == sectors.size())
					k = 0;

				// new lap? start again
				if (m_curLapIdx > prevLap) {

					sti.sectorNum = 0;
					sti.sectorStartPct = 0;
					sti.sectorTime = 0;
					m_lapInfo[m_curLapIdx]->splitTimeInfo.push_back(sti);
					prevLap = m_curLapIdx;
					k = 1;
				}
				else if ((k > 0) && (lapDist > sectors[k].sectorStartPct)) {

					sti.sectorNum = sectors[k].sectorNum;
					sti.sectorStartPct = sectors[k].sectorStartPct;
					sti.sectorTime = (float)((float)sessTime - (float)m_lapInfo[m_curLapIdx]->SessTimeIdx);

					m_lapInfo[m_curLapIdx]->splitTimeInfo.push_back(sti);
					k++;
				}
			}

			for (std::vector<CBaseChannel*>::iterator it = specialChannels.begin(); it != specialChannels.end(); ++it)
			{
				CBaseChannel* ch = *it;
				float v = 0;

				if (ch->GetName() == std::string(CH_SP_SPEED_KPH))
				{
					v = ((CChannel<float>*)exportedChannels[channelsMap[specialChannelsMap[CH_SP_SPEED_KPH]]])->GetChannelData(i);
					v *= 3.6f;
					((CChannel<float>*)ch)->AddSample(v);
				}
				else if (ch->GetName() == std::string(CH_SP_SPEED_MPH))
				{
					v = ((CChannel<float>*)exportedChannels[channelsMap[specialChannelsMap[CH_SP_SPEED_MPH]]])->GetChannelData(i);
					v *= 2.23694f;
					((CChannel<float>*)ch)->AddSample(v);
				}
				else if (ch->GetName() == std::string(CH_SP_LATG))
				{
					v = ((CChannel<float>*)exportedChannels[channelsMap[specialChannelsMap[CH_SP_LATG]]])->GetChannelData(i);
					v /= 9.81f;
					((CChannel<float>*)ch)->AddSample(v);
				}
				else if (ch->GetName() == std::string(CH_SP_LONGG))
				{
					v = ((CChannel<float>*)exportedChannels[channelsMap[specialChannelsMap[CH_SP_LONGG]]])->GetChannelData(i);
					v /= 9.81f;
					((CChannel<float>*)ch)->AddSample(v);
				}
			}
		}
		catch (...) {
			CErrEx* ex = new CErrEx(__MODULE__, (char*)typeid(this).name(), __FUNCTION__, __FILE__, ln, (DWORD)-1, true);
			throw ex;
		}
	}

	for (int i = 0; i < m_lapCnt; i++) {
		if (i > 0) {
			m_lapInfo[i - 1]->sampleCnt = (m_lapInfo[i]->sampleIdx - m_lapInfo[i - 1]->sampleIdx);
		}
	}

	if (varBuf)
		delete[] varBuf;
	varBuf = NULL;
}
