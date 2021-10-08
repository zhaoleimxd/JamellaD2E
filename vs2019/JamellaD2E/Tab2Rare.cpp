// Tab2Rare.cpp from D2E
#include "JamellaD2E.h"
HCURSOR hCurAdd;
static HWND hTVP, hTVS, hList;
static bool userselecting = false;
static bool RestrictELevel;
static int RestrictELevelValue;
#define ZEROATTR -4
#define RANDOMATTR -5
static DWORD WINAPI BruteForceAttack(LPVOID ThreadParameters)
{
	RareSearchThread* P = (RareSearchThread*)ThreadParameters;
	P->Advanced = true;
	P->Dialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2S), hMainDialog, (DLGPROC)Tab2SearchDialogProc, (LPARAM) P);
	if (!P->Dialog) return -1;
	Item* I = P->Item;
	I->Busy = true;
	int MagicLevel = 0;
	for (int z = 0; z < 6; z++)
	{
		if (!P->Attribute[z]) continue;
		MagicLevel = max(MagicLevel, P->Attribute[z]->ModLevel);
	}
	int nModMin = 0, nModMax = 0;
	// Precalculate Modifier Number Range
	{
		for (int z = 0; z < 6; z++)
		{
			if (P->iAttribute[z] == RANDOMATTR) {
				nModMax++;
			}
			if (!P->Attribute[z]) continue;
			nModMin++;
			nModMax++;
		}
	}
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

			if (P->NamePrefix >= 0)
				if (I->MD->RarePrefix != &RarePrefixTable[P->NamePrefix]) continue;
			if (P->NameSuffix >= 0)
				if (I->MD->RareSuffix != &RareSuffixTable[P->NameSuffix]) continue;

			if (nModMin > I->MD->nRareFix) continue;
			if (nModMax < I->MD->nRareFix) continue;

			int z;
			for (z = 0; z < 6; z++)
			{
				if (!P->Attribute[z]) continue;

				int m;
				for (m = 0; m < 6; m++)
				{
					if (!I->MD->RareFix[m]) continue;
					if (I->MD->RareFix[m] == P->Attribute[z])
						break;
				}
				if (m == 6) break;
			}
			if (z == 6) {
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
		}
		if (QuitMessage) break;
	}
	I->Busy = false;
	I->Decoded = false;
	EndDialog(P->Dialog, IDOK);
	if (MG >= MAXMODLEVEL || P->TraverseMagicLevels && !QuitMessage)
		MessageBox(NULL, "Searched all combinations in the scope of this item. Your desired combination is not available on the item.Select similar attributes and try again.",PROGRAMNAME,MB_OK | MB_ICONASTERISK);
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
inline void LoadNamePrefixTree(HWND hWnd)
{
	HWND hList = GetDlgItem(hWnd, IDC_TAB2Rare_NamePrefix);
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (int z = 0; z < nRarePrefixTable; z++)
	{
		if (RarePrefixTable[z].Text == 0) continue;
		if ((RarePrefixTable[z].RareMask & SelItem->Info->RareMask) == 0) continue;
		int index = SendMessage(hList, LB_ADDSTRING, 0, (DWORD)RarePrefixTable[z].Text);
		SendMessage(hList, LB_SETITEMDATA, index, (DWORD)z);
	}
	SendMessage(hList, LB_INSERTSTRING, 0, (DWORD)"unspecified");
	SendMessage(hList, LB_SETITEMDATA, 0, (DWORD)-1);
	SendMessage(hList, LB_SETSEL, TRUE, 0);
}
inline void LoadNameSuffixTree(HWND hWnd)
{
	HWND hList = GetDlgItem(hWnd, IDC_TAB2Rare_NameSuffix);
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	for (int z = 0; z < nRareSuffixTable; z++)
	{
		if (RareSuffixTable[z].Text == 0) continue;
		if ((RareSuffixTable[z].RareMask & SelItem->Info->RareMask) == 0) continue;
		int index = SendMessage(hList, LB_ADDSTRING, 0, (DWORD)RareSuffixTable[z].Text);
		SendMessage(hList, LB_SETITEMDATA, index, (DWORD)z);
	}
	SendMessage(hList, LB_INSERTSTRING, 0, (DWORD)"unspecified");
	SendMessage(hList, LB_SETITEMDATA, 0, (DWORD)-1);
	SendMessage(hList, LB_SETSEL, TRUE, 0);
}
inline void LoadAttributesTree(HWND hWnd)
{
	hTVP = GetDlgItem(hWnd, IDC_TAB2Rare_TreePrefix);
	hTVS = GetDlgItem(hWnd, IDC_TAB2Rare_TreeSuffix);
	TreeView_DeleteAllItems(hTVP);
	TreeView_DeleteAllItems(hTVS);
	TV_ITEM TVItem;
	TV_INSERTSTRUCT TVInsert;
	HTREEITEM hBranch;
	HTREEITEM hPrefix;
	// Prefix Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		TVItem.pszText = "Prefix";
		TVItem.lParam = (LPARAM)-1;
		TVItem.state = TVIS_EXPANDED;
		TVItem.stateMask = TVIS_EXPANDED;
		TVInsert.hParent = 0;
		TVInsert.hInsertAfter = TVI_ROOT;
		TVInsert.item = TVItem;
		hPrefix = TreeView_InsertItem(hTVP, &TVInsert);
	}
	// None Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "Zero Attribute";
		TVItem.lParam = (LPARAM)ZEROATTR;
		TVInsert.hParent = hPrefix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTVP, &TVInsert);
	}
	// Random Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "Random Attribute";
		TVItem.lParam = (LPARAM)RANDOMATTR;
		TVInsert.hParent = hPrefix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTVP, &TVInsert);
	}
	for (int z = 0; z < nMagicPreSuffixTree; z++)
		MagicPreSuffixTree[z].hTree = 0;
	for (int z = 0; z < nMagicPreSuffixTree; z++)
	{
		if (MagicPreSuffixTree[z].Depth > 1) continue;
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = MagicPreSuffixTree[z].Text;
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = hPrefix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		for (z++; z < nMagicPreSuffixTree; z++)
		{
			if (MagicPreSuffixTree[z].Depth < 2)
			{
				z--;
				break;
			}
			if (MagicPreSuffixTree[z].ModID & 256) continue;
			if ((MagicPrefixTable[MagicPreSuffixTree[z].ModID & 0xFF].MagicMask & SelItem->Info->MagicMask) == 0) continue;
			// Check if ELevel exceeded
			if (RestrictELevel && RestrictELevelValue < MagicPrefixTable[MagicPreSuffixTree[z].ModID].ELevel) continue;
			if (TVInsert.hParent == hPrefix)
			{
				hBranch = MagicPreSuffixTree[z].hTree =
					TreeView_InsertItem(hTVP, &TVInsert);
			}

			ZeroMemory(&TVItem, sizeof TVItem);
			TVItem.mask = TVIF_TEXT | TVIF_PARAM;
			TVItem.pszText = MagicPreSuffixTree[z].Text;
			TVItem.lParam = MagicPreSuffixTree[z].ModID;

			TVInsert.hParent = hBranch;
			TVInsert.hInsertAfter = TVI_LAST;
			TVInsert.item = TVItem;

			MagicPreSuffixTree[z].hTree = TreeView_InsertItem(hTVP, &TVInsert);
		}
	}
	HTREEITEM hSuffix;
	// Suffix Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		TVItem.pszText = "Suffix";
		TVItem.lParam = (LPARAM)-1;
		TVItem.state = TVIS_EXPANDED;
		TVItem.stateMask = TVIS_EXPANDED;
		TVInsert.hParent = 0;
		TVInsert.hInsertAfter = TVI_ROOT;
		TVInsert.item = TVItem;
		hSuffix = TreeView_InsertItem(hTVS, &TVInsert);
	}
	// None Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "Zero Attribute";
		TVItem.lParam = (LPARAM)ZEROATTR;
		TVInsert.hParent = hSuffix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTVS, &TVInsert);
	}
	// Random Entry
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = "Random Attribute";
		TVItem.lParam = (LPARAM)RANDOMATTR;
		TVInsert.hParent = hSuffix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		TreeView_InsertItem(hTVS, &TVInsert);
	}
	for (int z = 0; z < nMagicPreSuffixTree; z++)
	{
		if (MagicPreSuffixTree[z].Depth > 1) continue;
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = MagicPreSuffixTree[z].Text;
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = hSuffix;
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		for (z++; z < nMagicPreSuffixTree; z++)
		{
			if (MagicPreSuffixTree[z].Depth < 2)
			{
				z--;
				break;
			}
			if (!(MagicPreSuffixTree[z].ModID & 256)) continue;
			if ((MagicSuffixTable[MagicPreSuffixTree[z].ModID & 0xFF].MagicMask & SelItem->Info->MagicMask) == 0) continue;
			// Check if ELevel exceeded
			if (RestrictELevel && RestrictELevelValue < MagicSuffixTable[MagicPreSuffixTree[z].ModID & 0xFF].ELevel) continue;

			if (TVInsert.hParent == hSuffix)
			{
				hBranch = MagicPreSuffixTree[z].hTree =
					TreeView_InsertItem(hTVS, &TVInsert);
			}

			ZeroMemory(&TVItem, sizeof TVItem);
			TVItem.mask = TVIF_TEXT | TVIF_PARAM;
			TVItem.pszText = MagicPreSuffixTree[z].Text;
			TVItem.lParam = MagicPreSuffixTree[z].ModID;

			TVInsert.hParent = hBranch;
			TVInsert.hInsertAfter = TVI_LAST;
			TVInsert.item = TVItem;

			MagicPreSuffixTree[z].hTree = TreeView_InsertItem(hTVS, &TVInsert);
		}
	}
}
static int SaveModLevel;
static int SearchNamePrefix, SearchNameSuffix;
static struct
{
	int CheckID;
	int TextID;
	_MagicPreSuffix* Selected;
	int iSelected;
	bool tSelected;
}
Attribute[6] =
{
 { IDC_TAB2Rare_Check1, IDC_TAB2Rare_Text1 },
 { IDC_TAB2Rare_Check2, IDC_TAB2Rare_Text2 },
 { IDC_TAB2Rare_Check3, IDC_TAB2Rare_Text3 },
 { IDC_TAB2Rare_Check4, IDC_TAB2Rare_Text4 },
 { IDC_TAB2Rare_Check5, IDC_TAB2Rare_Text5 },
 { IDC_TAB2Rare_Check6, IDC_TAB2Rare_Text6 },
};
static int UserSelected;
static DWORD SearchAverage()
{
	int MagicLevel = 0;
	for (int z = 0; z < 6; z++)
	{
		if (!Attribute[z].Selected) continue;
		MagicLevel = max(MagicLevel, Attribute[z].Selected->ModLevel);
	}
	SelItem->SetMagicLevel(MagicLevel - 2);
	SelItem->MD->BuildMagicBuffers();
	SelItem->MD->BuildRareBuffers();

	DWORDLONG Average = 1;

	if (SearchNamePrefix >= 0)
		Average *= SelItem->MD->nRarePrefixBuffer;
	if (SearchNameSuffix >= 0)
		Average *= SelItem->MD->nRareSuffixBuffer;
	for (int z = 0; z < 6; z++)
	{
		if (!Attribute[z].Selected) continue;
		if (Attribute[z].tSelected == PREFIX)
			Average *= 2 * SelItem->MD->nPrefixBuffer;
		if (Attribute[z].tSelected == SUFFIX)
			Average *= 2 * SelItem->MD->nSuffixBuffer;
	}

	return DWORD(Average / 2 + 1);
}
void UpdateTab2Rare(HWND hWnd)
{
	for (int z = 0; z < 6; z++)
	{
		if (UserSelected == z)
			CheckDlgButton(hWnd, Attribute[z].CheckID, BST_CHECKED);
		else
			CheckDlgButton(hWnd, Attribute[z].CheckID, BST_UNCHECKED);
		if (Attribute[z].iSelected == ZEROATTR) {
			SetDlgItemText(hWnd, Attribute[z].CheckID, "Zero Attribute");
			SetDlgItemText(hWnd, Attribute[z].TextID, "");
		}
		else if (Attribute[z].iSelected == RANDOMATTR) {
			SetDlgItemText(hWnd, Attribute[z].CheckID, "Random Attribute");
			SetDlgItemText(hWnd, Attribute[z].TextID, "");
		}
		else if (Attribute[z].Selected) {
			SetDlgItemText(hWnd, Attribute[z].CheckID, Attribute[z].Selected->Text);
			SetDlgItemText(hWnd, Attribute[z].TextID, Attribute[z].Selected->Description);
		}
		else {
			SetDlgItemText(hWnd, Attribute[z].CheckID, "Error!!!");
			SetDlgItemText(hWnd, Attribute[z].TextID, "");
		}
	}
	// Display Average
	{
		DWORDLONG Average = SearchAverage();
		if (Average < 0)
			SetDlgItemText(hWnd, IDC_TAB2Rare_Average, "Combination can possibly never be found!");
		else
		{
			sprintf(buffer, "Average Tries: %lu", Average);
			SetDlgItemText(hWnd, IDC_TAB2Rare_Average, buffer);
		}
	}
	{ // Update ELevel Requirements

		int ELevel = 0;

		for (int z = 0; z < 6; z++)
		{
			if (Attribute[z].Selected) {
				if (Attribute[z].Selected->ELevel > ELevel)
					ELevel = Attribute[z].Selected->ELevel;
			}
		}

		SetDlgItemInt(hWnd, IDC_TAB2Rare_CurrentELevel, ELevel, FALSE);

	} // Update ELevel Requirements
	if (RestrictELevel)
	{
		CheckDlgButton(hWnd, IDC_TAB2Rare_LockELevel, TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB2Rare_LockValue), TRUE);
		SetDlgItemInt(hWnd, IDC_TAB2Rare_LockValue, RestrictELevelValue, FALSE);
	}
	else {
		CheckDlgButton(hWnd, IDC_TAB2Rare_LockELevel, FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB2Rare_LockValue), FALSE);
	}
}
static void TVMakeSelection(HWND hWnd, int n)
{
	for (int z = 0; z < nMagicPreSuffixTree; z++)
	{
		if (MagicPreSuffixTree[z].ModID == n && MagicPreSuffixTree[z].hTree) {

			userselecting = true;
			if (MagicPreSuffixTree[z].ModID & 256) {
				TreeView_SelectItem(hTVP, 0);
				TreeView_SelectItem(hTVS, MagicPreSuffixTree[z].hTree);
			}
			else {
				TreeView_SelectItem(hTVP, MagicPreSuffixTree[z].hTree);
				TreeView_SelectItem(hTVS, 0);
			}
			userselecting = false;
			break;
		}
	}
}
static bool MouseDraging = false;
static TV_ITEM MouseDragItem;
LRESULT CALLBACK Tab2RareDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_CHELP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconHelp);
		SaveModLevel = SelItem->MagicLevel();
		UserSelected = 0;
		LoadNamePrefixTree(hWnd);
		LoadNameSuffixTree(hWnd);
		LoadAttributesTree(hWnd);
		RestrictELevel = true;
		RestrictELevelValue = fc.gf.level;
		if (SelItem->MD->Decode())
		{
			SearchNamePrefix = FindRareNamePrefixByPointer(SelItem->MD->RarePrefix);
			SearchNameSuffix = FindRareNameSuffixByPointer(SelItem->MD->RareSuffix);
			if (SearchNamePrefix >= 0)
				SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_SELECTSTRING, -1, (LPARAM)RarePrefixTable[SearchNamePrefix].Text);
			if (SearchNameSuffix >= 0)
				SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_SELECTSTRING, -1, (LPARAM)RareSuffixTable[SearchNameSuffix].Text);

			for (int z = 0; z < 6; z++)
			{
				if (SelItem->MD->RareFix[z]) {
					Attribute[z].Selected = SelItem->MD->RareFix[z];
					Attribute[z].tSelected = SelItem->MD->tRareFix[z];
					Attribute[z].iSelected = FindMagicPreSuffixByPointer(Attribute[z].tSelected, Attribute[z].Selected);
				}
				else {
					Attribute[z].Selected = 0;
					Attribute[z].iSelected = ZEROATTR;
					Attribute[z].tSelected = PREFIX;
				}
			}
		}
		else {
			SearchNamePrefix = SearchNameSuffix = -1;

			for (int z = 0; z < 6; z++)
			{
				Attribute[z].Selected = 0;
				Attribute[z].iSelected = RANDOMATTR;
				Attribute[z].tSelected = PREFIX;
			}
		}
		UpdateTab2Rare(hWnd);
	}
	return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			// Get Name Pre- and Suffixes
			{
				int index = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_GETCURSEL, 0, 0);
				if (index < 0)
				{
					ErrorBox("You must select a Name Prefix!");
					return false;
				}
				SearchNamePrefix = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_GETITEMDATA, index, 0);
				index = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_GETCURSEL, 0, 0);
				if (index < 0)
				{
					ErrorBox("You must select a Name Suffix!");
					return false;
				}
				SearchNameSuffix = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_GETITEMDATA, index, 0);
			}
			// Check Prefix / Suffix Combinations
			{
				int n = 0;
				int nPrefix = 0, nSuffix = 0;
				for (int z = 0; z < 6; z++)
				{
					if (Attribute[z].iSelected == RANDOMATTR) {
						n++;
						continue;
					}
					if (!Attribute[z].Selected) continue;
					if (Attribute[z].tSelected == PREFIX) nPrefix++;
					if (Attribute[z].tSelected == SUFFIX) nSuffix++;
					n++;
				}
				if (nPrefix > 3)
				{
					ErrorBox("There can only be 3 Prefixes!");
					return false;
				}
				if (nSuffix > 3)
				{
					ErrorBox("There can only be 3 Suffixes!");
					return false;
				}
				if (n < 4)
				{
					ErrorBox("There must be at least 4 Attributes!");
					return false;
				}
				for (int z = 0; z < 6; z++)
				{
					if (!Attribute[z].Selected) continue;
					for (int y = z - 1; y >= 0; y--)
					{
						if (!Attribute[y].Selected) continue;
						if (Attribute[z].Selected->Group == Attribute[y].Selected->Group)
						{
							ErrorBox("Duplicate Attribute Group found!");
							return false;
						}
					}
				}
			}
			SelItem->DWBHistory.StepAdd(SelItem);
			HGLOBAL TD = GlobalAlloc(GMEM_ZEROINIT, sizeof RareSearchThread);
			struct RareSearchThread* NewThread = (RareSearchThread*)GlobalLock(TD);;
			NewThread->ThreadData = TD;
			NewThread->Item = SelItem;
			NewThread->NamePrefix = SearchNamePrefix;
			NewThread->NameSuffix = SearchNameSuffix;
			for (int z = 0; z < 6; z++)
			{
				NewThread->Attribute[z] = Attribute[z].Selected;
				NewThread->iAttribute[z] = Attribute[z].iSelected;
				if (NewThread->iAttribute[z] >= 0)
					NewThread->iAttribute[z] &= 256;
				NewThread->tAttribute[z] = Attribute[z].tSelected;
			}
			DWORD ThreadID;
			NewThread->Thread = CreateThread(NULL, 0, &BruteForceAttack, NewThread, 0, &ThreadID);
			EndDialog(hWnd, IDOK);
		}
		break;
		case IDCANCEL:
			SelItem->SetMagicLevel(SaveModLevel);
			EndDialog(hWnd, IDCANCEL);
			break;
		case IDC_TAB2Rare_NamePrefix:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				int index = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_GETCURSEL, 0, 0);
				SearchNamePrefix = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_GETITEMDATA, index, 0);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_NameSuffix:
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				int index = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_GETCURSEL, 0, 0);
				SearchNameSuffix = SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_GETITEMDATA, index, 0);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check1:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 0;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check2:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 1;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check3:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 2;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check4:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 3;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check5:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 4;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_Check6:
			if (HIWORD(wParam) == BN_CLICKED) {
				UserSelected = 5;
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
				UpdateTab2Rare(hWnd);
			}
			break;
		case IDC_TAB2Rare_LockELevel:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (IsDlgButtonChecked(hWnd, LOWORD(wParam)) == BST_UNCHECKED)
					RestrictELevel = true;
				else
					RestrictELevel = false;
				LoadAttributesTree(hWnd);
				UpdateTab2Rare(hWnd);
				TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
			}
			break;
		case IDC_TAB2Rare_LockValue:
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				if (IsDlgButtonChecked(hWnd, IDC_TAB2Rare_LockELevel) == BST_CHECKED) {
					int x = GetDlgItemInt(hWnd, IDC_TAB2Rare_LockValue, NULL, FALSE);
					if (x != RestrictELevelValue) {
						RestrictELevelValue = x;
						LoadAttributesTree(hWnd);
						UpdateTab2Rare(hWnd);
						TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
					}
				}
			}
			break;
		case IDC_CHELP:
			ToggleHelpBox(hWnd, IDH_TAB2Rare);
			break;
		case IDC_TAB2Rare_Clear:
		{
			SendDlgItemMessage(hWnd, IDC_TAB2Rare_NamePrefix, LB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hWnd, IDC_TAB2Rare_NameSuffix, LB_SETCURSEL, 0, 0);
			SearchNamePrefix = 0;
			SearchNameSuffix = 0;
			for (int z = 0; z < 6; z++)
			{
				Attribute[z].Selected = 0;
				Attribute[z].iSelected = ZEROATTR;
				Attribute[z].tSelected = PREFIX;
			}
			UserSelected = 0;
			TVMakeSelection(hWnd, Attribute[UserSelected].iSelected);
			UpdateTab2Rare(hWnd);
		}
		break;
		}
	}
	return false;
	case WM_NOTIFY:
		switch (((NMHDR*)lParam)->idFrom)
		{
		case IDC_TAB2Rare_TreePrefix:
		case IDC_TAB2Rare_TreeSuffix:
			switch (((NMHDR*)lParam)->code)
			{
			case TVN_SELCHANGED:
			{
				if (userselecting) break;
				userselecting = true;
				NM_TREEVIEW* NMTreeView = (NM_TREEVIEW*)lParam;
				if (UserSelected >= 0 && UserSelected <= 5) {
					int n = UserSelected;
					if (NMTreeView->itemNew.lParam >= 0)
					{
						Attribute[n].iSelected = NMTreeView->itemNew.lParam;

						if ((Attribute[n].iSelected & 256) == 0) {
							Attribute[n].tSelected = PREFIX;
							Attribute[n].Selected = &MagicPrefixTable[Attribute[n].iSelected];
							TreeView_SelectItem(hTVS, NULL);
						}
						else {
							Attribute[n].tSelected = SUFFIX;
							Attribute[n].Selected = &MagicSuffixTable[Attribute[n].iSelected & 255];
							TreeView_SelectItem(hTVP, NULL);
						}
					}
					else if (NMTreeView->itemNew.lParam == ZEROATTR || NMTreeView->itemNew.lParam == RANDOMATTR)
					{
						Attribute[n].iSelected = NMTreeView->itemNew.lParam;
						Attribute[n].tSelected = PREFIX;
						Attribute[n].Selected = 0;
					}

					UpdateTab2Rare(hWnd);
				}
				userselecting = false;
			}
			break;
			}
		}
		return false;
	case WM_CLOSE:
	{
		SelItem->SetMagicLevel(SaveModLevel);
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