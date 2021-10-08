// HelpBox.cpp from D2E
#include "JamellaD2E.h"
HWND hHelpBox = 0;
LRESULT CALLBACK HelpDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Position Help Box right of Parent Window
		{
			RECT ParentPos;
			GetWindowRect(GetParent(hWnd), &ParentPos);
			ParentPos.left = ParentPos.right;
			SetWindowPos(hWnd, HWND_TOP,
				ParentPos.left, ParentPos.top,
				0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		}
		// Load Help File from Resources
		HRSRC rcsrc = FindResource(hInstance, MAKEINTRESOURCE(lParam), "CHELP");
		HGLOBAL hglb = LoadResource(hInstance, rcsrc);
		BYTE* mem = (BYTE*)LockResource(hglb);

		RTFStreamSend(hWnd, IDC_HELP_Text, (const char*)mem);
		CHARRANGE cf = { -1,0 };
		SendDlgItemMessage(hWnd, IDC_HELP_Text, EM_EXSETSEL, 0, (LPARAM)&cf);
		ShowWindow(hWnd, SW_SHOWNA);
	}
	return true;
	case WM_WINDOWPOSCHANGED:
	{
		RECT Rect;
		GetClientRect(hWnd, &Rect);
		SetWindowPos(GetDlgItem(hWnd, IDC_HELP_Text), HWND_TOP, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOACTIVATE);
	}
	return false;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_HELP_Text:
			if (HIWORD(wParam) == EN_SETFOCUS)
			{
				CHARRANGE cf = { -1,0 };
				SendDlgItemMessage(hWnd, IDC_HELP_Text, EM_EXSETSEL, 0, (LPARAM)&cf);
			}
			return true;
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			hHelpBox = 0;
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, IDOK);
		hHelpBox = 0;
		CheckDlgButton(GetParent(hWnd), IDC_CHELP, BST_UNCHECKED);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}
bool ToggleHelpBox(HWND hWnd, int HELPID)
{
	if (hHelpBox) {
		CheckDlgButton(hWnd, IDC_CHELP, BST_UNCHECKED);
		CloseHelpBox();
		return false;
	}
	else {
		CheckDlgButton(hWnd, IDC_CHELP, BST_CHECKED);
		hHelpBox = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_HELP), hWnd, (DLGPROC)&HelpDialogProc, HELPID);
		SetFocus(hWnd);
		return true;
	}
}
void CloseHelpBox()
{
	if (hHelpBox) {
		EndDialog(hHelpBox, 0);
		hHelpBox = 0;
	}
}