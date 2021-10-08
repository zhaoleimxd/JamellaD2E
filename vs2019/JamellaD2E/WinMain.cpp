// Main.cpp from D2E
#include "JamellaD2E.h"
#include <time.h>
#include <shlwapi.h>
HINSTANCE hInstance;
inline void DoCRC();
DWORD GetDllVersion(LPCTSTR DllName)
{
	HINSTANCE hInst;
	DWORD dwVersion = 0;
	hInst = LoadLibrary(DllName);

	if (hInst)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hInst, "DllGetVersion");
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if (hr == NOERROR)
			{
				dwVersion = MAKEWORD(dvi.dwMinorVersion, dvi.dwMajorVersion);
			}
		}
		FreeLibrary(hInst);
	}
	return dwVersion;
}
int PASCAL WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine,
	int nCmdShow)
{
	::hInstance = hInstance;
	srand((unsigned)time(NULL));
	if (GetDllVersion(TEXT("comctl32.dll")) < _WIN32_IE)
	{
		ErrorBox("Your system uses a Common Control Library prior to 5.80.\nThe editor requires one >= 5.80.\nDownload it from my web pages http ://jamella.dyns.cx or from Microsoft.");
		return -1;
	}
	INITCOMMONCONTROLSEX cci;
	cci.dwSize = sizeof cci;
	cci.dwICC = ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES;
	if (!InitCommonControlsEx(&cci))
	{
		ErrorBox("The editor could not initialize the common control library.\nThis is major operating system failure. You better watch out.");
		return -1;
	}
	HINSTANCE hRichEdit = LoadLibrary("RICHED32.DLL");
	if (!hRichEdit)
	{
		ErrorBox("The editor could not load the RichEdit library.\nDownload it from my web pages http://jamella.dyns.cx or from Microsoft.");
		return -1;
	}
	// Parse Command Line Options
	ParseCommandLine();
	// DoCRC();
	LoadEditorRegistryValues();
	int x = MainDialog(lpszCmdLine);
	FreeLibrary(hRichEdit);
	return x;
}
int ErrorMessage()
{
	char str[260];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(), 0,
		(LPTSTR)str, 260,
		NULL);
	MessageBox(NULL, str, "Diablo 2 Save Game Editor",
		MB_OK | MB_ICONINFORMATION);
	return false;
}
inline void DoCRC()
{
	// Open File
	HANDLE hFile = CreateFile(ProgramFilePath(),
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Could not open program file for CRC checking!", PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		exit(0);
	}
	const int CRCsize = 1024 * 3 * 4;
	BYTE CRCbuffer[CRCsize];
	DWORD* CRCbufferd = (DWORD*)CRCbuffer;
	DWORD CRCread;
	DWORD fsizeread = 0;
	DWORD CRC = 0;
	while (ReadFile(hFile, CRCbuffer, CRCsize, &CRCread, NULL))
	{
		if (CRCread == 0) break;
		fsizeread += CRCread;
		for (int z = 0; z < CRCsize / 3 / 4; z += 3)
		{
			CRC += CRCbufferd[z + 0] * 1;
			CRC += CRCbufferd[z + 1] * 3;
			CRC += CRCbufferd[z + 2] * 7;
		}
	}
	if (CRC != 0x00000000)
	{
		MessageBox(NULL, "CRC check of program file failed! This executable was tampered with.\nGet a new one from http:// jamella.dyns.cx!", PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		exit(0);
	}
	CloseHandle(hFile);
}