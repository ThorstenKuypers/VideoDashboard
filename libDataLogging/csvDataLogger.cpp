
#include "csvDataLogger.h"

using namespace libDataLogging::DataLogger;

char* csvColumnHeaders[] = { "Time", "Corr Speed", "Gear", "Engine RPM", "Throttle Pos", "Brake Pos", "Steering", "Lap Number", "Ground Speed",
"G Force Lat", "G Force Long" };


CcsvDataLogger::CcsvDataLogger(wchar_t *logfile) : libDataLogging::DataLogger::CBaseDataLogger(logfile)
{
	//	CloseFile();

	//	m_logFile =logfile;

	m_lines = nullptr;
	m_filePos = 0;
	m_fileSize = GetFileSize(this->m_file, NULL);
	m_eof = false;

	m_csvHeader = new CSV_FILE_HEADER;
	if (m_csvHeader == nullptr)
	{
		throw (HRESULT)E_OUTOFMEMORY;
	}
	memset(m_csvHeader, 0, sizeof(CSV_FILE_HEADER));

	_csvVarHeaders = std::vector<CSV_VAR_HEADER>();
	//memset(&varIndex, 0, sizeof(int) * CSV_VARBUF_CNT);

	// NEW
	// open csv file for further processing
	//_csvFile = ifstream(logfile);

	exportedChannelNames = std::vector<string>();
	exportedChannels = std::vector<CBaseChannel*>();

	try {
		readCsvHeader();

		readVarHeaders();

		m_sampleCnt = (int)((float)m_csvHeader->duration / (float)((float)1 / (float)m_csvHeader->sampleRate)) + 1;

		// init channels
		for (int i = 0; i < (int)_csvVarHeaders.size(); i++)
		{
			// TODO: decide / check if other data types than float must be used
			exportedChannels.push_back((CBaseChannel*)new CChannel<float>((char*)_csvVarHeaders[i].name.c_str(), "", (char*)_csvVarHeaders[i].units.c_str(), m_sampleCnt));
			exportedChannelNames.push_back(_csvVarHeaders[i].name);
			channelsMap[_csvVarHeaders[i].name] = i;
		}

		processDataFile();
	}
	catch (...) {
		throw;
	}

}

CcsvDataLogger::~CcsvDataLogger()
{
	// close logfile
	//if (_csvFile.is_open())
	//	_csvFile.close();

	for (std::vector<CBaseChannel*>::iterator it = exportedChannels.begin(); it != exportedChannels.end(); ++it) {

		CBaseChannel* ch = (*it);
		if (ch != NULL) {
			delete ch;
		}
	}

	if (m_csvHeader != nullptr)
	{
		delete m_csvHeader;
		m_csvHeader = NULL;
	}

	int x = 0;
}

csv_row* CcsvDataLogger::splitCsvRow(string* str)
{
	size_t pos = 0;
	int c = 0, s = 0, e = 0;
	csv_row* row = new csv_row;

	while (true)
	{
		pos = str->find('"', pos);
		if (pos == std::string::npos)
			break;

		c++;
		if (c == 2)
		{
			// closing '"' found; so truncate empty substring
			c = 0;
			e = (pos - s);

			string substr = str->substr(s, e);
			row->push_back(substr);
		}
		else
		{
			s = pos + 1;
		}

		pos++;
	}

	return row;
}

void CcsvDataLogger::readCsvHeader()
{
	string* str;
	csv_row* row = nullptr;

	for (int i = 0; i < 11; i++)
	{
		str = readLine();
		if (str != nullptr)
		{
			row = splitCsvRow(str);
			if (row != nullptr)
			{
				for (size_t i = 0; i < row->size(); i++)
				{
					string rowStr = row->at(i);
					if (rowStr.compare("Sample Rate") == 0)
					{
						m_csvHeader->sampleRate = atoi(row->at(i + 1).c_str());
						i++;
					}
					if (rowStr.compare("Duration") == 0)
					{
						m_csvHeader->duration = (float)atof(row->at(i + 1).c_str());
						i++;
					}
				}
			}
		}

		if (row != nullptr) {
			row->clear();
			delete row;
			row = nullptr;
		}

		if (str != nullptr) {
			delete str;
			str = nullptr;
		}
	}

	return;
}

void CcsvDataLogger::readVarHeaders()
{
	string* str = nullptr;
	csv_row* namesRow = nullptr; // channel names row
	csv_row* unitsRow = nullptr;

	int lc = 0;
	int idx = 0;

	while (lc < 2)
	{
		str = readLine();
		if (str != nullptr)
		{
			char c = (char)str->data()[0];
			if (c == 13)
				continue;

			if (lc == 0)
				namesRow = splitCsvRow(str);
			else if (lc == 1)
				unitsRow = splitCsvRow(str);

			delete str;
			str = nullptr;
			lc++;
		}
	}

	if (namesRow != nullptr && unitsRow !=nullptr)
	{
		for (size_t i = 0; i < namesRow->size(); i++)
		{
			_csvVarHeader hdr = { 0 };
			hdr.idx = i;
			hdr.column = i;
			hdr.name = namesRow->at(i);
			hdr.units = unitsRow->at(i);

			_csvVarHeaders.push_back(hdr);

		}

		namesRow->clear();
		delete namesRow;
		namesRow = nullptr;

		unitsRow->clear();
		delete unitsRow;
		unitsRow = nullptr;
	}


}

void CcsvDataLogger::processDataFile()
{

	do
	{
		string* s = readLine();
		if (s != nullptr && s->at(0) !='\r')
		{
			csv_row* row = splitCsvRow(s);

			for (int i = 0; i < (int)_csvVarHeaders.size(); i++)
			{
				string val = row->at(i);
				std::replace(val.begin(), val.end(), ',', '.');
				float v = (float)atof(val.c_str());

				((CChannel<float>*)exportedChannels[i])->AddSample(v);			
			}
		}

	} while (!m_eof);
}

string* CcsvDataLogger::readLine()
{
	char c = 0;
	DWORD ret = 0;
	string* str = nullptr;

	m_filePos = SetFilePointer(this->m_file, 0, NULL, FILE_CURRENT);
	if (m_filePos >= m_fileSize)
	{
		m_eof = true;
		return nullptr;
	}

	str = new string();
	while (ReadFile(this->m_file, &c, 1, &ret, NULL) && c != '\n')
	{
		str->append(1, c);
	}

	return str;
}

int CcsvDataLogger::GetSampleTickRate()
{
	return 0;
}
