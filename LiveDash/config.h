#pragma once

#include <string>

#include "LiveDash.h"
#include "resource.h"

#ifdef _DEBUG
#define DASHBOARD_PATH L"D:\\Development\\Projects\\VideoDashboard\\bin\\Debug\\x86\\dashboards"
#endif


struct _config
{
	std::wstring dashboardPath;
	std::wstring selectedDashboard;
	int posX;
	int posY;
	float scale;
	int opacity;

};
typedef struct _config LIVEDASH_CONFIG;
