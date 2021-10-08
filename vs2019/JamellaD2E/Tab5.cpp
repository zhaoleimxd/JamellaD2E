// Tab5.cpp from D2E
#include "JamellaD2E.h"
DWORD* waypoints;
HBITMAP hBmpWaypointOn;
HBITMAP hBmpWaypointOff;
void inline updatewaypoints(HWND hWnd)
{
	for (int id = 0; id <= IDC_TAB5_Way33 - IDC_TAB5_Way01; id++)
	{
		SendDlgItemMessage(hWnd, IDC_TAB5_Way01 + id, STM_SETIMAGE, IMAGE_BITMAP,
			(*waypoints & (1 << id)) ? (LPARAM)hBmpWaypointOn : (LPARAM)hBmpWaypointOff);
	}
}
LRESULT CALLBACK Tab5DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Select default difficulty level
		CheckRadioButton(hWnd, IDC_TAB5_Diff1, IDC_TAB5_Diff3, IDC_TAB5_Diff1);
		waypoints = &fc.WS.waypoints1;
		updatewaypoints(hWnd);
		hToolTip = CreateToolTipCtrl(hWnd, IDD_TAB5, true);
	}
	return true;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_TAB5_Diff1 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB5_Diff1))
			{
				waypoints = &fc.WS.waypoints1;
				updatewaypoints(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB5_Diff2 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB5_Diff2))
			{
				waypoints = &fc.WS.waypoints2;
				updatewaypoints(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB5_Diff3 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB5_Diff3))
			{
				waypoints = &fc.WS.waypoints3;
				updatewaypoints(hWnd);
			}
		}
		else if (LOWORD(wParam) >= IDC_TAB5_Way01 && LOWORD(wParam) <= IDC_TAB5_Way33 &&
			(HIWORD(wParam) == BN_CLICKED || HIWORD(wParam) == BN_DBLCLK))
		{
			DWORD o = 1 << (LOWORD(wParam) - IDC_TAB5_Way01);
			if (*waypoints & o)
				*waypoints &= o ^ 0xFFFFFFFF;
			else
				*waypoints |= o;
			updatewaypoints(hWnd);
		}
		else if (LOWORD(wParam) == IDC_TAB5_Batch && HIWORD(wParam) == BN_CLICKED)
		{
			HMENU hMenu = GetSubMenu(hBatchMenu, 4);

			POINT Pos;
			GetCursorPos(&Pos);

			TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
				Pos.x, Pos.y, 0, hWnd, NULL);
		}
		else if (LOWORD(wParam) == IDR_TAB5_ActivateHereAll)
		{
			*waypoints = 0x3FFFFFFF;
			updatewaypoints(hWnd);
		}
		else if (LOWORD(wParam) == IDR_TAB5_ActivateAllAll)
		{
			fc.WS.waypoints1 = 0x3FFFFFFF;
			fc.WS.waypoints2 = 0x3FFFFFFF;
			fc.WS.waypoints3 = 0x3FFFFFFF;
			updatewaypoints(hWnd);
		}
		else if (LOWORD(wParam) == IDR_TAB5_DeactivateHereAll)
		{
			*waypoints = 0x0;
			updatewaypoints(hWnd);
		}
		else if (LOWORD(wParam) == IDR_TAB5_DeactivateAllAll)
		{
			fc.WS.waypoints1 = 0x0;
			fc.WS.waypoints2 = 0x0;
			fc.WS.waypoints3 = 0x0;
			updatewaypoints(hWnd);
		}
	}
	break;
	case WM_DESTROY:
	{
		DestroyWindow(hToolTip);
	}
	return false;
	}
	return false;
}