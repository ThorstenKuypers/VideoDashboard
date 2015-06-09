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

		class CDataLogger;

		class CBaseDataLogger
		{
			friend class CDataLogger;

		public:
			CBaseDataLogger(wchar_t* logfile);
			CBaseDataLogger();
			virtual ~CBaseDataLogger();

			int GetSampleCount();

			int GetSessionLapCount();
			PLAPINFO GetLapInfo(int lap);
			PFASTEST_LAP_INFO GetFastLapInfo();

			virtual int GetSampleTickRate() = 0;

			int ShiftRpm();
			int RedlineRpm();

		protected:
			virtual void processDataFile() = 0;

			HANDLE m_file;
			int m_lapCnt;		// number of laps in session
			int m_lastLap;		// previous lap
			int m_curLapIdx;	// current active lap

			PLAPINFO* m_lapInfo;			// lap-info array
			PFASTEST_LAP_INFO m_fastLapInfo;	// fastest lap info

			// NEW implementation
			std::vector<LAPINFO> _lapsInfo;
			std::tuple<int, float> _fastestLap;

			int m_sampleCnt;			// number of data samples per channel

			// array of all channel names in currently open logfile
			std::vector<string> exportedChannelNames;

			// array of actual channels
			std::vector<CBaseChannel*> exportedChannels;

			// mapping of channel name to channel index (in exported channels vector)
			std::map<string, int> channelsMap;

			// mapping of special channel names to internal channel names
			std::map<std::string, std::string>  specialChannelsMap;

			// special channels not directly exported by the logfile (but calculated from data)
			// => math channels
			std::vector<CBaseChannel*> specialChannels;

			// information about sectors (from sessionInfoString)
			std::vector<SECTOR_INFO> sectors;

			void GetFastestLapInSession();
			void GetFastestLapInStint();

			CAR_DATA_INFO m_carDataInfo;

		private:
			void CloseFile();

		}; // class CBaseDataLogger

	}
}