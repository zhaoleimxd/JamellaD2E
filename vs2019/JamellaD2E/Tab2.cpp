// Tab2.cpp from D2E
#include "JamellaD2E.h"
// Tab2 Controls
HWND hQuality;
// ItemTree
HWND hTV;
HIMAGELIST hTVImagelist = 0;
// Inventory Parts
HWND hInv, hStash, hBelt, hCube, hBody, hCopyBuffer, hLoad, hSave;
RECT rCube, rBelt, rStash;
inline int ContainerFromHWND(HWND hWnd)
{
	if (hWnd == hInv) return CNT_INVENTORY;
	if (hWnd == hStash) return CNT_STASH;
	if (hWnd == hBelt) return CNT_BELT;
	if (hWnd == hCube) return CNT_CUBE;
	if (hWnd == hBody) return CNT_BODY;
	if (hWnd == hCopyBuffer) return CNT_COPYBUFFER;
	return CNT_NONE;
}
HBITMAP hItemUnknown;
HBITMAP hBmpBodyWhole, hBmpNotPlaceable;
HCURSOR hCurMove, hCurMoveCopy, hCurCross, hCurNo;
HBRUSH hBrushBlack, hBrushNull;
HPEN hPenWhite, hPenGreen, hPenBusy, hPenUnwearable;
class Item* SelItem;
class Item* CopyBuffer = 0;
static UINT ClipBoardFormat, ClipBoardFormatJohnDoe;
static HGLOBAL ClipBoardData, ClipBoardDataJohnDoe;
bool MouseButtonDown = false;
bool MouseButtonMoved = false;
int xStartPos, yStartPos;
int xDragTol, yDragTol;
bool TVMouseDrag = false;
void MakeSelection(Item* i);
bool SelItemDeleteable();
// Item Context Menu Entries
#define IDR_TAB2_Delete 50000
#define IDR_TAB2_Socketed 50001
#define IDR_TAB2_Identified 50002
#define IDR_TAB2_Starter 50003
#define IDR_TAB2_SetQuantity 50004
#define IDR_TAB2_SetQuality 50005
#define IDR_TAB2_SetDurability 50006
#define IDR_TAB2_SetDefense 50007
#define IDR_TAB2_SetQualityCrude 50008
#define IDR_TAB2_SetQualityRegular 50009
#define IDR_TAB2_SetQualitySuperior 50010
#define IDR_TAB2_SetQualityMagical 50011
#define IDR_TAB2_SetQualityRare 50012
#define IDR_TAB2_SetQualitySet 50013
#define IDR_TAB2_SetQualityUnique 50014
#define IDR_TAB2_AttributeSelect 50015
#define IDR_TAB2_GemsEdit 50016
#define IDR_TAB2_SetEarProperties 50017
#define IDR_TAB2_ChangeRingImage 50018
#define IDR_TAB2_ChangeAmuletImage 50019
#define IDR_TAB2_Cut 50020
#define IDR_TAB2_Copy 50021
#define IDR_TAB2_Paste 50022
#pragma pack(1)
struct JamClipboardHead
{
	WORD IID;
	BYTE xSize;
	BYTE ySize;
	BYTE BodyPlace;
};
#pragma pack(4)
void LoadItemTree()
{
	TV_ITEM TVItem;
	TV_INSERTSTRUCT TVInsert;
	HTREEITEM hPrevBranches[5];
	hPrevBranches[0] = TVI_ROOT;
	for (int z1 = 0; z1 < nItemTree; z1++)
	{
		ZeroMemory(&TVItem, sizeof TVItem);
		TVItem.mask = TVIF_TEXT | TVIF_PARAM;
		TVItem.pszText = ItemTree[z1].Text;
		TVItem.lParam = (LPARAM)-1;
		TVInsert.hParent = hPrevBranches[ItemTree[z1].Depth - 1];
		TVInsert.hInsertAfter = TVI_LAST;
		TVInsert.item = TVItem;
		hPrevBranches[ItemTree[z1].Depth] =
			ItemTree[z1].hTree =
			TreeView_InsertItem(hTV, &TVInsert);
		for (int z2 = 0; z2 < nItemInfos; z2++)
		{
			if (ItemInfos[z2].TreeID != ItemTree[z1].TreeID) continue;
			ZeroMemory(&TVItem, sizeof TVItem);
			TVItem.mask = TVIF_TEXT | TVIF_PARAM;
			TVItem.pszText = ItemInfos[z2].ItemName;
			TVItem.lParam = z2;

			TVInsert.hParent = hPrevBranches[ItemTree[z1].Depth];
			TVInsert.hInsertAfter = TVI_LAST;
			TVInsert.item = TVItem;

			ItemInfos[z2].hTree = TreeView_InsertItem(hTV, &TVInsert);
		}
	}
}
void MakeEmptyBelt(int Column = -1)
{
	// Delete Items in Belt
	for (Item* I = Items; I != 0;)
	{
		if (I->Container() == CNT_BELT)
		{
			Item* J = I->Next();
			if (Column < 0 || I->xPos() == Column) {
				I->Delete();
			}
			I = J;
		}
		else
		{
			I = I->Next();
		}
	}
}
LRESULT CALLBACK QuantityDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		sprintf(buffer, "/ %i Max", SelItem->Info->Quantity);
		SetDlgItemText(hWnd, IDC_TAB2Quantity_Max, buffer);
		SetDlgItemInt(hWnd, IDC_TAB2Quantity_Set, SelItem->Quantity(), TRUE);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			BOOL nOK;
			int x = GetDlgItemInt(hWnd, IDC_TAB2Quantity_Set, &nOK, TRUE);
			if (nOK && x > 0 && (x <= 255 || RegOptions.ExceedQuantity))
			{
				SelItem->SetQuantity(x);
				EndDialog(hWnd, 0);
			}
			else
				MessageBeep(MB_ICONHAND);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
LRESULT CALLBACK DurabilityDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetDlgItemInt(hWnd, IDC_TAB2Durability_Set, SelItem->Durability(), TRUE);
		SetDlgItemInt(hWnd, IDC_TAB2Durability_Max, SelItem->DurabilityMax(), TRUE);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			BOOL nOK1, nOK2;
			int Is = GetDlgItemInt(hWnd, IDC_TAB2Durability_Set, &nOK1, TRUE);
			int Max = GetDlgItemInt(hWnd, IDC_TAB2Durability_Max, &nOK2, TRUE);
			if (nOK1 && nOK2 && Is > 0 && Max > 0 && Is <= 255 && Max <= 255 && Is <= Max)
			{
				SelItem->SetDurabilityMax(Max);
				SelItem->SetDurability(Is);
				EndDialog(hWnd, 0);
			}
			else
				MessageBeep(MB_ICONHAND);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
static DWORD WINAPI DefenseAttack(LPVOID ThreadParameters)
{
	DefenseSearchThread* P = (DefenseSearchThread*)ThreadParameters;
	P->Advanced = false;
	P->Dialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2SS), hMainDialog, (DLGPROC)Tab2SearchDialogProc, (LPARAM) P);
	if (!P->Dialog) return -1;
	Item* I = P->Item;
	I->Busy = true;
	while (!CheckPollMessages())
	{
		I->SetDWA(I->DWA() + 1);
		P->Counter++;
		if (I->BaseDefense() == P->SelectAC)
			break;
	}
	MessageBeep(MB_ICONASTERISK);
	I->Busy = false;
	EndDialog(P->Dialog, 0);
	UpdateTab2();
	CloseHandle(P->Thread);
	GlobalFree(P->ThreadData);
	return 0;
}
LRESULT CALLBACK DefenseDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		sprintf(buffer, "Range %i to %i", SelItem->Info->ACMin, SelItem->Info->ACMax - 1);
		SetDlgItemText(hWnd, IDC_TAB2Defense_Range, buffer);
		SetDlgItemInt(hWnd, IDC_TAB2Defense_Value, SelItem->BaseDefense(), TRUE);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			BOOL nOK;
			int Val = GetDlgItemInt(hWnd, IDC_TAB2Defense_Value, &nOK, TRUE);
			if (nOK && SelItem->Info->ACMin <= Val && Val <= SelItem->Info->ACMax - 1)
			{
				SelItem->DWBHistory.StepAdd(SelItem);
				HGLOBAL TD = GlobalAlloc(GMEM_ZEROINIT, sizeof DefenseSearchThread);
				struct DefenseSearchThread* NewThread = (DefenseSearchThread*)GlobalLock(TD);;
				NewThread->ThreadData = TD;
				NewThread->Item = SelItem;
				NewThread->SelectAC = Val;
				DWORD ThreadID;
				NewThread->Thread = CreateThread(NULL, 0, &DefenseAttack, NewThread, 0, &ThreadID);
				EndDialog(hWnd, 0);
			}
			else
				MessageBeep(MB_ICONHAND);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
