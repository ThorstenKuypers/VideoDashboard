
#include "Plugin.h"
#include "LiveDash.h"

// only global variables
static const CTSTR g_PluginName = L"LiveDash";
static const CTSTR g_PluginDesc = L"VideoDashboard live streaming telemetry dashboard plugin";
HINSTANCE pluginInst = NULL;
ILiveLogger* logger = nullptr;
IDashboardLayout* dash = nullptr;

using namespace libDataLogging;
using namespace libDataLogging::LiveDataLogger;
using namespace libLDF;

bool LoadPlugin()
{
	// all initial initialization goes here

	if (!make_instance(IID_LIVELOGGER, (void**)&logger, NULL)) {

		//TODO: add error log message
		return false;
	}
	dash = get_LDF();
	if (dash == nullptr)
		return false;

	//dash->SetDataLogger(logger);

	logger->Connected += MAKE_DELEGATE_FN(LoggerConnect);
	logger->Disconnected += MAKE_DELEGATE_FN(LoggerDisconnect);
	logger->DataUpdate += MAKE_DELEGATE_FN(LoggerUpdate);

	return true;
}

void UnloadPlugin()
{
	// all final cleanup goes here

	if (logger != NULL) {

		release_instance(logger);
		logger = NULL;
	}

	if (dash != NULL) {

		release_LDF(dash);
		dash = NULL;
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