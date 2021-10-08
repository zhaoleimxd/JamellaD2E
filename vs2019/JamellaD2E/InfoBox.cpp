// InfoBox.cpp from D2E
#include "JamellaD2E.h"
HBITMAP hBmpWebLink;
static int RTFStreamOffset;
static const char* RTFCharStream;
static DWORD CALLBACK RTFStreamCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR* pcb)
{
	if (!RTFCharStream) return -1;
	if (strlen(RTFCharStream) - RTFStreamOffset <= 0) return -1;
	*pcb = min((LONG)strlen(RTFCharStream) - RTFStreamOffset, cb);
	strncpy((char*)pbBuff, RTFCharStream + RTFStreamOffset, *pcb);
	RTFStreamOffset += *pcb;
	return 0;
}
void RTFStreamSend(HWND hWnd, int CtrlID, const char* Stream)
{
	RTFStreamOffset = 0;
	RTFCharStream = Stream;

	EDITSTREAM edstr;
	edstr.dwCookie = 0;
	edstr.dwError = 0;
	edstr.pfnCallback = &RTFStreamCallback;

	SendDlgItemMessage(hWnd, CtrlID, EM_STREAMIN, SF_RTF, (LPARAM)&edstr);
}
LRESULT CALLBACK InfoDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		sprintf(buffer, "%s\n%s", PROGRAMNAME, VERSION);
		SetDlgItemText(hWnd, IDC_INFO_Program, buffer);
		sprintf(buffer, "Built on %s at %s\nVersion Code: %s", __DATE__, __TIME__, RELEASECODE);
		SetDlgItemText(hWnd, IDC_INFO_Date, buffer);
		SetDlgItemText(hWnd, IDC_INFO_URL, PERMANENTURL);
		SetDlgItemText(hWnd, IDC_INFO_Email, EMAIL);
		SendDlgItemMessage(hWnd, IDC_INFO_LINK, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpWebLink);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_JamellaSound), hInstance, SND_RESOURCE | SND_ASYNC);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_INFO_LINK:
			ShellExecute(NULL, "open", PERMANENTURL, NULL, NULL, SW_SHOWMAXIMIZED);
			return true;
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