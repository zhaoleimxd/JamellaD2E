// CowLevel.cpp from D2E
#include "JamellaD2E.h"
LRESULT CALLBACK CowLevelDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, wParam);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}