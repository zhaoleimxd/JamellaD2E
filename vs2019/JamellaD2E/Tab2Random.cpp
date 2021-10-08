// Tab2R.cpp from D2E
#include "JamellaD2E.h"
// Tab2R ToolBox
HWND hRandomBox;
int RandomBoxDW;
void UpdateTab2Rnd()
{
	if (!hRandomBox) return;
	if (SelItem)
	{
		char output[2048] = "";
		RAND s, r = { 0, 666 };
		int x;
		if (RandomBoxDW == 0)
		{
			r.Seed = SelItem->DWA();
			x = -1;
		}
		else
		{
			r.Seed = SelItem->DWB();
			x = StartRandoms(SelItem, &s);
		}
		for (int z = 0; z < 50; z++)
		{
			sprintf(buffer, "%2d: %08X %08X\r\n", z + 1, r.Seed, r.Carry);
			strcat(output, buffer);
			if (x == z)
				strcat(output, "Prefix:\r\n");
			Random(&r);
		}
		SetDlgItemText(hRandomBox, IDC_TAB2Rnd_Edit, output);
	}
	else
	{
		SetDlgItemText(hRandomBox, IDC_TAB2Rnd_Edit, "");
	}
}
LRESULT CALLBACK Tab2RndDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	}
	return true;
	case WM_COMMAND:
	{
	}
	return false;
	case WM_CLOSE:
	{
		if (hRandomBox)
			DestroyWindow(hRandomBox);
	}
	return false;
	case WM_DESTROY:
	{
		CheckDlgButton(hExpertBox, IDC_TAB2E_RandA, BST_UNCHECKED);
		CheckDlgButton(hExpertBox, IDC_TAB2E_RandB, BST_UNCHECKED);
		hRandomBox = 0;
	}
	return false;
	}
	return false;
}