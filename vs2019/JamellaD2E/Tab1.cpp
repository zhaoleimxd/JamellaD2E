// Tab1.cpp from D2E
#include "JamellaD2E.h"
HBITMAP hBmpPlus;
/*
struct
{
 char *text;
 int value;
}
StartTowns[4*3] =
{
 { "Rogues' Encampment in Normal", 0x00 },
 { "Lut Gholein in Normal", 0x01 },
 { "Kurast Docks in Normal", 0x02 },
 { "Pandemonium Fortress in Normal", 0x03 },
 { "Rogues' Encampment in Nightmare", 0x10 },
 { "Lut Gholein in Nightmare", 0x11 },
 { "Kurast Docks in Nightmare", 0x12 },
 { "Pandemonium Fortress in Nightmare", 0x13 },
 { "Rogues' Encampment in Hell", 0x20 },
 { "Lut Gholein in Hell", 0x21 },
 { "Kurast Docks in Hell", 0x22 },
 { "Pandemonium Fortress in Hell", 0x23 }
};
*/
struct
{
	char* text;
	int value;
}
StartTowns[4] =
{
 { "Rogues' Encampment", 0x00 },
 { "Lut Gholein", 0x01 },
 { "Kurast Docks", 0x02 },
 { "Pandemonium Fortress", 0x03 },
};
const struct
{
	int Vitality[3];
	int Energy[3];
	int Level[3];
}
Boosts[5] =
{
 { // Amazon
 { 3, 0, 1 },
 { 0, 2, 0 },
 { 2, 2, 1 }
 },
 { // Sorceress
 { 2, 0, 1 },
 { 0, 2, 0 },
 { 1, 2, 1 }
 },
 { // Necromancer
 { 2, 0, 1 },
 { 0, 2, 0 },
 { 2, 2, 1 }
 },
 { // Paladin
 { 3, 0, 1 },
 { 0, 2, 0 },
 { 2, 2, 1 }
 },
 { // Barbarian
 { 4, 0, 1 },
 { 0, 1, 0 },
 { 2, 1, 1 }
 }
};
const struct
{
	int id;
	DWORD* val;
}
values[] =
{
 { IDC_TAB1_Level, &fc.gf.level },
 { IDC_TAB1_Experience, &fc.gf.experience },
 { IDC_TAB1_Char1, &fc.gf.strength },
 { IDC_TAB1_Char2, &fc.gf.dexterity },
 { IDC_TAB1_Char3, &fc.gf.vitality },
 { IDC_TAB1_Char4, &fc.gf.energy },
 { IDC_TAB1_Char5, &fc.gf.statsbonus },
 { IDC_TAB1_Health, &fc.gf.health },
 { IDC_TAB1_HealthMax, &fc.gf.healthmax },
 { IDC_TAB1_Mana, &fc.gf.mana },
 { IDC_TAB1_ManaMax, &fc.gf.manamax },
 { IDC_TAB1_Stamina, &fc.gf.stamina },
 { IDC_TAB1_StaminaMax, &fc.gf.staminamax },
 { IDC_TAB1_GoldPerson, &fc.gf.goldperson },
 { IDC_TAB1_GoldStash, &fc.gf.goldstash }
};
extern DWORD experiencelevels[100];
extern int goldstash[10];
bool editchanging = false;
DWORD level;
DWORD maxgoldperson, maxgoldstash;
void calc_maxgold(HWND hWnd)
{
	if (level > 0 && level <= 99)
	{
		maxgoldperson = level * 10000;
		maxgoldstash = goldstash[level / 10];
		sprintf(buffer, "/ %lu", maxgoldperson);
		SetDlgItemText(hWnd, IDC_TAB1_MaxGoldPerson, buffer);
		sprintf(buffer, "/ %lu", maxgoldstash);
		SetDlgItemText(hWnd, IDC_TAB1_MaxGoldStash, buffer);

		DWORD x = GetDlgItemInt(hWnd, IDC_TAB1_GoldPerson, 0, FALSE);
		if (x > maxgoldperson)
			SetDlgItemInt(hWnd, IDC_TAB1_GoldPerson, maxgoldperson, FALSE);

		x = GetDlgItemInt(hWnd, IDC_TAB1_GoldStash, 0, FALSE);
		if (x > maxgoldperson)
			SetDlgItemInt(hWnd, IDC_TAB1_GoldStash, maxgoldperson, FALSE);
	}
	else
	{
		SetDlgItemText(hWnd, IDC_TAB1_MaxGoldPerson, "/ ???????");
		SetDlgItemText(hWnd, IDC_TAB1_MaxGoldStash, "/ ???????");
	}
}
LRESULT CALLBACK Tab1DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		// Associate Bitmaps
		SendDlgItemMessage(hWnd, IDC_TAB1_Plus1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		SendDlgItemMessage(hWnd, IDC_TAB1_Plus2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		SendDlgItemMessage(hWnd, IDC_TAB1_Plus3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		SendDlgItemMessage(hWnd, IDC_TAB1_Plus4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		SendDlgItemMessage(hWnd, IDC_TAB1_Plus5, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpPlus);
		// Add Classes to Class List Box
		HWND hClass = GetDlgItem(hWnd, IDC_TAB1_Class);
		SendMessage(hClass, CB_RESETCONTENT, 0, 0);
		for (int i = 0; i < 5; i++)
			SendMessage(hClass, CB_ADDSTRING, 0, (LPARAM)CharClasses[i]);
		// Add Difficulties to List Box
		HWND hDiff = GetDlgItem(hWnd, IDC_TAB1_Difficulty);
		SendMessage(hDiff, CB_RESETCONTENT, 0, 0);
		for (int i = 0; i < 4; i++)
			SendMessage(hDiff, CB_ADDSTRING, 0, (LPARAM)Difficulties[i]);
		// Add Towns to List Box
		HWND hTown = GetDlgItem(hWnd, IDC_TAB1_StartTown);
		SendMessage(hTown, CB_RESETCONTENT, 0, 0);
		for (int i = 0; i < 4; i++)
			SendMessage(hTown, CB_ADDSTRING, 0, (LPARAM)StartTowns[i].text);
		// Load Character values
		SetDlgItemText(hWnd, IDC_TAB1_Name, fc.Header.playername);
		SendMessage(hClass, CB_SETCURSEL, fc.Header.playerclass, 0);
		for (int i = 0; i < 4; i++)
		{
			if (StartTowns[i].value == (fc.Header.startingtown & 0x0F))
				SendMessage(hTown, CB_SETCURSEL, i, 0);
		}
		for (int i = 0; i < sizeof values / sizeof values[0]; i++)
			SetDlgItemInt(hWnd, values[i].id, *values[i].val, FALSE);
		level = fc.gf.level;
		calc_maxgold(hWnd);
		CheckDlgButton(hWnd, IDC_TAB1_Hardcore,
			fc.Header.hardcore & 0x04 ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(hWnd, IDC_TAB1_Dead,
			fc.Header.hardcore & 0x08 ? BST_CHECKED : BST_UNCHECKED);
		HWND hDead = GetDlgItem(hWnd, IDC_TAB1_Dead);
		EnableWindow(hDead,
			(IsDlgButtonChecked(hWnd, IDC_TAB1_Hardcore) == BST_CHECKED));
		int diff = 0;
		if ((fc.Header.diff & 0x0C) == 0x04) diff = 1;
		if ((fc.Header.diff & 0x0C) == 0x08) diff = 2;
		if ((fc.Header.diff & 0x0C) == 0x0C) diff = 3;
		SendMessage(hDiff, CB_SETCURSEL, diff, 0);
		CheckDlgButton(hWnd, IDC_TAB1_StatsLink, BST_CHECKED);
		hToolTip = CreateToolTipCtrl(hWnd, IDD_TAB1, true);
	}
	return true;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_TAB1_Plus1:
		case IDC_TAB1_Plus2:
		case IDC_TAB1_Plus3:
		case IDC_TAB1_Plus4:
		case IDC_TAB1_Plus5:
			switch (HIWORD(wParam))
			{
			case STN_CLICKED:
			case STN_DBLCLK:
			{
				int id = LOWORD(wParam) - IDC_TAB1_Plus1;
				DWORD x = GetDlgItemInt(hWnd, IDC_TAB1_Char1 + id, NULL, FALSE);
				SetDlgItemInt(hWnd, IDC_TAB1_Char1 + id, x + 1, FALSE);
				int c = (BYTE)SendDlgItemMessage(hWnd, IDC_TAB1_Class, CB_GETCURSEL, 0, 0);
				if (IsDlgButtonChecked(hWnd, IDC_TAB1_StatsLink) == BST_CHECKED)
				{
					switch (LOWORD(wParam))
					{
					case IDC_TAB1_Plus3:
						x = GetDlgItemInt(hWnd, IDC_TAB1_HealthMax, NULL, FALSE);
						x += Boosts[c].Vitality[0];
						SetDlgItemInt(hWnd, IDC_TAB1_HealthMax, x, FALSE);

						x = GetDlgItemInt(hWnd, IDC_TAB1_ManaMax, NULL, FALSE);
						x += Boosts[c].Vitality[1];
						SetDlgItemInt(hWnd, IDC_TAB1_ManaMax, x, FALSE);

						x = GetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, NULL, FALSE);
						x += Boosts[c].Vitality[2];
						SetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, x, FALSE);
						break;
					case IDC_TAB1_Plus4:
						x = GetDlgItemInt(hWnd, IDC_TAB1_HealthMax, NULL, FALSE);
						x += Boosts[c].Energy[0];
						SetDlgItemInt(hWnd, IDC_TAB1_HealthMax, x, FALSE);

						x = GetDlgItemInt(hWnd, IDC_TAB1_ManaMax, NULL, FALSE);
						x += Boosts[c].Energy[1];
						SetDlgItemInt(hWnd, IDC_TAB1_ManaMax, x, FALSE);

						x = GetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, NULL, FALSE);
						x += Boosts[c].Energy[2];
						SetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, x, FALSE);
						break;
					}
				}
			}
			break;
			}
			break;
		case IDC_TAB1_Experience:
			if (editchanging) break;
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				char* end;
				editchanging = true;
				int anz = GetDlgItemText(hWnd, IDC_TAB1_Experience, buffer, 32);
				DWORD x = strtoul(buffer, &end, 10);
				if (end == buffer + anz && x < 4294967294)
				{
					int z = 0;
					for (z = 0; z < 99; z++)
					{
						if (experiencelevels[z] > x)
						{
							break;
						}
					}
					SetDlgItemInt(hWnd, IDC_TAB1_Level, z, FALSE);
					level = z;
				}
				else
				{
					SetDlgItemText(hWnd, IDC_TAB1_Level, "???");
					level = 0;
				}
				calc_maxgold(hWnd);
				editchanging = false;
			}
			}
			break;
		case IDC_TAB1_Level:
			if (editchanging) break;
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
			{
				char buffer[32];
				char* end;
				editchanging = true;
				int anz = GetDlgItemText(hWnd, IDC_TAB1_Level, buffer, 32);
				DWORD x = strtoul(buffer, &end, 10);
				if (end == buffer + anz && x > 0 && x <= 99)
				{
					DWORD lvl = experiencelevels[x - 1] + 100;
					SetDlgItemInt(hWnd, IDC_TAB1_Experience, lvl, FALSE);
					level = x;
				}
				else
				{
					SetDlgItemText(hWnd, IDC_TAB1_Experience, "???");
					level = 0;
				}
				editchanging = false;
				calc_maxgold(hWnd);
			}
			}
			break;
		case IDC_TAB1_Hardcore:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				EnableWindow(GetDlgItem(hWnd, IDC_TAB1_Dead),
					(IsDlgButtonChecked(hWnd, IDC_TAB1_Hardcore) == BST_CHECKED));
			}
			}
			break;
		case IDC_TAB1_Rename:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_RENAME), hWnd, (DLGPROC)RenameDialogProc);
				SetDlgItemText(hWnd, IDC_TAB1_Name, fc.Header.playername);
			}
			break;
		case IDC_TAB1_SetMaxGoldPerson:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				SetDlgItemInt(hWnd, IDC_TAB1_GoldPerson, maxgoldperson, FALSE);
			}
			}
			break;
		case IDC_TAB1_SetMaxGoldStash:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				SetDlgItemInt(hWnd, IDC_TAB1_GoldStash, maxgoldstash, FALSE);
			}
			}
			break;
		case IDC_TAB1_Batch:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				HMENU hMenu = GetSubMenu(hBatchMenu, 0);
				POINT Pos;
				GetCursorPos(&Pos);
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON,
					Pos.x, Pos.y, 0, hWnd, NULL);
			}
			break;
			}
			break;
		case IDR_TAB1_RestoreConstitution:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int X = GetDlgItemInt(hWnd, IDC_TAB1_HealthMax, NULL, FALSE);
				SetDlgItemInt(hWnd, IDC_TAB1_Health, X, FALSE);
				int Y = GetDlgItemInt(hWnd, IDC_TAB1_ManaMax, NULL, FALSE);
				SetDlgItemInt(hWnd, IDC_TAB1_Mana, Y, FALSE);
				int Z = GetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, NULL, FALSE);
				SetDlgItemInt(hWnd, IDC_TAB1_Stamina, Z, FALSE);
			}
			break;
		case IDR_TAB1_SetAllStats40:
		case IDR_TAB1_SetAllStats60:
		case IDR_TAB1_SetAllStats80:
		case IDR_TAB1_SetAllStats100:
		case IDR_TAB1_SetAllStats120:
		case IDR_TAB1_SetAllStats140:
		case IDR_TAB1_SetAllStats160:
		case IDR_TAB1_SetAllStats180:
		case IDR_TAB1_SetAllStats200:
		case IDR_TAB1_SetAllStats250:
		case IDR_TAB1_SetAllStats300:
		case IDR_TAB1_SetAllStats350:
		case IDR_TAB1_SetAllStats400:
		case IDR_TAB1_SetAllStats450:
		case IDR_TAB1_SetAllStats500:
		{
			struct
			{
				int ID;
				int Value;
			} Stats[] =
			{
			{ IDR_TAB1_SetAllStats40, 40 },
			{ IDR_TAB1_SetAllStats60, 60 },
			{ IDR_TAB1_SetAllStats80, 80 },
			{ IDR_TAB1_SetAllStats100, 100 },
			{ IDR_TAB1_SetAllStats120, 120 },
			{ IDR_TAB1_SetAllStats140, 140 },
			{ IDR_TAB1_SetAllStats160, 160 },
			{ IDR_TAB1_SetAllStats180, 180 },
			{ IDR_TAB1_SetAllStats200, 200 },
			{ IDR_TAB1_SetAllStats250, 250 },
			{ IDR_TAB1_SetAllStats300, 300 },
			{ IDR_TAB1_SetAllStats350, 350 },
			{ IDR_TAB1_SetAllStats400, 400 },
			{ IDR_TAB1_SetAllStats450, 450 },
			{ IDR_TAB1_SetAllStats500, 500 }
			};
			int V = 0;
			for (int z = 0; z < sizeof Stats / sizeof Stats[0]; z++) {
				if (Stats[z].ID == LOWORD(wParam))
					V = Stats[z].Value;
			}
			SetDlgItemInt(hWnd, IDC_TAB1_Char1, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_Char2, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_Char3, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_Char4, V, FALSE);
		}
		break;
		case IDR_TAB1_SetAllConstitution400:
		case IDR_TAB1_SetAllConstitution600:
		case IDR_TAB1_SetAllConstitution800:
		case IDR_TAB1_SetAllConstitution1000:
		case IDR_TAB1_SetAllConstitution1500:
		case IDR_TAB1_SetAllConstitution2000:
		case IDR_TAB1_SetAllConstitution3000:
		case IDR_TAB1_SetAllConstitution5000:
		{
			struct
			{
				int ID;
				int Value;
			} Consts[] =
			{
			{ IDR_TAB1_SetAllConstitution400, 400 },
			{ IDR_TAB1_SetAllConstitution600, 600 },
			{ IDR_TAB1_SetAllConstitution800, 800 },
			{ IDR_TAB1_SetAllConstitution1000, 1000 },
			{ IDR_TAB1_SetAllConstitution1500, 1500 },
			{ IDR_TAB1_SetAllConstitution2000, 2000 },
			{ IDR_TAB1_SetAllConstitution3000, 3000 },
			{ IDR_TAB1_SetAllConstitution5000, 5000 },
			};
			int V = 0;
			for (int z = 0; z < sizeof Consts / sizeof Consts[0]; z++) {
				if (Consts[z].ID == LOWORD(wParam))
					V = Consts[z].Value;
			}
			SetDlgItemInt(hWnd, IDC_TAB1_Health, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_HealthMax, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_Mana, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_ManaMax, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_Stamina, V, FALSE);
			SetDlgItemInt(hWnd, IDC_TAB1_StaminaMax, V, FALSE);
		}
		break;
		}
	}
	break;
	case WM_VALIDATE:
	{
		if (level == 0)
		{
			MessageBox(hWnd, "Level and Experience are invalid!!!\nPlease correct.", PROGRAMNAME, MB_OK);
			return true;
		}
		return false;
	}
	case WM_DESTROY:
	{
		// Retrieve User Changed Data
		GetDlgItemText(hWnd, IDC_TAB1_Name, fc.Header.playername, 16);
		fc.Header.playerclass = (BYTE)SendDlgItemMessage(hWnd, IDC_TAB1_Class, CB_GETCURSEL, 0, 0);
		for (int i = 0; i < sizeof values / sizeof values[0]; i++)
		{
			*values[i].val = GetDlgItemInt(hWnd, values[i].id, NULL, FALSE);
		}
		fc.Header.level = GetDlgItemInt(hWnd, IDC_TAB1_Level, NULL, FALSE);
		if (IsDlgButtonChecked(hWnd, IDC_TAB1_Hardcore) == BST_CHECKED)
			fc.Header.hardcore |= 0x04;
		else
			fc.Header.hardcore &= 0x04 ^ 0xFF;
		if (IsDlgButtonChecked(hWnd, IDC_TAB1_Dead) == BST_CHECKED)
			fc.Header.hardcore |= 0x08;
		else
			fc.Header.hardcore &= 0x08 ^ 0xFF;
		fc.Header.diff &= 0x0C;
		switch ((int)SendDlgItemMessage(hWnd, IDC_TAB1_Difficulty, CB_GETCURSEL, 0, 0))
		{
		case 0:
			fc.Header.diff |= 0x00;
			break;
		case 1:
			fc.Header.diff |= 0x04;
			break;
		case 2:
			fc.Header.diff |= 0x08;
			break;
		case 3:
			fc.Header.diff |= 0x0C;
			break;
		}
		int startingtown = (BYTE)SendDlgItemMessage(hWnd, IDC_TAB1_StartTown, CB_GETCURSEL, 0, 0);
		if (startingtown >= 0 && startingtown <= 3)
			fc.Header.startingtown = (fc.Header.startingtown & 0xF0) | (StartTowns[startingtown].value & 0x0F);

		if (hToolTip)
			DestroyWindow(hToolTip);
	}
	return false;
	}
	return false;
}
DWORD experiencelevels[100] =
{
	0,
	500,
	1500,
	3750,
	7875,
	14175,
	22680,
	32886,
	44396,
	57715,
	72144,
	90180,
	112725,
	140906,
	176132,
	220165,
	275207,
	344008,
	430010,
	537513,
	671891,
	839864,
	1049830,
	1312287,
	1640359,
	2050449,
	2563061,
	3203826,
	3902260,
	4663553,
	5493363,
	6397855,
	7383752,
	8458379,
	9629723,
	10906488,
	12298162,
	13815086,
	15468534,
	17270791,
	19235252,
	21376515,
	23710491,
	26254525,
	29027522,
	32050088,
	35344686,
	38935798,
	42850109,
	47116709,
	51767302,
	56836449,
	62361819,
	68384473,
	74949165,
	82104680,
	89904191,
	98405658,
	107672256,
	117772849,
	128782495,
	140783010,
	153863570,
	168121381,
	183662396,
	200602101,
	219066380,
	239192444,
	261129853,
	285041630,
	311105466,
	339515048,
	370481492,
	404234916,
	441026148,
	481128591,
	524840254,
	572485967,
	624419793,
	681027665,
	742730244,
	809986056,
	883294891,
	963201521,
	1050299747,
	1145236814,
	1248718217,
	1361512946,
	1484459201,
	1618470619,
	1764543065,
	1923762030,
	2097310703,
	2286478756,
	2492671933,
	2717422497,
	2962400612,
	3229426756,
	3520485254,
	4000000000
};
int goldstash[10] =
{ 50000,100000,150000,200000,250000,300000,350000,400000,450000,500000 };