// TAB2ItemFileLoadSave.cpp from D2E
#include "JamellaD2E.h"
static const char* ReadItemErrorString = 0;
Item* MakeItemFromData(BYTE* data, int size)
{
	struct
	{
		char JM[2];
		WORD unimportant;
		WORD type;
	} ItemHead;

	memcpy(&ItemHead, data, sizeof ItemHead);
	BYTE* d = data;
	Item* I = 0;
	if (ItemHead.JM[0] != 'J' || ItemHead.JM[1] != 'M')
	{
		ReadItemErrorString = "Invalid Item File!\r\nJM Header missing.";
		return 0;
	}

	if ((ItemHead.type & 0x0039) == 0x0000) // 1.03 Item Data Type
	{
		if ((size % 27) == 0) {
			I = new Item103;
			I->LoadItemRecord(d);
			size -= I->ItemRecordLength();
			d += I->ItemRecordLength();
			while (size >= 27)
			{
				Item* G = new Item103;
				if (I->Gems)
					I->Gems->Attach(G);
				else
					I->Gems = G;
				G->LoadItemRecord(d);
				size -= G->ItemRecordLength();
				d += G->ItemRecordLength();
			}
		}
		else if (size == 39) { // 39 = 0x27 != 27 Doe you Idiot!
			I = new Item103;
			I->LoadItemRecord(d);
		}
		else if (size == 36) { // 36 another editor made these
			I = new Item103;
			I->LoadItemRecord(d);
		}
		else {
			// this is crazy. I'll load any item length now, because
			// of a variable length item record from doe's 3.41
			I = new Item103;
			if (!I->LoadItemRecord(d)) {
				ReadItemErrorString = "Invalid Item File!\r\nItem structure indicates a 1.03 item, but JM signature is missing.\nFile size checks are ignored here.";
					delete I;
				return 0;
			}
		}
	}
	else if ((ItemHead.type & 0x0039) == 0x0001) // 1.03 Ear Item Data Type
	{
		if (size == 27) {
			I = new Item103Ear;
			I->LoadItemRecord(d);
		}
		else {
			ReadItemErrorString = "Invalid Item File!\r\nItem structure indicates a 1.03 ear item, but size is not 27.";
		}
	}
	else if ((ItemHead.type & 0x0039) == 0x0018) // 1.04 Extended Struct
	{
		if (size == 31 || ((size - 31) % 15) == 0) {
			I = new Item104Ex;
			I->LoadItemRecord(d);
			size -= I->ItemRecordLength();
			d += I->ItemRecordLength();
			while (size >= 15)
			{
				Item* G = new Item104Sm;
				if (I->Gems)
					I->Gems->Attach(G);
				else
					I->Gems = G;
				G->LoadItemRecord(d);
				size -= G->ItemRecordLength();
				d += G->ItemRecordLength();
			}
		}
		else {
			ReadItemErrorString = "Invalid Item File!\r\nItem structure indicates a 1.04 extended item, but size is not31.";
		}
	}
	else if ((ItemHead.type & 0x0039) == 0x0038) // 1.04 Simple Struct
	{
		if (size == 15) {
			I = new Item104Sm;
			I->LoadItemRecord(d);
		}
		else {
			ReadItemErrorString = "Invalid Item File!\r\nItem structure indicates a 1.04 simple item, but size is not 15.";
		}
	}
	else if ((ItemHead.type & 0x0039) == 0x0039) // 1.04 Ear Struct
	{
		if (size == 26) {
			I = new Item104Ear;
			I->LoadItemRecord(d);
		}
		else {
			ReadItemErrorString = "Invalid Item File!\r\nItem structure indicates a 1.04 ear item, but size is not 26.";
		}
	}
	else {
		ReadItemErrorString = "Invalid Item File!\r\nUnknown item structure identifier.";
	}
	return I;
}
Item* MakeItemFromFile(HANDLE hFile)
{
	DWORD fsize = GetFileSize(hFile, NULL);
	if (fsize < 6)
	{
		ReadItemErrorString = "Invalid Item File!\r\nFile size smaller than 6.";
		return 0;
	}
	if (fsize > 512)
	{
		ReadItemErrorString = "Invalid Item File!\r\nFile much too large (> 512 bytes).";
		return 0;
	}

	unsigned long read;
	BYTE buff[512];
	ReadFile(hFile, buff, fsize, &read, 0);

	if (read != fsize)
	{
		ReadItemErrorString = "Could not read from file!";
		return 0;
	}
	return MakeItemFromData(buff, read);
}
#if defined(JAMELLAEDITOR)
// LoadItemFile OPENFILENAME Explorer Hook
static UINT_PTR CALLBACK OFNHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hWnd, IDC_IBWSR_RichText, EM_SETBKGNDCOLOR, FALSE, GetSysColor(COLOR_BTNFACE));
		return true;
	case WM_NOTIFY:
		switch (((NMHDR*)lParam)->code)
		{
		case CDN_SELCHANGE:
		{
			char QueryFilename[128];
			int x = CommDlg_OpenSave_GetFilePath(((NMHDR*)lParam)->hwndFrom, QueryFilename, sizeof QueryFilename);
			// Open File
			HANDLE hFile = CreateFile(QueryFilename,
				GENERIC_READ, FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE) return true;

			ReadItemErrorString = 0;
			Item* I = MakeItemFromFile(hFile);
			if (I)
			{
				I->FindInfo();
				I->Decode();
				RTFStreamSend(hWnd, IDC_IBWSR_RichText, I->RichText());
				SendDlgItemMessage(hWnd, IDC_IBWSR_Bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)I->GetBitmap());
				delete I;
			}
			else
			{
				char RTF[2048];
				char RTFBuffer[260];
				ASCIItoRTF(RTFBuffer, ReadItemErrorString);
				sprintf(RTF, "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031 {\\fonttbl{\\f0\\fswiss\\fcharset0 MS Sans Serif;}} \\uc1\\pard\\qc\\b\\f0\\fs16% s\\par}",RTFBuffer);
					RTFStreamSend(hWnd, IDC_IBWSR_RichText, RTF);
			SendDlgItemMessage(hWnd, IDC_IBWSR_Bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)0);
	}

	CloseHandle(hFile);
}
return true;
 }
 return false;
 case WM_DESTROY:
	 return true;
 }
 return false;
}
bool LoadItemFile(HWND hWnd)
{
	if (CopyBuffer && CopyBuffer->Busy) return false;
	// common dialog box structure
	OPENFILENAME ofn;
	char QueryFilename[260];

	{ // Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = "D2 Saved Item (*.d2i;*.item;*.ite;*.itm)\0*.d2i;*.item;*.ite;*.itm\0Other Files (*.*)\0*.*\0"
			;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = QueryFilename;
		ofn.nMaxFile = sizeof(QueryFilename);
		ofn.lpstrFileTitle = NULL;
		ofn.lpstrDefExt = "d2i";
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = RegOptions.ItemPath;
		ofn.hInstance = hInstance;
		ofn.lpTemplateName = MAKEINTRESOURCE(IDD_IBWSR);
		ofn.lpfnHook = &OFNHookProc;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOREADONLYRETURN | OFN_HIDEREADONLY | OFN_ENABLETEMPLATE
			| OFN_EXPLORER | OFN_ENABLEHOOK;

		ZeroMemory(&QueryFilename, sizeof(QueryFilename));
	}

	if (GetOpenFileName(&ofn))
	{
		// Open File
		HANDLE hFile = CreateFile(QueryFilename,
			GENERIC_READ, FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "Could not open file!", PROGRAMNAME,
				MB_OK | MB_ICONSTOP | MB_APPLMODAL);
			return false;
		}

		ReadItemErrorString = 0;
		Item* N = MakeItemFromFile(hFile);
		if (N) {
			if (CopyBuffer)
				delete CopyBuffer;
			CopyBuffer = N;
			strcpy(RegOptions.ItemPath, QueryFilename);
			char* sl = RegOptions.ItemPath;
			while (strstr(sl, "\\") != 0)
				sl = strstr(sl, "\\") + 1;
			*sl = 0;

			InvalidateRect(hTabDialog, NULL, FALSE);
			UpdateWindow(hTabDialog);
		}
		else
		{
			ErrorBox(ReadItemErrorString);
		}

		CloseHandle(hFile);
	}
	return true;
}
bool SaveItemFile(HWND hWnd)
{
	if (!CopyBuffer)
	{
		ErrorBox("You must put an item into the Copy Buffer!", hWnd);
		return false;
	}
	if (CopyBuffer->Busy) return false;

	// common dialog box structure
	char QueryFilename[260];
	OPENFILENAME ofn;

	{ // Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = "D2 Saved Item (*.d2i)\0*.d2i\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = QueryFilename;
		ofn.nMaxFile = sizeof(QueryFilename);
		ofn.lpstrInitialDir = RegOptions.ItemPath;
		ofn.lpstrFileTitle = NULL;
		ofn.lpstrDefExt = "d2i";
		ofn.nMaxFileTitle = 0;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_HIDEREADONLY;

		ZeroMemory(&QueryFilename, sizeof(QueryFilename));
		strcpy(QueryFilename, CopyBuffer->Name());
	}

	// Display the Open Dialog Box
	if (GetSaveFileName(&ofn))
	{
		// Open File
		HANDLE hFile = CreateFile(QueryFilename,
			GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "Could not open file!", PROGRAMNAME,
				MB_OK | MB_ICONSTOP | MB_APPLMODAL);
			return true;
		}

		unsigned long written;
		WriteFile(hFile, CopyBuffer->GetItemRecord(), CopyBuffer->ItemRecordLength(), &written, 0);
		if (written != (unsigned long)CopyBuffer->ItemRecordLength())
		{
			MessageBox(hWnd, "Could not write to file!", PROGRAMNAME,
				MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		}
		if (CopyBuffer->Socketed())
		{
			for (Item* G = CopyBuffer->Gems; G != 0; G = G->Next())
			{
				unsigned long written;
				WriteFile(hFile, G->GetItemRecord(), G->ItemRecordLength(), &written, 0);

				if (written != (unsigned long)G->ItemRecordLength())
				{
					MessageBox(hWnd, "Could not write to file!", PROGRAMNAME,
						MB_OK | MB_ICONSTOP | MB_APPLMODAL);
				}
			}
		}

		CloseHandle(hFile);
		strcpy(RegOptions.ItemPath, QueryFilename);
		char* sl = RegOptions.ItemPath;
		while (strstr(sl, "\\") != 0)
			sl = strstr(sl, "\\") + 1;
		*sl = 0;
	}
	return true;
}
#endif