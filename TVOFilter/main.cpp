// file: main.cpp - defines the entry point(s) for the filter DLL 
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

#include "TVOfilter.h"
#include <dllsetup.h>

// Setup information

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

const AMOVIESETUP_FILTER sudTVOFilter =
{
    &CLSID_TVOFilter,         // Filter CLSID
    (WCHAR*)FILTER_NAME,       // String name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    sudpPins                // Pin information
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    { (WCHAR*) FILTER_NAME
    , &CLSID_TVOFilter
	, TVOFilter::CTVOFilter::CreateInstance
    , NULL
    , &sudTVOFilter 
	}
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterServer
//
// Handles sample registry and unregistry
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer


//
// DllEntryPoint
//
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
extern "C" BOOL WINAPI _DllMainCRTStartup(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY FilterDllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
_DllMainCRTStartup((HINSTANCE)hModule, dwReason, lpReserved);
return DllEntryPoint((HINSTANCE)hModule, dwReason, lpReserved);
}

// DLL setup
HRESULT DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
	if (bInstall)
		return AMovieDllRegisterServer2(TRUE);
	else
		return AMovieDllRegisterServer2(FALSE);

	return -1;
}
