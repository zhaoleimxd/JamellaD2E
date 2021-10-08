#include "JamellaD2E.h"
void StripRTF(char* d, const char* s)
{
	while (*s)
	{
		if (*s == '{')
		{
			int dp = 0;
			while (*s)
			{
				if (*s == '}' && dp == 0)
					break;
				else if (*s == '}' && dp > 0) {
					dp--;
					s++;
					continue;
				}
				else if (*s == '{') {
					dp++;
					s++;
					continue;
				}
				else if (*s == '\\' && dp == 1)
				{
					if (*(s + 1) == 'p' && *(s + 2) == 'a' && *(s + 3) == 'r')
						*d++ = '\n';

					while (*s && *s != ' ' && *s != '{')
						s++;

					if (*s == ' ') s++;
				}
				else if (dp == 1)
					*d++ = *s++;
				else
					s++;
			}
		}
	}
	*d = 0;
}
void WriteTextSummary(HWND hWnd)
{
	if (!fc.isloaded())
	{
		ErrorBox("You must load a character!", hWnd);
		return;
	}

	// common dialog box structure
	char QueryFilename[260];
	OPENFILENAME ofn;

	{ // Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = "Text File (*.txt)\0*.txt\0";
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = QueryFilename;
		ofn.nMaxFile = sizeof(QueryFilename);
		ofn.lpstrFileTitle = NULL;
		ofn.lpstrDefExt = "txt";
		ofn.nMaxFileTitle = 0;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_HIDEREADONLY;

		ZeroMemory(&QueryFilename, sizeof(QueryFilename));
		strcpy(QueryFilename, fc.Header.playername);
	}

	// Display the Open Dialog Box
	if (GetSaveFileName(&ofn))
	{
		// Open File
		FILE* f = fopen(QueryFilename, "wt");
		if (f == 0)
		{
			MessageBox(hWnd, "Could not open file!", PROGRAMNAME,
				MB_OK | MB_ICONSTOP | MB_APPLMODAL);
			return;
		}
		fprintf(f, "Diablo 2 Character Summary\nwritten by %s\n\n", PROGRAMNAME);


		fprintf(f, "Character Properties:\n");
		fprintf(f, "Name: %s\n", fc.Header.playername);
		fprintf(f, "Class: %s\n", CharClasses[fc.Header.playerclass]);
		fprintf(f, "Level: %u\n", fc.gf.level);
		fprintf(f, "Experience: %u\n", fc.gf.experience);
		int diff = 0;
		if ((fc.Header.diff & 0x0C) == 0x04) diff = 1;
		if ((fc.Header.diff & 0x0C) == 0x08) diff = 2;
		if ((fc.Header.diff & 0x0C) == 0x0C) diff = 3;
		fprintf(f, "Difficulty: %s\n", Difficulties[diff]);
		fprintf(f, "Hardcore: %s\n", fc.Header.hardcore & 0x04 ? "Yes" : "No");
		fprintf(f, "Dead: %s\n", fc.Header.hardcore & 0x08 ? "Yes" : "No");
		fprintf(f, "\nCharacter Stats:\n");
		fprintf(f, "Strength: %u\n", fc.gf.strength);
		fprintf(f, "Dexterity: %u\n", fc.gf.dexterity);
		fprintf(f, "Vitality: %u\n", fc.gf.vitality);
		fprintf(f, "Energy: %u\n", fc.gf.energy);
		fprintf(f, "Health: %u / %u\n", fc.gf.health, fc.gf.healthmax);
		fprintf(f, "Mana: %u / %u\n", fc.gf.mana, fc.gf.manamax);
		fprintf(f, "Stamina: %u / %u\n", fc.gf.stamina, fc.gf.staminamax);
		fprintf(f, "Gold on Body: %u\n", fc.gf.stamina, fc.gf.goldperson);
		fprintf(f, "Gold in Stash: %u\n", fc.gf.stamina, fc.gf.goldstash);

		fprintf(f, "\n%u Items:\n", Items->Count());
		for (Item* I = Items; I != 0; I = I->Next())
		{
			char description[2048];
			StripRTF(description, I->RichText());
			fprintf(f, "%s\n", description);
		}
		fclose(f);
	}
	return;
}