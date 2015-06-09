#pragma once
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

#include "common.h"
#include "ErrEx.h"
#include "libDataLogging.h"
#include "BaseDataLogger.h"
#include "ibtDataLogger.h"
#include "csvDataLogger.h"

#include <vector>
#include <string>


#pragma unmanaged

using namespace std;
using namespace iR_sdk;

namespace libDataLogging {
	namespace DataLogger {

		typedef std::map<std::string, int> channel_map;
		typedef std::map<std::string, std::string> specials_map;

#pragma region "CDataLogger - new implementation"
	
		// * CDataLogger - implementation of general DataLogger interface. Handles differentiation between the different
		// *			   file formats and instanciates the correct DataLogger object.
		class CDataLogger : public IDataLogger
		{

			public:	
				CDataLogger(wchar_t* logfile);
				~CDataLogger();

				int GetSampleCount();
				int GetSampleTickRate();

				int GetSessionLapCount();
				PLAPINFO GetLapInfo(int lap);
				PFASTEST_LAP_INFO GetFastLapInfo();

				virtual void GetChannelData(string channelName, int sampleIndex, double* val);
				virtual void GetChannelData(string channelName, int sampleIndex, int* val);
				virtual void GetChannelData(string channelName, int sampleIndex, float* val);
				virtual void GetChannelData(string channelName, int sampleIndex, bool* val);
				virtual void GetChannelData(string channelName, int sampleIndex, char* val);

				virtual void GetChannelData(string channelName, int sampleIndex, void* data);

				virtual std::vector<std::string>& GetExportedChannelNames();
				virtual std::vector<CBaseChannel*>& GetExportedChannels();
				virtual void GetSpecialChannelNames(std::vector<std::string>*);

				virtual int GetChannelDataType(string channelName);
				virtual CBaseChannel& GetChannel(string channelName);

				virtual int RedlineRPM();
				virtual int ShiftLightRPM();

			private:
				CBaseDataLogger* m_dataLogger;

				int m_lapCnt;		// number of laps in session
				int m_lastLap;		// previous lap
				int m_curLapIdx;	// current active lap

				PLAPINFO* m_lapInfo;			// lap-info array
				PFASTEST_LAP_INFO m_fastLapInfo;	// fastest lap info
				
				int m_LoggerType;

		};
#pragma endregion

	} //namespace DataLogger
} // namespace libDataLogging
