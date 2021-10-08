// Tab0.cpp from D2E
#include "JamellaD2E.h"
HBITMAP hBmpJamella;
LRESULT CALLBACK Tab0DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
#if RELEASE == PUB
		sprintf(buffer, "%s %s", PROGRAMNAME, VERSION);
#else
		sprintf(buffer, "%s %s [%s]", PROGRAMNAME, VERSION, __DATE__);
#endif
		SetDlgItemText(hWnd, IDC_TAB0_Version, buffer);
		SendDlgItemMessage(hWnd, IDC_TAB0_BmpJamella, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpJamella);
	}
	return true;
	}
	return false;
}
/*
bool ShowPopupHelp(HWND hWnd,LPARAM lParam)
{
 HELPINFO *HI = (LPHELPINFO) lParam;
 const PopupHelp *PH = 0;
 if ((HWND) HI->hItemHandle == hMainDialog) {
 RECT r;
 GetWindowRect(hTabDialog,&r);
 POINT spt = HI->MousePos;
 spt.x -= r.left;
 spt.y -= r.top;
 HWND hCtrl = ChildWindowFromPointEx(hTabDialog,spt,CWP_SKIPTRANSPARENT);
 if (!hCtrl) return false;
 int CtrlID = GetDlgCtrlID(hCtrl);
 PH = FindPopupHelp(CtrlID);
 }
 else {
 int CtrlID = GetDlgCtrlID((HWND) HI->hItemHandle);
 PH = FindPopupHelp(CtrlID);
 }
 if (!PH) {
 MessageBeep(MB_ICONHAND);
 return false;
 }

 HH_POPUP Popup;

 ZeroMemory(&Popup,sizeof Popup);
 Popup.cbStruct = sizeof Popup;
 Popup.idString = 0;
 Popup.pszText = PH->HelpText;
 Popup.pt = HI->MousePos;
 Popup.clrForeground = RGB(255,0,0);
 Popup.clrBackground = RGB(255,255,239);
 Popup.rcMargins.left = Popup.rcMargins.right = Popup.rcMargins.top = Popup.rcMargins.bottom = -1;
 Popup.pszFont = "MS Sans Serif, 8";

 HtmlHelp((HWND) HI->hItemHandle, 0,
 HH_DISPLAY_TEXT_POPUP, (DWORD) &Popup);
 return true;
}
*/