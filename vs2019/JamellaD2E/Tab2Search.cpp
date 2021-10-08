#include "JamellaD2E.h"
const struct
{
	char* Text;
	int PriorityID;
	int ClassID;
}
Priorities[] =
{
 { "Idle", THREAD_PRIORITY_IDLE, IDLE_PRIORITY_CLASS },
 { "Lowest", THREAD_PRIORITY_LOWEST, NORMAL_PRIORITY_CLASS },
 { "Below Normal", THREAD_PRIORITY_BELOW_NORMAL, NORMAL_PRIORITY_CLASS },
 { "Normal", THREAD_PRIORITY_NORMAL, NORMAL_PRIORITY_CLASS },
 { "Above Normal", THREAD_PRIORITY_ABOVE_NORMAL, HIGH_PRIORITY_CLASS },
 { "Highest", THREAD_PRIORITY_HIGHEST, HIGH_PRIORITY_CLASS },
 { "Time Critical", THREAD_PRIORITY_TIME_CRITICAL, REALTIME_PRIORITY_CLASS }
};
const int nPriorities = 7;
LRESULT CALLBACK Tab2SearchDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Save Parameter
		SetWindowLong(hWnd, GWL_USERDATA, (DWORD)lParam);
		SearchThread* P = (SearchThread*)lParam;
		P->Running = true;
		// Start Timer
		SetTimer(hWnd, IDT_TIMER, SEARCHPOLLER, NULL);
		PostMessage(hWnd, WM_TIMER, IDT_TIMER, 0);
		SendDlgItemMessage(hWnd, IDC_CHELP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconHelp);
		// Simple Box Controls
		if (!P->Advanced)
		{
			// Setup Progress Bar
			SendDlgItemMessage(hWnd, IDC_TAB2SS_Scope, PBM_SETRANGE, 0, MAKELPARAM(0, 16384));
			SendDlgItemMessage(hWnd, IDC_TAB2SS_Scope, PBM_SETBARCOLOR, 0, PROGRESSCOLOR);
			SendDlgItemMessage(hWnd, IDC_TAB2SS_Scope, PBM_SETPOS, 0, 0);
		}
		else
		{
			// Advanced Box Controls

			// Setup Progress Bar
			SendDlgItemMessage(hWnd, IDC_TAB2SA_Scope, PBM_SETRANGE, 0, MAKELPARAM(0, 16384));
			SendDlgItemMessage(hWnd, IDC_TAB2SA_Scope, PBM_SETBARCOLOR, 0, PROGRESSCOLOR);
			SendDlgItemMessage(hWnd, IDC_TAB2SA_Scope, PBM_SETPOS, 0, 0);

			// Setup Priority Slider
			SendDlgItemMessage(hWnd, IDC_TAB2SA_Slider, TBM_SETRANGE, 1, MAKELONG(0, nPriorities - 1));
			SendDlgItemMessage(hWnd, IDC_TAB2SA_Slider, TBM_SETPOS, 1, 3);
		}
		// Show Windows
		ShowWindow(hWnd, SW_SHOW);
		InvalidateRect(hTabDialog, NULL, FALSE);
	}
	return true;
	case WM_TIMER:
		switch (wParam)
		{
		case IDT_TIMER:
		{
			SearchThread* P = (SearchThread*)GetWindowLong(hWnd, GWL_USERDATA);
			if (!P) break;
			if (!P->Item) break;
			char strbuffer[32];
			sprintf(strbuffer, "%lu Tries", P->Counter);
			if (!P->Advanced)
			{
				SetDlgItemText(hWnd, IDC_TAB2SS_Counter, strbuffer);
				SetDlgItemText(hWnd, IDC_TAB2SS_Current, P->Item->Name());
				SendDlgItemMessage(hWnd, IDC_TAB2SS_Scope, PBM_SETPOS, (P->Counter >> 16 + 2), 0);
			}
			else
			{
				SetDlgItemText(hWnd, IDC_TAB2SA_Counter, strbuffer);
				SetDlgItemText(hWnd, IDC_TAB2SA_Current, P->Item->Name());
				SendDlgItemMessage(hWnd, IDC_TAB2SA_Scope, PBM_SETPOS, (P->Counter >> 16 + 2), 0);
				SetDlgItemInt(hWnd, IDC_TAB2SA_MagicLevel, P->Item->MagicLevel(), FALSE);

				if (!P->Running) {
					SetDlgItemText(hWnd, IDC_TAB2AS_Speed, "Search Stopped");
				}
				else if (P->Past[0] > 0) {
					DWORD Speed = (P->Past[BFASPEEDPAST - 1] - P->Past[0]) / (SEARCHPOLLER * BFASPEEDPAST / 1000);
					sprintf(strbuffer, "%lu Tries/sec", Speed);
					SetDlgItemText(hWnd, IDC_TAB2AS_Speed, strbuffer);
				}
				else
					SetDlgItemText(hWnd, IDC_TAB2AS_Speed, "Measuring");
			}
			for (int z = 0; z < BFASPEEDPAST - 1; z++)
				P->Past[z] = P->Past[z + 1];
			P->Past[BFASPEEDPAST - 1] = P->Counter;
		}
		break;
		}
		return false;
	case WM_COMMAND:
	{
		SearchThread* P = (SearchThread*)GetWindowLong(hWnd, GWL_USERDATA);
		switch (LOWORD(wParam))
		{
		case IDC_TAB2SS_Stop:
		case IDC_TAB2SA_Stop:

			if (P)
			{
				if (P->HitsSelection) {
					if (P->Running) {
						P->Running = false;
						SetDlgItemText(hWnd, IDC_TAB2SA_Stop, "Continue Search");
					}
					else {
						P->Running = true;
						SetDlgItemText(hWnd, LOWORD(wParam), "Stop Search");
					}
				}
				else
					PostQuitMessage(IDOK);
			}
			else
				PostQuitMessage(IDOK);
			break;
		case IDC_TAB2SA_TraverseMagicLevels:
		{
			if (!P) break;
			if (P->TraverseMagicLevels)
				P->TraverseMagicLevels = false;
			else
				P->TraverseMagicLevels = true;
			CheckDlgButton(hWnd, LOWORD(wParam), P->TraverseMagicLevels);
		}
		break;
		case IDC_TAB2SA_HitSelection:
		{
			if (!P) break;
			if (P->HitsSelection) {
				P->HitsSelection = false;
				SetDlgItemText(hWnd, IDC_TAB2SA_Stop, "Stop Search");
				EnableWindow(GetDlgItem(hWnd, IDC_TAB2SA_HitList), FALSE);
				P->Running = true;
			}
			else {
				P->HitsSelection = true;
				SetDlgItemText(hWnd, IDC_TAB2SA_Stop, "Pause Search");
				EnableWindow(GetDlgItem(hWnd, IDC_TAB2SA_HitList), TRUE);
			}
			CheckDlgButton(hWnd, LOWORD(wParam), P->HitsSelection);
		}
		break;
		case IDC_TAB2SA_HitList:
			if (HIWORD(wParam) == LBN_SELCHANGE && P)
			{
				P->Running = false;
				SetDlgItemText(hWnd, IDC_TAB2SA_Stop, "Continue Search");

				int ix = SendDlgItemMessage(hWnd, LOWORD(wParam), LB_GETCURSEL, 0, 0);
				SearchHit* H = (SearchHit*)SendDlgItemMessage(hWnd, LOWORD(wParam), LB_GETITEMDATA, ix, 0);

				P->Item->SetMagicLevel(H->MagicLevel);
				P->Item->SetDWA(H->DWA);
				P->Item->SetDWB(H->DWB);
				P->Item->Decoded = false;
				UpdateTab2();
				InvalidateRect(hTabDialog, NULL, FALSE);
			}
			else if (HIWORD(wParam) == LBN_DBLCLK && P)
			{
				P->Running = false;
				SetDlgItemText(hWnd, IDC_TAB2SA_Stop, "Continue Search");

				int ix = SendDlgItemMessage(hWnd, LOWORD(wParam), LB_GETCURSEL, 0, 0);
				SearchHit* H = (SearchHit*)SendDlgItemMessage(hWnd, LOWORD(wParam), LB_GETITEMDATA, ix, 0);

				P->Item->SetMagicLevel(H->MagicLevel);
				P->Item->SetDWA(H->DWA);
				P->Item->SetDWB(H->DWB);
				P->Item->Decoded = false;
				PostQuitMessage(IDOK);
			}
			break;
		case IDC_CHELP:
			ToggleHelpBox(hWnd, IDH_TAB2S);
			break;
		}
	}
	return false;
	case WM_USER:
	{
		SearchThread* P = (SearchThread*)GetWindowLong(hWnd, GWL_USERDATA);
		if (!P) break;
		SendDlgItemMessage(hWnd, IDC_TAB2SA_HitList, LB_RESETCONTENT, 0, 0);
		for (SearchHit* H = P->Hits; H != 0; H = H->List)
		{
			char buffer[64];
			sprintf(buffer, "MG %i | DWB %8X", H->MagicLevel, H->DWB);
			int ix = SendDlgItemMessage(hWnd, IDC_TAB2SA_HitList, LB_ADDSTRING, 0, (LPARAM)buffer);
			SendDlgItemMessage(hWnd, IDC_TAB2SA_HitList, LB_SETITEMDATA, ix, (LPARAM)H);
		}
	}
	return false;
	case WM_HSCROLL:
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2SA_Slider))
		{
			SearchThread* P = (SearchThread*)GetWindowLong(hWnd, GWL_USERDATA);
			int Prior = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			SetDlgItemText(hWnd, IDC_TAB2SA_Priority, Priorities[Prior].Text);
			// if (!SetPriorityClass(GetCurrentProcess(),Priorities[Prior].ClassID))
			// ErrorMessage();
			if (!SetThreadPriority(P->Thread, Priorities[Prior].PriorityID))
				ErrorMessage();
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(IDOK);
		return false;
	case WM_DESTROY:
		return false;
	}
	return false;
}