// Registry.cpp
#include "JamellaD2E.h"
static int GetUserConfirmation()
{
	return MessageBox(NULL, "The file types for *.d2s and *.d2i have not been registered in the explorer.\nDo you wish to do this now ? \nThis will make the files have nice icons and double - click open features.",PROGRAMNAME,MB_YESNO | MB_ICONEXCLAMATION);
}
static bool Confirmation;
static int CheckFileExtention(const char* Extention, const char* ProgID)
{
	char Query[260];
	DWORD Type;
	DWORD Size = sizeof Query;
	// Check Entry
	HKEY Key;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, Extention, 0, KEY_ALL_ACCESS, &Key) == ERROR_SUCCESS)
	{ // Check Key Contents
		if (RegQueryValueEx(Key, NULL, NULL, &Type, (unsigned char*)&Query, &Size) == ERROR_SUCCESS &&
			Type == REG_SZ &&
			strcmp(Query, ProgID) == 0)
		{
		}
		else
		{
			if (!Confirmation) {
				if (GetUserConfirmation() != IDYES) return false;
				else Confirmation = true;
			}

			RegCreateKeyEx(HKEY_CLASSES_ROOT, Extention, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)ProgID, strlen(ProgID));
		}
	} // Check Key Contents
	else
	{ // Add new entry

		if (!Confirmation) {
			if (GetUserConfirmation() != IDYES) return false;
			else Confirmation = true;
		}
		RegCreateKeyEx(HKEY_CLASSES_ROOT, Extention, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)ProgID, strlen(ProgID));

	} // Add new entry
	RegCloseKey(Key);
	return true;
}
static int CheckDescription(const char* ProgID, const char* Description)
{
	char Query[260];
	DWORD Type;
	DWORD Size = sizeof Query;
	// Check Entry
	HKEY Key;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, ProgID, 0, KEY_ALL_ACCESS, &Key) == ERROR_SUCCESS)
	{ // Check Key Contents
		if (RegQueryValueEx(Key, NULL, NULL, &Type, (unsigned char*)&Query, &Size) == ERROR_SUCCESS &&
			Type == REG_SZ &&
			strcmp(Query, Description) == 0)
		{
		}
		else
		{
			if (!Confirmation) {
				if (GetUserConfirmation() != IDYES) return false;
				else Confirmation = true;
			}

			RegCreateKeyEx(HKEY_CLASSES_ROOT, ProgID, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)Description, strlen(Description));
		}
	} // Check Key Contents
	else
	{ // Add new entry

		if (!Confirmation) {
			if (GetUserConfirmation() != IDYES) return false;
			else Confirmation = true;
		}
		RegCreateKeyEx(HKEY_CLASSES_ROOT, ProgID, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)Description, strlen(Description));

	} // Add new entry
	RegCloseKey(Key);
	return true;
}
static int CheckIcon(const char* ProgID, const char* IconPath, const int IconNum)
{
	char Query[260];
	DWORD Type;
	DWORD Size = sizeof Query;
	char KeyPath[260];
	sprintf(KeyPath, "%s\\DefaultIcon", ProgID);
	char FilePath[260];
	sprintf(FilePath, "%s,%i", IconPath, IconNum);
	// Check Entry
	HKEY Key;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, KEY_ALL_ACCESS, &Key) == ERROR_SUCCESS)
	{ // Check Key Contents
		if (RegQueryValueEx(Key, NULL, NULL, &Type, (unsigned char*)&Query, &Size) == ERROR_SUCCESS &&
			Type == REG_SZ &&
			strcmp(Query, FilePath) == 0)
		{
		}
		else
		{
			if (!Confirmation) {
				if (GetUserConfirmation() != IDYES) return false;
				else Confirmation = true;
			}

			RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)FilePath, strlen(FilePath));
		}
	} // Check Key Contents
	else
	{ // Add new entry

		if (!Confirmation) {
			if (GetUserConfirmation() != IDYES) return false;
			else Confirmation = true;
		}
		RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)FilePath, strlen(FilePath));

	} // Add new entry
	RegCloseKey(Key);
	return true;
}
static int CheckShellVerb(const char* ProgID, const char* Verb, const char* Command)
{
	char Query[260];
	DWORD Type;
	DWORD Size = sizeof Query;
	char KeyPath[260];
	sprintf(KeyPath, "%s\\shell\\%s\\command", ProgID, Verb);
	// Check Entry
	HKEY Key;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, KEY_ALL_ACCESS, &Key) == ERROR_SUCCESS)
	{ // Check Key Contents
		if (RegQueryValueEx(Key, NULL, NULL, &Type, (unsigned char*)&Query, &Size) == ERROR_SUCCESS &&
			Type == REG_SZ &&
			strcmp(Query, Command) == 0)
		{
		}
		else
		{
			if (!Confirmation) {
				if (GetUserConfirmation() != IDYES) return false;
				else Confirmation = true;
			}

			sprintf(KeyPath, "%s\\shell", ProgID);
			RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegCloseKey(Key);

			sprintf(KeyPath, "%s\\shell\\%s", ProgID, Verb);
			RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegCloseKey(Key);

			sprintf(KeyPath, "%s\\shell\\%s\\command", ProgID, Verb);
			RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
			RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)Command, strlen(Command));
		}
	} // Check Key Contents
	else
	{ // Add new entry

		if (!Confirmation) {
			if (GetUserConfirmation() != IDYES) return false;
			else Confirmation = true;
		}
		sprintf(KeyPath, "%s\\shell", ProgID);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegCloseKey(Key);
		sprintf(KeyPath, "%s\\shell\\%s", ProgID, Verb);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegCloseKey(Key);

		sprintf(KeyPath, "%s\\shell\\%s\\command", ProgID, Verb);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, KeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL);
		RegSetValueEx(Key, NULL, 0, REG_SZ, (unsigned char*)Command, strlen(Command));

	} // Add new entry
	RegCloseKey(Key);
	return true;
}
void CheckShellRegistry()
{
	if (!RegOptions.Associations) return;
	Confirmation = false;
	if (!CheckFileExtention(".d2s", D2SPROGID)) return;
	if (!CheckFileExtention(".d2i", D2IPROGID)) return;
	if (!CheckFileExtention(".item", D2IPROGID)) return;
	if (!CheckFileExtention(".ite", D2IPROGID)) return;
	if (!CheckFileExtention(".itm", D2IPROGID)) return;
	if (!CheckDescription(D2SPROGID, D2SDESCRIPTION)) return;
	if (!CheckDescription(D2IPROGID, D2IDESCRIPTION)) return;
	if (!CheckIcon(D2SPROGID, ProgramFilePath(), 1)) return;
	if (!CheckIcon(D2IPROGID, ProgramFilePath(), 2)) return;
	char command[260];
	sprintf(command, "\"%s\" \"%%1\"", ProgramFilePath());
	if (!CheckShellVerb(D2SPROGID, "open", command)) return;
	SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
}
struct _RegOptions RegOptions;
struct
{
	char* ValName;
	DWORD Type;
	BYTE* Ptr;
	DWORD PtrSize;
	int Default;
}
QueryValues[] =
{
 { "CreateItemRecordFormat",
 REG_DWORD,
 (BYTE*)&RegOptions.CreateItemRecordFormat,
 sizeof RegOptions.CreateItemRecordFormat,
 0 },

 { "AllItemsSocketable",
 REG_DWORD,
 (BYTE*)&RegOptions.AllItemsSocketable,
 sizeof RegOptions.AllItemsSocketable,
 0 },

 { "7Gems",
 REG_DWORD,
 (BYTE*)&RegOptions.A7Gems,
 sizeof RegOptions.A7Gems,
 0 },

 { "ExceedQuantity",
 REG_DWORD,
 (BYTE*)&RegOptions.ExceedQuantity,
 sizeof RegOptions.ExceedQuantity,
 0 },

 { "ItemPath",
 REG_SZ,
 (BYTE*)RegOptions.ItemPath,
 sizeof RegOptions.ItemPath,
 0 },
 { "Associations",
 REG_DWORD,
 (BYTE*)&RegOptions.Associations,
 sizeof RegOptions.Associations,
 1 },
 { "ToolTips",
 REG_DWORD,
 (BYTE*)&RegOptions.ToolTips,
 sizeof RegOptions.ToolTips,
 1 },
 { "AnnoyingMsgs",
 REG_DWORD,
 (BYTE*)&RegOptions.NoAnnoyingMsgs,
 sizeof RegOptions.NoAnnoyingMsgs,
 0 },
};
void LoadEditorRegistryValues()
{
	atexit(&SaveEditorRegistryValues);
	HKEY Key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRYKEY, 0, KEY_ALL_ACCESS, &Key) != ERROR_SUCCESS) {

		for (int z = 0; z < sizeof QueryValues / sizeof QueryValues[0]; z++)
		{
			if (QueryValues[z].Type == REG_DWORD) {
				*(DWORD*)QueryValues[z].Ptr = QueryValues[z].Default;
			}
		}
		return;
	}
	DWORD Type;
	for (int z = 0; z < sizeof QueryValues / sizeof QueryValues[0]; z++)
	{
		if (RegQueryValueEx(Key, QueryValues[z].ValName, 0, &Type, NULL, NULL) != ERROR_SUCCESS)
		{
			if (QueryValues[z].Type == REG_DWORD) {
				*(DWORD*)QueryValues[z].Ptr = QueryValues[z].Default;
			}
		}
		else if (Type == QueryValues[z].Type)
		{
			if (RegQueryValueEx(Key, QueryValues[z].ValName, 0, &Type, QueryValues[z].Ptr, &QueryValues[z].PtrSize) != ERROR_SUCCESS) {
				if (QueryValues[z].Type == REG_DWORD) {
					*(DWORD*)QueryValues[z].Ptr = QueryValues[z].Default;
				}
			}
		}
	}
	RegCloseKey(Key);
}
void SaveEditorRegistryValues()
{
	HKEY Key;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRYKEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Key, NULL) != ERROR_SUCCESS) return;
	for (int z = 0; z < sizeof QueryValues / sizeof QueryValues[0]; z++)
	{
		RegSetValueEx(Key, QueryValues[z].ValName, 0, QueryValues[z].Type, QueryValues[z].Ptr, QueryValues[z].PtrSize);
	}
	RegCloseKey(Key);
}