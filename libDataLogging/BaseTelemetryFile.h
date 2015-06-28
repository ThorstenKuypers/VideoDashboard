#pragma once

//#include <vector>

#include "common.h"
#include "libDataLogging.h"
#include "Channel.h"

//*** Special channel names; ***//
#define CH_SP_SPEED_KPH			"Speed_Kph"
#define CH_SP_SPEED_MPH			"Speed_Mph"
#define CH_SP_LATG				"LatG"
#define CH_SP_LONGG				"LongG"

#define CH_SP_LAP_TIME		"laptime"	// current elapsed lap time
#define CH_SP_LAP			"lap"		// alias for current lap number
#define CH_SP_BEST_LAP_TIME	"best_laptime" // best lap time in data session
#define CH_SP_BEST_LAP			"best_lap"	// best lap in session
#define CH_SP_CURRENT_SECTOR "sector_num" // current sector number
#define CH_SP_SECTOR_TIME	"sector_time" // absolute time in sector

// NOT IMPLEMENTED YET
#define CH_SP_SECTOR_SPLIT	"sector_split" // sector split time (diff between previous and next sector)

//...

/////////////////////////////////////////////////////////////////////


namespace libDataLogging {
	namespace DataLogger {

#pragma unmanaged

		using namespace std;

		//class CDataLogger;

		class CBaseTelemetryFile : public CBaseChannel
		{
			//friend class CDataLogger;

		public:
			CBaseTelemetryFile(const std::wstring& logfile);
			virtual ~CBaseTelemetryFile();

			int GetSampleCount();

			int GetSessionLapCount();
			LAPINFO& GetLapInfo(int lap);
			FASTEST_LAP_INFO& GetFastLapInfo();

			virtual int GetSampleTickRate() = 0;

			int ShiftRpm();
			int RedlineRpm();

			virtual bool GetSample(DataSample&);
			virtual SampleValue GetSampleData(DataSample& s, CDataChannel& c);

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

			//// array of all channel names in currently open logfile
			//std::vector<string> exportedChannelNames;

			//std::vector<CBaseChannel&> exportedChannels;

			//// mapping of channel name to channel index (in exported channels vector)
			//std::map<string, int> channelsMap;

			//// array of actual channels
			ChannelsMap channels;

			// special buffer containing all calculated "math" values aaranged similar to the original sample buffer
			/*std::vector*/

			// mapping of special channel names to internal channel names
			std::map<std::string, std::string>  mathChannelsMap;

			// special channels not directly exported by the logfile (but calculated from data)
			// => math channels
			std::vector<CDataChannel> mathChannels;

			// information about sectors (from sessionInfoString)
			std::vector<SECTOR_INFO> sectors;

			void GetFastestLapInSession();
			void GetFastestLapInStint();

			CAR_DATA_INFO m_carDataInfo;

			// THE sample buffer; contains RAW sample data as read from telemetry file;
			// the data is the mapped to according channels on request
			std::shared_ptr<BYTE> sampleBuffer;

		private:
			void CloseFile();

		}; // class CBaseTelemetryFile

	}
}