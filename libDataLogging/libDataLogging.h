#ifndef _LIB_DATALOGGING_H_
#define _LIB_DATALOGGING_H_

// This file is part of VideoDashboard project
// (C) 2014-2015 Thorsten Kuypers
// All Rights reseverd
//
// You may redistribute this file as part of the OpenGameAPI DataLogger package
// only if the content of this file remains unchanged!


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

#include "irsdk_defines.h"
#include "errex.h"
#include "Channel.h"

typedef GUID LIBDATALOGGING_ID;
typedef std::vector<BYTE> DataSample;

namespace libDataLogging {

	// {C8B8B0AE-A75A-4E54-9436-30966B2C9355}
	static const GUID IID_DATALOGGER =
	{ 0xc8b8b0ae, 0xa75a, 0x4e54, { 0x94, 0x36, 0x30, 0x96, 0x6b, 0x2c, 0x93, 0x55 } };

	// {0C503D03-D60A-43AE-87B4-85046E3DCE49}
	static const GUID IID_LIVELOGGER =
	{ 0xc503d03, 0xd60a, 0x43ae, { 0x87, 0xb4, 0x85, 0x4, 0x6e, 0x3d, 0xce, 0x49 } };

	// alias for original irsdk varBuf type
	typedef iR_sdk::irsdk_VarType ChannelType;

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

	}FASTEST_LAP_INFO, *PFASTEST_LAP_INFO;

#pragma endregion


	namespace DataLogger {


#pragma region // IDataLogging interface
		// This defines the interface DataLogger interface
		struct IDataLogger
		{
		public:
			virtual int GetSampleCount() = 0;
			virtual int GetSampleTickRate() = 0;

			virtual void GetChannelData(std::string channelName, int sampleIndex, double* data) = 0;
			virtual void GetChannelData(std::string channelName, int sampleIndex, float* data) = 0;
			virtual void GetChannelData(std::string channelName, int sampleIndex, int* data) = 0;
			virtual void GetChannelData(std::string channelName, int sampleIndex, char* data) = 0;
			virtual void GetChannelData(std::string channelName, int sampleIndex, bool* data) = 0;

			virtual void GetChannelData(std::string channelName, int sampleIndex, void* data) = 0;

			virtual int GetSessionLapCount() = 0;
			virtual PLAPINFO GetLapInfo(int lap) = 0;
			virtual PFASTEST_LAP_INFO GetFastLapInfo() = 0;

			virtual int ShiftLightRPM() = 0;
			virtual int RedlineRPM() = 0;

			virtual std::vector<std::string>& GetExportedChannelNames() = 0;
			virtual std::vector<CBaseChannel*>& GetExportedChannels() = 0;
			virtual void GetSpecialChannelNames(std::vector<std::string>*) = 0;

			virtual int GetChannelDataType(std::string channelName) = 0;
			virtual CBaseChannel& GetChannel(std::string channelName) = 0;

		};
#pragma endregion


	} //namespace DataLogger
} // namespace libDataLogging


// Class factory function exports
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -- make_instance
// creates an object instance
// iid: object identifier
// inst: the returned object instance if successfull
// param: additional parameter (see documentation for details)
// 
// returns: true on success; false otherwise
extern "C" LIBDATALOGGING_API bool make_instance(LIBDATALOGGING_ID iid, void** inst, void* param);

// -- release_instance
// releases an instance previously aqcuired by make_instance
// inst: object instance
//
// returns: nothing
extern "C" LIBDATALOGGING_API void release_instance(void* inst);

#endif // _LIB_DATALOGGING_H_