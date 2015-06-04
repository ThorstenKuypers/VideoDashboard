#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVOFilter -- DirectShow Transform Filter implementation
//
//	Copyright 2011-2012 Thorsten Kuypers
//	All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

/* TVOFilter exports/(imports) */

#ifdef FILTERDLL_LIB
#define FILTERDLL_EXPORT __declspec(dllexport)
#else 
#define FILTERDLL_EXPORT __declspec(dllimport)
#endif

// filter configuration structure
struct _filterConfig {

	char* appPath;
	char* dataLogFile;
	int vidStartFrame;
	int vidEndFrame;
	float frameRate;

	char* dashLayoutFile;

	char* baseDashFile;
	int dashOverlay;

	int overlaySize;
	int overlayMargin;
	int overlayTransp;

};
typedef struct _filterConfig FILTER_CONFIG, *PFILTER_CONFIG;


int FILTERDLL_EXPORT filter_setConfig(void*, PFILTER_CONFIG);
