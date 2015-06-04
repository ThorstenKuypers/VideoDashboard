#pragma once

// Telemetry data table structure
typedef struct _DataTableEntry {

	double time;
	float corrSpeed;
	int gear;
	int engineRpm;
	float throttlePos;
	float brakePos;
	float steerAngle;
	int LapNo;
	float groundSpeed;
	float latG;
	float longG;

	struct _videoSync {	// video frame to telemetry data syncronization
		LONGLONG videoFrameNo;
	}videoSync;
	
}TELEMETRY_TABLE_ENTRY, *PTELEMETRY_TABLE_ENTRY;

// session info data struct
typedef struct _SessionInfoData {

	int dummy;

}SESSION_INFO_DATA, *PSESSION_INFO_DATA;


typedef struct _TelemetryDataTable {

	TELEMETRY_TABLE_ENTRY* pTableEntries;	// array of numEntries TELEMETRY_TABLE_ENTRY
	int numEntries;

	SESSION_INFO_DATA* pSessionInfo;

	float dataSyncStart;	// start time value for video-data sync.
	float dataSyncEnd;		// end time value for video-data sync.

}TELEMETRY_DATA_TABLE, *PTELEMETRY_DATA_TABLE;