LRESULT CALLBACK EarDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Set Name
		SetDlgItemText(hWnd, IDC_TAB2Ear_Name, SelItem->OpponentName());
		// Fill and set character class
		SendDlgItemMessage(hWnd, IDC_TAB2Ear_Class, CB_RESETCONTENT, 0, 0);
		for (int i = 0; i < 5; i++)
			SendDlgItemMessage(hWnd, IDC_TAB2Ear_Class, CB_ADDSTRING, 0, (LPARAM)CharClasses[i]);
		SendDlgItemMessage(hWnd, IDC_TAB2Ear_Class, CB_SETCURSEL, SelItem->OpponentClass(), 0);
		// Set Level
		SetDlgItemInt(hWnd, IDC_TAB2Ear_Level, SelItem->OpponentLevel(), FALSE);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			GetDlgItemText(hWnd, IDC_TAB2Ear_Name, buffer, 32);
			SelItem->SetOpponentName(buffer);
			SelItem->SetOpponentClass(SendDlgItemMessage(hWnd, IDC_TAB2Ear_Class, CB_GETCURSEL, 0, 0));

			SelItem->SetOpponentLevel(GetDlgItemInt(hWnd, IDC_TAB2Ear_Level, NULL, FALSE));
			EndDialog(hWnd, 0);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
static DWORD WINAPI RingAmuletAttack(LPVOID ThreadParameters)
{
	RingAmuletSearchThread* P = (RingAmuletSearchThread*)ThreadParameters;
	P->Advanced = false;
	P->Dialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2SS), hMainDialog, (DLGPROC)Tab2SearchDialogProc, (LPARAM) P);
	if (!P->Dialog) return -1;
	Item* I = P->Item;
	I->Busy = true;
	while (!CheckPollMessages())
	{
		I->SetDWA(I->DWA() + 1);
		P->Counter++;
		if (P->RingAmulet == false) {
			if ((I->DWARandomOffset(1) % 5) == P->Image)
				break;
		}
		else {
			if ((I->DWARandomOffset(1) % 3) == P->Image)
				break;
		}
	}
	MessageBeep(MB_ICONASTERISK);
	I->Busy = false;
	I->Info = 0;
	EndDialog(P->Dialog, IDOK);
	InvalidateRect(hTabDialog, NULL, FALSE);
	UpdateTab2();
	CloseHandle(P->Thread);
	GlobalFree(P->ThreadData);
	return 0;
}
LRESULT CALLBACK RingImageDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Fill in Images
		for (int z = 0; z < 5; z++)
			SendDlgItemMessage(hWnd, IDC_TAB2RingImage_Image1 + z, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)RingImages[z].hBmp)
			;
		int x = SelItem->DWARandomOffset(1) % 5;
		CheckRadioButton(hWnd, IDC_TAB2RingImage_Image1, IDC_TAB2RingImage_Image5, IDC_TAB2RingImage_Image1 + x);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			int s = -1;
			for (int z = 0; z < 5; z++) {
				if (IsDlgButtonChecked(hWnd, IDC_TAB2RingImage_Image1 + z) == BST_CHECKED)
					s = z;
			}
			if (s < 0)
				EndDialog(hWnd, IDOK);
			SelItem->DWBHistory.StepAdd(SelItem);

			HGLOBAL TD = GlobalAlloc(GMEM_ZEROINIT, sizeof RingAmuletSearchThread);
			struct RingAmuletSearchThread* NewThread = (RingAmuletSearchThread*)GlobalLock(TD);;
			NewThread->ThreadData = TD;
			NewThread->Item = SelItem;
			NewThread->RingAmulet = false;
			NewThread->Image = s;

			DWORD ThreadID;
			NewThread->Thread = CreateThread(NULL, 0, &RingAmuletAttack, NewThread, 0, &ThreadID);

			EndDialog(hWnd, IDOK);
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
LRESULT CALLBACK AmuletImageDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Fill in Images
		for (int z = 0; z < 3; z++)
			SendDlgItemMessage(hWnd, IDC_TAB2AmuletImage_Image1 + z, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)AmuletImages[z].hBmp);
		int x = SelItem->DWARandomOffset(1) % 3;
		CheckRadioButton(hWnd, IDC_TAB2AmuletImage_Image1, IDC_TAB2AmuletImage_Image3,
			IDC_TAB2AmuletImage_Image1 + x);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
			int s = -1;
			for (int z = 0; z < 3; z++) {
				if (IsDlgButtonChecked(hWnd, IDC_TAB2AmuletImage_Image1 + z) == BST_CHECKED)
					s = z;
			}
			if (s < 0)
				EndDialog(hWnd, IDOK);
			SelItem->DWBHistory.StepAdd(SelItem);

			HGLOBAL TD = GlobalAlloc(GMEM_ZEROINIT, sizeof RingAmuletSearchThread);
			struct RingAmuletSearchThread* NewThread = (RingAmuletSearchThread*)GlobalLock(TD);;
			NewThread->ThreadData = TD;

			NewThread->Item = SelItem;
			NewThread->RingAmulet = true;
			NewThread->Image = s;

			DWORD ThreadID;
			NewThread->Thread = CreateThread(NULL, 0, &RingAmuletAttack, NewThread, 0, &ThreadID);

			EndDialog(hWnd, IDOK);
		}
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
		}
		break;
	}
	return false;
}
bool SaveToClipboard(Item* I)
{
	if (!OpenClipboard(hMainDialog))
		return false;

	if (!EmptyClipboard())
		return false;

	Item* G = 0;

	// Jamella Clipboard Format
	{
		if (ClipBoardData)
			GlobalFree(ClipBoardData);

		int ClipSize = I->ItemRecordLength();

		for (Item* G = I->Gems; G != 0; G = G->Next())
			ClipSize += G->ItemRecordLength();

		ClipSize += sizeof(JamClipboardHead);

		ClipBoardData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, ClipSize);
		BYTE* Clip = (BYTE*)GlobalLock(ClipBoardData);

		struct JamClipboardHead* JamHead = (JamClipboardHead*)Clip;
		JamHead->IID = I->ItemRecordID();
		JamHead->xSize = I->xSize();
		JamHead->ySize = I->ySize();
		JamHead->BodyPlace = I->BodyPlace();

		int o = sizeof(JamClipboardHead);

		memcpy(Clip + o, I->GetItemRecord(), I->ItemRecordLength());
		o += I->ItemRecordLength();

		for (G = I->Gems; G != 0; G = G->Next())
		{
			memcpy(Clip + o, G->GetItemRecord(), G->ItemRecordLength());
			o += G->ItemRecordLength();
		}

		GlobalUnlock(ClipBoardData);

		SetClipboardData(ClipBoardFormat, ClipBoardData);
	}
	if (I->ItemRecordID() == IT_103)
	{
		// John Doe Clipboard Format
		if (ClipBoardDataJohnDoe)
			GlobalFree(ClipBoardDataJohnDoe);

		int ClipSize = I->ItemRecordLength();

		for (G = I->Gems; G != 0; G = G->Next())
			ClipSize += G->ItemRecordLength();

		ClipSize += 4;

		ClipBoardDataJohnDoe = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, ClipSize);
		BYTE* Clip = (BYTE*)GlobalLock(ClipBoardDataJohnDoe);

		*(DWORD*)Clip = I->Gems->Count();

		int o = 4;
		memcpy(Clip + o, I->GetItemRecord(), I->ItemRecordLength());
		o += I->ItemRecordLength();

		for (G = I->Gems; G != 0; G = G->Next())
		{
			memcpy(Clip + o, G->GetItemRecord(), G->ItemRecordLength());
			o += G->ItemRecordLength();
		}

		GlobalUnlock(ClipBoardDataJohnDoe);

		SetClipboardData(ClipBoardFormatJohnDoe, ClipBoardDataJohnDoe);
	}
	CloseClipboard();
	return true;
}
static struct
{
	int code;
	char* text;
	int index;
}
Qualities[] =
{
 { CRUDEITEM, "Crude" },
 { USUALITEM, "Regular" },
 { SUPERIORITEM, "Superior" },
 { MAGICITEM, "Magical" },
 { SETITEM, "Set Item" },
 { RAREITEM, "Rare Item" },
 { UNIQUEITEM, "Unique Item" }
};
bool FindCube();
LRESULT CALLBACK ProcessItemMessage(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		// Item Context Menu
	case IDR_TAB2_Delete:
	{
		if (MouseButtonDown) break;
		if (!SelItem) break;
		if (SelItem->Busy) break;

		if (SelItemDeleteable())
		{
			if (SelItem == CopyBuffer)
			{
				delete CopyBuffer;
				CopyBuffer = 0;
			}
			else
			{
				SelItem->Delete();
			}
			MakeSelection(0);
		}
	}
	break;
	case IDR_TAB2_Copy:
	{
		if (!SelItem || SelItem->Busy) break;
		SaveToClipboard(SelItem);
	}
	break;
	case IDR_TAB2_Cut:
	{
		if (!SelItem || SelItem->Busy) break;
		SaveToClipboard(SelItem);

		if (SelItemDeleteable()) {
			if (SelItem == CopyBuffer) {
				delete CopyBuffer;
				CopyBuffer = 0;
			}
			else {
				SelItem->Delete();
			}
			MakeSelection(0);
		}
	}
	break;
	case IDR_TAB2_Socketed:
		if (SelItem->Socketed() || SelItem->Socketable() || RegOptions.AllItemsSocketable)
		{
			if (SelItem->Socketed()) {
				SelItem->SetSocketed(false);
				SelItem->SetGemNum(0);
				if (SelItem->Gems) {
					delete SelItem->Gems;
					SelItem->Gems = 0;
				}
			}
			else
				SelItem->SetSocketed(true);
			UpdateTab2();
		}
		break;
	case IDR_TAB2_Identified:
	{
		if (SelItem->Identified())
			SelItem->SetIdentified(false);
		else
			SelItem->SetIdentified(true);
		UpdateTab2();
	}
	break;
	case IDR_TAB2_Starter:
	{
		if (SelItem->Starter())
			SelItem->SetStarter(false);
		else
			SelItem->SetStarter(true);
		UpdateTab2();
	}
	break;
	case IDR_TAB2_SetQuantity:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Quantity), hWnd, (DLGPROC)QuantityDialogProc);
		UpdateTab2();
	}
	break;
	case IDR_TAB2_SetDurability:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Durability), hWnd, (DLGPROC)DurabilityDialogProc);
		UpdateTab2();
	}
	break;
	case IDR_TAB2_SetDefense:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Defense), hWnd, (DLGPROC)DefenseDialogProc);
		UpdateTab2();
	}
	break;

	case IDR_TAB2_SetQualityCrude:
		SelItem->SetQuality(CRUDEITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualityRegular:
		SelItem->SetQuality(USUALITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualitySuperior:
		SelItem->SetQuality(SUPERIORITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualityMagical:
		SelItem->SetQuality(MAGICITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualityRare:
		SelItem->SetQuality(RAREITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualitySet:
		SelItem->SetQuality(SETITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;
	case IDR_TAB2_SetQualityUnique:
		SelItem->SetQuality(UNIQUEITEM);
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
		break;

	case IDR_TAB2_AttributeSelect:
		if (SelItem->Quality() == MAGICITEM)
		{
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Magic), hTabDialog, (DLGPROC)Tab2MagicDialogProc);
			SelItem->Decoded = 0;
			UpdateTab2();
		}
		else if (SelItem->Quality() == RAREITEM)
		{
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Rare), hTabDialog, (DLGPROC)Tab2RareDialogProc);
			SelItem->Decoded = 0;
			UpdateTab2();
		}
		break;
	case IDR_TAB2_GemsEdit:
	{
		if (!SelItem->Socketed()) break;

		if (SelItem->Quality() == MAGICITEM || SelItem->Quality() == RAREITEM && RegOptions.NoAnnoyingMsgs)
		{
			MessageBox(hWnd, "WARNING:\nSocketed magical or rare items cannot be created in the game, they are trulyhacked!\nBlizzard may bring out a blocking patch one day!",PROGRAMNAME,MB_OK | MB_ICONHAND);
		}

		if (SelItem->Info->Sockets == 0 || RegOptions.A7Gems) {
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Gems7), hTabDialog, (DLGPROC)Tab2GemsDialogProc);
		}
		else if (SelItem->Info->Sockets == 1) {
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Gems1), hTabDialog, (DLGPROC)Tab2GemsDialogProc);
		}
		else if (SelItem->Info->Sockets == 2) {
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Gems2), hTabDialog, (DLGPROC)Tab2GemsDialogProc);
		}
		else if (SelItem->Info->Sockets == 3) {
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Gems3), hTabDialog, (DLGPROC)Tab2GemsDialogProc);
		}

		SelItem->Decoded = 0;
		UpdateTab2();
	}
	break;
	case IDR_TAB2_SetEarProperties:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2Ear), hTabDialog, (DLGPROC)EarDialogProc);
		SelItem->Decoded = 0;
		UpdateTab2();
	}
	break;
	case IDR_TAB2_ChangeRingImage:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2RingImage), hTabDialog, (DLGPROC)RingImageDialogProc);
		SelItem->Info = 0;
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
	}
	break;
	case IDR_TAB2_ChangeAmuletImage:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_TAB2AmuletImage), hTabDialog, (DLGPROC)AmuletImageDialogProc);
		SelItem->Info = 0;
		UpdateTab2();
		InvalidateRect(hTabDialog, NULL, FALSE);
	}
	break;
	}
	return true;
}
LRESULT CALLBACK Tab2DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Set up ItemTree
		hTV = GetDlgItem(hWnd, IDC_TAB2_ItemTree);
		LoadItemTree();
		hBody = GetDlgItem(hWnd, IDC_TAB2_Body);
		hInv = GetDlgItem(hWnd, IDC_TAB2_Inv);
