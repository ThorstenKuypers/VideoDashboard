#pragma once
#ifndef _BASETELEMETRYFILE_H_
#define _BASETELEMETRYFILE_H_

#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <Windows.h>

#include "liboga.h"

namespace libOGA {
	namespace FileLogs {

		typedef std::vector<BYTE> SampleBufferRow;

#pragma unmanaged

		using namespace std;

		//class CDataLogger;

		class CBaseTelemetryFile :public IDataLogFile
		{
		public:
			CBaseTelemetryFile(const std::wstring& logfile);
			virtual ~CBaseTelemetryFile();

			virtual int GetSampleCount();

			virtual int GetSessionLapCount();
			virtual LAPINFO& GetLapInfo(int lap);
			virtual FASTEST_LAP_INFO& GetFastLapInfo();

			virtual int GetSampleTickRate() = 0;

			virtual int ShiftRpm();
			virtual int RedlineRpm();

			virtual CDataChannel& GetChannel(std::string& name);

			virtual vector<std::string> GetExportedChannels()
			{
				vector<std::string> v;

				for (auto c : channels) {
					v.emplace_back(c.second.get_Name());
				}

				return std::move(v);
			}

			virtual vector<std::string> GetSpecialChannelNames()
			{
				vector<std::string> v;

				for (auto c : mathChannels) {
					v.emplace_back(c.second.get_Name());
				}

				return std::move(v);
			}

		protected:
			virtual void processDataFile() = 0;

			HANDLE m_file;
			int m_lapCnt;		// number of laps in session
			int m_lastLap;		// previous lap
			int m_curLapIdx;	// current active lap

			std::vector<LAPINFO> m_lapInfo;			// lap-info array
			FASTEST_LAP_INFO m_fastLapInfo;	// fastest lap info

			// NEW implementation
			std::vector<LAPINFO> _lapsInfo;
			std::tuple<int, float> _fastestLap;

			int m_sampleCnt;			// number of data samples per channel

			//// array of actual channels
			ChannelsMap channels;

			// mapping of special channel names to internal channel names
			std::map<std::string, std::string>  mathChannelsMap;

			// special channels not directly exported by the logfile (but calculated from data)
			// => math channels
			ChannelsMap mathChannels;

			// information about sectors (from sessionInfoString)
			std::map<int, float> sectors;

			void GetFastestLapInSession();
			void GetFastestLapInStint();

			CAR_DATA_INFO m_carDataInfo;

			// THE sample buffer; contains RAW sample data as read from telemetry file;
			// the data is the mapped to according channels on request
			//std::shared_ptr<BYTE> sampleBuffer;
			std::vector<SampleBufferRow> sampleBuffer;

			// sample buffer of math channels
			std::vector<std::vector<SampleValue>> mathChannelBuf;

		private:
			void CloseFile();

		}; // class CBaseTelemetryFile

	}
}

#endif // _BASETELEMETRYFILE_H_