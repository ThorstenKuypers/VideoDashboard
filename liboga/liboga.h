#pragma once
#ifndef _LIBOGA_H_
#define _LIBOGA_H_
// OpenGameAPI telemetry data logger implementation
//
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
#ifdef LIBOGA_EXPORTS
#define LIBOGA_API __declspec(dllexport)
#else
#define LIBOGA_API __declspec(dllimport)
#endif

// Windows API includes
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "irsdk_defines.h"
#include "exception.h"

#define __MODULE__ "DataLogger.dll"


// main library namespace 
namespace libOGA {

	using namespace iR_sdk;

	//typedef std::shared_ptr<BYTE> DataSample;
	typedef std::vector<BYTE> DataSample;

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

#pragma region // Structures

	enum sessionType {
		sessionType_practice = 1,
		sessionType_qualy,
		sessionType_race,
		sessionType_misc
	};

	typedef struct CarResultsInfo
	{
		int Position;
		int CarIdx;
		int Lap;
		float Time;
		int FastestLap;
		float FastestTime;
		float LastTime;
		int LapsLed;
		int LapsComplete;
		float LapsDriven;
		int Incidents;
		int ReasonOutId;
		char* ReasonOutStr;

		// driver of car info
		char* driverName;
		int driverID;
		int carNumber;
		char* carClassShortName;
		int carClassRelSpeed;

	}CAR_RESULTS_INFO, *PCAR_RESULTS_INFO;
	///////////////////////////////////////////////////////////////////////////////
	// _DriverInfo structure
	///////////////////////////////////////////////////////////////////////////////
	typedef struct DriverInfo
	{
		char* driverName;
		int memberID;

	}DRIVER_INFO, *PDRIVER_INFO;

	///////////////////////////////////////////////////////////////////////////////
	// _carDataInfo structure - holds car specific values ( e.g. shiftLight RPM)
	///////////////////////////////////////////////////////////////////////////////
	typedef struct carDataInfo
	{
		int shiftLightRpm;
		int redlineRpm;

	}CAR_DATA_INFO, *PCAR_DATA_INFO;


	///////////////////////////////////////////////////////////////////////////////
	// _SessionInfo structure
	///////////////////////////////////////////////////////////////////////////////
	typedef struct SessionInfo
	{
		int SessionLaps;
		float SessionTime;
		int SessionNumLapsToAvg;
		int SessionType;

		CAR_RESULTS_INFO* cars; // cars array
		int carsInSession;	// number of cars in session

	}SESSION_INFO, *PSESSION_INFO;

	///////////////////////////////////////////////////////////////////////////////
	// _WeekendInfo structure
	///////////////////////////////////////////////////////////////////////////////
	typedef struct WeekendInfo
	{
		const char* TrackDisplayName;
		float TrackLength;
		float TrackSurfaceTemp;
		float TrackAirTemp;
		float TrackAirPressure;
		float TrackWindVel;
		float TrackWindDir;
		int SessionID;

	}WEEKEND_INFO, *PWEEKEND_INFO;


	///////////////////////////////////////////////////////////////////////////////
	// _DriverInfo structure
	///////////////////////////////////////////////////////////////////////////////
	typedef struct SessionInfoData
	{
		PWEEKEND_INFO wei_data;
		PSESSION_INFO sessi_data;
	}SESSION_INFO_DATA, *PSESSION_INFO_DATA;


	typedef struct SectorInfo
	{
		char sectorNum;
		float sectorStartPct;
	}SECTOR_INFO, *PSECTOR_INFO;

	typedef struct SectorTimingInfo
	{
		char sectorNum;
		float sectorStartPct;
		float sectorTime;

	}SECTOR_TIMING_INFO, *PSECTOR_TIMING_INFO;

