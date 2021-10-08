// Tab2ItemList.cpp from D2E
#include "JamellaD2E.h"
HWND hItemListBox;
void UpdateTab2ItemList()
{
	if (!hItemListBox) return;
	HWND hItemList = GetDlgItem(hItemListBox, IDC_TAB2ItemList);
	SendMessage(hItemList, LB_RESETCONTENT, 0, 0);
	for (Item* I = Items; I != 0; I = I->Next())
	{
		int ix = SendMessage(hItemList, LB_ADDSTRING, 0, (LPARAM)I->Name());
		SendMessage(hItemList, LB_SETITEMDATA, ix, (LPARAM)I);
		if (I == SelItem)
			SendMessage(hItemList, LB_SETCURSEL, ix, 0);
	}
}
LRESULT CALLBACK Tab2ItemListDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		{
			RECT rWindow, DialogPos;
			GetWindowRect(hWnd, &rWindow);
			GetWindowRect(hMainDialog, &DialogPos);
			DialogPos.left -= rWindow.right - rWindow.left;
			SetWindowPos(hWnd, HWND_TOP,
				DialogPos.left, DialogPos.top,
				0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		}
		CheckDlgButton(hExpertBox, IDC_TAB2E_ItemList, BST_CHECKED);
		hItemListBox = hWnd;
		UpdateTab2ItemList();
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	}
	return true;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_TAB2ItemList && HIWORD(wParam) == LBN_SELCHANGE) {
			int ix = SendDlgItemMessage(hWnd, IDC_TAB2ItemList, LB_GETCURSEL, 0, 0);
			SelItem = (Item*)SendDlgItemMessage(hWnd, IDC_TAB2ItemList, LB_GETITEMDATA, ix, 0);
			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
		}
		return false;
	case WM_CLOSE:
	{
		if (hItemListBox)
			DestroyWindow(hItemListBox);
	}
	return false;
	case WM_DESTROY:
	{
		CheckDlgButton(hExpertBox, IDC_TAB2E_ItemList, BST_UNCHECKED);
		hItemListBox = 0;
	}
	return false;
	}
	return false;
}