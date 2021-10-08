// ToolTips.cpp for Jamella's Diablo 2 Editor
#include "JamellaD2E.h"
struct
{
	int ToolSet;
	int CtrlID;
	char* Text;
}
ToolTips[] =
{
{ IDD_TAB1,IDC_TAB1_Name, "" },
{ IDD_TAB1,IDC_TAB1_Rename, "" },
{ IDD_TAB1,IDC_TAB1_Class, "Changes your character's class.\n\rOptions: Amazon, Barbarian, Necromancer, Sorceress, Palad in.\n\rNote that when changing your class, your character's skills are changed too. Please adapt these in the Skills Tab." },
{ IDD_TAB1,IDC_TAB1_Level, "" },
{ IDD_TAB1,IDC_TAB1_Experience, "" },
{ IDD_TAB1,IDC_TAB1_StartTown, "" },
{ IDD_TAB1,IDC_TAB1_Char1, "" },
{ IDD_TAB1,IDC_TAB1_Char2, "" },
{ IDD_TAB1,IDC_TAB1_Char3, "" },
{ IDD_TAB1,IDC_TAB1_Char4, "" },
{ IDD_TAB1,IDC_TAB1_Char5, "" },
{ IDD_TAB1,IDC_TAB1_Plus1, "" },
{ IDD_TAB1,IDC_TAB1_Plus2, "" },
{ IDD_TAB1,IDC_TAB1_Plus3, "" },
{ IDD_TAB1,IDC_TAB1_Plus4, "" },
{ IDD_TAB1,IDC_TAB1_Plus5, "" },
{ IDD_TAB1,IDC_TAB1_StatsLink, "" },
{ IDD_TAB1,IDC_TAB1_Batch, "" },
{ IDD_TAB1,IDC_TAB1_Hardcore, "" },
{ IDD_TAB1,IDC_TAB1_Dead, "" },
{ IDD_TAB1,IDC_TAB1_Difficulty, "" },
{ IDD_TAB1,IDC_TAB1_Health, "" },
{ IDD_TAB1,IDC_TAB1_HealthMax, "" },
{ IDD_TAB1,IDC_TAB1_Stamina, "" },
{ IDD_TAB1,IDC_TAB1_StaminaMax, "" },
{ IDD_TAB1,IDC_TAB1_Mana, "" },
{ IDD_TAB1,IDC_TAB1_ManaMax, "" },
{ IDD_TAB1,IDC_TAB1_GoldPerson, "" },
{ IDD_TAB1,IDC_TAB1_SetMaxGoldPerson, "" },
{ IDD_TAB1,IDC_TAB1_GoldStash, "" },
{ IDD_TAB1,IDC_TAB1_SetMaxGoldStash, "" },
{ IDD_TAB2,IDC_TAB2_Body, "" },
{ IDD_TAB2,IDC_TAB2_Inv, "" },
{ IDD_TAB2,IDC_TAB2_Cube, "" },
{ IDD_TAB2,IDC_TAB2_Belt, "" },
{ IDD_TAB2,IDC_TAB2_Stash, "" },
{ IDD_TAB2,IDC_TAB2_CopyBuffer, "This buffer field acts as an item duplicator.\r\nDrop an item into this buffer and you can copy it multiple times from it.After finished with duplicating you can delete it.\r\nYou can also load and save the contents of this buffer using the two buttons above." },
{ IDD_TAB2,IDC_TAB2_Save, "" },
{ IDD_TAB2,IDC_TAB2_Load, "" },
{ IDD_TAB2,IDC_TAB2_RichText, "" },
{ IDD_TAB2,IDC_TAB2_AttrRandom, "" },
{ IDD_TAB2,IDC_TAB2_Batch, "" },
{ IDD_TAB2,IDC_TAB2_ExpertMode, "" },
{ IDD_TAB2,IDC_TAB2_Save, "Clicking this button you can save the contents of the Copy Buffer into a file.\r\nThis file will contain the 27 bytes of item data.\r\nNo inserted gems will be saved along!" },
{ IDD_TAB2,IDC_TAB2_Load, "With this button you can load a saved item into the Copy Buffer below." },
{ IDD_TAB2,IDC_TAB2_ItemTree, "This item tree contains all of the items in the game. You can drag-drop entries into your inventory to create a new item." },
{ IDD_TAB3,IDC_TAB3_Track, "" },
{ IDD_TAB3,IDC_TAB3_Bonus, "" },
{ IDD_TAB3,IDC_TAB3_BonusPlus, "" },
{ IDD_TAB3,IDC_TAB3_Batch, "" },
{ IDD_TAB3,IDC_TAB3_Prop_Bitmap, "" },
{ IDD_TAB4,IDC_TAB4_Diff1, "" },
{ IDD_TAB4,IDC_TAB4_Diff2, "" },
{ IDD_TAB4,IDC_TAB4_Diff3, "" },
{ IDD_TAB4,IDC_TAB4_Act1, "" },
{ IDD_TAB4,IDC_TAB4_Act2, "" },
{ IDD_TAB4,IDC_TAB4_Act3, "" },
{ IDD_TAB4,IDC_TAB4_Act4, "" },
{ IDD_TAB4,IDC_TAB4_Act2_On, "" },
{ IDD_TAB4,IDC_TAB4_Act3_On, "" },
{ IDD_TAB4,IDC_TAB4_Act4_On, "" },
{ IDD_TAB4,IDC_TAB4_SelAll, "" },
{ IDD_TAB4,IDC_TAB4_SetAll1, "" },
{ IDD_TAB4,IDC_TAB4_SetAll2, "" },
{ IDD_TAB4,IDC_TAB4_CowLevel, "" },
{ IDD_TAB4,IDC_TAB4_Sel1, "" },
{ IDD_TAB4,IDC_TAB4_Sel2, "" },
{ IDD_TAB4,IDC_TAB4_Sel3, "" },
{ IDD_TAB4,IDC_TAB4_Sel4, "" },
{ IDD_TAB4,IDC_TAB4_Sel5, "" },
{ IDD_TAB4,IDC_TAB4_Sel6, "" },
{ IDD_TAB4,IDC_TAB4_Bmp1, "" },
{ IDD_TAB4,IDC_TAB4_Bmp2, "" },
{ IDD_TAB4,IDC_TAB4_Bmp3, "" },
{ IDD_TAB4,IDC_TAB4_Bmp4, "" },
{ IDD_TAB4,IDC_TAB4_Bmp5, "" },
{ IDD_TAB4,IDC_TAB4_Bmp6, "" },
{ IDD_TAB5,IDC_TAB5_Diff1, "" },
{ IDD_TAB5,IDC_TAB5_Diff2, "" },
{ IDD_TAB5,IDC_TAB5_Diff3, "" },
{ IDD_TAB5,IDC_TAB5_Batch, "" },
{ IDD_TAB5,IDC_TAB5_Way01, "" },
{ IDD_TAB5,IDC_TAB5_Way02, "" },
{ IDD_TAB5,IDC_TAB5_Way03, "" },
{ IDD_TAB5,IDC_TAB5_Way04, "" },
{ IDD_TAB5,IDC_TAB5_Way05, "" },
{ IDD_TAB5,IDC_TAB5_Way06, "" },
{ IDD_TAB5,IDC_TAB5_Way11, "" },
{ IDD_TAB5,IDC_TAB5_Way12, "" },
{ IDD_TAB5,IDC_TAB5_Way13, "" },
{ IDD_TAB5,IDC_TAB5_Way14, "" },
{ IDD_TAB5,IDC_TAB5_Way15, "" },
{ IDD_TAB5,IDC_TAB5_Way16, "" },
{ IDD_TAB5,IDC_TAB5_Way21, "" },
{ IDD_TAB5,IDC_TAB5_Way22, "" },
{ IDD_TAB5,IDC_TAB5_Way23, "" },
{ IDD_TAB5,IDC_TAB5_Way24, "" },
{ IDD_TAB5,IDC_TAB5_Way25, "" },
{ IDD_TAB5,IDC_TAB5_Way26, "" },
{ IDD_TAB5,IDC_TAB5_Way31, "" },
{ IDD_TAB5,IDC_TAB5_Way32, "" },
{ IDD_TAB5,IDC_TAB5_Way33, "" },
};
HWND hToolTip;
HWND CreateToolTipCtrl(HWND hParent, int ToolSet, bool Relay)
{
	if (!RegOptions.ToolTips) return 0;
	HWND hTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hParent, NULL, hInstance,
		NULL);
	SetWindowPos(hTip, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	SendMessage(hTip, TTM_ACTIVATE, TRUE, 0);
	SendMessage(hTip, TTM_SETTIPTEXTCOLOR, RGB(255, 50, 50), 0);
	SendMessage(hTip, TTM_SETTIPBKCOLOR, RGB(255, 255, 220), 0);
	SendMessage(hTip, TTM_SETDELAYTIME, TTDT_AUTOMATIC, 1000);
	SendMessage(hTip, TTM_SETMAXTIPWIDTH, 0, 300);
	TOOLINFO ti;
	ZeroMemory(&ti, sizeof ti);
	ti.cbSize = sizeof ti;
	ti.uFlags = TTF_IDISHWND | (Relay ? TTF_SUBCLASS : 0) | TTF_TRANSPARENT;
	ti.hwnd = hParent;
	ti.hinst = hInstance;
	for (int z = 0; z < sizeof ToolTips / sizeof ToolTips[0]; z++)
	{
		if (ToolTips[z].ToolSet == ToolSet)
		{
			ti.uId = (int)GetDlgItem(hParent, ToolTips[z].CtrlID);
			ti.lpszText = ToolTips[z].Text;
			if (!ToolTips[z].Text || *ToolTips[z].Text == 0) continue;
			SendMessage(hTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
		}
	}
	return hTip;
}