#if INVGRIDS == 0
		hStash = GetDlgItem(hWnd, IDC_TAB2_Stash);
		hBelt = GetDlgItem(hWnd, IDC_TAB2_Belt);
		hCube = GetDlgItem(hWnd, IDC_TAB2_Cube);
#endif
		hCopyBuffer = GetDlgItem(hWnd, IDC_TAB2_CopyBuffer);
		hLoad = GetDlgItem(hWnd, IDC_TAB2_Load);
		hSave = GetDlgItem(hWnd, IDC_TAB2_Save);
		SetWindowLong(hBody, GWL_WNDPROC, (LONG)Tab2InventoryProc);
		SetWindowLong(hInv, GWL_WNDPROC, (LONG)Tab2InventoryProc);
#if INVGRIDS == 0
		SetWindowLong(hStash, GWL_WNDPROC, (LONG)Tab2InventoryProc);
		SetWindowLong(hBelt, GWL_WNDPROC, (LONG)Tab2InventoryProc);
		SetWindowLong(hCube, GWL_WNDPROC, (LONG)Tab2InventoryProc);
#endif
		SetWindowLong(hCopyBuffer, GWL_WNDPROC, (LONG)Tab2InventoryProc);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB2_AttrRandom), FALSE);
		SelItem = 0;
		xDragTol = GetSystemMetrics(SM_CXDRAG);
		yDragTol = GetSystemMetrics(SM_CYDRAG);
		hExpertBox = 0;
		CheckDlgButton(hWnd, IDC_TAB2_ExpertMode, BST_UNCHECKED);
		SendDlgItemMessage(hWnd, IDC_TAB2_RichText, EM_SETBKGNDCOLOR, FALSE, GetSysColor(COLOR_BTNFACE));
		hToolTip = CreateToolTipCtrl(hWnd, IDD_TAB2, true);
		ClipBoardFormat = RegisterClipboardFormat(CLIPBOARDFORMAT);
		ClipBoardFormatJohnDoe = RegisterClipboardFormat("Diablo2Item");
	}
	return true;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case IDC_TAB2_ItemTree:
		{
			NM_TREEVIEW* TreeView = (NM_TREEVIEW FAR*) lParam;
			switch (TreeView->hdr.code)
			{
			case TVN_SELCHANGING:
				if (SelItem)
				{
					MakeSelection(0);
				}
				return false;
			case TVN_SELCHANGED:
				if (TreeView->itemNew.lParam >= 0 && TreeView->itemNew.lParam < nItemInfos)
				{
					SelInfo = &ItemInfos[TreeView->itemNew.lParam];
				}
				return false;
			case TVN_BEGINDRAG:
				if (TreeView->itemNew.lParam >= 0 && TreeView->itemNew.lParam < nItemInfos)
				{
					MakeSelection(0);
					SelInfo = &ItemInfos[TreeView->itemNew.lParam];
					SetCapture(hInv);
					MouseButtonDown = true;
					xStartPos = TreeView->ptDrag.x;
					yStartPos = TreeView->ptDrag.y;
					FindCube();
				}
				return false;
			}
		}
		break;
		}
		return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		default:
			ProcessItemMessage(hWnd, wParam, lParam);
			break;
			// Batch Action Context Menu
		case IDR_TAB2_BeltEmpty:
		{
			MakeEmptyBelt();
			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
		}
		break;
		case IDR_TAB2_BeltMinorHealing:
		case IDR_TAB2_BeltLightHealing:
		case IDR_TAB2_BeltHealing:
		case IDR_TAB2_BeltGreaterHealing:
		case IDR_TAB2_BeltSuperHealing:
		case IDR_TAB2_BeltMinorMana:
		case IDR_TAB2_BeltMana:
		case IDR_TAB2_BeltLightMana:
		case IDR_TAB2_BeltGreaterMana:
		case IDR_TAB2_BeltSuperMana:
		case IDR_TAB2_BeltRejuv:
		case IDR_TAB2_BeltFullRejuv:
		case IDR_TAB2_Slot1MinorHealing:
		case IDR_TAB2_Slot1LightHealing:
		case IDR_TAB2_Slot1Healing:
		case IDR_TAB2_Slot1GreaterHealing:
		case IDR_TAB2_Slot1SuperHealing:
		case IDR_TAB2_Slot1MinorMana:
		case IDR_TAB2_Slot1Mana:
		case IDR_TAB2_Slot1LightMana:
		case IDR_TAB2_Slot1GreaterMana:
		case IDR_TAB2_Slot1SuperMana:
		case IDR_TAB2_Slot1Rejuv:
		case IDR_TAB2_Slot1FullRejuv:
		case IDR_TAB2_Slot2MinorHealing:
		case IDR_TAB2_Slot2LightHealing:
		case IDR_TAB2_Slot2Healing:
		case IDR_TAB2_Slot2GreaterHealing:
		case IDR_TAB2_Slot2SuperHealing:
		case IDR_TAB2_Slot2MinorMana:
		case IDR_TAB2_Slot2Mana:
		case IDR_TAB2_Slot2LightMana:
		case IDR_TAB2_Slot2GreaterMana:
		case IDR_TAB2_Slot2SuperMana:
		case IDR_TAB2_Slot2Rejuv:
		case IDR_TAB2_Slot2FullRejuv:
		case IDR_TAB2_Slot3MinorHealing:
		case IDR_TAB2_Slot3LightHealing:
		case IDR_TAB2_Slot3Healing:
		case IDR_TAB2_Slot3GreaterHealing:
		case IDR_TAB2_Slot3SuperHealing:
		case IDR_TAB2_Slot3MinorMana:
		case IDR_TAB2_Slot3Mana:
		case IDR_TAB2_Slot3LightMana:
		case IDR_TAB2_Slot3GreaterMana:
		case IDR_TAB2_Slot3SuperMana:
		case IDR_TAB2_Slot3Rejuv:
		case IDR_TAB2_Slot3FullRejuv:
		case IDR_TAB2_Slot4MinorHealing:
		case IDR_TAB2_Slot4LightHealing:
		case IDR_TAB2_Slot4Healing:
		case IDR_TAB2_Slot4GreaterHealing:
		case IDR_TAB2_Slot4SuperHealing:
		case IDR_TAB2_Slot4MinorMana:
		case IDR_TAB2_Slot4Mana:
		case IDR_TAB2_Slot4LightMana:
		case IDR_TAB2_Slot4GreaterMana:
		case IDR_TAB2_Slot4SuperMana:
		case IDR_TAB2_Slot4Rejuv:
		case IDR_TAB2_Slot4FullRejuv:
		{
			int IC;
			int Col;
			switch (LOWORD(wParam))
			{
			case IDR_TAB2_BeltMinorHealing: IC = 0x15A0; Col = -1; break;
			case IDR_TAB2_BeltLightHealing: IC = 0x15B0; Col = -1; break;
			case IDR_TAB2_BeltHealing: IC = 0x15C0; Col = -1; break;
			case IDR_TAB2_BeltGreaterHealing: IC = 0x15D0; Col = -1; break;
			case IDR_TAB2_BeltSuperHealing: IC = 0x15E0; Col = -1; break;
			case IDR_TAB2_BeltMinorMana: IC = 0x15F0; Col = -1; break;
			case IDR_TAB2_BeltLightMana: IC = 0x1600; Col = -1; break;
			case IDR_TAB2_BeltMana: IC = 0x1610; Col = -1; break;
			case IDR_TAB2_BeltGreaterMana: IC = 0x1620; Col = -1; break;
			case IDR_TAB2_BeltSuperMana: IC = 0x1630; Col = -1; break;
			case IDR_TAB2_BeltRejuv: IC = 0x1120; Col = -1; break;
			case IDR_TAB2_BeltFullRejuv: IC = 0x1130; Col = -1; break;
			case IDR_TAB2_Slot1MinorHealing: IC = 0x15A0; Col = 0; break;
			case IDR_TAB2_Slot1LightHealing: IC = 0x15B0; Col = 0; break;
			case IDR_TAB2_Slot1Healing: IC = 0x15C0; Col = 0; break;
			case IDR_TAB2_Slot1GreaterHealing: IC = 0x15D0; Col = 0; break;
			case IDR_TAB2_Slot1SuperHealing: IC = 0x15E0; Col = 0; break;
			case IDR_TAB2_Slot1MinorMana: IC = 0x15F0; Col = 0; break;
			case IDR_TAB2_Slot1LightMana: IC = 0x1600; Col = 0; break;
			case IDR_TAB2_Slot1Mana: IC = 0x1610; Col = 0; break;
			case IDR_TAB2_Slot1GreaterMana: IC = 0x1620; Col = 0; break;
			case IDR_TAB2_Slot1SuperMana: IC = 0x1630; Col = 0; break;
			case IDR_TAB2_Slot1Rejuv: IC = 0x1120; Col = 0; break;
			case IDR_TAB2_Slot1FullRejuv: IC = 0x1130; Col = 0; break;
			case IDR_TAB2_Slot2MinorHealing: IC = 0x15A0; Col = 1; break;
			case IDR_TAB2_Slot2LightHealing: IC = 0x15B0; Col = 1; break;
			case IDR_TAB2_Slot2Healing: IC = 0x15C0; Col = 1; break;
			case IDR_TAB2_Slot2GreaterHealing: IC = 0x15D0; Col = 1; break;
			case IDR_TAB2_Slot2SuperHealing: IC = 0x15E0; Col = 1; break;
			case IDR_TAB2_Slot2MinorMana: IC = 0x15F0; Col = 1; break;
			case IDR_TAB2_Slot2LightMana: IC = 0x1600; Col = 1; break;
			case IDR_TAB2_Slot2Mana: IC = 0x1610; Col = 1; break;
			case IDR_TAB2_Slot2GreaterMana: IC = 0x1620; Col = 1; break;
			case IDR_TAB2_Slot2SuperMana: IC = 0x1630; Col = 1; break;
			case IDR_TAB2_Slot2Rejuv: IC = 0x1120; Col = 1; break;
			case IDR_TAB2_Slot2FullRejuv: IC = 0x1130; Col = 1; break;
			case IDR_TAB2_Slot3MinorHealing: IC = 0x15A0; Col = 2; break;
			case IDR_TAB2_Slot3LightHealing: IC = 0x15B0; Col = 2; break;
			case IDR_TAB2_Slot3Healing: IC = 0x15C0; Col = 2; break;
			case IDR_TAB2_Slot3GreaterHealing: IC = 0x15D0; Col = 2; break;
			case IDR_TAB2_Slot3SuperHealing: IC = 0x15E0; Col = 2; break;
			case IDR_TAB2_Slot3MinorMana: IC = 0x15F0; Col = 2; break;
			case IDR_TAB2_Slot3LightMana: IC = 0x1600; Col = 2; break;
			case IDR_TAB2_Slot3Mana: IC = 0x1610; Col = 2; break;
			case IDR_TAB2_Slot3GreaterMana: IC = 0x1620; Col = 2; break;
			case IDR_TAB2_Slot3SuperMana: IC = 0x1630; Col = 2; break;
			case IDR_TAB2_Slot3Rejuv: IC = 0x1120; Col = 2; break;
			case IDR_TAB2_Slot3FullRejuv: IC = 0x1130; Col = 2; break;
			case IDR_TAB2_Slot4MinorHealing: IC = 0x15A0; Col = 3; break;
			case IDR_TAB2_Slot4LightHealing: IC = 0x15B0; Col = 3; break;
			case IDR_TAB2_Slot4Healing: IC = 0x15C0; Col = 3; break;
			case IDR_TAB2_Slot4GreaterHealing: IC = 0x15D0; Col = 3; break;
			case IDR_TAB2_Slot4SuperHealing: IC = 0x15E0; Col = 3; break;
			case IDR_TAB2_Slot4MinorMana: IC = 0x15F0; Col = 3; break;
			case IDR_TAB2_Slot4LightMana: IC = 0x1600; Col = 3; break;
			case IDR_TAB2_Slot4Mana: IC = 0x1610; Col = 3; break;
			case IDR_TAB2_Slot4GreaterMana: IC = 0x1620; Col = 3; break;
			case IDR_TAB2_Slot4SuperMana: IC = 0x1630; Col = 3; break;
			case IDR_TAB2_Slot4Rejuv: IC = 0x1120; Col = 3; break;
			case IDR_TAB2_Slot4FullRejuv: IC = 0x1130; Col = 3; break;
			default: IC = 0x0000; Col = -1; break;
			}
			IC = FindItemInfoByItemCode(IC);
			MakeEmptyBelt(Col);
			if (Col < 0)
			{
				for (int x = 0; x < 4; x++)
				{
					for (int y = 0; y < 4; y++)
					{
						Item* I = CreateItem(&Items, &ItemInfos[IC]);
						I->SetCoordinates(CNT_BELT, x, y);
					}
				}
			}
			else
			{
				for (int y = 0; y < 4; y++)
				{
					Item* I = CreateItem(&Items, &ItemInfos[IC]);
					I->SetCoordinates(CNT_BELT, Col, y);
				}
			}

			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
		}
		break;
		case IDR_TAB2_RepairAll:
		{
			Item* I;
			// Repair all Items (Set Durability to MaxDurability)
			for (I = Items; I != 0; I = I->Next())
			{
				I->SetDurability(I->DurabilityMax());
			}
			UpdateTab2();
		}
		break;
		// Save & Load Items into Load Buffer
		case IDC_TAB2_Save:
			SaveItemFile(hWnd);
			InvalidateRect(hTabDialog, NULL, FALSE);
			break;
		case IDC_TAB2_Load:
			LoadItemFile(hWnd);
			InvalidateRect(hTabDialog, NULL, FALSE);
			break;
		case IDC_TAB2_AttrRandom:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				if (SelItem)
				{
					SelItem->DWBHistory.StepAdd(SelItem);
					SelItem->SetMagicLevel((rand() % 100) + 1);
					SelItem->SetDWB(rand() + (rand() << 16));
					SelItem->Decoded = 0;
					UpdateTab2();
				}
			}
			break;
			}
			break;
		case IDC_TAB2_HistoryBack:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (SelItem) {
					SelItem->DWBHistory.StepBack(SelItem);
					SelItem->Decoded = 0;
					UpdateTab2();
					InvalidateRect(hTabDialog, NULL, FALSE);
				}
			}
			break;
		case IDC_TAB2_HistoryNext:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (SelItem) {
					SelItem->DWBHistory.StepNext(SelItem);
					SelItem->Decoded = 0;
					UpdateTab2();
					InvalidateRect(hTabDialog, NULL, FALSE);
				}
			}
			break;
		case IDC_TAB2_Batch:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				HMENU hMenu = GetSubMenu(hBatchMenu, 1);
				POINT Pos;
				GetCursorPos(&Pos);
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
					Pos.x, Pos.y, 0, hWnd, NULL);
			}
			break;
			}
			break;
		case IDC_TAB2_ExpertMode:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				if (IsDlgButtonChecked(hTabDialog, IDC_TAB2_ExpertMode))
				{
#if BUGGYMESSAGES == 1
					MessageBox(hWnd, "You really shouldn't do this! One wrong number kills your character!", PROGR
						AMNAME, MB_OK | MB_ICONSTOP | MB_APPLMODAL);
#endif
					if (hExpertBox) {
						DestroyWindow(hExpertBox);
					}
					hExpertBox = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TAB2E), hTabDialog, (DLGPROC)&Tab2EDialogProc);
					UpdateTab2();
				}
				else
				{
					SendMessage(hExpertBox, WM_CLOSE, 0, 0);
				}
			}
			break;
			}
			break;
