// TAB2E.cpp from D2E
#include "JamellaD2E.h"
#define IDT_TIMER 12345
// TAB2E ToolBox
HWND hExpertBox;
const int nRawData = 32;
struct
{
	int DialogID;
	HWND hWindow;
}
Controls[] =
{
 { IDC_TAB2E_Raw00 },
 { IDC_TAB2E_Raw01 },
 { IDC_TAB2E_Raw02 },
 { IDC_TAB2E_Raw03 },
 { IDC_TAB2E_Raw04 },
 { IDC_TAB2E_Raw05 },
 { IDC_TAB2E_Raw06 },
 { IDC_TAB2E_Raw07 },
 { IDC_TAB2E_Raw08 },
 { IDC_TAB2E_Raw09 },
 { IDC_TAB2E_Raw0A },
 { IDC_TAB2E_Raw0B },
 { IDC_TAB2E_Raw0C },
 { IDC_TAB2E_Raw0D },
 { IDC_TAB2E_Raw0E },
 { IDC_TAB2E_Raw0F },
 { IDC_TAB2E_Raw10 },
 { IDC_TAB2E_Raw11 },
 { IDC_TAB2E_Raw12 },
 { IDC_TAB2E_Raw13 },
 { IDC_TAB2E_Raw14 },
 { IDC_TAB2E_Raw15 },
 { IDC_TAB2E_Raw16 },
 { IDC_TAB2E_Raw17 },
 { IDC_TAB2E_Raw18 },
 { IDC_TAB2E_Raw19 },
 { IDC_TAB2E_Raw1A },
 { IDC_TAB2E_Raw1B },
 { IDC_TAB2E_Raw1C },
 { IDC_TAB2E_Raw1D },
 { IDC_TAB2E_Raw1E },
 { IDC_TAB2E_Raw1F },
 { IDC_TAB2E_ItemCode },
 { IDC_TAB2E_UniqueCode },
 { IDC_TAB2E_DWA },
 { IDC_TAB2E_DWB },
 { IDC_TAB2E_MagicLevel },
 { IDC_TAB2E_GemNum },
};
inline char* MakeHex(int v, int digits)
{
	sprintf(buffer, "%0*X", digits, v);
	return buffer;
}
inline char* MakeDez(int v)
{
	sprintf(buffer, "%d", v);
	return buffer;
}
inline void ConvHex(int DlgID, BYTE* dest)
{
	GetDlgItemText(hExpertBox, DlgID, buffer, 256);
	if (strlen(buffer) > 2) return;
	char* test;
	int x = strtoul(buffer, &test, 16);
	if (*test == 0) *dest = x;
}
inline void ConvHex(int DlgID, WORD* dest)
{
	GetDlgItemText(hExpertBox, DlgID, buffer, 256);
	if (strlen(buffer) > 4) return;
	char* test;
	int x = strtoul(buffer, &test, 16);
	if (*test == 0) *dest = x;
}
inline void ConvHex(int DlgID, DWORD* dest)
{
	GetDlgItemText(hExpertBox, DlgID, buffer, 256);
	if (strlen(buffer) > 8) return;
	char* test;
	int x = strtoul(buffer, &test, 16);
	if (*test == 0) *dest = x;
}
inline bool ConvDezProp(int DlgID, int* dest)
{
	GetDlgItemText(hExpertBox, DlgID, buffer, 256);
	char* test;
	int x = strtoul(buffer, &test, 10);
	if (*test == 0) {
		*dest = x;
		return true;
	}
	return false;
}
inline bool ConvHexProp(int DlgID, int* dest)
{
	GetDlgItemText(hExpertBox, DlgID, buffer, 256);
	char* test;
	int x = strtoul(buffer, &test, 16);
	if (*test == 0) {
		*dest = x;
		return true;
	}
	return false;
}
void UpdateTab2E()
{
	if (!hExpertBox) return;
	if (SelItem)
	{
		for (int z = 0; z < sizeof Controls / sizeof Controls[0]; z++)
		{
			EnableWindow(Controls[z].hWindow, TRUE);
		}
		memset(buffer, 0, sizeof buffer);
		SetDlgItemText(hExpertBox, IDC_TAB2E_ItemRecordID, SelItem->ItemRecordName());
		BYTE* RawData = SelItem->GetItemRecord();
		int RawDataLen = SelItem->ItemRecordLength();
		for (int n = 0; n < nRawData; n++)
		{
			if (n < RawDataLen)
				SetDlgItemText(hExpertBox, Controls[n].DialogID, MakeHex(RawData[n], 2));
			else
				SetDlgItemText(hExpertBox, Controls[n].DialogID, "");
		}
		SetDlgItemText(hExpertBox, IDC_TAB2E_ItemCode, MakeHex(SelItem->ItemCode(), 8));
		SetDlgItemText(hExpertBox, IDC_TAB2E_ItemCodeChar, CodeString(SelItem->ItemCode()));
		SetDlgItemText(hExpertBox, IDC_TAB2E_UniqueCode, MakeHex(SelItem->UniqueCode(), 2));
		SetDlgItemText(hExpertBox, IDC_TAB2E_Container, MakeDez(SelItem->Container()));
		SetDlgItemText(hExpertBox, IDC_TAB2E_Xoord, MakeDez(SelItem->xPos()));
		SetDlgItemText(hExpertBox, IDC_TAB2E_Yoord, MakeDez(SelItem->yPos()));
		SetDlgItemText(hExpertBox, IDC_TAB2E_BodyCode, MakeDez(SelItem->BodyCode()));
		SetDlgItemText(hExpertBox, IDC_TAB2E_DWA, MakeHex(SelItem->DWA(), 8));
		SetDlgItemText(hExpertBox, IDC_TAB2E_DWB, MakeHex(SelItem->DWB(), 8));
		SetDlgItemText(hExpertBox, IDC_TAB2E_MagicLevel, MakeHex(SelItem->MagicLevel(), 2));
		SetDlgItemText(hExpertBox, IDC_TAB2E_GemNum, MakeDez(SelItem->GemNum()));
		/*
		 if (SelItem->MagicPrefix)
		 {
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val8,MakeDez(SelItem->MagicPrefix->N));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val9,MakeDez(SelItem->MagicPrefix->nMod));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val10,MakeDez(SelItem->modMagicPrefix));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val11,MakeDez(SelItem->modpickMagicPrefix));
		 }
		 else
		 {
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val8,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val9,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val10,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val11,"");
		 }
		 if (SelItem->MagicSuffix)
		 {
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val12,MakeDez(SelItem->MagicSuffix->N & 0xFF));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val13,MakeDez(SelItem->MagicSuffix->nMod));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val14,MakeDez(SelItem->modMagicSuffix));
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val15,MakeDez(SelItem->modpickMagicSuffix));
		 }
		 else
		 {
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val12,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val13,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val14,"");
		 SetDlgItemText(hExpertBox,IDC_TAB2E_Val15,"");
		 }
		*/
	}
	else
	{
		SetDlgItemText(hExpertBox, IDC_TAB2E_ItemRecordID, "");
		for (int z = 0; z < sizeof Controls / sizeof Controls[0]; z++)
		{
			EnableWindow(Controls[z].hWindow, FALSE);
			SetWindowText(Controls[z].hWindow, "");
		}
	}
	if (hRandomBox)
		UpdateTab2Rnd();
	if (hItemListBox)
		UpdateTab2ItemList();
}
LRESULT CALLBACK Tab2EDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		{
			RECT DialogPos;
			GetWindowRect(hMainDialog, &DialogPos);
			DialogPos.left = DialogPos.right;
			SetWindowPos(hWnd, HWND_TOP,
				DialogPos.left, DialogPos.top,
				0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		}
		for (int z = 0; z < sizeof Controls / sizeof Controls[0]; z++)
		{
			Controls[z].hWindow = GetDlgItem(hWnd, Controls[z].DialogID);
		}
		SetTimer(hWnd, IDT_TIMER, EXPERTBOXPOLLER, NULL);
		hExpertBox = hWnd;
		UpdateTab2E();
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	}
	return true;
	case WM_TIMER:
		switch (wParam)
		{
		case IDT_TIMER:
		{
			if (SelItem && SelItem->Busy) {
				UpdateTab2E();
				InvalidateRect(hExpertBox, NULL, FALSE);
			}
		}
		break;
		}
		return false;
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			switch (LOWORD(wParam))
			{
			case IDC_TAB2E_Raw00: case IDC_TAB2E_Raw10:
			case IDC_TAB2E_Raw01: case IDC_TAB2E_Raw11:
			case IDC_TAB2E_Raw02: case IDC_TAB2E_Raw12:
			case IDC_TAB2E_Raw03: case IDC_TAB2E_Raw13:
			case IDC_TAB2E_Raw04: case IDC_TAB2E_Raw14:
			case IDC_TAB2E_Raw05: case IDC_TAB2E_Raw15:
			case IDC_TAB2E_Raw06: case IDC_TAB2E_Raw16:
			case IDC_TAB2E_Raw07: case IDC_TAB2E_Raw17:
			case IDC_TAB2E_Raw08: case IDC_TAB2E_Raw18:
			case IDC_TAB2E_Raw09: case IDC_TAB2E_Raw19:
			case IDC_TAB2E_Raw0A: case IDC_TAB2E_Raw1A:
			case IDC_TAB2E_Raw0B: case IDC_TAB2E_Raw1B:
			case IDC_TAB2E_Raw0C: case IDC_TAB2E_Raw1C:
			case IDC_TAB2E_Raw0D: case IDC_TAB2E_Raw1D:
			case IDC_TAB2E_Raw0E: case IDC_TAB2E_Raw1E:
			case IDC_TAB2E_Raw0F: case IDC_TAB2E_Raw1F:
			{
				if (SelItem)
				{
					BYTE* RawData = SelItem->GetItemRecord();
					int RawDataLen = SelItem->ItemRecordLength();

					for (int n = 0; n < nRawData; n++)
					{
						if (n < RawDataLen) {
							ConvHex(Controls[n].DialogID, &RawData[n]);
						}
					}
					SelItem->Info = 0;
					UpdateTab2();
					InvalidateRect(hTabDialog, NULL, FALSE);
				}
			}
			break;
			case IDC_TAB2E_ItemCode:
			case IDC_TAB2E_UniqueCode:
			case IDC_TAB2E_DWA:
			case IDC_TAB2E_DWB:
			case IDC_TAB2E_MagicLevel:
			case IDC_TAB2E_GemNum:
			{
				if (SelItem)
				{
					int z;
					if (ConvHexProp(IDC_TAB2E_ItemCode, &z)) SelItem->SetItemCode(z);
					if (ConvHexProp(IDC_TAB2E_UniqueCode, &z)) SelItem->SetUniqueCode(z);
					if (ConvHexProp(IDC_TAB2E_DWA, &z)) SelItem->SetDWA(z);
					if (ConvHexProp(IDC_TAB2E_DWB, &z)) SelItem->SetDWB(z);
					if (ConvHexProp(IDC_TAB2E_MagicLevel, &z)) SelItem->SetMagicLevel(z);
					if (ConvDezProp(IDC_TAB2E_GemNum, &z)) SelItem->SetGemNum(z);
					// if (SelItem->MagicPrefix)
					{
						// if (ConvDezProp(IDC_TAB2E_Val8,&z)) MagicPrefixTable[SelItem->iMagicPrefix].ModLevel = z
						;
						//if (ConvDezProp(IDC_TAB2E_Val9,&z)) SelItem->MagicPrefix->nMod = z;
					}
					// if (SelItem->MagicSuffix)
					{
						// if (ConvDezProp(IDC_TAB2E_Val12,&z)) MagicSuffixTable[SelItem->iMagicSuffix].ModLevel =
						z;
						//if (ConvDezProp(IDC_TAB2E_Val13,&z)) SelItem->MagicSuffix->nMod = z;
					}
					SelItem->Info = 0;
					SelItem->Decoded = false;
					UpdateTab2();
					InvalidateRect(hTabDialog, NULL, FALSE);
				}
			}
			break;
			}
		}
		else if (LOWORD(wParam) == IDC_TAB2E_RandA || LOWORD(wParam) == IDC_TAB2E_RandB)
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hExpertBox, LOWORD(wParam)))
				{
					if (hRandomBox) {
						DestroyWindow(hRandomBox);
					}
					CheckDlgButton(hWnd, LOWORD(wParam), BST_UNCHECKED);
				}
				else
				{
					if (LOWORD(wParam) == IDC_TAB2E_RandA)
					{
						CheckDlgButton(hWnd, IDC_TAB2E_RandA, BST_CHECKED);
						CheckDlgButton(hWnd, IDC_TAB2E_RandB, BST_UNCHECKED);
						RandomBoxDW = 0;
					}
					else if (LOWORD(wParam) == IDC_TAB2E_RandB)
					{
						CheckDlgButton(hWnd, IDC_TAB2E_RandA, BST_UNCHECKED);
						CheckDlgButton(hWnd, IDC_TAB2E_RandB, BST_CHECKED);
						RandomBoxDW = 1;
					}
					if (!hRandomBox) {
						hRandomBox = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TAB2Rnd), hTabDialog, (DLGPROC)&Tab2RndDialogProc);
					}
					UpdateTab2();
				}
			}
		}
		else if (LOWORD(wParam) == IDC_TAB2E_FindInfo && HIWORD(wParam) == BN_CLICKED)
		{
			if (!SelItem) return false;
			SelItem->Info = 0;
			SelItem->FindInfo();
			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
		}
		else if (LOWORD(wParam) == IDC_TAB2E_Decode && HIWORD(wParam) == BN_CLICKED)
		{
			if (!SelItem) return false;
			SelItem->Decoded = 0;
			SelItem->Decode();
			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
		}
		else if (LOWORD(wParam) == IDC_TAB2E_ItemList && HIWORD(wParam) == BN_CLICKED)
		{
			if (hItemListBox) {
				DestroyWindow(hItemListBox);
			}
			else {
				hItemListBox = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TAB2ItemList), hTabDialog, (DLGPROC)&Tab2ItemListDialogProc);
			}
		}
	}
	return false;
	case WM_CLOSE:
	{
		if (hExpertBox)
			DestroyWindow(hExpertBox);
		if (hItemListBox)
			DestroyWindow(hItemListBox);
	}
	return false;
	case WM_DESTROY:
	{
		if (hRandomBox)
			DestroyWindow(hRandomBox);
		CheckDlgButton(hTabDialog, IDC_TAB2_ExpertMode, BST_UNCHECKED);
		hExpertBox = 0;
	}
	return false;
	}
	return false;
}