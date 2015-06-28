#pragma once
#ifndef _IBT_FILE_H_
#define _IBT_FILE_H_

#include "BaseTelemetryFile.h"


namespace libOGA {
	namespace FileLogs {

		class CIracingBinaryTelemetryFile : public CBaseTelemetryFile, public CBaseChannel
		{
		public:
			CIracingBinaryTelemetryFile(const std::wstring& logfile);
			virtual ~CIracingBinaryTelemetryFile();

			virtual int GetSampleTickRate();

			virtual bool GetSample(DataSample&, size_t idx) override;
			virtual bool GetSample(DataSample&) override;

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

		};

	}
}

#endif // _IBT_FILE_H_