#if INVGRIDS > 0
		case IDC_TAB2_OpenCube:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (hCube) {
					DestroyWindow(hCube);
				}
				else {
					CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2Grid), hTabDialog, (DLGPROC)&Tab2ExGridDialogP
						roc, CNT_CUBE);
				}
			}
			break;
		case IDC_TAB2_OpenBelt:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (hBelt) {
					DestroyWindow(hBelt);
				}
				else {
					CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2Grid), hTabDialog, (DLGPROC)&Tab2ExGridDialogP
						roc, CNT_BELT);
				}
			}
			break;
		case IDC_TAB2_OpenStash:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (hStash) {
					DestroyWindow(hStash);
				}
				else {
					CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TAB2Grid), hTabDialog, (DLGPROC)&Tab2ExGridDialogP
						roc, CNT_STASH);
				}
			}
			break;
#endif
		}
	}
	return false;
	case WM_VALIDATE:
	{
		for (Item* i = Items; i != 0; i = i->Next())
		{
			if (i->Busy)
			{
				ErrorBox("Search Treads still running!\nClose them first");
				return true;
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		if (hExpertBox) {
			SendMessage(hExpertBox, WM_CLOSE, 0, 0);
		}
	}
	return false;
	}
	return false;
}
#if INVGRIDS > 0
LRESULT CALLBACK Tab2ExGridDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		RECT* r;
		if (lParam == CNT_CUBE)
		{
			r = &rCube;
			hCube = hWnd;
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenCube, BST_CHECKED);
			SetWindowText(hWnd, "Cube Grid");

			r->right = InvGrids.xCube * 28 + 1;
			r->bottom = InvGrids.yCube * 28 + 1;
		}
		else if (lParam == CNT_BELT)
		{
			r = &rBelt;
			hBelt = hWnd;
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenBelt, BST_CHECKED);
			SetWindowText(hWnd, "Belt Grid");
			r->right = InvGrids.xBelt * 28 + 1;
			r->bottom = InvGrids.yBelt * 28 + 1;
		}
		else if (lParam == CNT_STASH)
		{
			r = &rStash;
			hStash = hWnd;
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenStash, BST_CHECKED);
			SetWindowText(hWnd, "Stash Grid");
			r->right = InvGrids.xStash * 28 + 1;
			r->bottom = InvGrids.yStash * 28 + 1;
		}
		if (r->top)
		{
			SetWindowPos(hWnd, HWND_TOP,
				r->left, r->top,
				r->right, r->bottom, SWP_NOACTIVATE);
		}
		else
		{
			SetWindowPos(hWnd, HWND_TOP,
				r->left, r->top,
				r->right, r->bottom, SWP_NOMOVE | SWP_NOACTIVATE);
		}
		SetWindowLong(hWnd, GWL_USERDATA, lParam);
		ShowWindow(hWnd, SW_SHOWNOACTIVATE);
	}
	return true;
	case WM_COMMAND:
	{
		ProcessItemMessage(hWnd, wParam, lParam);
	}
	return false;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return false;
	case WM_DESTROY:
	{
		switch (GetWindowLong(hWnd, GWL_USERDATA))
		{
		case CNT_CUBE:
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenCube, BST_UNCHECKED);
			hCube = 0;
			GetWindowRect(hWnd, &rCube);
			break;
		case CNT_BELT:
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenBelt, BST_UNCHECKED);
			hBelt = 0;
			GetWindowRect(hWnd, &rBelt);
			break;
		case CNT_STASH:
			CheckDlgButton(hTabDialog, IDC_TAB2_OpenStash, BST_UNCHECKED);
			hStash = 0;
			GetWindowRect(hWnd, &rStash);
			break;
		}
	}
	return false;
	}
	return false;
}
#endif
bool FoundCube = false;
bool FindCube()
{
	for (class Item* i = Items; i != 0; i = i->Next())
	{
		if (i->ItemCode() == 0x1340 || i->ItemCode() == ' xob')
		{
			FoundCube = true;
			return true;
		}
	}
	FoundCube = false;
	return false;
}
void DrawGrid(HDC hdc, int Container)
{
	int boxesX, boxesY;
	switch (Container)
	{
	case CNT_INVENTORY:
		boxesX = InvGrids.xInventory;
		boxesY = InvGrids.yInventory;
		break;
	case CNT_STASH:
		boxesX = InvGrids.xStash;
		boxesY = InvGrids.yStash;
		break;
	case CNT_CUBE:
		boxesX = InvGrids.xCube;
		boxesY = FindCube() ? InvGrids.yCube : 0;
		break;
	case CNT_BELT:
		boxesX = InvGrids.xBelt;
		boxesY = InvGrids.xBelt;
		break;
	case CNT_COPYBUFFER:
		boxesX = 2;
		boxesY = 4;
	}
	SelectObject(hdc, hBrushBlack);
	SelectObject(hdc, hPenWhite);
	HDC hdcbitmap = CreateCompatibleDC(hdc);
	SelectObject(hdcbitmap, hBmpNotPlaceable);
	for (int y = 3; y >= 0; y--)
	{
		for (int x = 0; x < boxesX; x++)
		{
			Rectangle(hdc, x * 29, y * 29, x * 29 + 30, y * 29 + 30);
			if (y >= boxesY)
			{
				BitBlt(hdc, x * 29 + 1, y * 29 + 1, 28, 28, hdcbitmap, 0, 0, SRCCOPY);
			}
		}
	}
	DeleteDC(hdcbitmap);
}
inline void DrawItemFrames(HDC hdc, Item* i, int x, int y, int sx, int sy)
{
	if (!i->Info)
		i->FindInfo();
	if (!i->Decoded)
		i->Decode();
	if (SelItem == i)
	{
		// Draw Selection Box
		SelectObject(hdc, hBrushNull);
		SelectObject(hdc, SelItem->Busy ? hPenBusy : hPenGreen);
		Rectangle(hdc, x, y, x + sx + 1, y + sy + 1);
	}
	if (i && !i->isWearable())
	{
		// Draw Red Frame
		SelectObject(hdc, hBrushNull);
		SelectObject(hdc, hPenUnwearable);
		Rectangle(hdc, x + 1, y + 1, x + sx - 1, y + sy - 1);
	}
}
void DrawItem(HDC hdc, class Item* i)
{
	int x = i->xPixelPos() + 2;
	int y = i->yPixelPos() + 2;
	int sx = i->xPixelSize();
	int sy = i->yPixelSize();
	HBITMAP bmp = i->GetBitmap();

	HDC hdcbitmap = CreateCompatibleDC(hdc);
	SelectObject(hdcbitmap, bmp);
	BitBlt(hdc, x, y, sx, sy, hdcbitmap, 0, 0, SRCCOPY);
	DeleteDC(hdcbitmap);
	DrawItemFrames(hdc, i, x, y, sx, sy);
}
void DrawItemSimple(HDC hdc, class Item* i)
{
	int sx = i->xPixelSize();
	int sy = i->yPixelSize();
	HBITMAP bmp = i->GetBitmap();

	HDC hdcbitmap = CreateCompatibleDC(hdc);
	SelectObject(hdcbitmap, bmp);
	BitBlt(hdc, 1, 1, sx, sy, hdcbitmap, 0, 0, SRCCOPY);
	DeleteDC(hdcbitmap);
	DrawItemFrames(hdc, i, 1, 1, sx, sy);
}
#define BODY_POTION 0
#define BODY_HELM 1
#define BODY_AMULET 2
#define BODY_ARMOR 3
#define BODY_HANDR 4
#define BODY_HANDL 5
#define BODY_RINGR 6
#define BODY_RINGL 7
#define BODY_BELT 8
#define BODY_BOOTS 9
#define BODY_GLOVES 10
struct
{
	int xOrig, yOrig;
	int xSize, ySize;
	int equalto;
}
BodyParts[] =
{
 { 0, 0, 0, 0, 0 }, // Nothing: Belt
 { 131, 2, 60, 60, 0 }, // Helm
 { 204, 31, 30, 30, 0 }, // Amulet
 { 131, 74, 60, 88, 0 }, // Armor
 { 16, 46, 60,115, 0 }, // Weapon R
 { 247, 46, 60,115, 0 }, // Weapon L
 { 90,176, 30, 30, 7 }, // Ring R
 { 204,176, 30, 30, 6 }, // Ring L
 { 132,176, 60, 30, 0 }, // Belt
 { 247,176, 60, 60, 0 }, // Boots
 { 16,176, 60, 60 ,0 } // Gloves
};
int BodyPartsNum = 10;
void DrawItemBody(HDC hdc, class Item* i)
{
	if (i->Container() != CNT_BODY) return;

	int c = i->BodyCode();
	if (c <= 0 || c > BodyPartsNum) return;

	int x = BodyParts[c].xOrig + (BodyParts[c].xSize - i->xPixelSize()) / 2;
	int y = BodyParts[c].yOrig + (BodyParts[c].ySize - i->yPixelSize()) / 2;
	int sx = i->xPixelSize();
	int sy = i->yPixelSize();
	HBITMAP bmp = i->GetBitmap();

	HDC hdcbitmap = CreateCompatibleDC(hdc);
	SelectObject(hdcbitmap, bmp);
	BitBlt(hdc, x, y, sx, sy, hdcbitmap, 0, 0, SRCCOPY);
	DeleteDC(hdcbitmap);
	DrawItemFrames(hdc, i, x, y, sx, sy);
}
void UpdateTab2()
{
	if (!SelItem)
	{
		RTFStreamSend(hTabDialog, IDC_TAB2_RichText, "");
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_AttrRandom), FALSE);
		if (hExpertBox)
			UpdateTab2E();
		return;
	}
	if (!SelItem->Info) SelItem->FindInfo();
	SelItem->Decode();
	RTFStreamSend(hTabDialog, IDC_TAB2_RichText, SelItem->RichText());
	if (SelItem->Quality() == MAGICITEM ||
		SelItem->Quality() == RAREITEM ||
		SelItem->Quality() == SETITEM ||
		SelItem->Quality() == UNIQUEITEM)
	{
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_AttrRandom), TRUE);
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_HistoryBack), SelItem->DWBHistory.isBack());
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_HistoryNext), SelItem->DWBHistory.isNext());
	}
	else
	{
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_AttrRandom), FALSE);
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_HistoryBack), FALSE);
		EnableWindow(GetDlgItem(hTabDialog, IDC_TAB2_HistoryNext), FALSE);
	}
	if (hExpertBox)
		UpdateTab2E();
}
void MakeSelection(class Item* i)
{
	SelItem = i;
	UpdateTab2();
	InvalidateRect(hTabDialog, NULL, FALSE);
}
Item* GetItemAtBodyPlace(int BodyPlace)
{
	for (Item* I = Items; I != 0; I = I->Next())
	{
		if (I->Container() != CNT_BODY)
			continue;

		if (I->BodyCode() != BodyPlace) continue;

		return I;
	}
	return 0;
}
bool ItemPlaceable(int Container, int xPos, int yPos, int xSize, int ySize, int BodyPlace)
{
	// First Sanity Check
	switch (Container)
	{
	case CNT_CUBE:
		// Is there a cube in the Inventory
		if (!FoundCube) return false;
		// Check if it's a Horadric Cube in a Horadric Cube
		if (SelItem)
			if (SelItem->ItemCode() == 0x1340 || SelItem->ItemCode() == ' xob') return false;
			else if (SelInfo)
				if (SelInfo->ItemCode == 0x1340) return false;
		break;
	case CNT_BELT:
		// Check if it's a different Item than a Potion
		if (BodyPlace != 0) return false;

		break;
	}
	// Coordinates Checker
	switch (Container)
	{
	default:
		return false;
	case CNT_BODY:
	{
		for (int z = 1; z <= BodyPartsNum; z++)
		{
			if (xPos >= BodyParts[z].xOrig && xPos <= BodyParts[z].xOrig + BodyParts[z].xSize &&
				yPos >= BodyParts[z].yOrig && yPos <= BodyParts[z].yOrig + BodyParts[z].ySize)
			{
				// Check if occupied
				Item* O = GetItemAtBodyPlace(z);
				if (O != 0 && O != SelItem) return false;
				if (BodyPlace == BODY_RINGR && (z == BODY_RINGL || z == BODY_RINGR)) return true;
				if ((BodyPlace == BODY_HANDR || BodyPlace == BODY_HANDL) && z == BODY_HANDL) {
					O = GetItemAtBodyPlace(BODY_HANDR);
					if (O && O->Info && O->Info->Hands > 1) return false;
					else return true;
				}
				if ((BodyPlace == BODY_HANDR || BodyPlace == BODY_HANDL) && z == BODY_HANDR) {
					O = GetItemAtBodyPlace(BODY_HANDL);
					if (O && O->Info && O->Info->Hands > 1) return false;
					else return true;
				}
				if (BodyPlace == z) return true;
				return false;
			}
		}
		return false;
	}
	return false;
	case CNT_COPYBUFFER:
	{
		if (CopyBuffer)
			return false;
		else
			return true;
	}
	case CNT_INVENTORY:
	case CNT_STASH:
	case CNT_CUBE:
	case CNT_BELT:
	{
		xPos = (xPos - 2) / 29;
		yPos = (yPos - 2) / 29;
		if (Container == CNT_INVENTORY) {
			if (xPos + xSize > InvGrids.xInventory) return false;
			if (yPos + ySize > InvGrids.yInventory) return false;
		}
		else if (Container == CNT_STASH) {
			if (xPos + xSize > InvGrids.xStash) return false;
			if (yPos + ySize > InvGrids.yStash) return false;
		}
		else if (Container == CNT_CUBE) {
			if (xPos + xSize > InvGrids.xCube) return false;
			if (yPos + ySize > InvGrids.yCube) return false;
		}
		else if (Container == CNT_BELT) {
			if (xPos + xSize > InvGrids.xBelt) return false;
			if (yPos + ySize > InvGrids.yBelt) return false;
		}
		for (Item* I = Items; I != 0; I = I->Next())
		{
			if (!I->Info)
				I->FindInfo();
			if (I->Container() != Container)
				continue;
			if (SelItem && SelItem == I) continue;
			if (xPos >= I->xPos() + I->xSize()) continue;
			if (xPos + xSize <= I->xPos()) continue;
			if (yPos >= I->yPos() + I->ySize()) continue;
			if (yPos + ySize <= I->yPos()) continue;
			return false;
		}
		return true;
	}
	}
}
bool SelItemPlaceable(int IntoContainer, int xPos, int yPos, POINT Pos, HWND hPos)
{
	bool retbool = false;
	int xSize, ySize, BodyPlace;
	if (SelItem) {
		xSize = SelItem->xSize();
		ySize = SelItem->ySize();
		BodyPlace = SelItem->BodyPlace();
	}
	else {
		xSize = SelInfo->SizeX;
		ySize = SelInfo->SizeY;
		BodyPlace = SelInfo->BodyPlace;
	}
	return ItemPlaceable(IntoContainer, xPos, yPos, xSize, ySize, BodyPlace);
}
bool SelItemDeleteable()
{
	if (!SelItem) return false;
	if (SelItem == CopyBuffer) return true;
	// Check if we're deleting the Horadric Cube
	if (SelItem->ItemCode() == 0x1340 || SelItem->ItemCode() == ' xob')
	{
		// Count Cubes :-)
		int cn = 0;
		for (Item* I = Items; I != 0; I = I->Next())
			if (I->ItemCode() == 0x1340 || I->ItemCode() == ' xob')
				cn++;
		// Last Cube ?
		if (cn == 1)
			for (Item* i = Items; i != 0; i = i->Next())
				if (i->Container() == CNT_CUBE)
				{
					MessageBox(hTabDialog, "Horadric Cube is not empty!\nPlease delete all contained Items first.", PROGRAMNAME, MB_OK | MB_ICONSTOP | MB_APPLMODAL);
					return false;
				}
	}
	return true;
}
void ShowContextMenu(HWND hWnd, Item* I, int xPos, int yPos)
{
	POINT Pos;
	GetCursorPos(&Pos);
	if (I)
	{
		if (I->Busy) return;
		if (!I->Info) I->FindInfo();
		HMENU hMenu = CreatePopupMenu();

		// Selection Popup
		if (I->Info->QualityMask)
		{
			HMENU hSubMenu = CreatePopupMenu();
			if (I->Info->QualityMask & 1)
			{
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == CRUDEITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualityCrude, "Crude");
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == USUALITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualityRegular, "Regular");
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == SUPERIORITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualitySuperior, "Superior");
			}
			if (I->Info->QualityMask & 2)
			{
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == MAGICITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualityMagical, "Magical");
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == RAREITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualityRare, "Rare");
			}
			if (I->Info->QualityMask & 4 && I->Quality() == SETITEM)
			{
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == SETITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualitySet, "Set Item");
			}
			if (I->Info->QualityMask & 8 && I->Quality() == UNIQUEITEM)
			{
				AppendMenu(hSubMenu, MF_STRING | (I->Quality() == UNIQUEITEM ? MF_CHECKED : MF_UNCHECKED),
					IDR_TAB2_SetQualityUnique, "Unique Item");
			}
			AppendMenu(hMenu, MF_POPUP, (UINT)hSubMenu, "Item Quality");
		}

		if (I->Quality() == MAGICITEM) {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_AttributeSelect, "Select Magical Attributes");
		}
		if (I->Quality() == RAREITEM) {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_AttributeSelect, "Select Rare Attributes");
		}

		if (I->Socketed()) {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_GemsEdit, "Change Inserted Gems");
		}

		if (I->ItemRecordID() == IT_103EAR || I->ItemRecordID() == IT_104EAR) {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_SetEarProperties, "Change Ear Properties");
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
		}

		if (I->ItemCode() == 0x1190 || I->ItemCode() == ' nir') {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_ChangeRingImage, "Change Ring Image");
		}
		if (I->ItemCode() == 0x1170 || I->ItemCode() == ' uma') {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_ChangeAmuletImage, "Change Amulet Image");
		}

		// Quantity Selection Entry
		if (I->Info->Quantity)
		{
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_SetQuantity, "Set Quantity");
		}

		// Durability Selection Entry
		if (I->Info->Durability)
		{
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_SetDurability, "Set Durability");
		}

		// Defense Selection Entry
		if (I->Info->ACMin > 0)
		{
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_SetDefense, "Select Defense");
		}

		// Seperator
		if (I->Info->Quantity || I->Info->Durability || I->Info->QualityMask)
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);

		// Add Item Flags Selections
		if (I->Socketed() || I->Socketable() || RegOptions.AllItemsSocketable)
		{
			AppendMenu(hMenu, MF_STRING | (I->Socketed() ? MF_CHECKED : MF_UNCHECKED),
				IDR_TAB2_Socketed, "Socketed");
		}

		AppendMenu(hMenu, MF_STRING | (I->Identified() ? MF_CHECKED : MF_UNCHECKED),
			IDR_TAB2_Identified, "Identified");

		AppendMenu(hMenu, MF_STRING | (I->Starter() ? MF_CHECKED : MF_UNCHECKED),
			IDR_TAB2_Starter, "Starter Item");

		AppendMenu(hMenu, MF_SEPARATOR, 0, 0);

		AppendMenu(hMenu, MF_STRING, IDR_TAB2_Cut, "Cut to Clipboard");
		AppendMenu(hMenu, MF_STRING, IDR_TAB2_Copy, "Copy to Clipboard");
		AppendMenu(hMenu, MF_STRING, IDR_TAB2_Delete, "Delete");

		// Show Context Menu
		TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
			Pos.x, Pos.y, 0, hTabDialog, NULL);

		DestroyMenu(hMenu);
	}
	else
	{
		if (!IsClipboardFormatAvailable(ClipBoardFormat) && !IsClipboardFormatAvailable(ClipBoardFormatJohnDoe)) {
			HMENU hMenu = CreatePopupMenu();

			AppendMenu(hMenu, MF_STRING | MF_GRAYED, 0, "Paste: No Item in Clipboard");

			// Show Context Menu
			TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
				Pos.x, Pos.y, 0, hWnd, NULL);

			DestroyMenu(hMenu);
			return;
		}
		if (!OpenClipboard(hMainDialog))
			return;
		bool ok = false;
		if (IsClipboardFormatAvailable(ClipBoardFormat))
		{
			HGLOBAL Clip = GetClipboardData(ClipBoardFormat);
			struct JamClipboardHead* JamHead = (JamClipboardHead*)GlobalLock(Clip);
			ok = ItemPlaceable(ContainerFromHWND(hWnd), xPos, yPos, JamHead->xSize, JamHead->ySize, JamHead->BodyPlace);
			GlobalUnlock(Clip);
		}
		else if (IsClipboardFormatAvailable(ClipBoardFormatJohnDoe))
		{
			if (hWnd != hCopyBuffer) {
				HMENU hMenu = CreatePopupMenu();

				AppendMenu(hMenu, MF_STRING | MF_GRAYED, 0, "Paste: John Doe's Item only in CopyBuffer");

				// Show Context Menu
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
					Pos.x, Pos.y, 0, hWnd, NULL);

				DestroyMenu(hMenu);
				return;
			}
			ok = true;
		}
		CloseClipboard();

		HMENU hMenu = CreatePopupMenu();
		if (!ok) {
			AppendMenu(hMenu, MF_STRING | MF_GRAYED, 0, "Paste: Not Placeable here!");
		}
		else {
			AppendMenu(hMenu, MF_STRING, IDR_TAB2_Paste, "Paste from Clipboard");
		}
		// Show Context Menu
		TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
			Pos.x, Pos.y, 0, hWnd, NULL);

		xStartPos = xPos;
		yStartPos = yPos;
		DestroyMenu(hMenu);
	}
}
LRESULT CALLBACK Tab2InventoryProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (hWnd == hInv)
		{
			DrawGrid(hdc, CNT_INVENTORY);
			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == CNT_INVENTORY)
				{
					DrawItem(hdc, i);
				}
			}
		}
		else if (hWnd == hBelt)
		{
			DrawGrid(hdc, CNT_BELT);
			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == CNT_BELT)
				{
					DrawItem(hdc, i);
				}
			}
		}
		else if (hWnd == hCube)
		{
			DrawGrid(hdc, CNT_CUBE);
			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == CNT_CUBE)
				{
					DrawItem(hdc, i);
				}
			}
		}
		else if (hWnd == hStash)
		{
			DrawGrid(hdc, CNT_STASH);
			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == CNT_STASH)
				{
					DrawItem(hdc, i);
				}
			}
		}
		else if (hWnd == hCopyBuffer)
		{
			DrawGrid(hdc, CNT_COPYBUFFER);
			if (CopyBuffer)
			{
				DrawItemSimple(hdc, CopyBuffer);
			}
		}
		else if (hWnd == hBody)
		{
			// Put bg image
			{
				HDC hdcbitmap = CreateCompatibleDC(hdc);
				SelectObject(hdcbitmap, hBmpBodyWhole);
				BITMAP bmpinfo;
				GetObject(hBmpBodyWhole, sizeof bmpinfo, &bmpinfo);
				BitBlt(hdc, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, hdcbitmap, 0, 0, SRCCOPY);
				DeleteDC(hdcbitmap);
			}
			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == CNT_BODY)
				{
					DrawItemBody(hdc, i);
				}
			}
		}

		EndPaint(hWnd, &ps);
	}
	return true;
	case WM_LBUTTONDOWN:
	{
		SetFocus(hWnd);
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		if (hWnd == hInv || hWnd == hBelt || hWnd == hCube || hWnd == hStash)
		{
			int cd = ContainerFromHWND(hWnd);

			for (Item* i = Items; i != 0; i = i->Next())
			{
				if (i->Container() == cd)
				{
					if (i->isInRegion(xPos - 2, yPos - 2))
					{
						MakeSelection(i);
						break;
					}
				}
			}
		}
		else if (hWnd == hBody)
		{
			for (int z = 1; z <= BodyPartsNum; z++)
			{
				if (xPos >= BodyParts[z].xOrig && xPos <= BodyParts[z].xOrig + BodyParts[z].xSize &&
					yPos >= BodyParts[z].yOrig && yPos <= BodyParts[z].yOrig + BodyParts[z].ySize)
				{
					for (Item* i = Items; i != 0; i = i->Next())
					{
						if (i->Container() == CNT_BODY)
						{
							if (z != i->BodyCode()) continue;
							MakeSelection(i);
							break;
						}
					}
				}
			}
		}
		else if (hWnd == hCopyBuffer)
		{
			if (CopyBuffer)
			{
				MakeSelection(CopyBuffer);
			}
		}
		if (SelItem && !SelItem->Busy)
		{
			MouseButtonDown = true;
			xStartPos = xPos;
			yStartPos = yPos;
			FindCube();
			SetCapture(hWnd);
		}
	}
	return true;
	case WM_RBUTTONDOWN:
	{
		if (MouseButtonDown) return false;
		SetFocus(hWnd);
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		POINT pt = { xPos, yPos };
		ClientToScreen(hWnd, &pt);
		if (hWnd == hInv || hWnd == hBelt || hWnd == hCube || hWnd == hStash)
		{
			int cd = ContainerFromHWND(hWnd);
			Item* i = Items;
			for (i != 0; i = i->Next();)
			{
				if (i->Container() == cd)
				{
					if (i->isInRegion(xPos - 2, yPos - 2))
					{
						MakeSelection(i);
						break;
					}
				}
			}
			if (i == 0)
				MakeSelection(0);
		}
		else if (hWnd == hBody)
		{
			MakeSelection(0);
			for (int z = 1; z <= BodyPartsNum; z++)
			{
				if (xPos >= BodyParts[z].xOrig && xPos <= BodyParts[z].xOrig + BodyParts[z].xSize &&
					yPos >= BodyParts[z].yOrig && yPos <= BodyParts[z].yOrig + BodyParts[z].ySize)
				{
					Item* i = Items;
					for (i != 0; i = i->Next();)
					{
						if (i->Container() == CNT_BODY)
						{
							if (z != i->BodyCode()) continue;
							MakeSelection(i);
							break;
						}
					}
					if (i == 0)
						MakeSelection(0);
				}
			}
		}
		else if (hWnd == hCopyBuffer)
		{
			if (CopyBuffer)
				MakeSelection(CopyBuffer);
			else
				MakeSelection(0);
		}

		ShowContextMenu(hWnd, SelItem, xPos, yPos);
	}
	return true;
	case WM_CAPTURECHANGED:
	{
		MouseButtonDown = false;
	}
	return true;
	case WM_MOUSEMOVE:
	{
		if (!MouseButtonDown) return false;
		if (!(abs(xStartPos - LOWORD(lParam)) > xDragTol ||
			abs(yStartPos - HIWORD(lParam)) > yDragTol)) return false;
		MouseButtonMoved = true;
		// Mouse Position
		POINT Pos;
		GetCursorPos(&Pos);
		HWND hPos = WindowFromPoint(Pos);
		RECT rect;
		GetWindowRect(hPos, &rect);
		int xPos = Pos.x - rect.left;
		int yPos = Pos.y - rect.top;
		HCURSOR hSetCur = hCurNo;
		int CNT = ContainerFromHWND(hPos);
		if (CNT == CNT_COPYBUFFER)
		{
			if (SelItemPlaceable(CNT, xPos, yPos, Pos, hPos))
				hSetCur = hCurMoveCopy;
			else
				hSetCur = hCurCross;
		}
		else if (CNT)
		{
			if (SelItemPlaceable(CNT, xPos, yPos, Pos, hPos))
				hSetCur = hCurMove;
			else
				hSetCur = hCurCross;
		}
		SetCursor(hSetCur);
	}
	return true;
	case WM_LBUTTONUP:
	{
		MouseButtonDown = false;
		ReleaseCapture();
		if (!MouseButtonMoved) break;
		MouseButtonMoved = false;
		// Mouse Position
		POINT Pos;
		GetCursorPos(&Pos);
		HWND hPos = WindowFromPoint(Pos);
		RECT rect;
		GetWindowRect(hPos, &rect);
		int xPos = Pos.x - rect.left;
		int yPos = Pos.y - rect.top;
		int CNT = ContainerFromHWND(hPos);
		if (SelItemPlaceable(CNT, xPos, yPos, Pos, hPos))
		{
			if (!SelItem && CNT != CNT_COPYBUFFER)
			{
				// Create New Item
				SelItem = CreateItem(&Items, SelInfo);
			}
			else if (!SelItem && CNT == CNT_COPYBUFFER)
			{
				// Create New Item in Copy Buffer
				if (CopyBuffer) {
					delete CopyBuffer;
					CopyBuffer = 0;
				}
				SelItem = CreateItem(&CopyBuffer, SelInfo);
			}
			else if (SelItem == CopyBuffer)
			{
				SelItem = ItemDuplicate(CopyBuffer);
				SelItem->LoadItemRecord(CopyBuffer->GetItemRecord());
				if (CopyBuffer->Gems)
				{
					for (Item* G = CopyBuffer->Gems; G != 0; G = G->Next())
					{
						Item* GN = ItemDuplicate(G);
						GN->LoadItemRecord(G->GetItemRecord());

						if (SelItem->Gems)
							SelItem->Gems->Attach(GN);
						else
							SelItem->Gems = GN;
					}
				}
				Items->Attach(SelItem);
			}
			switch (CNT)
			{
			case CNT_INVENTORY:
			case CNT_STASH:
			case CNT_CUBE:
			case CNT_BELT:
			{
				xPos = (xPos - 2) / 29;
				yPos = (yPos - 2) / 29;
				SelItem->SetCoordinates(CNT, xPos, yPos);
			}
			break;
			case CNT_BODY:
			{
				for (int z = 1; z <= BodyPartsNum; z++)
				{
					if (xPos >= BodyParts[z].xOrig && xPos <= BodyParts[z].xOrig + BodyParts[z].xSize &&
						yPos >= BodyParts[z].yOrig && yPos <= BodyParts[z].yOrig + BodyParts[z].ySize)
					{
						SelItem->SetCoordinates(CNT, z, 0);
					}
				}
			}
			break;
			case CNT_COPYBUFFER:
			{
				if (SelItem != CopyBuffer) {

					if (CopyBuffer) {
						delete CopyBuffer;
						CopyBuffer = 0;
					}

					CopyBuffer = ItemDuplicate(SelItem);
					CopyBuffer->LoadItemRecord(SelItem->GetItemRecord());
					if (SelItem->Gems)
					{
						for (Item* G = SelItem->Gems; G != 0; G = G->Next())
						{
							Item* GN = ItemDuplicate(G);
							GN->LoadItemRecord(G->GetItemRecord());
							if (CopyBuffer->Gems)
								CopyBuffer->Gems->Attach(GN);
							else
								CopyBuffer->Gems = GN;
						}
					}

					SelItem = CopyBuffer;
				}
			}
			break;
			}
			MakeSelection(SelItem);
			UpdateTab2();
			InvalidateRect(hTabDialog, NULL, FALSE);
			UpdateWindow(hTabDialog);
		}
		else
		{
			MessageBeep(MB_ICONHAND);
		}
	}
	return true;
	case WM_KEYDOWN:
	{
		switch ((int)wParam)
		{
		case VK_DELETE:
			if (MouseButtonDown) break;
			if (!SelItem) break;
			if (SelItem->Busy) break;
			if (SelItemDeleteable()) {
				if (SelItem == CopyBuffer) {
					delete CopyBuffer;
					CopyBuffer = 0;
				}
				else {
					SelItem->Delete();
				}
				MakeSelection(0);
			}
			break;
		}
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDR_TAB2_Paste)
		{
			if (!IsClipboardFormatAvailable(ClipBoardFormat) && !IsClipboardFormatAvailable(ClipBoardFormatJohnDoe))
				return false;
			if (!OpenClipboard(hMainDialog))
				return false;
			int xPos = xStartPos;
			int yPos = yStartPos;
			int CNT = ContainerFromHWND(hWnd);

			Item* N = 0;
			if (IsClipboardFormatAvailable(ClipBoardFormat))
			{
				HGLOBAL Clip = GetClipboardData(ClipBoardFormat);
				BYTE* ClipData = (BYTE*)GlobalLock(Clip);

				struct JamClipboardHead* JamHead = (JamClipboardHead*)ClipData;
				if (!ItemPlaceable(CNT, xPos, yPos, JamHead->xSize, JamHead->ySize, JamHead->BodyPlace)) {
					GlobalUnlock(Clip);
					CloseClipboard();
					return false;
				}
				BYTE* o = ClipData + sizeof(JamClipboardHead);
				int s = GlobalSize(Clip);
				switch (JamHead->IID)
				{
				default:
					GlobalUnlock(Clip);
					CloseClipboard();
					return false;
				case IT_103:
					N = new Item103;
					N->LoadItemRecord(o);
					o += N->ItemRecordLength();
					while (s - (o - ClipData) >= 27)
					{
						Item* G = new Item103;
						G->LoadItemRecord(o);
						o += G->ItemRecordLength();
						if (N->Gems)
							N->Gems->Attach(G);
						else
							N->Gems = G;
					}
					break;
				case IT_103EAR:
					N = new Item103Ear;
					N->LoadItemRecord(o);
					o += N->ItemRecordLength();
					break;
				case IT_104EX:
					N = new Item104Ex;
					N->LoadItemRecord(o);
					o += N->ItemRecordLength();
					while (s - (o - ClipData) >= 15)
					{
						Item* G = new Item104Sm;
						G->LoadItemRecord(o);
						o += G->ItemRecordLength();
						if (N->Gems)
							N->Gems->Attach(G);
						else
							N->Gems = G;
					}
					break;
				case IT_104SM:
					N = new Item104Sm;
					N->LoadItemRecord(o);
					o += N->ItemRecordLength();
					break;
				case IT_104EAR:
					N = new Item104Ear;
					N->LoadItemRecord(o);
					o += N->ItemRecordLength();
					break;
				}
				GlobalUnlock(Clip);
			}
			else if (IsClipboardFormatAvailable(ClipBoardFormatJohnDoe))
			{
				HGLOBAL Clip = GetClipboardData(ClipBoardFormatJohnDoe);
				BYTE* ClipData = (BYTE*)GlobalLock(Clip);

				DWORD nGems = *(DWORD*)ClipData;
				BYTE* o = ClipData + 4;
				int s = GlobalSize(Clip);
				N = new Item103;
				N->LoadItemRecord(o);
				o += N->ItemRecordLength();
				for (DWORD z = 0; z < nGems && s - (o - ClipData) >= 27; z++)
				{
					Item* G = new Item103;
					G->LoadItemRecord(o);
					o += G->ItemRecordLength();

					if (N->Gems)
						N->Gems->Attach(G);
					else
						N->Gems = G;
				}
				GlobalUnlock(Clip);
			}
			CloseClipboard();
			if (N)
			{
				switch (CNT)
				{
				case CNT_INVENTORY:
				case CNT_STASH:
				case CNT_CUBE:
				case CNT_BELT:
				{
					xPos = (xPos - 2) / 29;
					yPos = (yPos - 2) / 29;
					N->SetCoordinates(CNT, xPos, yPos);
					if (Items)
						Items->Attach(N);
					else
						Items = N;
				}
				break;
				case CNT_BODY:
				{
					for (int z = 1; z <= BodyPartsNum; z++)
					{
						if (xPos >= BodyParts[z].xOrig && xPos <= BodyParts[z].xOrig + BodyParts[z].xSize &&
							yPos >= BodyParts[z].yOrig && yPos <= BodyParts[z].yOrig + BodyParts[z].ySize)
						{
							N->SetCoordinates(CNT, z, 0);
							if (Items)
								Items->Attach(N);
							else
								Items = N;
						}
					}
				}
				break;
				case CNT_COPYBUFFER:
				{
					if (CopyBuffer) {
						delete CopyBuffer;
						CopyBuffer = 0;
					}
					CopyBuffer = N;
				}
				break;
				}
				MakeSelection(N);
				UpdateTab2();
				InvalidateRect(hTabDialog, NULL, FALSE);
				UpdateWindow(hTabDialog);
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}