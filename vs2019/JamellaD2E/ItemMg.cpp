// ItemMg.cpp
#include "JamellaD2E.h"
int ItemMg::BaseDefense()
{
	if (!Info) FindInfo();
	int BaseAC = Info->ACMin;
	if (Info->ACMax - Info->ACMin > 0)
		BaseAC += DWARandomOffset(2) % (Info->ACMax - Info->ACMin);
	return BaseAC;
}
ArmorClass ItemMg::Defense()
{
	ArmorClass AC;
	AC.BaseAC = AC.AC = BaseDefense();
	AC.Magic = false;
	if (Decoded) {
		for (int z = 0; z < MD->nMods; z++)
		{
			if (!MD->Mod[z].Code) continue;

			if (MD->Mod[z].Code == 'ar00')
			{
				AC.AC += MD->Mod[z].Mag;
				AC.Magic = true;
			}
			else if (MD->Mod[z].Code == 'ar02')
			{
				AC.AC += (AC.AC * MD->Mod[z].Mag) / 100;
				AC.Magic = true;
			}
		}
	}
	return AC;
}
Damage ItemMg::WeaponDamage()
{
	Damage Dmg;
	Dmg.Hands = Info->Hands;
	Dmg.Magic = false;
	if (Decoded) {

		int Percent = 100;
		int BoostMax = 0;
		int BoostMin = 0;
		for (int z = 0; z < MD->nMods; z++)
		{
			if (MD->Mod[z].Code == 'dm00')
			{
				BoostMax += MD->Mod[z].Mag;
				Dmg.Magic = true;
			}
			else if (MD->Mod[z].Code == 'dm01')
			{
				BoostMin += MD->Mod[z].Mag;
				Dmg.Magic = true;
			}
			else if (MD->Mod[z].Code == 'dm02')
			{
				Percent += MD->Mod[z].Mag;
				Dmg.Magic = true;
			}
		}
		Dmg.OneHandMin = (Info->OneHandDmgMin * Percent) / 100 + BoostMin;
		Dmg.OneHandMax = (Info->OneHandDmgMax * Percent) / 100 + BoostMax;
		Dmg.TwoHandMin = (Info->TwoHandDmgMin * Percent) / 100 + BoostMin;
		Dmg.TwoHandMax = (Info->TwoHandDmgMax * Percent) / 100 + BoostMax;
	}
	return Dmg;
}
unsigned int ItemMg::RequiredStrength()
{
	if (!Info)
		if (!FindInfo()) return 0;
	if (!Decoded)
		Decode();
	int MinStr = Info->MinStr;
	if (Decoded) {

		int Percent = 100;
		for (int z = 0; z < MD->nMods; z++)
		{
			if (MD->Mod[z].Code == 'rq00')
			{
				Percent += MD->Mod[z].Mag;
			}
		}
		MinStr = (MinStr * Percent) / 100;
	}
	return MinStr;
}
unsigned int ItemMg::RequiredDexterity()
{
	if (!Info)
		if (!FindInfo()) return 0;
	if (!Decoded)
		Decode();
	return Info->MinDex;
}
unsigned int ItemMg::RequiredELevel()
{
	int ELevel = 0;
	if (Quality() == MAGICITEM && Decoded)
	{
		if (MD->MagicPrefix)
			ELevel = max(ELevel, MD->MagicPrefix->ELevel);
		if (MD->MagicSuffix)
			ELevel = max(ELevel, MD->MagicSuffix->ELevel);
	}
	else if (Quality() == RAREITEM && Decoded)
	{
		for (int z = 0; z < 6; z++) {
			if (MD->RareFix[z])
				ELevel = max(ELevel, MD->RareFix[z]->ELevel);
		}
	}
	return ELevel;
}
bool ItemMg::Decode()
{
	return MD->Decode();
}
const char* ItemMg::Name()
{
	if (!Decoded) Decode();
	return MD->Name();
}
const char* ItemMg::RichText()
{
	char* s = RichTextTmp;
	StartRTF(s);
	if (!Decoded) Decode();
	char buffers[2][64];
	switch (Quality())
	{
	case CRUDEITEM:
		strmcat(s, "\\cf0 ", MD->Name(), 0);
		break;
	default:
		if (Socketed()) {
			if (!GemNum())
				strmcat(s, "\\cf5 ", MD->Name(), 0);
			else
				strmcat(s, "\\cf5 Gemmed ", MD->Name(), 0);
		}
		else
			strmcat(s, "\\cf0 ", MD->Name(), 0);
		break;
	case SUPERIORITEM:
		strmcat(s, "\\cf0 ", MD->Name(), 0);
		break;
	case MAGICITEM:
		strmcat(s, "\\cf1 ", MD->Name(), 0);
		break;
	case RAREITEM:
		strmcat(s, "\\cf2 ", MD->Name(), "\\par ", Info->ItemName, 0);
		break;
	case SETITEM:
		strmcat(s, "\\cf3 ", MD->Name(), "\\par ", Info->BaseItemName, 0);
		break;
	case UNIQUEITEM:
		strmcat(s, "\\cf4 ", MD->Name(), "\\par ", Info->BaseItemName, 0);
		break;
	}
	if (Info->Hands > 0)
	{
		Damage Dmg = WeaponDamage();
		if (Dmg.Hands >= 1)
		{
			if (Dmg.Magic) {
				strmcat(s, "\\par\\cf0 One-Hand Damage: \\cf1 ", itoa(Dmg.OneHandMin, buffers[0], 10), 0);
				strmcat(s, " to ", itoa(Dmg.OneHandMax, buffers[0], 10), 0);
			}
			else {
				strmcat(s, "\\par\\cf0 One-Hand Damage: ", itoa(Dmg.OneHandMin, buffers[0], 10), 0);
				strmcat(s, " to ", itoa(Dmg.OneHandMax, buffers[0], 10), 0);
			}
		}
		if (Dmg.Hands >= 2)
		{
			if (Dmg.Magic) {
				strmcat(s, "\\par\\cf0 Two-Hand Damage: \\cf1 ", itoa(Dmg.TwoHandMin, buffers[0], 10), 0);
				strmcat(s, " to ", itoa(Dmg.TwoHandMax, buffers[0], 10), 0);
			}
			else {
				strmcat(s, "\\par\\cf0 Two-Hand Damage: ", itoa(Dmg.TwoHandMin, buffers[0], 10), 0);
				strmcat(s, " to ", itoa(Dmg.TwoHandMax, buffers[0], 10), 0);
			}
		}
	}
	if (Info->ACMin > 0)
	{
		ArmorClass AC = Defense();
		if (AC.Magic)
			strmcat(s, "\\par\\cf0 Defense: \\cf1 ", itoa(AC.AC, buffers[0], 10), 0);
		else
			strmcat(s, "\\par\\cf0 Defense: ", itoa(AC.AC, buffers[0], 10), 0);
	}
	if (Info->Quantity > 0)
	{
		strmcat(s, "\\par\\cf0 Quantity: ", itoa(Quantity(), buffers[0], 10), 0);
	}
	if (Info->Durability > 0)
	{
		strmcat(s, "\\par\\cf0 Durability: ", itoa(Durability(), buffers[0], 10), 0);
		strmcat(s, " of ", itoa(DurabilityMax(), buffers[0], 10), 0);
	}
#if defined(JAMELLAEDITOR)
	if (RequiredStrength() > 0)
	{
		strmcat(s, "\\par", RequiredStrength() > fc.gf.strength ? "\\cf6" : "\\cf0", " Required Strength: ", itoa(RequiredStrength(), buffers[0], 10), 0);
	}
	if (RequiredDexterity() > 0)
	{
		strmcat(s, "\\par", RequiredDexterity() > fc.gf.dexterity ? "\\cf6" : "\\cf0", " Required Dexterity: ", itoa(RequiredDexterity(), buffers[0], 10), 0);
	}
	if (RequiredELevel() > 0)
	{
		strmcat(s, "\\par", RequiredELevel() > fc.gf.level ? "\\cf6" : "\\cf0", " Required Level: ", itoa(RequiredELevel(), buffers[0], 10), 0);
	}
#else
	if (RequiredStrength() > 0)
	{
		strmcat(s, "\\par\\cf0 Required Strength: ", itoa(RequiredStrength(), buffers[0], 10), 0);
	}
	if (RequiredDexterity() > 0)
	{
		strmcat(s, "\\par\\cf0 Required Dexterity: ", itoa(RequiredDexterity(), buffers[0], 10), 0);
	}
	if (RequiredELevel() > 0)
	{
		strmcat(s, "\\par\\cf0 Required Level: ", itoa(RequiredELevel(), buffers[0], 10), 0);
	}
#endif
	switch (Quality())
	{
	default:
		if (Socketed()) {
			strmcat(s, "\\cf1 ", MD->RichAttributes(), 0);
		}
		break;
	case MAGICITEM:
	case SETITEM:
	case RAREITEM:
	case UNIQUEITEM:
		strmcat(s, "\\cf1 ", MD->RichAttributes(), 0);
		break;
	}
	if (Info->UndeadBonus > 100)
	{
		strmcat(s, "\\par\\cf1 ", itoa(Info->UndeadBonus, buffers[0], 10), "% Damage to Undead ", 0);
	}
	if (Socketed())
	{
		sprintf(buffers[0], "Socketed [%i Gems]", GemNum());
		strmcat(s, "\\par\\cf1 ", buffers[0], 0);
	}
	if (Info->Description)
	{
		strmcat(s, "\\par\\cf0 ", Info->Description, 0);
	}
	strcat(s, "\\par }");
	return s;
}