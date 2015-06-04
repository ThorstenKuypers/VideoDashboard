#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBDATALOGGING_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBDATALOGGING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBDATALOGGING_EXPORTS
#define LIBDATALOGGING_API __declspec(dllexport)
#else
#define LIBDATALOGGING_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "Channel.h"
#include "errex.h"

#pragma region // Enums

#pragma endregion

#pragma region // Structures

typedef struct SectorInfo
{
	char sectorNum;
	float sectorStartPct;
}SECTOR_INFO, *PSECTOR_INFO;

typedef struct _SectorTimingInfo
{
	char sectorNum;
	float sectorStartPct;
	float sectorTime;

}SECTOR_TIMING_INFO, *PSECTOR_TIMING_INFO;

typedef struct _LapInfo
{
	int lapNo;	// lap number
	int sampleIdx;		// index in sample array (begin of lap)
	int sampleCnt;		// data samples count
	double SessTimeIdx;	// session time index (begin of lap)
	float lapTime;

	std::vector<SECTOR_TIMING_INFO> splitTimeInfo;
}LAPINFO, *PLAPINFO;

typedef struct _FastestLapInfo {

	// fastest lap in session (from SessionInfoString)
	float fastestLapTimeInSession;
	float fastestLapTimeInStint;
	int fastestLapSession;
	int fastestLapStint;

}FAST_LAP_INFO, *PFAST_LAP_INFO;

#pragma endregion

#pragma region // --- libDataLogging ---

namespace libDataLogging {
	namespace DataLogger {


		using namespace std;

#pragma region // IDataLogging interface
		// This structure defines the interface into the DataLogging Library
		struct IDataLogging
		{
		public:
			virtual int GetSampleCount() = 0;
			virtual int GetSampleTickRate() = 0;

			virtual void GetChannelData(string channelName, int sampleIndex, double* data) = 0;
			virtual void GetChannelData(string channelName, int sampleIndex, float* data) = 0;
			virtual void GetChannelData(string channelName, int sampleIndex, int* data) = 0;
			virtual void GetChannelData(string channelName, int sampleIndex, char* data) = 0;
			virtual void GetChannelData(string channelName, int sampleIndex, bool* data) = 0;

			virtual void GetChannelData(string channelName, int sampleIndex, void* data) = 0;

			virtual int GetSessionLapCount() = 0;
			virtual PLAPINFO GetLapInfo(int lap) = 0;
			virtual PFAST_LAP_INFO GetFastLapInfo() = 0;

			virtual int ShiftLightRPM() = 0;
			virtual int RedlineRPM() = 0;

			virtual std::vector<std::string>& GetExportedChannelNames() = 0;
			virtual std::vector<CBaseChannel*>& GetExportedChannels() = 0;
			virtual void GetSpecialChannelNames(std::vector<std::string>*) = 0;

			virtual int GetChannelDataType(string channelName) = 0;
			virtual CBaseChannel& GetChannel(string channelName) = 0;

		};
#pragma endregion


	} //namespace DataLogger
} // namespace libDataLogging

#pragma endregion


extern "C" LIBDATALOGGING_API struct libDataLogging::DataLogger::IDataLogging* init_instance(wchar_t* logfile);
extern "C" LIBDATALOGGING_API void release_instance(struct libDataLogging::DataLogger::IDataLogging* inst);
