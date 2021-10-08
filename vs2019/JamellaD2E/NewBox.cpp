// NewBox.cpp from D2E
#include "JamellaD2E.h"
inline bool ValidateName(const char* s)
{
	if (*s == 0) return false;
	while (*s)
	{
		if (!isalpha(*s))
			return false;
		s++;
	}
	return true;
}
inline void MangleNames(char* d, const char* file, const char* nname, const char* oname)
{
	int ol = strlen(oname);
	d += strlen(d);
	while (*file)
	{
		if (oname && *file == *oname && strncmp(file, oname, ol) == 0) {
			strcpy(d, nname);
			d += strlen(nname);
			file += ol;
			oname = 0;
		}
		else {
			*d++ = *file++;
			*d = 0;
		}
	}
}
struct
{
	char* name;
	char* rc;
	char* description;
}
CharTemplates[] =
{
 { "Tyrael", MAKEINTRESOURCE(IDN_SetTyrael), "Fully equipped set: Angelic Raidament" }
};
LRESULT CALLBACK NewDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_NEW_Class, CB_RESETCONTENT, 0, 0);
		for (int i = 0; i < 5; i++)
			SendDlgItemMessage(hWnd, IDC_NEW_Class, CB_ADDSTRING, 0, (LPARAM)CharClasses[i]);
		SendDlgItemMessage(hWnd, IDC_NEW_Class, CB_SETCURSEL, 0, 0);
		//for(int z=0;z
		//IDC_NEW_Templates
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NEW_CreateNewbie:
		{
			GetDlgItemText(hWnd, IDC_NEW_Name, buffer, 256);
			if (!ValidateName(buffer))
			{
				ErrorBox("Invalid Name:\nOnly A-Z & a-z allowed!");
				return false;
			}
			int Class = SendDlgItemMessage(hWnd, IDC_NEW_Class, CB_GETCURSEL, 0, 0);

			const char* rc = (Class == 0) ? MAKEINTRESOURCE(IDN_Amazon) :
				(Class == 1) ? MAKEINTRESOURCE(IDN_Sorceress) :
				(Class == 2) ? MAKEINTRESOURCE(IDN_Necromancer) :
				(Class == 3) ? MAKEINTRESOURCE(IDN_Paladin) :
				(Class == 4) ? MAKEINTRESOURCE(IDN_Barbarian) : 0;

			HRSRC rcsrc = FindResource(hInstance, rc, "D2S");
			HGLOBAL hglb = LoadResource(hInstance, rcsrc);
			int size = SizeofResource(hInstance, rcsrc);
			BYTE* mem = (BYTE*)LockResource(hglb);
			int erroffset = fc.transferdata(mem, size);
			if (erroffset < 0)
			{
				fc.loaded = true;

				fc.setfilename(buffer);
				strcpy(fc.Header.playername, buffer);
			}
			else
			{
				char text[80];

				sprintf(text, "Corrupt field encountered in save game data! (@ offset %i)", erroffset);
				MessageBox(hWnd, text, PROGRAMNAME, MB_OK | MB_ICONSTOP | MB_APPLMODAL);
				fc.clear();
			}
			EndDialog(hWnd, IDOK);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, wParam);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}
LRESULT CALLBACK RenameDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetDlgItemText(hWnd, IDC_RENAME_Name, fc.Header.playername);
		CheckRadioButton(hWnd, IDC_RENAME_Move, IDC_RENAME_Copy, IDC_RENAME_Move);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			char NewName[32];
			GetDlgItemText(hWnd, IDC_RENAME_Name, NewName, 32);
			if (!ValidateName(NewName))
			{
				ErrorBox("Invalid Name:\nOnly A-Z & a-z allowed!");
				return false;
			}
			char sdir[260];
			if (GetDiabloSaveDirectory(sdir))
			{
				char srchdir[260];
				sprintf(srchdir, "%s\\%s.*", sdir, NewName);
				WIN32_FIND_DATA FData;
				HANDLE hFind = FindFirstFile(srchdir, &FData);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					if (MessageBox(hWnd, "A character with the name already exists!\nDo you want to delete him/her?",
						PROGRAMNAME, MB_YESNO | MB_ICONEXCLAMATION) != IDYES) {
						FindClose(hFind);
						return false;
					}
					int fLoop = true;
					while (fLoop)
					{
						char dfile[260];
						sprintf(dfile, "%s\\%s", sdir, FData.cFileName);
						DeleteFile(dfile);

						fLoop = FindNextFile(hFind, &FData);
					}
					FindClose(hFind);
				}
			}

			if (IsDlgButtonChecked(hWnd, IDC_RENAME_Move) == BST_CHECKED)
			{
				char srch[260];
				sprintf(srch, "%s\\%s.*", sdir, fc.Header.playername);
				WIN32_FIND_DATA FData;
				HANDLE hFind = FindFirstFile(srch, &FData);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					int fLoop = true;
					while (fLoop)
					{
						char nfile[260];
						strcpy(nfile, sdir);
						strcat(nfile, "\\");
						MangleNames(nfile, FData.cFileName, NewName, fc.Header.playername);
						if (!MoveFile(FData.cFileName, nfile))
						{
							sprintf(buffer, "Error moving file '%s' to '%s'!\nPress OK to continue.", FData.cFileName,
								nfile);
							ErrorBox(buffer);
						}

						fLoop = FindNextFile(hFind, &FData);
					}
					FindClose(hFind);
				}

				fc.setfilename(NewName);
				strcpy(fc.Header.playername, NewName);
			}
			else
			{
				fc.setfilename(NewName);
				strcpy(fc.Header.playername, NewName);
			}
			EndDialog(hWnd, IDOK);
			return true;
		}
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, wParam);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}