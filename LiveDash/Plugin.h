#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "OBSApi.h"
//#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "libLDF.h"
#include "libDataLogging.h"
#include "LiveDataLogger.h"

#include "config.h"

extern "C" __declspec(dllexport) bool LoadPlugin();
extern "C" __declspec(dllexport) void UnloadPlugin();
extern "C" __declspec(dllexport) CTSTR GetPluginName();
extern "C" __declspec(dllexport) CTSTR GetPluginDescription();

extern "C" __declspec(dllexport) void ConfigPlugin(HWND hwnd);
//extern "C" __declspec(dllexport) void OnStartStream();
//extern "C" __declspec(dllexport) void OnStopStream();
//extern "C" __declspec(dllexport) void OnStartStreaming();
//extern "C" __declspec(dllexport) void OnStopStreaming();

extern HINSTANCE pluginInst;

#endif