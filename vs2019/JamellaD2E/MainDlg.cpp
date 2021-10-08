// MainDlg.cpp from D2E
#include "JamellaD2E.h"
HWND hMainDialog = 0;
RECT rMainDialog;
// Tab Control
HWND hTab;
RECT rTab, rTabDialog;
int TabSelection = 0;
HWND hTabDialog;
// Procedure Prototypes
LRESULT CALLBACK MainDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TabDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
struct maindlgtab Tabs[] =
{
 { IDD_TAB1, IDB_TAB1, "Stats", &Tab1DialogProc },
 { IDD_TAB2, IDB_TAB2, "Inventory", &Tab2DialogProc },
 /*
 #if INVGRIDS == 0
  { IDD_TAB2, IDB_TAB2, "Inventory", &Tab2DialogProc },
 #else
  { IDD_TAB2ExGrid, IDB_TAB2, "Inventory", &Tab2DialogProc },
 #endif
 */
  { IDD_TAB3, IDB_TAB3, "Skills", &Tab3DialogProc },
  { IDD_TAB4, IDB_TAB4, "Quests", &Tab4DialogProc },
  { IDD_TAB5, IDB_TAB5, "Waypoints", &Tab5DialogProc },
};
struct maindlgtab JamellaTab =
{ IDD_TAB0, 0, "Jamella", &Tab0DialogProc };
const int Tabsn = sizeof Tabs / sizeof Tabs[0];
void ResizeDialog()
{
	// Calculate Tab Control Size
	GetWindowRect(hTabDialog, &rTab);
	TabCtrl_AdjustRect(hTab, TRUE, &rTab);
	rTab.right -= rTab.left;
	rTab.bottom -= rTab.top;
	rTab.top = 0;
	rTab.left = 0;
	SetWindowPos(hTab, NULL,
		0, 0,
		rTab.right, rTab.bottom,
		SWP_NOZORDER);
	// Adjust for Dialog Size
	CopyRect(&rTabDialog, &rTab);
	TabCtrl_AdjustRect(hTab, FALSE, &rTabDialog);
	// Center and resize Main Dialog Box
	{
		RECT WorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);

		int width = rTab.right + 2 * GetSystemMetrics(SM_CXDLGFRAME);
		int height = rTab.bottom + 2 * GetSystemMetrics(SM_CXDLGFRAME)
			+ GetSystemMetrics(SM_CYMENU)
			+ GetSystemMetrics(SM_CYCAPTION);
		int top = (WorkArea.bottom - WorkArea.top - height) / 2;
		int left = (WorkArea.right - WorkArea.left - width) / 2;
		SetWindowPos(hMainDialog, NULL,
			left, top,
			width, height,
			SWP_NOZORDER);
	}
}
void switchtab(int i)
{
	if (i < 0 && TabSelection >= 0) {

		if (hTabDialog) {
			DestroyWindow(hTabDialog);
		}
		// Show Jamella
		TabCtrl_SetCurSel(hTab, -1);
		TabSelection = -1;
		hTabDialog = CreateDialogIndirect(
			hInstance,
			JamellaTab.dialogtemplate,
			hTab,
			(DLGPROC)JamellaTab.proc);
		SetWindowPos(hTabDialog, HWND_TOP,
			rTabDialog.left, rTabDialog.top, 0, 0,
			SWP_NOSIZE);
		ResizeDialog();

		ShowWindow(hTabDialog, SW_SHOW);
	}
	else {
		TabCtrl_SetCurSel(hTab, i);
		TabSelection = i;
	}
}
bool validatetab()
{
	if (hTabDialog)
		if (!SendMessage(hTabDialog, WM_VALIDATE, 0, 0))
		{
			return true;
		}
	return false;
}
int MainDialog(const char* CmdLine)
{
	// First register a Window Class
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = MainDialogProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "D2EMainDialog";
	wcx.hIconSm = wcx.hIcon;
	RegisterClassEx(&wcx);
	HMENU hMainMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MAINDIALOG));
	// Secondly create the Main Dialog Box
	hMainDialog = CreateWindowEx(
		0,
		"D2EMainDialog", PROGRAMNAME,
		WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		500,
		(HWND)NULL,
		hMainMenu,
		hInstance,
		(LPVOID)NULL);
	if (!hMainDialog) return ErrorMessage();
	// Setup Tab Control
	HIMAGELIST himl = ImageList_Create(17, 16, ILC_COLORDDB, Tabsn, 0);
	for (int i = 0; i < Tabsn; i++)
	{
		HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(Tabs[i].idicon));
		Tabs[i].imagelistindex = ImageList_Add(himl, hBmp, NULL);
		DeleteObject(hBmp);
	}

	GetClientRect(hMainDialog, &rMainDialog);
	// Create Tab Control
	hTab = CreateWindow(WC_TABCONTROL, "",
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_HOTTRACK,
		rMainDialog.left, rMainDialog.top,
		rMainDialog.right - rMainDialog.left, rMainDialog.bottom - rMainDialog.top,
		hMainDialog, NULL, hInstance, NULL);

	if (!hTab) {
		ErrorMessage();
		DestroyWindow(hMainDialog);
		return false;
	}
	TabCtrl_SetImageList(hTab, himl);
	// Set nice font
	HFONT fTab = CreateFont(8, 0,
		0, 0, 0,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		"MS Sans Serif");
	PostMessage(hTab, WM_SETFONT, (WPARAM)fTab, TRUE);
	// Load Tab Dialogs
	for (int j = 0; j < Tabsn; j++)
	{
		TC_ITEM tie;
		tie.mask = TCIF_TEXT | (Tabs[j].idicon ? TCIF_IMAGE : NULL);
		tie.iImage = Tabs[j].imagelistindex;
		tie.pszText = Tabs[j].text;
		TabCtrl_InsertItem(hTab, j, &tie);
		HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(Tabs[j].iddialog), RT_DIALOG);
		HGLOBAL hglb = LoadResource(hInstance, hrsrc);
		Tabs[j].dialogtemplate = (DLGTEMPLATE*)LockResource(hglb);
	}
	// Load Jamella Dialog
	{
		HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(JamellaTab.iddialog), RT_DIALOG);
		HGLOBAL hglb = LoadResource(hInstance, hrsrc);
		JamellaTab.dialogtemplate = (DLGTEMPLATE*)LockResource(hglb);
	}
	// Load Progress Dialog
	{
		HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(IDD_PROGRESS), RT_DIALOG);
		HGLOBAL hglb = LoadResource(hInstance, hrsrc);
		DLGTEMPLATE* dlgtemp = (DLGTEMPLATE*)LockResource(hglb);
		hTabDialog = CreateDialogIndirect(
			hInstance,
			dlgtemp,
			hTab,
			(DLGPROC)TabDialogProc);
	}
	TabCtrl_SetCurSel(hTab, -1);
	TabSelection = -1;
	// Calculate Tab Control Size
	GetWindowRect(hTabDialog, &rTab);
	TabCtrl_AdjustRect(hTab, TRUE, &rTab);
	rTab.right -= rTab.left;
	rTab.bottom -= rTab.top;
	rTab.top = 0;
	rTab.left = 0;
	SetWindowPos(hTab, NULL,
		0, 0,
		rTab.right, rTab.bottom,
		SWP_NOZORDER);
	// Adjust for Dialog Size
	CopyRect(&rTabDialog, &rTab);
	TabCtrl_AdjustRect(hTab, FALSE, &rTabDialog);
	SetWindowPos(hTabDialog, HWND_TOP,
		rTabDialog.left, rTabDialog.top, 0, 0,
		SWP_NOSIZE);
	ResizeDialog();
	// Set initial states of main controls
	SendDlgItemMessage(hTabDialog, IDC_PROGRESS_Bar, PBM_SETBARCOLOR, 0, PROGRESSCOLOR);
	EnableWindow(hTab, FALSE);
	for (int z = 0; z < GetMenuItemCount(hMainMenu); z++) {
		EnableMenuItem(hMainMenu, z, MF_BYPOSITION | MF_GRAYED);
	}
	// Show Progress Dialog and Load Resources
	ShowWindow(hMainDialog, SW_SHOW);
	ShowWindow(hTabDialog, SW_SHOW);
	D2ELoadResources();
	CheckShellRegistry();
	// Load inital file
	if (CmdLineFile())
		fc.loadfile(hTabDialog, CmdLineFile());
	SetInvGridPreset(1);
	// Set working states of main controls
	EnableWindow(hTab, TRUE);
	for (int z = 0; z < GetMenuItemCount(hMainMenu); z++) {
		EnableMenuItem(hMainMenu, z, MF_BYPOSITION | MF_ENABLED);
	}
	// Switch to Jamella's Dialog
	TabSelection = 0;
	switchtab(-1);
	// Load Accelerators
	HACCEL accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN));
	// Start the message loop
	MSG msg;
	while (GetMessage(&msg, (HWND)NULL, 0, 0))
	{
		if (!TranslateAccelerator(hMainDialog, accel, &msg)) {
			TranslateMessage(&msg);
		}
		DispatchMessage(&msg);
	}
	D2EUnLoadResources();
	// Free Main Dialog Resources
	DeleteObject(fTab);
	ImageList_Destroy(himl);
	DestroyMenu(hMainMenu);
	return msg.wParam;
}
char QueryFilename[260];
int QueryOpenFileName(HWND hWnd)
{
	// Retrieve Registry Key from Diablo 2
	char fdir[260];
	{
		HKEY regkey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Blizzard Entertainment\\Diablo II", 0, KEY_READ, &regkey) == ERROR_SUCCESS)
		{
			DWORD type = REG_SZ;
			DWORD fdirsize = 260;
			DWORD x = RegQueryValueEx(regkey, "Save Path", 0, &type, (unsigned char*)fdir, &fdirsize);
			if (x != ERROR_SUCCESS)
			{
				memset(fdir, 0, sizeof fdir);
			}
			RegCloseKey(regkey);
		}
	}
	// common dialog box structure
	OPENFILENAME ofn;

	{ // Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = "D2 Save Game (*.d2s)\0*.d2s\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = QueryFilename;
		ofn.nMaxFile = sizeof(QueryFilename);
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		if (fdir[0]) ofn.lpstrInitialDir = fdir;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOREADONLYRETURN | OFN_HIDEREADONLY;

		ZeroMemory(&QueryFilename, sizeof(QueryFilename));
	}
	// Display the Open Dialog Box
	return GetOpenFileName(&ofn);
}
LRESULT CALLBACK MainDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDR_OPEN:
			if (!validatetab())
			{
				if (fc.isloaded())
				{
					if (!RegOptions.NoAnnoyingMsgs) {
						int user = MessageBox(hWnd, "Do you really want to discard all changes?", PROGRAMNAME,
							MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

						if (user != IDYES) break;
					}
					fc.discardfile(hWnd);
					switchtab(-1);
				}
				if (QueryOpenFileName(hWnd))
				{
					fc.loadfile(hWnd, QueryFilename);
				}
			}
			return true;
		case IDR_NEW:
			if (!validatetab())
			{
				if (fc.isloaded())
				{
					if (!RegOptions.NoAnnoyingMsgs) {

						int user = MessageBox(hWnd, "Do you really want to discard all changes?", PROGRAMNAME,
							MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

						if (user != IDYES) break;
					}
					fc.discardfile(hWnd);
					switchtab(-1);
				}
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_NEW), hWnd, (DLGPROC)NewDialogProc);
			}
			return true;
		case IDR_SAVE:
			if (!validatetab())
			{
				if (fc.isloaded())
				{
					SendMessage(hTabDialog, WM_DESTROY, 0, 0);
					fc.savefile(hWnd);
				}
				else
				{
					MessageBox(hWnd, "You didn't open a file!", PROGRAMNAME,
						MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
				}
			}
			return true;
		case IDR_RELOAD:
		{
			if (fc.isloaded())
			{
				if (!RegOptions.NoAnnoyingMsgs) {
					int user = MessageBox(hWnd, "Do you really want to discard all changes?", PROGRAMNAME,
						MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

					if (user != IDYES) break;
				}
				switchtab(-1);
				fc.reloadfile(hWnd);
			}
			else
			{
				MessageBox(hWnd, "You didn't open a file!", PROGRAMNAME,
					MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
			}
		}
		return true;
		case IDR_CLOSE:
		{
			if (fc.isloaded())
			{
				if (!RegOptions.NoAnnoyingMsgs) {
					int user = MessageBox(hWnd, "Do you really want to discard all changes?", PROGRAMNAME,
						MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

					if (user != IDYES) break;
				}
				fc.discardfile(hWnd);
				switchtab(-1);
			}
			else
			{
				MessageBox(hWnd, "You didn't open a file!", PROGRAMNAME,
					MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
			}
		}
		return true;
		case IDR_UOPTIONS:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_UOPTIONS), hWnd, (DLGPROC)UOptionsDialogProc);
			return true;
		case IDR_EOPTIONS:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_EOPTIONS), hWnd, (DLGPROC)EOptionsDialogProc);
			return true;
		case IDR_TEXTFILE:
			WriteTextSummary(hWnd);
			return true;
		case IDR_INFO:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_INFO), hWnd, (DLGPROC)InfoDialogProc);
			return true;
		case IDCANCEL:
		case IDR_EXIT:
			if (fc.isloaded())
			{
				if (RegOptions.NoAnnoyingMsgs || MessageBox(hMainDialog, "Are you sure you want to exit and discard all changes ? ",
					PROGRAMNAME, MB_YESNO) == IDYES)
				{
					PostQuitMessage(0);
					DestroyWindow(hMainDialog);
				}
			}
			else
			{
				PostQuitMessage(0);
				DestroyWindow(hMainDialog);
			}
			return true;
		}
		break;
	case WM_NOTIFY:
	{
		NMHDR nmh = *(LPNMHDR)lParam;
		if (nmh.hwndFrom == hTab)
		{
			switch (nmh.code)
			{
			case TCN_SELCHANGING:
				if (!fc.isloaded())
				{
					if (QueryOpenFileName(hWnd))
					{
						fc.loadfile(hWnd, QueryFilename);
					}
					return true;
				}
				if (hTabDialog)
					if (!SendMessage(hTabDialog, WM_VALIDATE, 0, 0))
					{
						return true;
					}
				return false;

			case TCN_SELCHANGE:
			{
				// Get Selection
				TabSelection = TabCtrl_GetCurSel(hTab);
				// Destroy the current child dialog box, if any.
				if (hTabDialog != NULL)
					DestroyWindow(hTabDialog);
				// Create the new child dialog box.
				if (Tabs[TabSelection].dialogtemplate)
				{
					hTabDialog = CreateDialogIndirect(
						hInstance,
						Tabs[TabSelection].dialogtemplate,
						hTab,
						(DLGPROC)(Tabs[TabSelection].proc ? Tabs[TabSelection].proc : TabDialogProc));
					SetWindowPos(hTabDialog, HWND_TOP,
						rTabDialog.left, rTabDialog.top, 0, 0,
						SWP_NOSIZE);
					ResizeDialog();
					ShowWindow(hTabDialog, SW_SHOW);
				}
				return true;
			}
			}
		}
	}
	break;
	case WM_CLOSE:
		if (fc.isloaded())
		{
			if (RegOptions.NoAnnoyingMsgs || MessageBox(hMainDialog, "Are you sure you want to exit and discard all changes ? ",
				PROGRAMNAME, MB_YESNO) == IDYES)
			{
				PostQuitMessage(0);
				DestroyWindow(hMainDialog);
			}
		}
		else
		{
			PostQuitMessage(0);
			DestroyWindow(hMainDialog);
		}
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return false;
	case WM_CHAR:
		PostMessage(hTabDialog, WM_CHAR, wParam, lParam);
		return false;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK TabDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
	}
	return true;
	}
	return false;
}