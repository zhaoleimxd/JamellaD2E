// Tab3.cpp from D2E
#include "JamellaD2E.h"
char* groups[5][3] =
{
 { "Javelin and Spear Skills","Passive and Magic Skills","Box and Crossbow Skills" },
 { "Cold Spells","Lightning Spells","Fire Spells" },
 { "Summoning Spells","Posion and Bone Spells","Curses" },
 { "Defensive Auras","Offensive Auras","Combat Skills" },
 { "Warcries","Combat Masteries","Combat Skills" }
};
struct skill skills[5 * 30] =
{
	// 0 = Amazon
	 { 5,"Jab", IDS_SKILL_A05,IDB_SKILL_A04 },
	 { 9,"Power Strike", IDS_SKILL_A09,IDB_SKILL_A09 },
	 { 10,"Poison Javelin", IDS_SKILL_A10,IDB_SKILL_A10 },
	 { 14,"Impale", IDS_SKILL_A14,IDB_SKILL_A14 },
	 { 15,"Lightning Bolt", IDS_SKILL_A15,IDB_SKILL_A15 },
	 { 19,"Charged Bolt", IDS_SKILL_A19,IDB_SKILL_A19 },
	 { 20,"Plague Javelin", IDS_SKILL_A20,IDB_SKILL_A20 },
	 { 25,"Fend", IDS_SKILL_A25,IDB_SKILL_A25 },
	 { 29,"Lightning Strike", IDS_SKILL_A29,IDB_SKILL_A29 },
	 { 30,"Lightning Fury", IDS_SKILL_A30,IDB_SKILL_A30 },
	 { 3,"Inner Sight", IDS_SKILL_A03,IDB_SKILL_A03 },
	 { 4,"Critical Strike", IDS_SKILL_A04,IDB_SKILL_A04 },
	 { 8,"Dodge", IDS_SKILL_A08,IDB_SKILL_A08 },
	 { 12,"Slow Missiles", IDS_SKILL_A12,IDB_SKILL_A12 },
	 { 13,"Avoid", IDS_SKILL_A13,IDB_SKILL_A13 },
	 { 18,"Penetrate", IDS_SKILL_A18,IDB_SKILL_A18 },
	 { 23,"Decoy", IDS_SKILL_A23,IDB_SKILL_A23 },
	 { 24,"Evade", IDS_SKILL_A24,IDB_SKILL_A24 },
	 { 27,"Valkyrie", IDS_SKILL_A27,IDB_SKILL_A27 },
	 { 28,"Pierce", IDS_SKILL_A28,IDB_SKILL_A28 },
	 { 1,"Magic Arrow", IDS_SKILL_A01,IDB_SKILL_A01 },
	 { 2,"Fire Arrow", IDS_SKILL_A02,IDB_SKILL_A02 },
	 { 6,"Cold Arrow", IDS_SKILL_A06,IDB_SKILL_A06 },
	 { 7,"Multiple Shots", IDS_SKILL_A07,IDB_SKILL_A07 },
	 { 11,"Exploding Arrow", IDS_SKILL_A11,IDB_SKILL_A11 },
	 { 16,"Ice Arrow", IDS_SKILL_A16,IDB_SKILL_A16 },
	 { 17,"Guided Arrow", IDS_SKILL_A17,IDB_SKILL_A17 },
	 { 21,"Strafe", IDS_SKILL_A21,IDB_SKILL_A21 },
	 { 22,"Immolation Arrow", IDS_SKILL_A22,IDB_SKILL_A22 },
	 { 26,"Freezing Arrow", IDS_SKILL_A26,IDB_SKILL_A26 },
	 // 1 = Sorceress
	  { 4,"Ice Bolt", IDS_SKILL_S04,IDB_SKILL_S04 },
	  { 5,"Frozen Armor", IDS_SKILL_S05,IDB_SKILL_S05 },
	  { 9,"Frost Nova", IDS_SKILL_S09,IDB_SKILL_S09 },
	  { 10,"Ice Blast", IDS_SKILL_S10,IDB_SKILL_S10 },
	  { 15,"Shiver Armor", IDS_SKILL_S15,IDB_SKILL_S15 },
	  { 20,"Glacial Spike", IDS_SKILL_S20,IDB_SKILL_S20 },
	  { 24,"Blizzard", IDS_SKILL_S24,IDB_SKILL_S24 },
	  { 25,"Chilling Armor", IDS_SKILL_S25,IDB_SKILL_S25 },
	  { 29,"Frozen Orb", IDS_SKILL_S29,IDB_SKILL_S29 },
	  { 30,"Cold Mastery", IDS_SKILL_S30,IDB_SKILL_S30 },
	  { 3,"Charged Bolt", IDS_SKILL_S03,IDB_SKILL_S03 },
	  { 7,"Static Field", IDS_SKILL_S07,IDB_SKILL_S07 },
	  { 8,"Telekinesis", IDS_SKILL_S08,IDB_SKILL_S08 },
	  { 13,"Nova", IDS_SKILL_S13,IDB_SKILL_S13 },
	  { 14,"Lightning", IDS_SKILL_S14,IDB_SKILL_S14 },
	  { 18,"Chain Lightning", IDS_SKILL_S18,IDB_SKILL_S18 },
	  { 19,"Teleport", IDS_SKILL_S19,IDB_SKILL_S19 },
	  { 22,"Thunder Storm", IDS_SKILL_S22,IDB_SKILL_S22 },
	  { 23,"Energy Shield", IDS_SKILL_S23,IDB_SKILL_S23 },
	  { 28,"Lightning Mastery", IDS_SKILL_S28,IDB_SKILL_S28 },
	  { 1,"Fire Bolt", IDS_SKILL_S01,IDB_SKILL_S01 },
	  { 2,"Warmth", IDS_SKILL_S02,IDB_SKILL_S02 },
	  { 6,"Inferno", IDS_SKILL_S06,IDB_SKILL_S06 },
	  { 11,"Blaze", IDS_SKILL_S11,IDB_SKILL_S11 },
	  { 12,"Fire Ball", IDS_SKILL_S12,IDB_SKILL_S12 },
	  { 16,"Fire Wall", IDS_SKILL_S16,IDB_SKILL_S16 },
	  { 17,"Enchant", IDS_SKILL_S17,IDB_SKILL_S17 },
	  { 21,"Meteor", IDS_SKILL_S21,IDB_SKILL_S21 },
	  { 26,"Fire Mastery", IDS_SKILL_S26,IDB_SKILL_S26 },
	  { 27,"Hydra", IDS_SKILL_S27,IDB_SKILL_S27 },
	  // 2 = Necromancer
	   { 4,"Skeleton Mastery", IDS_SKILL_N04,IDB_SKILL_N04 },
	   { 5,"Raise Skeleton", IDS_SKILL_N05,IDB_SKILL_N05 },
	   { 10,"Clay Golem", IDS_SKILL_N10,IDB_SKILL_N10 },
	   { 14,"Golem Mastery", IDS_SKILL_N14,IDB_SKILL_N14 },
	   { 15,"Raise Skeletal Mage", IDS_SKILL_N15,IDB_SKILL_N15 },
	   { 20,"Blood Golem", IDS_SKILL_N20,IDB_SKILL_N20 },
	   { 24,"Summon Resist", IDS_SKILL_N24,IDB_SKILL_N24 },
	   { 25,"Iron Golem", IDS_SKILL_N25,IDB_SKILL_N25 },
	   { 29,"Fire Golem", IDS_SKILL_N29,IDB_SKILL_N29 },
	   { 30,"Revive", IDS_SKILL_N30,IDB_SKILL_N30 },
	   { 2,"Teeth", IDS_SKILL_N02,IDB_SKILL_N02 },
	   { 3,"Bone Armor", IDS_SKILL_N03,IDB_SKILL_N03 },
	   { 8,"Poison Dagger", IDS_SKILL_N08,IDB_SKILL_N08 },
	   { 9,"Corpse Explosion", IDS_SKILL_N09,IDB_SKILL_N09 },
	   { 13,"Bone Wall", IDS_SKILL_N13,IDB_SKILL_N13 },
	   { 18,"Poison Explosion", IDS_SKILL_N18,IDB_SKILL_N18 },
	   { 19,"Bone Spear", IDS_SKILL_N19,IDB_SKILL_N19 },
	   { 23,"Bone Prison", IDS_SKILL_N23,IDB_SKILL_N23 },
	   { 27,"Poison Nova", IDS_SKILL_N27,IDB_SKILL_N27 },
	   { 28,"Bone Spirit", IDS_SKILL_N28,IDB_SKILL_N28 },
	   { 1,"Amplify Damage", IDS_SKILL_N01,IDB_SKILL_N01 },
	   { 6,"Dim Vision", IDS_SKILL_N06,IDB_SKILL_N06 },
	   { 7,"Weaken", IDS_SKILL_N07,IDB_SKILL_N07 },
	   { 11,"Iron Maiden", IDS_SKILL_N11,IDB_SKILL_N11 },
	   { 12,"Terror", IDS_SKILL_N12,IDB_SKILL_N12 },
	   { 16,"Confuse", IDS_SKILL_N16,IDB_SKILL_N16 },
	   { 17,"Life Tap", IDS_SKILL_N17,IDB_SKILL_N17 },
	   { 21,"Attract", IDS_SKILL_N21,IDB_SKILL_N21 },
	   { 22,"Decrepify", IDS_SKILL_N22,IDB_SKILL_N22 },
	   { 26,"Lower Resist", IDS_SKILL_N26,IDB_SKILL_N26 },
	   // 3 = Paladin
		{ 4,"Prayer", IDS_SKILL_P04,IDB_SKILL_P04 },
		{ 5,"Resist Fire", IDS_SKILL_P05,IDB_SKILL_P05 },
		{ 9,"Defiance", IDS_SKILL_P09,IDB_SKILL_P09 },
		{ 10,"Resist Cold", IDS_SKILL_P10,IDB_SKILL_P10 },
		{ 14,"Cleansing", IDS_SKILL_P14,IDB_SKILL_P14 },
		{ 15,"Resist Lightning", IDS_SKILL_P15,IDB_SKILL_P15 },
		{ 20,"Vigor", IDS_SKILL_P20,IDB_SKILL_P20 },
		{ 25,"Meditation", IDS_SKILL_P25,IDB_SKILL_P25 },
		{ 29,"Redemption", IDS_SKILL_P29,IDB_SKILL_P29 },
		{ 30,"Salvation", IDS_SKILL_P30,IDB_SKILL_P30 },
		{ 3,"Might", IDS_SKILL_P03,IDB_SKILL_P03 },
		{ 7,"Holy Fire", IDS_SKILL_P07,IDB_SKILL_P07 },
		{ 8,"Thorns", IDS_SKILL_P08,IDB_SKILL_P08 },
		{ 13,"Blessed Armor", IDS_SKILL_P13,IDB_SKILL_P13 },
		{ 18,"Concentration", IDS_SKILL_P18,IDB_SKILL_P18 },
		{ 19,"Holy Freeze", IDS_SKILL_P19,IDB_SKILL_P19 },
		{ 23,"Holy Shock", IDS_SKILL_P23,IDB_SKILL_P23 },
		{ 24,"Sanctuary", IDS_SKILL_P24,IDB_SKILL_P24 },
		{ 27,"Fanaticism", IDS_SKILL_P27,IDB_SKILL_P27 },
		{ 28,"Conviction", IDS_SKILL_P28,IDB_SKILL_P28 },
		{ 1,"Sacrifice", IDS_SKILL_P01,IDB_SKILL_P01 },
		{ 2,"Smite", IDS_SKILL_P02,IDB_SKILL_P02 },
		{ 6,"Holy Bolt", IDS_SKILL_P06,IDB_SKILL_P06 },
		{ 11,"Zeal", IDS_SKILL_P11,IDB_SKILL_P11 },
		{ 12,"Charge", IDS_SKILL_P12,IDB_SKILL_P12 },
		{ 16,"Vengeance", IDS_SKILL_P16,IDB_SKILL_P16 },
		{ 17,"Blessed Hammer", IDS_SKILL_P17,IDB_SKILL_P17 },
		{ 21,"Conversion", IDS_SKILL_P21,IDB_SKILL_P21 },
		{ 22,"Holy Shield", IDS_SKILL_P22,IDB_SKILL_P22 },
		{ 26,"Fist of Heavens", IDS_SKILL_P26,IDB_SKILL_P26 },
		// 4 = Barbarian
		 { 5,"Howl", IDS_SKILL_B05,IDB_SKILL_B05 },
		 { 6,"Find Potion", IDS_SKILL_B06,IDB_SKILL_B06 },
		 { 12,"Taunt", IDS_SKILL_B12,IDB_SKILL_B12 },
		 { 13,"Shout", IDS_SKILL_B13,IDB_SKILL_B13 },
		 { 17,"Find Item", IDS_SKILL_B17,IDB_SKILL_B17 },
		 { 21,"Battle Cry", IDS_SKILL_B21,IDB_SKILL_B21 },
		 { 24,"Battle Orders", IDS_SKILL_B24,IDB_SKILL_B24 },
		 { 25,"Grim Wand", IDS_SKILL_B25,IDB_SKILL_B25 },
		 { 29,"War Cry", IDS_SKILL_B29,IDB_SKILL_B29 },
		 { 30,"Battle Command", IDS_SKILL_B30,IDB_SKILL_B30 },
		 { 2,"Sword Mastery", IDS_SKILL_B02,IDB_SKILL_B02 },
		 { 3,"Axe Mastery", IDS_SKILL_B03,IDB_SKILL_B03 },
		 { 4,"Mace Mastery", IDS_SKILL_B04,IDB_SKILL_B04 },
		 { 9,"Pole Mastery", IDS_SKILL_B09,IDB_SKILL_B09 },
		 { 10,"Throwing Mastery", IDS_SKILL_B10,IDB_SKILL_B10 },
		 { 11,"Speare Mastery", IDS_SKILL_B11,IDB_SKILL_B11 },
		 { 16,"Increase Stamina", IDS_SKILL_B16,IDB_SKILL_B16 },
		 { 20,"Iron Skin", IDS_SKILL_B20,IDB_SKILL_B20 },
		 { 23,"Increase Speed", IDS_SKILL_B23,IDB_SKILL_B23 },
		 { 28,"Natural Resistances", IDS_SKILL_P28,IDB_SKILL_B28 },
		 { 1,"Bash", IDS_SKILL_B01,IDB_SKILL_B01 },
		 { 7,"Leap", IDS_SKILL_B07,IDB_SKILL_B07 },
		 { 8,"Double Swing", IDS_SKILL_B08,IDB_SKILL_B08 },
		 { 14,"Stun", IDS_SKILL_B14,IDB_SKILL_B14 },
		 { 15,"Double Throw", IDS_SKILL_B15,IDB_SKILL_B15 },
		 { 18,"Leap Attack", IDS_SKILL_B18,IDB_SKILL_B18 },
		 { 19,"Concentrate", IDS_SKILL_B19,IDB_SKILL_B19 },
		 { 22,"Frenzy", IDS_SKILL_B22,IDB_SKILL_B22 },
		 { 26,"Whirlwind", IDS_SKILL_B26,IDB_SKILL_B26 },
		 { 27,"Berserk", IDS_SKILL_B27,IDB_SKILL_B27 }
};
struct skillprops
{
	int idbitmap;
	int idedit;
};
struct skillprops props[30] =
{
 { IDC_TAB3_Bitmap_A0,IDC_TAB3_Edit_A0 },
 { IDC_TAB3_Bitmap_A1,IDC_TAB3_Edit_A1 },
 { IDC_TAB3_Bitmap_A2,IDC_TAB3_Edit_A2 },
 { IDC_TAB3_Bitmap_A3,IDC_TAB3_Edit_A3 },
 { IDC_TAB3_Bitmap_A4,IDC_TAB3_Edit_A4 },
 { IDC_TAB3_Bitmap_A5,IDC_TAB3_Edit_A5 },
 { IDC_TAB3_Bitmap_A6,IDC_TAB3_Edit_A6 },
 { IDC_TAB3_Bitmap_A7,IDC_TAB3_Edit_A7 },
 { IDC_TAB3_Bitmap_A8,IDC_TAB3_Edit_A8 },
 { IDC_TAB3_Bitmap_A9,IDC_TAB3_Edit_A9 },
 { IDC_TAB3_Bitmap_B0,IDC_TAB3_Edit_B0 },
 { IDC_TAB3_Bitmap_B1,IDC_TAB3_Edit_B1 },
 { IDC_TAB3_Bitmap_B2,IDC_TAB3_Edit_B2 },
 { IDC_TAB3_Bitmap_B3,IDC_TAB3_Edit_B3 },
 { IDC_TAB3_Bitmap_B4,IDC_TAB3_Edit_B4 },
 { IDC_TAB3_Bitmap_B5,IDC_TAB3_Edit_B5 },
 { IDC_TAB3_Bitmap_B6,IDC_TAB3_Edit_B6 },
 { IDC_TAB3_Bitmap_B7,IDC_TAB3_Edit_B7 },
 { IDC_TAB3_Bitmap_B8,IDC_TAB3_Edit_B8 },
 { IDC_TAB3_Bitmap_B9,IDC_TAB3_Edit_B9 },
 { IDC_TAB3_Bitmap_C0,IDC_TAB3_Edit_C0 },
 { IDC_TAB3_Bitmap_C1,IDC_TAB3_Edit_C1 },
 { IDC_TAB3_Bitmap_C2,IDC_TAB3_Edit_C2 },
 { IDC_TAB3_Bitmap_C3,IDC_TAB3_Edit_C3 },
 { IDC_TAB3_Bitmap_C4,IDC_TAB3_Edit_C4 },
 { IDC_TAB3_Bitmap_C5,IDC_TAB3_Edit_C5 },
 { IDC_TAB3_Bitmap_C6,IDC_TAB3_Edit_C6 },
 { IDC_TAB3_Bitmap_C7,IDC_TAB3_Edit_C7 },
 { IDC_TAB3_Bitmap_C8,IDC_TAB3_Edit_C8 },
 { IDC_TAB3_Bitmap_C9,IDC_TAB3_Edit_C9 }
};
int selection;
skill* classskills;
HWND hTrack;
char skilleffect[256];
LRESULT CALLBACK Tab3DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_TAB3_BonusPlus, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		// Setup Track Bar
		hTrack = GetDlgItem(hWnd, IDC_TAB3_Track);
		SendMessage(hTrack, TBM_SETRANGE,
			FALSE, (LPARAM)MAKELONG(0, 20));
		SendMessage(hTrack, TBM_SETTICFREQ, (LPARAM)1, 0);
		SendMessage(hTrack, TBM_SETPAGESIZE, FALSE, (LPARAM)1);
		SendMessage(hTrack, TBM_SETPOS, TRUE, (LPARAM)0);
		// Insert right bitmaps and show skill levels
		for (int i = 0; i < 3; i++)
		{
			SetDlgItemText(hWnd, IDC_TAB3_Frame_A + i, groups[fc.Header.playerclass][i]);
		}
		classskills = &skills[fc.Header.playerclass * 30];
		for (int i = 0; i < 30; i++)
		{
			SendDlgItemMessage(hWnd, props[i].idbitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)classskills[i].hbitmap);
			SetDlgItemInt(hWnd, props[i].idedit, fc.IF.skills[classskills[i].ifoffset - 1], FALSE);
		}
		selection = -1;
		// Setup user variables
		SetDlgItemInt(hWnd, IDC_TAB3_Bonus, fc.gf.skillbonus, FALSE);
		hToolTip = CreateToolTipCtrl(hWnd, IDD_TAB3, true);
	}
	return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_TAB3_Bitmap_A0: case IDC_TAB3_Bitmap_A1:
		case IDC_TAB3_Bitmap_A2: case IDC_TAB3_Bitmap_A3:
		case IDC_TAB3_Bitmap_A4: case IDC_TAB3_Bitmap_A5:
		case IDC_TAB3_Bitmap_A6: case IDC_TAB3_Bitmap_A7:
		case IDC_TAB3_Bitmap_A8: case IDC_TAB3_Bitmap_A9:
		case IDC_TAB3_Bitmap_B0: case IDC_TAB3_Bitmap_B1:
		case IDC_TAB3_Bitmap_B2: case IDC_TAB3_Bitmap_B3:
		case IDC_TAB3_Bitmap_B4: case IDC_TAB3_Bitmap_B5:
		case IDC_TAB3_Bitmap_B6: case IDC_TAB3_Bitmap_B7:
		case IDC_TAB3_Bitmap_B8: case IDC_TAB3_Bitmap_B9:
		case IDC_TAB3_Bitmap_C0: case IDC_TAB3_Bitmap_C1:
		case IDC_TAB3_Bitmap_C2: case IDC_TAB3_Bitmap_C3:
		case IDC_TAB3_Bitmap_C4: case IDC_TAB3_Bitmap_C5:
		case IDC_TAB3_Bitmap_C6: case IDC_TAB3_Bitmap_C7:
		case IDC_TAB3_Bitmap_C8: case IDC_TAB3_Bitmap_C9:
		{
			HWND hBmp = GetDlgItem(hWnd, IDC_TAB3_Prop_Bitmap);
			HWND hEdit = GetDlgItem(hWnd, IDC_TAB3_Prop_Edit);
			HWND hEffect = GetDlgItem(hWnd, IDC_TAB3_Prop_Effect);

			if (selection < 0)
			{
				ShowWindow(hBmp, SW_SHOW);
				ShowWindow(hEdit, SW_SHOW);
				ShowWindow(hEffect, SW_SHOW);
				ShowWindow(hTrack, SW_SHOW);
			}
			if (LOWORD(wParam) - IDC_TAB3_Bitmap_A0 == selection)
			{
				fc.IF.skills[classskills[selection].ifoffset - 1]++;
				if (fc.IF.skills[classskills[selection].ifoffset - 1] > 20)
					fc.IF.skills[classskills[selection].ifoffset - 1] = 20;
				SetDlgItemInt(hWnd, props[selection].idedit, fc.IF.skills[classskills[selection].ifoffset - 1], FALSE
				);
				SendMessage(hTrack, TBM_SETPOS, TRUE, (LPARAM)fc.IF.skills[classskills[selection].ifoffset - 1]);
			}
			else
			{
				selection = LOWORD(wParam) - IDC_TAB3_Bitmap_A0;

				SendMessage(hBmp, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)classskills[selection].hbitmap);
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)classskills[selection].name);
				SendMessage(hTrack, TBM_SETPOS, TRUE, (LPARAM)fc.IF.skills[classskills[selection].ifoffset - 1]);
				LoadString(hInstance, classskills[selection].idstring, skilleffect, 256);
				SendMessage(hEffect, WM_SETTEXT, 0, (LPARAM)skilleffect);
			}
		}
		break;
		case IDC_TAB3_BonusPlus:
		{
			DWORD x = GetDlgItemInt(hWnd, IDC_TAB3_Bonus, NULL, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB3_Bonus, x + 1, FALSE);
		}
		break;
		case IDR_TAB3_MaximizeAll:
		{
			for (int i = 0; i < 30; i++)
			{
				fc.IF.skills[i] = 20;
				SetDlgItemInt(hWnd, props[i].idedit, 20, FALSE);
				if (selection > 0)
				{
					SendMessage(hTrack, TBM_SETPOS, TRUE, (LPARAM)fc.IF.skills[classskills[selection].ifoffset - 1])
						;
				}
			}
		}
		break;
		case IDR_TAB3_SetAll0:
		case IDR_TAB3_SetAll4:
		case IDR_TAB3_SetAll8:
		case IDR_TAB3_SetAll12:
		case IDR_TAB3_SetAll16:
		case IDR_TAB3_SetAll20:
		{
			for (int i = 0; i < 30; i++)
			{
				fc.IF.skills[i] =
					(LOWORD(wParam) == IDR_TAB3_SetAll0) ? 0 :
					(LOWORD(wParam) == IDR_TAB3_SetAll4) ? 4 :
					(LOWORD(wParam) == IDR_TAB3_SetAll8) ? 8 :
					(LOWORD(wParam) == IDR_TAB3_SetAll12) ? 12 :
					(LOWORD(wParam) == IDR_TAB3_SetAll16) ? 16 :
					(LOWORD(wParam) == IDR_TAB3_SetAll20) ? 20 : 0;
				SetDlgItemInt(hWnd, props[i].idedit, fc.IF.skills[i], FALSE);
				if (selection > 0) {
					SendMessage(hTrack, TBM_SETPOS, TRUE, (LPARAM)fc.IF.skills[classskills[selection].ifoffset - 1])
						;
				}
			}
		}
		break;
		case IDC_TAB3_Batch:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				HMENU hMenu = GetSubMenu(hBatchMenu, 2);
				POINT Pos;
				GetCursorPos(&Pos);
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
					Pos.x, Pos.y, 0, hWnd, NULL);
			}
			break;
			}
			break;
		}
	}
	break;
	case WM_HSCROLL:
		if ((HWND)lParam == hTrack)
		{
			fc.IF.skills[classskills[selection].ifoffset - 1] = (BYTE)SendMessage(hTrack, TBM_GETPOS, 0, 0);
			SetDlgItemInt(hWnd, props[selection].idedit, fc.IF.skills[classskills[selection].ifoffset - 1], FALSE);
			break;
		}
		break;
	case WM_DESTROY:
	{
		// Retrieve user variables
		fc.gf.skillbonus = GetDlgItemInt(hWnd, IDC_TAB3_Bonus, NULL, FALSE);
		DestroyWindow(hToolTip);
	}
	return false;
	}
	return false;
}