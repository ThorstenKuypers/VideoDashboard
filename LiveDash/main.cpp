
#include "Plugin.h"
#include "LiveDashImageSource.h"

// only global variables
static const CTSTR g_PluginName = L"LiveDash";
static const CTSTR g_PluginDesc = L"VideoDashboard live streaming telemetry dashboard plugin";
HINSTANCE pluginInst = NULL;
CLiveDashboard* g_dash = nullptr;

bool LoadPlugin()
{
	// all initial initialization goes here
	try {
		g_dash = new CLiveDashboard();
		if (g_dash == nullptr)
			return false;
	}
	catch (std::exception e)
	{
		// TODO: write exception message to plugin logfile
		return false;
	}

	OBSRegisterImageSourceClass(TEXT("LiveDashSource"), TEXT("Live Dash Source"), (OBSCREATEPROC)CreateLDImage, (OBSCONFIGPROC)NULL);
	return true;
}

void UnloadPlugin()
{
	// all final cleanup goes here
	if (g_dash != nullptr){
		delete g_dash;
		g_dash = nullptr;
	}
}

CTSTR GetPluginName()
{
	return (CTSTR)g_PluginName;
}

CTSTR GetPluginDescription()
{
	return (CTSTR)g_PluginDesc;
}

ImageSource* STDCALL CreateLDImage(XElement* data)
{
	if (g_dash != nullptr) {

		return new CLiveDashImageSource(g_dash);

	}
	return nullptr;
}

void OnStartStream()
{
	g_dash->OnStartStream();
}

void OnStopStream()
{
	g_dash->OnStopStream();
}

void OnStartStreaming()
{
	g_dash->OnStartStreaming();
}

void OnStopStreaming()
{
	g_dash->OnStopStreaming();
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID p)
{
	switch (reason) {

	case DLL_PROCESS_ATTACH:
		pluginInst = hInst;
		break;
	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}

	return true;
}