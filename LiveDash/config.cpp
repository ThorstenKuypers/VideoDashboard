
#include <windowsx.h>
#include <string>
#include "Plugin.h"
#include "config.h"

using namespace std;
using namespace libLDF;

void ShowDashboardOverlay(HWND hwnd, LIVEDASH_CONFIG* cfg, const std::wstring file)
{
	std::string fn(file.begin(), file.end());
	if (cfg != nullptr) {

		cfg->selectedDashboard = file;
		if (cfg->logger != nullptr && cfg->dash != nullptr) {

			cfg->dash->SetActiveDashboard(fn);
			cfg->dash->Parse();

			Bitmap* bmp = (Bitmap*)cfg->dash->RenderDashboard(*cfg->logger, 0, true);
			HWND pv = GetDlgItem(hwnd, IDC_PREVIEW);
			HDC hdc = GetWindowDC(pv);

			Graphics* g = Graphics::FromHDC(hdc);

			DWORD bg = GetSysColor(COLOR_WINDOW);
			g->Clear(Gdiplus::Color{ bg });

			RECT rc = { 0 };
			GetClientRect(pv, &rc);
			Vect2 frameSize = API->GetBaseSize();
			int w = bmp->GetWidth(), h = bmp->GetHeight();
			float a = (float)0;

			a = (float)((float)rc.right / w);
			w = (int)((float)a*(float)bmp->GetWidth());
			h = (int)((float)a*(float)bmp->GetHeight());

			g->DrawImage(bmp, 0, 0, w, h);
			ReleaseDC(pv, hdc);

			if (bmp != nullptr) {
				delete bmp;
				bmp = nullptr;
			}
			delete g;
		}
	}
}
 

