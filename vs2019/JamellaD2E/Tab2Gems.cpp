// Tab2Gems.cpp from D2E
#include "JamellaD2E.h"
struct
{
	int Frame, Bitmap, ListBox, InfoEdit;
	bool Active;
	int ItemCode;
}
GemID[] =
{
 { IDC_TAB2Gems_Frame1, IDC_TAB2Gems_Bmp1, IDC_TAB2Gems_Sel1, IDC_TAB2Gems_Info1 },
 { IDC_TAB2Gems_Frame2, IDC_TAB2Gems_Bmp2, IDC_TAB2Gems_Sel2, IDC_TAB2Gems_Info2 },
 { IDC_TAB2Gems_Frame3, IDC_TAB2Gems_Bmp3, IDC_TAB2Gems_Sel3, IDC_TAB2Gems_Info3 },
 { IDC_TAB2Gems_Frame4, IDC_TAB2Gems_Bmp4, IDC_TAB2Gems_Sel4, IDC_TAB2Gems_Info4 },
 { IDC_TAB2Gems_Frame5, IDC_TAB2Gems_Bmp5, IDC_TAB2Gems_Sel5, IDC_TAB2Gems_Info5 },
 { IDC_TAB2Gems_Frame6, IDC_TAB2Gems_Bmp6, IDC_TAB2Gems_Sel6, IDC_TAB2Gems_Info6 },
 { IDC_TAB2Gems_Frame7, IDC_TAB2Gems_Bmp7, IDC_TAB2Gems_Sel7, IDC_TAB2Gems_Info7 }
};
int FindnGemInfo(DWORD ICode)
{
	for (int n = 0; n < nGemInfos; n++) {
		if (GemInfos[n].ItemCode == ICode || GemInfos[n].IC == ICode)
			return n;
	}
	return -1;
}
ItemInfo* FindItemInfo(DWORD ICode)
{
	for (int n = 0; n < nItemInfos; n++) {
		if (ItemInfos[n].ItemCode == ICode || ItemInfos[n].IC == ICode)
			return &ItemInfos[n];
	}
	return 0;
}
inline int FindListBoxID(int DlgID)
{
	for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++) {
		if (GemID[z].ListBox == DlgID)
			return z;
	}
	return -1;
}
char MergeGemTextsBuffer[256];
const char* MergeGemTexts(const GemInfo* Info)
{
	char* s = MergeGemTextsBuffer;
	*s = 0;
	switch (SelItem->Info->GemClass)
	{
	case 'W':
	{
		for (int z = 0; z < 3; z++) {
			if (!Info->WeaponMod[z].Code) continue;
			const char* e = GetEffect(Info->WeaponMod[z].Code);
			sprintf(buffer, e, Info->WeaponMod[z].Min);
			strcat(s, buffer);
			strcat(s, "\r\n");
		}
		break;
	}
	case 'H':
	{
		for (int z = 0; z < 3; z++) {
			if (!Info->HelmMod[z].Code) continue;
			const char* e = GetEffect(Info->HelmMod[z].Code);
			sprintf(buffer, e, Info->HelmMod[z].Min);
			strcat(s, buffer);
			strcat(s, "\r\n");
		}
		break;
	}
	case 'S':
	{
		for (int z = 0; z < 3; z++) {
			if (!Info->ShieldMod[z].Code) continue;
			const char* e = GetEffect(Info->ShieldMod[z].Code);
			sprintf(buffer, e, Info->ShieldMod[z].Min);
			strcat(s, buffer);
			strcat(s, "\r\n");
		}
		break;
	}
	}
	return s;
}
void UpdateTab2Gems(HWND hWnd)
{
	for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
	{
		if (GemID[z].Active)
		{
			EnableWindow(GetDlgItem(hWnd, GemID[z].Frame), TRUE);
			EnableWindow(GetDlgItem(hWnd, GemID[z].Bitmap), TRUE);
			EnableWindow(GetDlgItem(hWnd, GemID[z].ListBox), TRUE);
			EnableWindow(GetDlgItem(hWnd, GemID[z].InfoEdit), TRUE);
			if (GemID[z].ItemCode)
			{
				int nGI = FindnGemInfo(GemID[z].ItemCode);
				if (nGI < 0) {
					SendDlgItemMessage(hWnd, GemID[z].Bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)0);
					SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_SETCURSEL, 0, 0);
					SetDlgItemText(hWnd, GemID[z].InfoEdit, "???");
				}
				else {
					const GemInfo* GI = &GemInfos[nGI];
					ItemInfo* II = FindItemInfo(GemID[z].ItemCode);
					SendDlgItemMessage(hWnd, GemID[z].Bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ItemInfoGetBitmap(II));
					SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_SETCURSEL, nGI, 0);
					SetDlgItemText(hWnd, GemID[z].InfoEdit, MergeGemTexts(GI));
				}
			}
			else
			{
				SendDlgItemMessage(hWnd, GemID[z].Bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)0);
				SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_SETCURSEL, 0, 0);
				SetDlgItemText(hWnd, GemID[z].InfoEdit, "");
			}
		}
		else
		{
			EnableWindow(GetDlgItem(hWnd, GemID[z].Frame), FALSE);
			SendDlgItemMessage(hWnd, GemID[z].Bitmap, STM_SETIMAGE, IMAGE_BITMAP, 0);
			EnableWindow(GetDlgItem(hWnd, GemID[z].Bitmap), FALSE);
			EnableWindow(GetDlgItem(hWnd, GemID[z].ListBox), FALSE);
			EnableWindow(GetDlgItem(hWnd, GemID[z].InfoEdit), FALSE);
			SetDlgItemText(hWnd, GemID[z].InfoEdit, "");
		}
	}
}
void LoadGems()
{
	Item* G = SelItem->Gems;
	for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
	{
		GemID[z].Active = true;
		if (SelItem->Info->Sockets > z || RegOptions.A7Gems)
		{
			if (G) {
				GemID[z].ItemCode = G->ItemCode();
			}
			else {
				GemID[z].ItemCode = 0x0000;
			}
			if (G) G = G->Next();
		}
		else {
			GemID[z].Active = false;
		}
	}
}
void SaveGems()
{
	if (SelItem->Gems) {
		delete SelItem->Gems;
		SelItem->Gems = 0;
	}
	for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
	{
		if (!GemID[z].Active) continue;
		if (!GemID[z].ItemCode) continue;
		ItemInfo* II = FindItemInfo(GemID[z].ItemCode);
		Item* G = CreateItem(&SelItem->Gems, II);
		G->SetCoordinates(CNT_SOCKET, z, 0);
		G->SetQuality(USUALITEM + 1);
	}
	SelItem->SetGemNum(SelItem->Gems->Count());
}
LRESULT CALLBACK Tab2GemsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		LoadGems();
		for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
			SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_RESETCONTENT, 0, 0);
		for (int n = 0; n < nGemInfos; n++)
		{
			for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
			{
				SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_ADDSTRING, 0, (LPARAM)GemInfos[n].Name);
				SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_SETITEMDATA, n, (LPARAM)GemInfos[n].ItemCode);
			}
		}
		for (int z = 0; z < sizeof GemID / sizeof GemID[0]; z++)
			SendDlgItemMessage(hWnd, GemID[z].ListBox, CB_SETCURSEL, 0, 0);

		UpdateTab2Gems(hWnd);
	}
	return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			SaveGems();
			EndDialog(hWnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			break;
		case IDC_TAB2Gems_Sel1:
		case IDC_TAB2Gems_Sel2:
		case IDC_TAB2Gems_Sel3:
		case IDC_TAB2Gems_Sel4:
		case IDC_TAB2Gems_Sel5:
		case IDC_TAB2Gems_Sel6:
		case IDC_TAB2Gems_Sel7:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				int ID = FindListBoxID(LOWORD(wParam));
				if (!GemID[ID].Active) break;
				HWND hCtl = (HWND)lParam;
				int i = SendMessage(hCtl, CB_GETCURSEL, 0, 0);
				GemID[ID].ItemCode = SendMessage(hCtl, CB_GETITEMDATA, i, 0);
				UpdateTab2Gems(hWnd);
			}
		}
		break;
		}
	}
	return false;
	case WM_CLOSE:
		EndDialog(hWnd, IDCANCEL);
		return false;
	case WM_DESTROY:
	{
	}
	return false;
	}
	return false;
}