// Tab4.cpp from D2E
#include "JamellaD2E.h"
struct queststatus actA1[] =
{
 { "Not started", 0x0000 },
 { "Look for the Den", 0x0004 },
 { "Get reward from Akara", 0x0004 },
 { "Quest just completed", 0x901D },
 { "Quest completed", 0x001D },
 { 0,0 }
};
struct queststatus actA2[] =
{
 { "Not started", 0x0000 },
 { "Look for Blood Raven", 0x0004 },
 { "Kill Blood Raven", 0x001C },
 { "Get reward from Akara", 0x001E },
 { "Quest just completed", 0x901D },
 { "Quest completed", 0x001D },
 { 0,0 }
};
struct queststatus actA3[] =
{
 { "Not started", 0x0000 },
 { "Look for the Scroll", 0x0004 },
 { "Get reward from Akara", 0x000E },
 { "Quest just completed", 0x900D },
 { "Killed Cow King!", 0x0114 },
 { "Quest completed", 0x001D },
 { 0,0 }
};
struct queststatus actA4[] =
{
 { "Not started", 0x0000 },
 { "Look for Forgotten Tower", 0x0004 },
 { "Explore the cellar dungeons", 0x0044 },
 { "Dispose of evil Countess", 0x0054 },
 { "Quest just completed", 0x9055 },
 { "Quest completed", 0x001D },
 { 0,0 }
};
struct queststatus actA5[] =
{
 { "Not started", 0x0000 },
 { "Look for ...", 0x0004 },
 { "Charsi will Imbue Item", 0x004E },
 { "Just finished", 0x001D },
 { "Long finished", 0x0001 },
 { 0,0 }
};
struct queststatus simple[] =
{
 { "Not started", 0x0000 },
 { "Look for ...", 0x0004 },
 { "Just finished", 0x001D },
 { "Long finished", 0x0001 },
 { 0,0 }
};
struct queststatus allcombo[] =
{
 { "to not started", 0x0000 },
 { "to look for ...", 0x0004 },
 { "to just finished", 0x001D },
 { "to long finished", 0x0001 },
 { 0,0 }
};
struct quest quests[21] =
{
 { IDS_QUEST11,IDB_QUEST11,actA1, 1 },
 { IDS_QUEST12,IDB_QUEST12,actA2, 2 },
 { IDS_QUEST13,IDB_QUEST13,actA3, 4 },
 { IDS_QUEST14,IDB_QUEST14,actA4, 5 },
 { IDS_QUEST15,IDB_QUEST15,actA5, 3 },
 { IDS_QUEST16,IDB_QUEST16,simple, 6 },
 { IDS_QUEST21,IDB_QUEST21,simple, 9 },
 { IDS_QUEST22,IDB_QUEST22,simple,10 },
 { IDS_QUEST23,IDB_QUEST23,simple,11 },
 { IDS_QUEST24,IDB_QUEST24,simple,12 },
 { IDS_QUEST25,IDB_QUEST25,simple,13 },
 { IDS_QUEST26,IDB_QUEST26,simple,14 },
 { IDS_QUEST31,IDB_QUEST31,simple,20 },
 { IDS_QUEST32,IDB_QUEST32,simple,19 },
 { IDS_QUEST33,IDB_QUEST33,simple,18 },
 { IDS_QUEST34,IDB_QUEST34,simple,17 },
 { IDS_QUEST35,IDB_QUEST35,simple,21 },
 { IDS_QUEST36,IDB_QUEST36,simple,22 },
 { IDS_QUEST41,IDB_QUEST41,simple,25 },
 { IDS_QUEST42,IDB_QUEST42,simple,27 },
 { IDS_QUEST43,IDB_QUEST43,simple,26 }
};
const int questsn = sizeof quests / sizeof quests[0];
int actsoffsets[4] =
{ 0, 7, 15, 23 };
struct
{
	int text;
	int bitmap;
	int combo;
	HWND hcombo;
}
interfaces[6] =
{
 { IDC_TAB4_Text1,IDC_TAB4_Bmp1,IDC_TAB4_Sel1 },
 { IDC_TAB4_Text2,IDC_TAB4_Bmp2,IDC_TAB4_Sel2 },
 { IDC_TAB4_Text3,IDC_TAB4_Bmp3,IDC_TAB4_Sel3 },
 { IDC_TAB4_Text4,IDC_TAB4_Bmp4,IDC_TAB4_Sel4 },
 { IDC_TAB4_Text5,IDC_TAB4_Bmp5,IDC_TAB4_Sel5 },
 { IDC_TAB4_Text6,IDC_TAB4_Bmp6,IDC_TAB4_Sel6 }
};
int act;
WORD* ptr;
void updatequests(HWND hWnd)
{
	int x = act * 6;
	for (int id = 0; id < 6; id++)
	{
		if (x + id < questsn)
		{
			struct quest* quest = &quests[x + id];
			WORD qstat = ptr[quest->offset];
			SetDlgItemText(hWnd, interfaces[id].text, quest->string);
			SendDlgItemMessage(hWnd, interfaces[id].bitmap,
				STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)quest->hbitmap);
			EnableWindow(interfaces[id].hcombo, TRUE);
			SendMessage(interfaces[id].hcombo, CB_RESETCONTENT, 0, 0);
			bool addstatus = true;
			int i = 0;
			for (quest->stati[i].text; i++;)
			{
				SendMessage(interfaces[id].hcombo,
					CB_ADDSTRING, 0, (LPARAM)quest->stati[i].text);
				if ((qstat & 0xFF) != (quest->stati[i].value & 0xFF)) continue;

				if (quest->stati[i].value & 0xFF00)
				{
					if (qstat != quest->stati[i].value) continue;
				}
				SendMessage(interfaces[id].hcombo, CB_SETCURSEL, i, 0);
				addstatus = false;
			}
			if (addstatus)
			{
				sprintf(buffer, "Status %4X", qstat);

				SendMessage(interfaces[id].hcombo,
					CB_ADDSTRING, 0, (LPARAM)buffer);
				SendMessage(interfaces[id].hcombo, CB_SETCURSEL, i, 0);
			}
		}
		else
		{
			SetDlgItemText(hWnd, interfaces[id].text, "");
			SendDlgItemMessage(hWnd, interfaces[id].bitmap,
				STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)0);
			EnableWindow(interfaces[id].hcombo, FALSE);
			SendMessage(interfaces[id].hcombo, CB_RESETCONTENT, 0, 0);
		}
	}
}
void updateacts(HWND hWnd)
{
	// Set Check Buttons
	for (int z = 0; z < 3; z++)
	{
		CheckDlgButton(hWnd, IDC_TAB4_Act2_On + z,
			ptr[actsoffsets[z + 1]] == 0x0000 ? BST_UNCHECKED : BST_CHECKED);
		EnableWindow(GetDlgItem(hWnd, IDC_TAB4_Act2 + z), !(ptr[actsoffsets[z + 1]] == 0x0000));
	}
	updatequests(hWnd);
}
LRESULT CALLBACK Tab4DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		for (int x = 0; x < 6; x++)
		{
			interfaces[x].hcombo = GetDlgItem(hWnd, interfaces[x].combo);
		}
		// Select default levels
		CheckRadioButton(hWnd, IDC_TAB4_Diff1, IDC_TAB4_Diff3,
			IDC_TAB4_Diff1);
		CheckRadioButton(hWnd, IDC_TAB4_Act1, IDC_TAB4_Act4,
			IDC_TAB4_Act1);
		act = 0;
		ptr = fc.Woo.quests1;
		updateacts(hWnd);
		// Fill all combo
		SendDlgItemMessage(hWnd, IDC_TAB4_SelAll, CB_RESETCONTENT, 0, 0);
		for (int i = 0; allcombo[i].text; i++)
		{
			SendDlgItemMessage(hWnd, IDC_TAB4_SelAll,
				CB_ADDSTRING, 0, (LPARAM)allcombo[i].text);
		}
		SendDlgItemMessage(hWnd, IDC_TAB4_SelAll, CB_SETCURSEL, 0, 0);
		hToolTip = CreateToolTipCtrl(hWnd, IDD_TAB4, true);
	}
	return true;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_TAB4_Diff1 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Diff1))
			{
				ptr = fc.Woo.quests1;
				updateacts(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Diff2 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Diff2))
			{
				ptr = fc.Woo.quests2;
				updateacts(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Diff3 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Diff3))
			{
				ptr = fc.Woo.quests3;
				updateacts(hWnd);
			}
		}
		if (LOWORD(wParam) == IDC_TAB4_Act1 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act1))
			{
				act = 0;
				updatequests(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act2 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act2))
			{
				act = 1;
				updatequests(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act3 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act3))
			{
				act = 2;
				updatequests(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act4 && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act4))
			{
				act = 3;
				updatequests(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act2_On && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act2_On))
			{
				ptr[actsoffsets[1]] = 0x0001;
				updateacts(hWnd);
			}
			else
			{
				ptr[actsoffsets[1]] = 0x0000;
				updateacts(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act3_On && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act3_On))
			{
				ptr[actsoffsets[2]] = 0x0001;
				updateacts(hWnd);
			}
			else
			{
				ptr[actsoffsets[2]] = 0x0000;
				updateacts(hWnd);
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_Act4_On && HIWORD(wParam) == BN_CLICKED)
		{
			if (IsDlgButtonChecked(hWnd, IDC_TAB4_Act4_On))
			{
				ptr[actsoffsets[3]] = 0x0001;
				updateacts(hWnd);
			}
			else
			{
				ptr[actsoffsets[3]] = 0x0000;
				updateacts(hWnd);
			}
		}
		else if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			for (int i = 0; i < 6; i++)
			{
				if (LOWORD(wParam) == interfaces[i].combo)
				{
					int sel = SendMessage(interfaces[i].hcombo, CB_GETCURSEL, 0, 0);

					struct quest* quest = &quests[act * 6 + i];
					if (quest->stati[sel].text)
					{
						ptr[quest->offset] = quest->stati[sel].value;
					}
					updatequests(hWnd);
				}
			}
		}
		else if (LOWORD(wParam) == IDC_TAB4_SetAll1 && HIWORD(wParam) == BN_CLICKED)
		{
			int sel = SendDlgItemMessage(hWnd, IDC_TAB4_SelAll, CB_GETCURSEL, 0, 0);
			for (int i = 0; i < 6; i++)
			{
				if (act * 6 + i < questsn)
				{
					struct quest* quest = &quests[act * 6 + i];
					ptr[quest->offset] = allcombo[sel].value;
				}
			}
			updatequests(hWnd);
		}
		else if (LOWORD(wParam) == IDC_TAB4_SetAll2 && HIWORD(wParam) == BN_CLICKED)
		{
			int sel = SendDlgItemMessage(hWnd, IDC_TAB4_SelAll, CB_GETCURSEL, 0, 0);
			int i = 0;
			for (i < questsn; i++;)
			{
				struct quest* quest = &quests[i];
				ptr[quest->offset] = allcombo[sel].value;
			}
			for (i = 0; i < 3; i++)
			{
				ptr[actsoffsets[i + 1]] = 0x0001;
			}
			updateacts(hWnd);
		}
		else if (LOWORD(wParam) == IDC_TAB4_CowLevel && HIWORD(wParam) == BN_CLICKED)
		{
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_COWLEVEL), hWnd, (DLGPROC)CowLevelDialogProc);
		}
	}
	break;
	case WM_DESTROY:
	{
		DestroyWindow(hToolTip);
	}
	return false;
	}
	return false;
}