INT_PTR CALLBACK ConfigDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const size_t sbufLen = 128;
	wchar_t sbuf[sbufLen]{0}; // general string buffer
	DWORD sliderVal = 0;

	HWND sliderPosX = GetDlgItem(hwnd, IDC_SLIDER_POSX);
	HWND sliderPosY = GetDlgItem(hwnd, IDC_SLIDER_POSY);
	HWND sliderScale = GetDlgItem(hwnd, IDC_SLIDER_SCALE);
	HWND sliderOpacity = GetDlgItem(hwnd, IDC_SLIDER_OPACITY);

	LIVEDASH_CONFIG* cfg = (LIVEDASH_CONFIG*)GetWindowLongPtr(hwnd, DWLP_USER);

	switch (msg)
	{
	case WM_INITDIALOG:
	{
		cfg = (LIVEDASH_CONFIG*)lParam; // GetWindowLongPtr(hwnd, DWL_MSGRESULT);
		if (cfg != NULL) {
			SetWindowLongPtr(hwnd, DWLP_USER, (LONG)cfg);
		}
		// initialize dashboard list
		HWND hwndDashList = NULL;
		hwndDashList = GetDlgItem(hwnd, IDC_OVERLAY_LIST);
		wstring file;
		wstring path = DASHBOARD_PATH;
		HANDLE hfind = NULL;
		OSFindData fd = { 0 };

		path.append(L"\\*.layout");

		hfind = OSFindFirstFile((CTSTR)path.c_str(), fd);
		if (hfind != INVALID_HANDLE_VALUE && GetLastError() != ERROR_FILE_NOT_FOUND) {

			file = fd.fileName;
			size_t pos = file.rfind(L".");
			file.erase(pos, wstring::npos);

			ComboBox_AddString(hwndDashList, file.c_str());

			while (OSFindNextFile(hfind, fd)) {

				file = fd.fileName;
				size_t pos = file.rfind(L".");
				file.erase(pos, wstring::npos);
				ComboBox_AddString(hwndDashList, file.c_str());
			}

			OSFindClose(hfind);
		}

		// initialize slider controls
		DWORD range = 0;
		range = MAKELPARAM(0, 200);
		SendMessage(sliderPosX, TBM_SETRANGE, TRUE, range);
		SendMessage(sliderPosY, TBM_SETRANGE, TRUE, range);
		SendMessage(sliderPosX, TBM_SETPOS, TRUE, 0);
		SendMessage(sliderPosY, TBM_SETPOS, TRUE, 0);

		swprintf_s(sbuf, sbufLen, L"%d", 0);
		Static_SetText(GetDlgItem(hwnd, IDC_POSX_VAL), sbuf);
		Static_SetText(GetDlgItem(hwnd, IDC_POSY_VAL), sbuf);

		range = MAKELPARAM(0, 100);
		SendMessage(sliderOpacity, TBM_SETRANGE, TRUE, range);
		SendMessage(sliderOpacity, TBM_SETPOS, TRUE, 100);

		swprintf_s(sbuf, sbufLen, L"%d %%", 100);
		Static_SetText(GetDlgItem(hwnd, IDC_OPACITY_VAL), sbuf);

		range = MAKELPARAM(50, 150);
		SendMessage(sliderScale, TBM_SETRANGE, TRUE, range);
		SendMessage(sliderScale, TBM_SETPOS, TRUE, 100);

		swprintf_s(sbuf, sbufLen, L"%d %%", 100);
		Static_SetText(GetDlgItem(hwnd, IDC_SCALE_VAL), sbuf);

		ComboBox_SetCurSel(hwndDashList, 0);
		//ComboBox_GetLBText(hwndDashList, 0, sbuf);

		//file = DASHBOARD_PATH;
		//file.append(L"\\");
		//file.append(sbuf);
		//file.append(L".layout");

		//ShowDashboardOverlay(hwnd, cfg, file);
		return TRUE;
	}

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			cfg->opacity = (int)SendMessage(GetDlgItem(hwnd, IDC_SLIDER_OPACITY), TBM_GETPOS, 0, 0);

			EndDialog(hwnd, TRUE);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;

		case IDC_BTN_RESET_ALL:
		{
			DWORD range = 0;
			range = MAKELPARAM(0, 200);
			SendMessage(sliderPosX, TBM_SETRANGE, TRUE, range);
			SendMessage(sliderPosY, TBM_SETRANGE, TRUE, range);
			SendMessage(sliderPosX, TBM_SETPOS, TRUE, 0);
			SendMessage(sliderPosY, TBM_SETPOS, TRUE, 0);

			swprintf_s(sbuf, sbufLen, L"%d", 0);
			Static_SetText(GetDlgItem(hwnd, IDC_POSX_VAL), sbuf);
			Static_SetText(GetDlgItem(hwnd, IDC_POSY_VAL), sbuf);

			range = MAKELPARAM(0, 100);
			SendMessage(sliderOpacity, TBM_SETRANGE, TRUE, range);
			SendMessage(sliderOpacity, TBM_SETPOS, TRUE, 100);

			swprintf_s(sbuf, sbufLen, L"%d %%", 100);
			Static_SetText(GetDlgItem(hwnd, IDC_OPACITY_VAL), sbuf);

			range = MAKELPARAM(50, 150);
			SendMessage(sliderScale, TBM_SETRANGE, TRUE, range);
			SendMessage(sliderScale, TBM_SETPOS, TRUE, 100);

			swprintf_s(sbuf, sbufLen, L"%d %%", 100);
			Static_SetText(GetDlgItem(hwnd, IDC_SCALE_VAL), sbuf);

			return TRUE;
		}
		case IDC_OVERLAY_LIST:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {

				int idx = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				if (idx > -1)
					ComboBox_GetLBText((HWND)lParam, idx, sbuf);
				else
					return FALSE;

				std::wstring file{ DASHBOARD_PATH };
				file.append(L"\\");
				file.append(sbuf);
				file.append(L".layout");

				cfg->dashboardPath = DASHBOARD_PATH;
				cfg->selectedDashboard = file;

				ShowDashboardOverlay(hwnd, cfg, file);
			}
			return TRUE;
		}

		}
		return FALSE;

	case WM_DESTROY:
		return TRUE;

	case WM_NOTIFY:
		break;

	case WM_HSCROLL:

		switch (LOWORD(wParam))
		{

		case TB_THUMBTRACK:
			if ((HWND)lParam == GetDlgItem(hwnd, IDC_SLIDER_POSX)) {

				sliderVal = HIWORD(wParam);

				swprintf_s(sbuf, sbufLen, L"%d", sliderVal);
				Static_SetText(GetDlgItem(hwnd, IDC_POSX_VAL), sbuf);
			}

			if ((HWND)lParam == GetDlgItem(hwnd, IDC_SLIDER_POSY)) {

				sliderVal = HIWORD(wParam);

				swprintf_s(sbuf, sbufLen, L"%d", sliderVal);
				Static_SetText(GetDlgItem(hwnd, IDC_POSY_VAL), sbuf);
			}

			if ((HWND)lParam == GetDlgItem(hwnd, IDC_SLIDER_OPACITY)) {

				sliderVal = HIWORD(wParam);

				swprintf_s(sbuf, sbufLen, L"%d %%", sliderVal);
				Static_SetText(GetDlgItem(hwnd, IDC_OPACITY_VAL), sbuf);
			}

			if ((HWND)lParam == GetDlgItem(hwnd, IDC_SLIDER_SCALE)) {

				sliderVal = HIWORD(wParam);

				swprintf_s(sbuf, sbufLen, L"%d %%", sliderVal);
				Static_SetText(GetDlgItem(hwnd, IDC_SCALE_VAL), sbuf);
			}
			break;

		default:
			break;
		}
		return TRUE;

	default:
		break;
	}

	return FALSE;
}

void ConfigPlugin(HWND hwnd)
{
	LIVEDASH_CONFIG& cfg = g_dash->GetConfig();
	LONG_PTR* p = (LONG_PTR*)&cfg;

	INT_PTR ret = OBSDialogBox(pluginInst, MAKEINTRESOURCE(IDD_LIVEDASH_CFG_DLG), API->GetMainWindow(), ConfigDialogProc, (LPARAM)p);
	switch (ret) {

	case TRUE:
#ifdef _DEBUG
		OBSMessageBox(API->GetMainWindow(), L"save config", L"CFG", MB_OK);
#endif		
		break;

	case FALSE:
#ifdef _DEBUG
		OBSMessageBox(API->GetMainWindow(), L"parent HWND invalid", L"CFG", MB_OK);
#endif
		break;

	case IDCANCEL:
		// cancel clicked -> restore previous config data
		break;

	default:
		DWORD err = GetLastError();
		std::wstringstream ss;
		ss << "invalid parameter error: " << err;
		std::wstring ws{ ss.str().c_str() };

		OBSMessageBox(API->GetMainWindow(), ws.c_str(), L"CFG", MB_OK);
		break;
	}
}

