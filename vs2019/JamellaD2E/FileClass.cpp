// FileClass.cpp from D2E
#include "JamellaD2E.h"
class fileclass fc;
// Character Structs
const char Sign_Header[9] = "\x55\xAA\x55\xAA\x47\x00\x00\x00";
const char Sign_Woo[9] = "\x57\x6F\x6F\x21\x06\x00\x00\x00";
const char Sign_WS[9] = "\x57\x53\x01\x00\x00\x00\x50\x00";
const char Sign_w4[5] = "\x01\x77\x34\x00";
const char Sign_gf[3] = "\x67\x66";
const char Sign_if[3] = "\x69\x66";
const char Sign_JM[3] = "\x4A\x4D";
BYTE* rest;
int restsize;
fileclass::fileclass()
{
	clear();
}
fileclass::~fileclass()
{
	clear();
}
void fileclass::clear()
{
	loaded = false;
	ZeroMemory(&Header, sizeof(Header));
	ZeroMemory(&Woo, sizeof(Woo));
	ZeroMemory(&WS, sizeof(WS));
	ZeroMemory(&w4, sizeof(w4));
	ZeroMemory(&gf, sizeof(gf));
	ZeroMemory(&IF, sizeof(IF));
	ZeroMemory(&JMinv, sizeof(JMinv));
	if (Items)
	{
		delete Items;
		Items = 0;
	}
	if (rest)
	{
		delete[restsize] rest;
		rest = 0;
		restsize = 0;
	}
}
int fileclass::transferdata(BYTE* data, int size)
{
	int JMn = 0;
	for (BYTE* a = data; a < data + size;)
	{
		char* Sign = (char*)a;
		if (strncmp(Sign, Sign_Header, sizeof Sign_Header - 1) == 0)
		{
			memcpy(&Header, a, sizeof Header);
			a += sizeof Header;
		}
		else if (strncmp(Sign, Sign_Woo, sizeof Sign_Woo - 1) == 0)
		{
			memcpy(&Woo, a, sizeof Woo);
			a += sizeof Woo;
		}
		else if (strncmp(Sign, Sign_WS, sizeof Sign_WS - 1) == 0)
		{
			memcpy(&WS, a, sizeof WS);
			a += sizeof WS;
		}
		else if (strncmp(Sign, Sign_w4, sizeof Sign_w4 - 1) == 0)
		{
			memcpy(&w4, a, sizeof w4);
			a += sizeof w4;
		}
		else if (strncmp(Sign, Sign_gf, sizeof Sign_gf - 1) == 0)
		{
			memset(&gf, 0, sizeof gf);
			memcpy(&gf, a, sizeof 5); // sig & field info
			a += 5;
			WORD fields = gf.fields;

			for (int i = 0; i < 16; i++)
			{
				if (fields & 1)
				{
					((DWORD*)&gf.strength)[i] = *(DWORD*)a;
					a += 4;
				}
				fields /= 2;
			}
			gf.health /= 256;
			gf.healthmax /= 256;
			gf.mana /= 256;
			gf.manamax /= 256;
			gf.stamina /= 256;
			gf.staminamax /= 256;
		}
		else if (strncmp(Sign, Sign_if, sizeof Sign_if - 1) == 0)
		{
			memcpy(&IF, a, sizeof IF);
			a += sizeof IF;
		}
		else if (strncmp(Sign, Sign_JM, sizeof Sign_JM - 1) == 0)
		{
			if (JMn == 0)
			{ // first entry
				memcpy(&JMinv, a, sizeof JMinv);
				a += sizeof JMinv;
				JMn++;
			} // first entry
			else if (Items->Count() < JMinv.num)
			{ // this belongs into the inventory
				struct
				{
					char JM[2];
					WORD unimportant;
					WORD type;
				} ItemHead;
				memcpy(&ItemHead, a, sizeof ItemHead);
				class Item* I = 0;

				if ((ItemHead.type & 0x0039) == 0x0000) // 1.03 Item Data Type
				{
					I = new Item103;
				}
				else if ((ItemHead.type & 0x0039) == 0x0001) // 1.03 Ear Item Data Type
				{
					I = new Item103Ear;
				}
				else if ((ItemHead.type & 0x0039) == 0x0018) // 1.04 Extended Struct
				{
					I = new Item104Ex;
				}
				else if ((ItemHead.type & 0x0039) == 0x0038) // 1.04 Simple Struct
				{
					I = new Item104Sm;
				}
				else if ((ItemHead.type & 0x0039) == 0x0039) // 1.04 Ear Struct
				{
					I = new Item104Ear;
				}
				else {
					return a - data;
				}
				if (Items) Items->Attach(I);
				else Items = I;

				I->LoadItemRecord(a);
				a += I->ItemRecordLength();
				if (I->Socketed())
				{// Read additional socketed gem records
					for (int g = 0; g < I->GemNum(); g++)
					{
						memcpy(&ItemHead, a, sizeof ItemHead);

						class Item* G = 0;
						if ((ItemHead.type & 0x0039) == 0x0000) // 1.03 Item Data Type
						{
							G = new Item103;
						}
						else if ((ItemHead.type & 0x0039) == 0x0038) // 1.04 Simple Struct
						{
							G = new Item104Sm;
						}
						else {
							return a - data;
						}
						if (I->Gems == 0) I->Gems = G;
						else I->Gems->Attach(G);
						G->LoadItemRecord(a);
						a += G->ItemRecordLength();
					}
				}// Read additional socketed gem records
			} // this belongs into the inventory
			else
			{ // rest data
				restsize = data + size - a;
				rest = new BYTE[restsize];
				memcpy(rest, a, restsize);
				a += restsize;
			} // rest data
		}
		else
			return a - data;
	}
	return -1;
}
bool fileclass::loadfile(HWND hWnd, const char* tempfilename)
{
	if (loaded)
	{
		return false;
	}
	// Open File
	HANDLE hFile = CreateFile(tempfilename,
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "Could not open file!", PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}

	DWORD filesize = GetFileSize(hFile, NULL);

	BYTE* filedata = new BYTE[filesize];
	DWORD fileread;

	// read whole file
	ReadFile(hFile, filedata, filesize, &fileread, NULL);

	if (filesize != fileread)
	{
		delete filedata;
		CloseHandle(hFile);
		MessageBox(hWnd, "Could not read from file!", PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}
	int erroffset = transferdata(filedata, filesize);
	if (erroffset >= 0)
	{
		delete filedata;
		CloseHandle(hFile);
		char text[80];

		sprintf(text, "Corrupt field encountered in save game file! (@ offset %i)", erroffset);
		MessageBox(hWnd, text, PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		clear();
		return false;
	}

	delete filedata;
	CloseHandle(hFile);
	loaded = true;
	memcpy(filename, tempfilename, sizeof filename);
	return true;
}
bool fileclass::reloadfile(HWND hWnd)
{
	clear();
	return loadfile(hWnd, filename);
}
DWORD offset;
inline bool check_written(HWND hWnd, DWORD a, DWORD b, HANDLE hFile)
{
	if (a != b)
	{
		sprintf(buffer, "Could not write to file at offset %i!", offset);
		MessageBox(hWnd, buffer, PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		CloseHandle(hFile);
		return true;
	}
	else return false;
}
bool fileclass::savefile(HWND hWnd)
{
	if (!loaded)
	{
		MessageBox(hWnd, "You didn't open a file!", PROGRAMNAME,
			MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
		return false;
	}
	int user = DialogBox(hInstance, MAKEINTRESOURCE(IDD_SAVE), hWnd, (DLGPROC)SaveDialogProc);
	if (user == 0) return false;
	if (user == 2)
	{ // Copy saved game file
		char newfilename[260];
		strcpy(newfilename, filename);
		strupr(newfilename);
		char* testd2s, * lastd2s = newfilename;
		while (testd2s = strstr(lastd2s, ".D2S"))
			lastd2s = testd2s + 1;
		strncpy(lastd2s, "JAM", 3);
		if (!CopyFile(filename, newfilename, FALSE))
		{
			MessageBox(hWnd, "Could not make backup copy!\nStopping save.", PROGRAMNAME,
				MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
			return false;
		}
	} // Copy saved game file
	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "Could not create file!", PROGRAMNAME,
			MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}
	offset = 0;
	DWORD written;
	WriteFile(hFile, &Header, sizeof Header, &written, NULL);
	if (check_written(hWnd, sizeof Header, written, hFile)) return false;
	offset += written;
	WriteFile(hFile, &Woo, sizeof Woo, &written, NULL);
	if (check_written(hWnd, sizeof Woo, written, hFile)) return false;
	offset += written;
	WriteFile(hFile, &WS, sizeof WS, &written, NULL);
	if (check_written(hWnd, sizeof WS, written, hFile)) return false;
	offset += written;
	WriteFile(hFile, &w4, sizeof w4, &written, NULL);
	if (check_written(hWnd, sizeof w4, written, hFile)) return false;
	offset += written;
	{
		gf.health *= 256;
		gf.healthmax *= 256;
		gf.mana *= 256;
		gf.manamax *= 256;
		gf.stamina *= 256;
		gf.staminamax *= 256;
		DWORD* a = &gf.strength;
		BYTE gftmp[70];
		BYTE* d = (gftmp + 5);
		*(WORD*)(&gftmp[0]) = 0x6667;
		*(WORD*)(&gftmp[2]) = 0x0000;
		WORD* fields = (WORD*)(&gftmp[2]);
		for (int i = 0; i < 16; i++)
		{
			if (a[i] != 0)
			{
				*fields |= 1 << i;
				*(DWORD*)d = a[i];
				d += 4;
			}
		}
		WriteFile(hFile, &gftmp, d - gftmp, &written, NULL);
		offset += written;
		gf.health /= 256;
		gf.healthmax /= 256;
		gf.mana /= 256;
		gf.manamax /= 256;
		gf.stamina /= 256;
		gf.staminamax /= 256;
	}
	WriteFile(hFile, &IF, sizeof IF, &written, NULL);
	if (check_written(hWnd, sizeof IF, written, hFile)) return false;
	offset += written;
	{ // Write Inventory
		JMinv.num = Items->Count();
		WriteFile(hFile, &JMinv, sizeof JMinv, &written, NULL);
		if (check_written(hWnd, sizeof JMinv, written, hFile)) return false;
		offset += written;

		for (Item* i = Items; i != 0; i = i->Next())
		{
			WriteFile(hFile, i->GetItemRecord(), i->ItemRecordLength(), &written, NULL);
			if (check_written(hWnd, i->ItemRecordLength(), written, hFile)) return false;
			offset += written;
			if (i->Socketed())
			{
				Item* G = i->Gems;
				for (int g = 0; g < i->GemNum(); g++)
				{
					if (!G)
					{
						MessageBox(hWnd, "FATAL Gemmed Item Error!", PROGRAMNAME,
							MB_OK | MB_ICONSTOP | MB_APPLMODAL);
						return false;
					}
					WriteFile(hFile, G->GetItemRecord(), G->ItemRecordLength(), &written, NULL);
					if (check_written(hWnd, G->ItemRecordLength(), written, hFile)) return false;
					offset += written;
					G = G->Next();
				}
			}
		}
	}
	WriteFile(hFile, rest, restsize, &written, NULL);
	if (check_written(hWnd, restsize, written, hFile)) return false;
	offset += written;
	CloseHandle(hFile);
	MessageBeep(MB_ICONASTERISK);
	return true;
}
bool fileclass::discardfile(HWND hWnd)
{
	clear();
	return true;
}
void fileclass::setfilename(const char* file)
{
	char fdir[260];
	if (GetDiabloSaveDirectory(fdir))
	{
		sprintf(filename, "%s\\%s.d2s", fdir, file);
	}
}