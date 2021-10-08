#include "JamellaD2E.h"
LRESULT CALLBACK SaveDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (IsDlgButtonChecked(hWnd, IDC_SAVE_Backup) == BST_CHECKED)
				EndDialog(hWnd, 2);
			else
				EndDialog(hWnd, 1);
			return true;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}