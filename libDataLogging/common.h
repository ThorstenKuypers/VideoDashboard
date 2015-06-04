#pragma once

#include <windows.h>
#include <typeinfo.h>
#include <cassert>
#include <tuple>

#define __MODULE__ "DataLogger.dll"


namespace libDataLogging
{
	namespace DataLogger
	{
#pragma unmanaged
		
enum DataLoggerType {
	Ibt =1,
	Csv,
	//Mtc
};

enum _sessionType {
	sessionType_practice =1,
	sessionType_qualy,
	sessionType_race,
	sessionType_misc
};

typedef struct _CarResultsInfo 
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
typedef struct _DriverInfo
{
	char* driverName;
	int memberID;

}DRIVER_INFO, *PDRIVER_INFO;

///////////////////////////////////////////////////////////////////////////////
// _carDataInfo structure - holds car specific values ( e.g. shiftLight RPM)
///////////////////////////////////////////////////////////////////////////////
typedef struct _carDataInfo
{
	int shiftLightRpm;
	int redlineRpm;

}CAR_DATA_INFO, *PCAR_DATA_INFO;


///////////////////////////////////////////////////////////////////////////////
// _SessionInfo structure
///////////////////////////////////////////////////////////////////////////////
typedef struct _SessionInfo
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
typedef struct _WeekendInfo
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
typedef struct _SessionInfoData
{
	PWEEKEND_INFO wei_data;
	PSESSION_INFO sessi_data;
}SESSION_INFO_DATA, *PSESSION_INFO_DATA;

}
}