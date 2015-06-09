
#include <windowsx.h>
#include <string>
#include "Plugin.h"
#include "config.h"

using namespace std;
using namespace libLDF;

INT_PTR CALLBACK ConfigDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const size_t sbufLen = 128;
	wchar_t sbuf[sbufLen]{0}; // general string buffer
	DWORD sliderVal = 0;

	HWND sliderPosX = GetDlgItem(hwnd, IDC_SLIDER_POSX);
	HWND sliderPosY = GetDlgItem(hwnd, IDC_SLIDER_POSY);
	HWND sliderScale = GetDlgItem(hwnd, IDC_SLIDER_SCALE);
	HWND sliderOpacity = GetDlgItem(hwnd, IDC_SLIDER_OPACITY);

	static LIVEDASH_CONFIG* cfg = NULL;// (LIVEDASH_CONFIG*)lParam;

	switch (msg)
	{
	case WM_INITDIALOG:
	{
		cfg = (LIVEDASH_CONFIG*)lParam; // GetWindowLongPtr(hwnd, DWL_MSGRESULT);
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

		ComboBox_SetCurSel(hwndDashList, 0);

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

		return TRUE;
	}

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			int idx = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_OVERLAY_LIST));
			if (idx > -1) {
				if (ComboBox_GetLBTextLen(GetDlgItem(hwnd, IDC_OVERLAY_LIST), idx) < sbufLen) {
					ComboBox_GetLBText(GetDlgItem(hwnd, IDC_OVERLAY_LIST), idx, (LPCTSTR)&sbuf[0]);
					cfg->selectedDashboard = std::wstring(sbuf);
					cfg->selectedDashboard.append(L".layout");
				}
			}

			cfg->posX = (int)SendMessage(GetDlgItem(hwnd, IDC_SLIDER_POSX), TBM_GETPOS, 0, 0);
			cfg->posY = (int)SendMessage(GetDlgItem(hwnd, IDC_SLIDER_POSY), TBM_GETPOS, 0, 0);
			cfg->scale = (float)((float)SendMessage(GetDlgItem(hwnd, IDC_SLIDER_SCALE), TBM_GETPOS, 0, 0) / 100.0f);
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

				//int idx = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				//ComboBox_GetLBText((HWND)lParam, idx, sbuf);

				//wchar_t b[512]{0};

				//wsprintfW((LPWSTR)b, L"%ws\\%ws.layout", DASHBOARD_PATH, sbuf);
				//std::wstring file{ b };
				//file.append(L"\\");
				//file.append(sbuf);
				//file.append(L".layout");

				IDashboardLayout* d = get_LDF();
				//std::string fn(file.begin(), file.end());
				//std::string fn("D:\\Development\\Projects\\VideoDashboard\\bin\\Debug\\x86\\dashboards\\RedCarbon_300KPH.layout");
				//d->ParseLayoutFile("D:\\Development\\Projects\\VideoDashboard\\bin\\Debug\\x86\\dashboards\\RedCarbon_300KPH.layout");
				//Bitmap* bmp = (Bitmap*)d->RenderDashboard(fn, 0);


				release_LDF(d);
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
	LIVEDASH_CONFIG cfg{ L"", L"", 0, 0, 0.0f, 0 };
	LONG_PTR* p = (LONG_PTR*)&cfg;

	INT_PTR ret = OBSDialogBox(pluginInst, MAKEINTRESOURCE(IDD_LIVEDASH_CFG_DLG), API->GetMainWindow(), ConfigDialogProc, (LPARAM)p);
	switch (ret) {

	case TRUE:
		OBSMessageBox(API->GetMainWindow(), L"save config", L"CFG", MB_OK);
		break;

	case FALSE:
		OBSMessageBox(API->GetMainWindow(), L"parent HWND invalid", L"CFG", MB_OK);
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

