// Tab2M.cpp from D2E
#include "JamellaD2E.h"
/*
static DWORD WINAPI BruteForceAttack(LPVOID ThreadParameters)
{
 MagicSearchThread *P = (MagicSearchThread *)ThreadParameters;
 P->Advanced = true;
 P->Dialog = CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_TAB2S),hMainDialog,(DLGPROC) Tab2SearchDialogProc,(LPARA
M) P);
 if (!P->Dialog) return -1;
 Item* I = P->Item;
 I->Busy = true;
 int MagicLevel = 0;
 if (P->PrefixMatch && P->Prefix)
 MagicLevel = max(MagicLevel,P->Prefix->ModLevel);
 if (P->SuffixMatch && P->Suffix)
 MagicLevel = max(MagicLevel,P->Suffix->ModLevel);
 I->SetMagicLevel(MagicLevel - 2);
 while(!CheckPollMessages())
 {
 I->SetDWB(I->DWB() + 1);
 I->MD->QuickDecode();
 P->Counter++;
 if (!I->Decoded) continue;
 if (P->PrefixMatch)
 {
 if (P->Prefix != I->MD->MagicPrefix) continue;
 if (P->ForcePrefixMatch[0])
 if (P->ForcePrefixValue[0] != I->MD->MagicPrefixMag[0]) continue;
 if (P->ForcePrefixMatch[1])
 if (P->ForcePrefixValue[1] != I->MD->MagicPrefixMag[1]) continue;
 if (P->ForcePrefixMatch[2])
 if (P->ForcePrefixValue[2] != I->MD->MagicPrefixMag[2]) continue;
 if (P->ForcePrefixMatch[3])
 if (P->ForcePrefixValue[3] != I->MD->MagicPrefixMag[3]) continue;
 }
 if (P->SuffixMatch)
 {
 if (P->Suffix != I->MD->MagicSuffix) continue;
 if (P->ForceSuffixMatch[0])
 if (P->ForceSuffixValue[0] != I->MD->MagicSuffixMag[0]) continue;
 }
 break;
 }
 MessageBeep(MB_ICONASTERISK);
 I->Busy = false;
 EndDialog(P->Dialog,0);
 UpdateTab2();
 CloseHandle(P->Thread);
 GlobalFree(P->ThreadData);
 return 0;
}
*/
static DWORD WINAPI BruteForceAttack(LPVOID ThreadParameters)
{
	MagicSearchThread* P = (MagicSearchThread*)ThreadParameters;
	P->Advanced = true;
	P->Dialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2S), hMainDialog, (DLGPROC)Tab2SearchDialogProc, (LPARAM) P);
	if (!P->Dialog) return -1;
	Item* I = P->Item;
	I->Busy = true;
	int MagicLevel = 0;
	if (P->PrefixMatch && P->Prefix)
		MagicLevel = max(MagicLevel, P->Prefix->ModLevel);
	if (P->SuffixMatch && P->Suffix)
		MagicLevel = max(MagicLevel, P->Suffix->ModLevel);
	I->SetMagicLevel(MagicLevel - 2);
	bool QuitMessage = false;
	DWORD SDWB = I->DWB();
	int MG = MagicLevel;
	for (MG == MagicLevel || (P->TraverseMagicLevels && MG <= MAXMODLEVEL); MG++;)
	{
		I->SetMagicLevel(MG - 2);
		DWORD RDWB = SDWB;
		P->Counter = 0;

		while (!(QuitMessage = CheckPollMessages()))
		{
			if (!P->Running)
				continue;
			I->SetDWB(RDWB++);

			P->Counter++;
			if (P->Counter == 0)
				break;
			if (!I->MD->QuickDecode()) continue;

			if (P->PrefixMatch)
			{
				if (P->Prefix != I->MD->MagicPrefix) continue;

				if (P->ForcePrefixMatch[0])
					if (P->ForcePrefixValue[0] != I->MD->MagicPrefixMag[0]) continue;
				if (P->ForcePrefixMatch[1])
					if (P->ForcePrefixValue[1] != I->MD->MagicPrefixMag[1]) continue;
				if (P->ForcePrefixMatch[2])
					if (P->ForcePrefixValue[2] != I->MD->MagicPrefixMag[2]) continue;
				if (P->ForcePrefixMatch[3])
					if (P->ForcePrefixValue[3] != I->MD->MagicPrefixMag[3]) continue;
			}
			if (P->SuffixMatch)
			{
				if (P->Suffix != I->MD->MagicSuffix) continue;

				if (P->ForceSuffixMatch[0])
					if (P->ForceSuffixValue[0] != I->MD->MagicSuffixMag[0]) continue;
			}

			if (P->HitsSelection) {

				SearchHit* H = new SearchHit;
				H->MagicLevel = I->MagicLevel();
				H->DWA = I->DWA();
				H->DWB = I->DWB();
				H->List = 0;

				if (P->Hits) {

					SearchHit* S = P->Hits;
					while (S->List)
						S = S->List;
					S->List = H;
				}
				else
					P->Hits = H;

				PostMessage(P->Dialog, WM_USER, 0, 0);
				MessageBeep(MB_ICONASTERISK);
			}
			else
				break;
		}

		if (QuitMessage) break;
	}
	I->Busy = false;
	I->Decoded = false;
	EndDialog(P->Dialog, IDOK);
	if (MG >= MAXMODLEVEL || P->TraverseMagicLevels && !QuitMessage)
		MessageBox(NULL, "Searched all combinations in the scope of this item. Your desired combination is not availableon the item.Select similar attributes and try again.",PROGRAMNAME,MB_OK | MB_ICONASTERISK);
	else
			MessageBeep(MB_ICONASTERISK);
	UpdateTab2();
	InvalidateRect(hTabDialog, NULL, FALSE);
	CloseHandle(P->Thread);
	for (SearchHit* H = P->Hits; H != 0;) {
		SearchHit* T = H->List;
		delete H;
		H = T;
	}
	GlobalFree(P->ThreadData);
	return 0;
}
static bool RestrictELevel;
static int RestrictELevelValue;
inline void LoadPreSuffixTree(HWND hWnd)
{
	HWND hTV = GetDlgItem(hWnd, IDC_TAB2Magic_PrefixTree);
	TreeView_DeleteAllItems(hTV);
	TV_ITEM TVItem;
	TV_INSERTSTRUCT TVInsert;
	HTREEITEM hBranch;
	// None Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "None";
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = TVI_ROOT;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTV, &TVInsert);
	}

	for (int z = 0; z < nMagicPreSuffixTree; z++)
	{
		MagicPreSuffixTree[z].hTree = 0;
		if (MagicPreSuffixTree[z].Depth > 1) continue;
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = MagicPreSuffixTree[z].Text;
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = TVI_ROOT;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		for (z++; z < nMagicPreSuffixTree; z++)
		{
			if (MagicPreSuffixTree[z].Depth < 2)
			{
				z--;
				break;
			}
			// Load only Prefixes
			if (MagicPreSuffixTree[z].ModID & 256) continue;
			MagicPreSuffixTree[z].hTree = 0;
			// Check if attribute is applyable
			if ((MagicPrefixTable[MagicPreSuffixTree[z].ModID].MagicMask & SelItem->Info->MagicMask) == 0) continue;
			// Check if ELevel exceeded
			if (RestrictELevel && RestrictELevelValue < MagicPrefixTable[MagicPreSuffixTree[z].ModID].ELevel) continue;
			if (TVInsert.hParent == TVI_ROOT)
			{
				hBranch = MagicPreSuffixTree[z].hTree =
					TreeView_InsertItem(hTV, &TVInsert);
			}

			ZeroMemory(&TVItem, sizeof TVItem);
			TVItem.mask = TVIF_TEXT | TVIF_PARAM;
			TVItem.pszText = MagicPreSuffixTree[z].Text;
			TVItem.lParam = MagicPreSuffixTree[z].ModID;

			TVInsert.hParent = hBranch;
			TVInsert.hInsertAfter = TVI_LAST;
			TVInsert.item = TVItem;

			MagicPreSuffixTree[z].hTree = TreeView_InsertItem(hTV, &TVInsert);
		}
	}
}
inline void LoadSuffixTree(HWND hWnd)
{
	HWND hTV = GetDlgItem(hWnd, IDC_TAB2Magic_SuffixTree);
	TreeView_DeleteAllItems(hTV);
	TV_ITEM TVItem;
	TV_INSERTSTRUCT TVInsert;
	HTREEITEM hBranch;
	// None Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "None";
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = TVI_ROOT;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTV, &TVInsert);
	}

	for (int z = 0; z < nMagicPreSuffixTree; z++)
	{
		MagicPreSuffixTree[z].hTree = 0;
		if (MagicPreSuffixTree[z].Depth > 1) continue;
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = MagicPreSuffixTree[z].Text;
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = TVI_ROOT;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		for (z++; z < nMagicPreSuffixTree; z++)
		{
			if (MagicPreSuffixTree[z].Depth < 2)
			{
				z--;
				break;
			}
			// Load only Suffixes
			if (!(MagicPreSuffixTree[z].ModID & 256)) continue;
			MagicPreSuffixTree[z].hTree = 0;
			// Check if attributes is applyable
			if ((MagicSuffixTable[MagicPreSuffixTree[z].ModID & 0xFF].MagicMask & SelItem->Info->MagicMask) == 0)
				continue;
			// Check if ELevel exceeded
			if (RestrictELevel && RestrictELevelValue < MagicSuffixTable[MagicPreSuffixTree[z].ModID & 0xFF].ELevel) continue;
			if (TVInsert.hParent == TVI_ROOT)
			{
				hBranch = MagicPreSuffixTree[z].hTree =
					TreeView_InsertItem(hTV, &TVInsert);
			}

			ZeroMemory(&TVItem, sizeof TVItem);
			TVItem.mask = TVIF_TEXT | TVIF_PARAM;
			TVItem.pszText = MagicPreSuffixTree[z].Text;
			TVItem.lParam = MagicPreSuffixTree[z].ModID;

			TVInsert.hParent = hBranch;
			TVInsert.hInsertAfter = TVI_LAST;
			TVInsert.item = TVItem;

			MagicPreSuffixTree[z].hTree = TreeView_InsertItem(hTV, &TVInsert);
		}
	}
}
inline void EnablePrefixControls(HWND hWnd, int enable)
{
	EnableWindow(GetDlgItem(hWnd, IDC_TAB2Magic_PrefixTree), enable);
}
inline void EnableSuffixControls(HWND hWnd, int enable)
{
	EnableWindow(GetDlgItem(hWnd, IDC_TAB2Magic_SuffixTree), enable);
}
struct
{
	int MatchBox;
	int Slider;
	int Min;
	int Max;
	bool ForceMatch;
	int ForceValue;
}
PrefixControls[] =
{
{ IDC_TAB2Magic_Prefix1Match, IDC_TAB2Magic_Prefix1Value, IDC_TAB2Magic_Prefix1ValueMin, IDC_TAB2Magic_Prefix1ValueMax }
,
{ IDC_TAB2Magic_Prefix2Match, IDC_TAB2Magic_Prefix2Value, IDC_TAB2Magic_Prefix2ValueMin, IDC_TAB2Magic_Prefix2ValueMax }
,
{ IDC_TAB2Magic_Prefix3Match, IDC_TAB2Magic_Prefix3Value, IDC_TAB2Magic_Prefix3ValueMin, IDC_TAB2Magic_Prefix3ValueMax }
,
{ IDC_TAB2Magic_Prefix4Match, IDC_TAB2Magic_Prefix4Value, IDC_TAB2Magic_Prefix4ValueMin, IDC_TAB2Magic_Prefix4ValueMax }
,
};
struct
{
	int MatchBox;
	int Slider;
	int Min;
	int Max;
	bool ForceMatch;
	int ForceValue;
}
SuffixControls[] =
{
{ IDC_TAB2Magic_Suffix1Match, IDC_TAB2Magic_Suffix1Value, IDC_TAB2Magic_Suffix1ValueMin, IDC_TAB2Magic_Suffix1ValueMax }
,
};
void EnablePrefixControls(HWND hWnd, int Controls, int enable)
{
	if (Controls < 0)
	{
		for (int z = 0; z < 4; z++)
		{
			EnableWindow(GetDlgItem(hWnd, PrefixControls[z].Slider), enable);
			EnableWindow(GetDlgItem(hWnd, PrefixControls[z].Min), enable);
			EnableWindow(GetDlgItem(hWnd, PrefixControls[z].Max), enable);
		}
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, PrefixControls[Controls].Slider), enable);
		EnableWindow(GetDlgItem(hWnd, PrefixControls[Controls].Min), enable);
		EnableWindow(GetDlgItem(hWnd, PrefixControls[Controls].Max), enable);
	}

}
void EnableSuffixControls(HWND hWnd, int Controls, int enable)
{
	if (Controls < 0)
	{
		for (int z = 0; z < 4; z++)
		{
			EnableWindow(GetDlgItem(hWnd, SuffixControls[z].Slider), enable);
			EnableWindow(GetDlgItem(hWnd, SuffixControls[z].Min), enable);
			EnableWindow(GetDlgItem(hWnd, SuffixControls[z].Max), enable);
		}
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd, SuffixControls[Controls].Slider), enable);
		EnableWindow(GetDlgItem(hWnd, SuffixControls[Controls].Min), enable);
		EnableWindow(GetDlgItem(hWnd, SuffixControls[Controls].Max), enable);
	}

}
static bool PrefixMatch = false;
static int PrefixTree;
static _MagicPreSuffix* Prefix;
static bool SuffixMatch = false;
static int SuffixTree;
static _MagicPreSuffix* Suffix;
static int OrigModLevel;
static DWORD SearchAverage()
{
	int MagicLevel = 0;
	if (PrefixMatch && Prefix)
		MagicLevel = max(MagicLevel, Prefix->ModLevel);
	if (SuffixMatch && Suffix)
		MagicLevel = max(MagicLevel, Suffix->ModLevel);

	SelItem->SetMagicLevel(MagicLevel - 2);

	SelItem->MD->BuildMagicBuffers();

	DWORDLONG Average = 1;

	if (PrefixMatch)
	{
		Average *= 2 * SelItem->MD->nPrefixBuffer;

		if (Prefix)
			for (int z = 0; z < 4; z++)
			{
				if (!PrefixControls[z].ForceMatch) continue;
				if (Prefix->Mod[z].Code == 0) continue;

				Average *= Prefix->Mod[z].Max - Prefix->Mod[z].Min;
			}
	}

	if (SuffixMatch)
	{
		Average *= 2 * SelItem->MD->nSuffixBuffer;

		if (Suffix)
			for (int z = 0; z < 1; z++)
			{
				if (!SuffixControls[z].ForceMatch) continue;
				if (Suffix->Mod[z].Code == 0) continue;

				Average *= Suffix->Mod[z].Max - Suffix->Mod[z].Min;
			}
	}
	return DWORD(Average / 2 + 1);
}
static void UpdateTab2Magic(HWND hWnd)
{
	for (int n = 0; n < 4; n++)
	{
		char* nth = n == 0 ? "1st" :
			n == 1 ? "2nd" :
			n == 2 ? "3rd" : "4th";
		if (!PrefixMatch || !Prefix || !Prefix->Mod[n].Code)
		{
			EnableWindow(GetDlgItem(hWnd, PrefixControls[n].MatchBox), FALSE);
			CheckDlgButton(hWnd, PrefixControls[n].MatchBox, BST_UNCHECKED);
			EnablePrefixControls(hWnd, n, FALSE);

			sprintf(buffer, "No %s Prefix Effect", nth);
			SetDlgItemText(hWnd, PrefixControls[n].MatchBox, buffer);

			SendDlgItemMessage(hWnd, PrefixControls[n].Slider, TBM_SETPOS, TRUE, 0);
			SetDlgItemText(hWnd, PrefixControls[n].Min, "");
			SetDlgItemText(hWnd, PrefixControls[n].Max, "");
			PrefixControls[n].ForceMatch = 0;
			PrefixControls[n].ForceValue = 0;
		}
		else if (Prefix->Mod[n].Max - Prefix->Mod[n].Min <= 0)
		{
			EnableWindow(GetDlgItem(hWnd, PrefixControls[n].MatchBox), FALSE);
			CheckDlgButton(hWnd, PrefixControls[n].MatchBox, BST_UNCHECKED);
			EnablePrefixControls(hWnd, n, FALSE);

			sprintf(buffer, "%s Prefix Effect fixed to %i", nth, Prefix->Mod[n].Min);
			SetDlgItemText(hWnd, PrefixControls[n].MatchBox, buffer);

			SendDlgItemMessage(hWnd, PrefixControls[n].Slider, TBM_SETPOS, TRUE, 0);
			SetDlgItemInt(hWnd, PrefixControls[n].Min, Prefix->Mod[n].Min, 0);
			SetDlgItemInt(hWnd, PrefixControls[n].Max, Prefix->Mod[n].Max - 1, 0);
			PrefixControls[n].ForceMatch = 0;
			PrefixControls[n].ForceValue = 0;
		}
		else
		{
			EnableWindow(GetDlgItem(hWnd, PrefixControls[n].MatchBox), TRUE);

			if (PrefixControls[n].ForceMatch)
			{
				EnablePrefixControls(hWnd, n, TRUE);

				sprintf(buffer, "Force Value of %s Prefix Effect to %i", nth, Prefix->Mod[n].Min + PrefixControls[n].ForceValue);
				SetDlgItemText(hWnd, PrefixControls[n].MatchBox, buffer);

				SetDlgItemInt(hWnd, PrefixControls[n].Min, Prefix->Mod[n].Min, 0);
				SetDlgItemInt(hWnd, PrefixControls[n].Max, Prefix->Mod[n].Max - 1, 0);

				SendDlgItemMessage(hWnd, PrefixControls[n].Slider, TBM_SETRANGE, TRUE,
					MAKELONG(0, Prefix->Mod[n].Max - Prefix->Mod[n].Min - 1));
				SendDlgItemMessage(hWnd, PrefixControls[n].Slider, TBM_SETPOS, TRUE, PrefixControls[n].ForceValue);
			}
			else
			{
				EnablePrefixControls(hWnd, n, FALSE);

				sprintf(buffer, "Force Value of %s Prefix Effect", nth);
				SetDlgItemText(hWnd, PrefixControls[n].MatchBox, buffer);
				SendDlgItemMessage(hWnd, PrefixControls[n].Slider, TBM_SETPOS, TRUE, 0);
				SetDlgItemInt(hWnd, PrefixControls[n].Min, Prefix->Mod[n].Min, 0);
				SetDlgItemInt(hWnd, PrefixControls[n].Max, Prefix->Mod[n].Max - 1, 0);
			}
		}
	}
	for (int n = 0; n < 1; n++)
	{
		char* nth = n == 0 ? "1st" :
			n == 1 ? "2nd" :
			n == 2 ? "3rd" : "4th";
		if (!SuffixMatch || !Suffix || !Suffix->Mod[n].Code)
		{
			EnableWindow(GetDlgItem(hWnd, SuffixControls[n].MatchBox), FALSE);
			CheckDlgButton(hWnd, SuffixControls[n].MatchBox, BST_UNCHECKED);
			EnableSuffixControls(hWnd, n, FALSE);

			sprintf(buffer, "No %s Suffix Effect", nth);
			SetDlgItemText(hWnd, SuffixControls[n].MatchBox, buffer);

			SendDlgItemMessage(hWnd, SuffixControls[n].Slider, TBM_SETPOS, TRUE, 0);
			SetDlgItemText(hWnd, SuffixControls[n].Min, "");
			SetDlgItemText(hWnd, SuffixControls[n].Max, "");
			SuffixControls[n].ForceMatch = 0;
			SuffixControls[n].ForceValue = 0;
		}
		else if (Suffix->Mod[n].Max - Suffix->Mod[n].Min <= 0)
		{
			EnableWindow(GetDlgItem(hWnd, SuffixControls[n].MatchBox), FALSE);
			CheckDlgButton(hWnd, SuffixControls[n].MatchBox, BST_UNCHECKED);
			EnableSuffixControls(hWnd, n, FALSE);

			sprintf(buffer, "%s Suffix Effect fixed to %i", nth, Suffix->Mod[n].Min);
			SetDlgItemText(hWnd, SuffixControls[n].MatchBox, buffer);

			SendDlgItemMessage(hWnd, SuffixControls[n].Slider, TBM_SETPOS, TRUE, 0);
			SetDlgItemInt(hWnd, SuffixControls[n].Min, Suffix->Mod[n].Min, 0);
			SetDlgItemInt(hWnd, SuffixControls[n].Max, Suffix->Mod[n].Max - 1, 0);
			SuffixControls[n].ForceMatch = 0;
			SuffixControls[n].ForceValue = 0;
		}
		else
		{
			EnableWindow(GetDlgItem(hWnd, SuffixControls[n].MatchBox), TRUE);

			if (SuffixControls[n].ForceMatch)
			{
				EnableSuffixControls(hWnd, n, TRUE);

				sprintf(buffer, "Force Value of %s Suffix Effect to %i", nth, Suffix->Mod[n].Min + SuffixControls[n].ForceValue);
				SetDlgItemText(hWnd, SuffixControls[n].MatchBox, buffer);

				SetDlgItemInt(hWnd, SuffixControls[n].Min, Suffix->Mod[n].Min, 0);
				SetDlgItemInt(hWnd, SuffixControls[n].Max, Suffix->Mod[n].Max - 1, 0);

				SendDlgItemMessage(hWnd, SuffixControls[n].Slider, TBM_SETRANGE, TRUE,
					MAKELONG(0, Suffix->Mod[n].Max - Suffix->Mod[n].Min - 1));
				SendDlgItemMessage(hWnd, SuffixControls[n].Slider, TBM_SETPOS, TRUE, SuffixControls[n].ForceValue);
			}
			else
			{
				EnableSuffixControls(hWnd, n, FALSE);

				sprintf(buffer, "Force Value of %s Suffix Effect", nth);
				SetDlgItemText(hWnd, SuffixControls[n].MatchBox, buffer);
				SendDlgItemMessage(hWnd, SuffixControls[n].Slider, TBM_SETPOS, TRUE, 0);
				SetDlgItemInt(hWnd, SuffixControls[n].Min, Suffix->Mod[n].Min, 0);
				SetDlgItemInt(hWnd, SuffixControls[n].Max, Suffix->Mod[n].Max - 1, 0);
			}
		}
	}
	{
		DWORDLONG Average = SearchAverage();
		if (Average < 0)
			SetDlgItemText(hWnd, IDC_TAB2Magic_Average, "Combination can possibly never be found!");
		else
		{
			sprintf(buffer, "Average Tries: %lu", Average);
			SetDlgItemText(hWnd, IDC_TAB2Magic_Average, buffer);
		}
	}
	{ // Update ELevel Requirements
		int ELevel = 0;
		if (Prefix) {
			if (Prefix->ELevel > ELevel)
				ELevel = Prefix->ELevel;
		}
		if (Suffix) {
			if (Suffix->ELevel > ELevel)
				ELevel = Suffix->ELevel;
		}
		SetDlgItemInt(hWnd, IDC_TAB2Magic_CurrentELevel, ELevel, FALSE);
	} // Update ELevel Requirements
}
void UpdateTab2MagicTrees(HWND hWnd)
{
	LoadPreSuffixTree(hWnd);
	LoadSuffixTree(hWnd);
	if (RestrictELevel)
	{
		CheckDlgButton(hWnd, IDC_TAB2Magic_LockELevel, TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB2Magic_LockValue), TRUE);
		SetDlgItemInt(hWnd, IDC_TAB2Magic_LockValue, RestrictELevelValue, FALSE);
	}
	else {
		CheckDlgButton(hWnd, IDC_TAB2Magic_LockELevel, FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB2Magic_LockValue), FALSE);
	}
}
LRESULT CALLBACK Tab2MagicDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_CHELP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconHelp);
		RestrictELevel = true;
		RestrictELevelValue = fc.gf.level;
		UpdateTab2MagicTrees(hWnd);
		PrefixMatch = false;
		PrefixControls[0].ForceMatch = false;
		PrefixControls[1].ForceMatch = false;
		PrefixControls[2].ForceMatch = false;
		PrefixControls[3].ForceMatch = false;
		EnablePrefixControls(hWnd, FALSE);
		SuffixMatch = false;
		SuffixControls[0].ForceMatch = false;
		EnableSuffixControls(hWnd, FALSE);
		OrigModLevel = SelItem->MagicLevel();
		UpdateTab2Magic(hWnd);
	}
	return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			// Check Combination
			{
				if (PrefixMatch && SuffixMatch)
				{
					if (!Prefix && !Suffix)
					{
						MessageBox(hWnd, "You must select either a Prefix or a Suffix.", PROGRAMNAME, MB_OK | MB_ICONSTOP);
						break;
					}
				}
			}
			SelItem->DWBHistory.StepAdd(SelItem);
			HGLOBAL TD = GlobalAlloc(GMEM_ZEROINIT, sizeof MagicSearchThread);
			struct MagicSearchThread* NewThread = (MagicSearchThread*)GlobalLock(TD);;
			NewThread->ThreadData = TD;
			NewThread->Item = SelItem;
			NewThread->PrefixMatch = PrefixMatch;
			NewThread->Prefix = Prefix;
			NewThread->SuffixMatch = SuffixMatch;
			NewThread->Suffix = Suffix;
			NewThread->ForcePrefixMatch[0] = PrefixControls[0].ForceMatch;
			NewThread->ForcePrefixMatch[1] = PrefixControls[1].ForceMatch;
			NewThread->ForcePrefixMatch[2] = PrefixControls[2].ForceMatch;
			NewThread->ForcePrefixMatch[3] = PrefixControls[3].ForceMatch;
			NewThread->ForcePrefixValue[0] = PrefixControls[0].ForceValue;
			NewThread->ForcePrefixValue[1] = PrefixControls[1].ForceValue;
			NewThread->ForcePrefixValue[2] = PrefixControls[2].ForceValue;
			NewThread->ForcePrefixValue[3] = PrefixControls[3].ForceValue;
			NewThread->ForceSuffixMatch[0] = SuffixControls[0].ForceMatch;
			NewThread->ForceSuffixValue[0] = SuffixControls[0].ForceValue;
			DWORD ThreadID;
			NewThread->Thread = CreateThread(NULL, 0, &BruteForceAttack, NewThread, 0, &ThreadID);
			EndDialog(hWnd, IDOK);
		}
		break;
		case IDCANCEL:
			SelItem->SetMagicLevel(OrigModLevel);
			EndDialog(hWnd, IDCANCEL);
			break;
		case IDC_TAB2Magic_PrefixMatch:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hWnd, IDC_TAB2Magic_PrefixMatch) == BST_CHECKED)
				{
					EnablePrefixControls(hWnd, TRUE);
					PrefixMatch = true;
				}
				else
				{
					EnablePrefixControls(hWnd, FALSE);
					PrefixMatch = false;
				}
				UpdateTab2Magic(hWnd);
			}
			break;
		case IDC_TAB2Magic_SuffixMatch:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hWnd, IDC_TAB2Magic_SuffixMatch) == BST_CHECKED)
				{
					EnableSuffixControls(hWnd, TRUE);
					SuffixMatch = true;
				}
				else
				{
					EnableSuffixControls(hWnd, FALSE);
					SuffixMatch = false;
				}
				UpdateTab2Magic(hWnd);
			}
			break;
		case IDC_TAB2Magic_Prefix1Match:
		case IDC_TAB2Magic_Prefix2Match:
		case IDC_TAB2Magic_Prefix3Match:
		case IDC_TAB2Magic_Prefix4Match:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int n = LOWORD(wParam) - IDC_TAB2Magic_Prefix1Match;
				if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == BST_CHECKED)
					PrefixControls[n].ForceMatch = true;
				else
					PrefixControls[n].ForceMatch = false;
				PrefixControls[n].ForceValue = 0;
				UpdateTab2Magic(hWnd);
			}
			break;
		case IDC_TAB2Magic_Suffix1Match:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == BST_CHECKED)
					SuffixControls[0].ForceMatch = true;
				else
					SuffixControls[0].ForceMatch = false;
				SuffixControls[0].ForceValue = 0;
				UpdateTab2Magic(hWnd);
			}
			break;
		case IDC_TAB2Magic_LockELevel:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == BST_UNCHECKED)
					RestrictELevel = true;
				else
					RestrictELevel = false;
				UpdateTab2MagicTrees(hWnd);
			}
			break;
		case IDC_TAB2Magic_LockValue:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				if (IsDlgButtonChecked(hWnd, IDC_TAB2Magic_LockELevel) == BST_CHECKED) {
					int x = GetDlgItemInt(hWnd, IDC_TAB2Magic_LockValue, NULL, FALSE);
					if (x != RestrictELevelValue) {
						RestrictELevelValue = x;
						UpdateTab2MagicTrees(hWnd);
					}
				}
			}
			break;
		case IDC_CHELP:
			ToggleHelpBox(hWnd, IDH_TAB2Magic);
			break;
		case IDC_TAB2Magic_Clear:
		{
			PrefixMatch = false;
			PrefixControls[0].ForceMatch = false;
			PrefixControls[1].ForceMatch = false;
			PrefixControls[2].ForceMatch = false;
			PrefixControls[3].ForceMatch = false;
			EnablePrefixControls(hWnd, FALSE);

			SuffixMatch = false;
			SuffixControls[0].ForceMatch = false;
			EnableSuffixControls(hWnd, FALSE);
			CheckDlgButton(hWnd, IDC_TAB2Magic_PrefixMatch, PrefixMatch);
			CheckDlgButton(hWnd, IDC_TAB2Magic_SuffixMatch, SuffixMatch);

			UpdateTab2Magic(hWnd);
		}
		break;
		}
	}
	return false;
	case WM_NOTIFY:
		switch (((NMHDR*)lParam)->idFrom)
		{
		case IDC_TAB2Magic_PrefixTree:
			switch (((NMHDR*)lParam)->code)
			{
			case TVN_SELCHANGED:
			{
				NM_TREEVIEW* NMTreeView = (NM_TREEVIEW*)lParam;
				PrefixTree = NMTreeView->itemNew.lParam;
				if (PrefixTree >= 0)
				{
					Prefix = &MagicPrefixTable[PrefixTree];
					PrefixControls[0].ForceValue = 0;
					PrefixControls[1].ForceValue = 0;
					PrefixControls[2].ForceValue = 0;
					PrefixControls[3].ForceValue = 0;
				}
				else
					Prefix = 0;
				UpdateTab2Magic(hWnd);
			}
			break;
			}
			break;
		case IDC_TAB2Magic_SuffixTree:
			switch (((NMHDR*)lParam)->code)
			{
			case TVN_SELCHANGED:
			{
				NM_TREEVIEW* NMTreeView = (NM_TREEVIEW*)lParam;
				SuffixTree = NMTreeView->itemNew.lParam;
				if (SuffixTree >= 0)
				{
					Suffix = &MagicSuffixTable[SuffixTree & 0xFF];
					SuffixControls[0].ForceValue = 0;
				}
				else
					Suffix = 0;
				UpdateTab2Magic(hWnd);
			}
			break;
			}
			break;
		}
		return false;
	case WM_HSCROLL:
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2Magic_Prefix1Value))
		{
			PrefixControls[0].ForceValue = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			UpdateTab2Magic(hWnd);
		}
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2Magic_Prefix2Value))
		{
			PrefixControls[1].ForceValue = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			UpdateTab2Magic(hWnd);
		}
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2Magic_Prefix3Value))
		{
			PrefixControls[2].ForceValue = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			UpdateTab2Magic(hWnd);
		}
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2Magic_Prefix4Value))
		{
			PrefixControls[3].ForceValue = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			UpdateTab2Magic(hWnd);
		}
		if ((HWND)lParam == GetDlgItem(hWnd, IDC_TAB2Magic_Suffix1Value))
		{
			SuffixControls[0].ForceValue = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			UpdateTab2Magic(hWnd);
		}
		break;
	case WM_CLOSE:
	{
		SelItem->SetMagicLevel(OrigModLevel);
		EndDialog(hWnd, IDCANCEL);
	}
	return false;
	case WM_DESTROY:
	{
		CloseHelpBox();
	}
	return false;
	}
	return false;
}