#pragma once

#include "common.h"
#include "libDataLogging.h"
#include "BaseTelemetryFile.h"
#include "ErrEx.h"

#include "irsdk_defines.h"
#include "yaml_parser.h"

using namespace iR_sdk;

namespace libDataLogging {
	namespace DataLogger {


		class CIracingBinaryTelemetryFile : libDataLogging::DataLogger::CBaseTelemetryFile
		{
		public:
			CIracingBinaryTelemetryFile(const std::wstring& logfile);
			virtual ~CIracingBinaryTelemetryFile();

			int GetSampleTickRate();

		protected:

		private:
			irsdk_header m_header;
			std::vector<irsdk_varHeader> m_varHeaders;
			irsdk_diskSubHeader m_subHeader;

			std::string m_sessionInfo;

			void processDataFile();

			void ParseSessionInfo(const std::string& sessionInfoStr);

			void readIBTHeader();
			void readSubHeader();
			void readVarHeaders();
			void readSessInfo();
			void readDataBuf();
			//void extractChannelData(char* buf);

		};

	}
}