#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include "common.h"
#include "libDataLogging.h"
#include "BaseDataLogger.h"
#include "ErrEx.h"

using namespace std;

namespace libDataLogging {
	namespace DataLogger {

#define CSV_VARBUF_CNT 11

		typedef struct _csvFileHeader
		{
			int sampleRate;
			float duration;

		}CSV_FILE_HEADER, *PCSV_FILE_HEADER;

		typedef struct _csvVarHeader
		{
			int idx;
			string name;	//char name[32]; // data value name
			string units;	// char unit[16]; // data value unit
			int column;	   // column where data is located in the csv data table

		}CSV_VAR_HEADER, *PCSV_VAR_HEADER;

		typedef vector<string> csv_row;
		typedef vector<csv_row*> csv_line;


		class CcsvDataLogger : libDataLogging::DataLogger::CBaseDataLogger
		{
		public:
			CcsvDataLogger(wchar_t* logfile);
			~CcsvDataLogger();

			int GetSampleTickRate();

		protected:

		private:

			//	wchar_t* m_logFile;
			ifstream _csvFile;

			csv_line* m_lines;
			int m_lineCnt;
			bool m_eof;
			DWORD m_filePos;
			DWORD m_fileSize;

			CSV_FILE_HEADER* m_csvHeader;
			vector<CSV_VAR_HEADER> _csvVarHeaders;

			csv_row* splitCsvRow(string* str);

			void readCsvHeader();
			void readVarHeaders();
			void processDataFile();
			string* readLine();
		};

#pragma endregion

	}
}