	typedef struct LapInfo
	{
		int lapNo;	// lap number
		int sampleIdx;		// index in sample array (begin of lap)
		int sampleCnt;		// data samples count
		double SessTimeIdx;	// session time index (begin of lap)
		double lapTime;

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

	struct IGenericLogger;

	// common telemetry data value class
	class SampleValue
	{
	private:
		int _iVal;
		float _fVal;
		double _dVal;
		char _cVal;
		bool _bVal;
		unsigned long _uVal; // bitmask

		irsdk_VarType _type;

	public:
		SampleValue() :
			_iVal(0),
			_fVal(0),
			_dVal(0),
			_cVal(0),
			_bVal(0),
			_uVal(0),
			_type(irsdk_bool)
		{}

		//SampleValue& SampleValue::operator=(SampleValue&) = delete;
		//SampleValue& SampleValue::operator=(SampleValue&& o)
		//{
		//	_iVal = o._iVal;
		//	o._iVal = 0;

		//	_fVal = o._fVal;
		//	o._fVal = 0;

		//	_dVal = o._dVal;
		//	o._dVal = 0;

		//	_cVal = o._cVal;
		//	o._cVal = 0;

		//	_bVal = o._bVal;
		//	o._bVal = false;

		//	_uVal = o._uVal;
		//	o._uVal = 0;

		//	return *this;
		//}

		SampleValue(int val) { _iVal = val; _type = irsdk_int; }
		SampleValue(float val) { _fVal = val; _type = irsdk_float; }
		SampleValue(double val) { _dVal = val; _type = irsdk_double; }
		SampleValue(char val) { _cVal = val; _type = irsdk_char; }
		SampleValue(bool val) { _bVal = val; _type = irsdk_bool; }
		SampleValue(unsigned long val) { _uVal = val; _type = irsdk_bitField; }

		irsdk_VarType type() { return _type; }

		template<typename T>
		T get_value() {}

		template<>
		int get_value<int>() { return _iVal; }

		template<>
		float get_value<float>() { return _fVal; }

		template<>
		double get_value<double>() { return _dVal; }

		template<>
		char get_value<char>() { return _cVal; }

		template<>
		bool get_value<bool>() { return _bVal; }

		template<>
		unsigned long get_value<unsigned long>() { return _uVal; }


	};

	enum ChannelType
	{
		ChannelType_Unknown = -1,
		ChannelType_File = 1,
		ChannelType_Math = 2
	};


	// This class defines the data channel
	class CDataChannel
	{
	public:
		CDataChannel();
		CDataChannel(std::string name,
			std::string unit,
			std::string desc,
			int key,
			int type,
			int offset,
			ChannelType ct);
		~CDataChannel();

		CDataChannel(CDataChannel&) {}
		CDataChannel(CDataChannel&& t);

		CDataChannel& operator=(CDataChannel& t) {}
		CDataChannel& operator=(CDataChannel&& t);

		const char* get_Name();
		const char* get_Unit();
		const char* get_Description();
		int get_Type();
		int get_Key();
		int get_Offset();

		ChannelType get_ChannelType();

		static LIBOGA_API SampleValue GetSampleData(DataSample& s, CDataChannel& c);
		static LIBOGA_API SampleValue GetSampleData(IGenericLogger& logger, DataSample& s, CDataChannel& c);

	private:

		std::string _name; // channel name
		std::string _desc; // channel description
		std::string _unit; // channel units
		int _type; // channels data type
		int _key; // channels index in channel map
		ChannelType _channelType; // type of channel (default data channel or math channel)
		int _offset; // offset of data inside the sample buffer
	};
	typedef std::map<std::string, CDataChannel> ChannelsMap; // mapping of channel descriptor to data buffer index; the value is found at the int index in the data buffer

	// abstract channel interface
	class CBaseChannel
	{
	public:
		virtual bool GetSample(DataSample&) = 0;
		virtual bool GetSample(DataSample&, size_t) = 0;
	};

	struct IGenericLogger {

		virtual int GetSampleTickRate() = 0;
		virtual int ShiftRpm() = 0;
		virtual int RedlineRpm() = 0;
		virtual bool GetSample(DataSample&, size_t idx) = 0;
		virtual bool GetSample(DataSample& s) = 0;
		virtual CDataChannel& GetChannel(std::string& name) = 0;
	};
	// namespace for file logs
	namespace FileLogs {

		// forward declare this class(es) because the compiler doen't know about content of FileLogs namespace since
		// neither IracingBinaryTelemetryFile.h nor BaseTelemetryFile.h are included in this header.
		// forward declare is enough because __get_inst returns only a pointer to CIracingBinaryTelemetryFile and
		// does not use a full object!

		class CIracingBinaryTelemetryFile;

		struct IDataLogFile : IGenericLogger
		{
			virtual int GetSampleCount() = 0;
			virtual int GetSessionLapCount() = 0;
			virtual LAPINFO& GetLapInfo(int lap) = 0;
			virtual FASTEST_LAP_INFO& GetFastLapInfo() = 0;
			//virtual int GetSampleTickRate() = 0;
			//virtual int ShiftRpm() = 0;
			//virtual int RedlineRpm() = 0;
			virtual std::vector<std::string> GetSpecialChannelNames() = 0;
			virtual std::vector<std::string> GetExportedChannels() = 0;

		};
	} // FileLog

	namespace LiveLogs {

		// This interfaces provides callback functionallity for clients to be notified when certain events
		// in the live logger occur
		struct LIBOGA_API ILiveLoggingCallback
		{
		public:
			virtual void SimConnected() = 0; // called when connection between sim interface and logger is established
			virtual void SimDisconnected() = 0; // called when the above mentioned connection is teared down
			virtual void SimDataUpdate(DataSample& sample) = 0; // called for each data update received from the sim
		};

		// forward declarations for live logging namespace
		class CLiveDataLogger;

		struct ILiveDataLogger : IGenericLogger
		{
			virtual void Start() = 0;
			virtual void Stop() = 0;

		};
	}

	// 
	class LIBOGA_API LoggerInstanceFactory
	{
	public:
		static libOGA::FileLogs::IDataLogFile* GetFileLogInstance(const wchar_t* fileName);
		static void ReleaseFileLogInstance(libOGA::FileLogs::IDataLogFile* inst);

		static libOGA::LiveLogs::ILiveDataLogger* GetLiveLoggerInstance(libOGA::LiveLogs::ILiveLoggingCallback&);
		static void ReleaseLiveLoggerInstance(libOGA::LiveLogs::ILiveDataLogger* inst);
	};

} // liboga

//// TESTING interface
//
//LIBOGA_API libOGA::FileLogs::IDataLogFile* __get_inst(std::wstring file);
//
//LIBOGA_API libOGA::LiveLogs::ILiveDataLogger* __get_live(libOGA::LiveLogs::ILiveLoggingCallback&);
//
//LIBOGA_API void __release_inst(void* inst);

#endif // _LIBOGA_H_