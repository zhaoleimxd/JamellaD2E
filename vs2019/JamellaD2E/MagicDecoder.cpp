// MagicDecoder.cpp from D2E
// Contains methods for the Class MagicDecoder
#include "JamellaD2E.h"
MagicDecoder::MagicDecoder(Item* IP)
{
	ZeroMemory(this, sizeof * this);
	I = IP;
	PrefixBuffer = new _MagicPreSuffix * [nMagicPrefixTable];
	SuffixBuffer = new _MagicPreSuffix * [nMagicSuffixTable];
	RarePrefixBuffer = new const _RarePreSuffix * [nRarePrefixTable];
	RareSuffixBuffer = new const _RarePreSuffix * [nRareSuffixTable];
}
MagicDecoder::~MagicDecoder()
{
	if (PrefixBuffer) delete[] PrefixBuffer;
	if (SuffixBuffer) delete[] SuffixBuffer;
	if (RarePrefixBuffer) delete[] RarePrefixBuffer;
	if (RareSuffixBuffer) delete[] RareSuffixBuffer;
}
// Buffer Builders
void MagicDecoder::BuildMagicBuffers()
{
	if (PrefixBuffer && SuffixBuffer)
		if (ItemBuffered == I->ItemCode() && ItemBufferedModLevel == I->MagicLevel())
			return;
	ItemBuffered = I->ItemCode();
	ItemBufferedModLevel = I->MagicLevel();
	_MagicPreSuffix* Buffer[128];
	// Build Prefix Buffer
	nPrefixBuffer = 0;
	for (int z = 0; z < nMagicPrefixTable; z++)
	{
		if (MagicPrefixTable[z].ModLevel - 2 > I->MagicLevel()) continue;
		if ((MagicPrefixTable[z].MagicMask & I->Info->MagicMask) == 0) continue;
		Buffer[nPrefixBuffer++] = &MagicPrefixTable[z];
	}
	memcpy(PrefixBuffer, Buffer, sizeof Buffer[0] * nPrefixBuffer);
	// Build Suffix Buffer
	nSuffixBuffer = 0;
	for (int z = 0; z < nMagicSuffixTable; z++)
	{
		if (MagicSuffixTable[z].ModLevel - 2 > I->MagicLevel()) continue;
		if ((MagicSuffixTable[z].MagicMask & I->Info->MagicMask) == 0) continue;
		Buffer[nSuffixBuffer++] = &MagicSuffixTable[z];
	}
	memcpy(SuffixBuffer, Buffer, sizeof Buffer[0] * nSuffixBuffer);
}
void MagicDecoder::BuildRareBuffers()
{
	if (RarePrefixBuffer && RareSuffixBuffer)
		if (RareItemCodeBuffered == I->ItemCode())
			return;
	RareItemCodeBuffered = I->ItemCode();
	const _RarePreSuffix* Buffer[128];
	// Build Prefix Buffer
	nRarePrefixBuffer = 0;
	for (int z = 0; z < nRarePrefixTable; z++)
	{
		if ((RarePrefixTable[z].RareMask & I->Info->RareMask) == 0) continue;
		Buffer[nRarePrefixBuffer++] = &RarePrefixTable[z];
	}
	memcpy(RarePrefixBuffer, Buffer, sizeof Buffer[0] * nRarePrefixBuffer);
	// Build Suffix Buffer
	nRareSuffixBuffer = 0;
	for (int z = 0; z < nRareSuffixTable; z++)
	{
		if ((RareSuffixTable[z].RareMask & I->Info->RareMask) == 0) continue;
		Buffer[nRareSuffixBuffer++] = &RareSuffixTable[z];
	}
	memcpy(RareSuffixBuffer, Buffer, sizeof Buffer[0] * nRareSuffixBuffer);
}
void MagicDecoder::DecodeCrude()
{
	RAND rnd;
	StartRandoms(I, &rnd);
	const char* CrudePrefixes[4] =
	{ "Crude","Cracked","Damaged","Low Quality" };
	CrudePrefix = CrudePrefixes[Random(&rnd) % 4];
}
void MagicDecoder::DecodeMagical()
{
	RAND rnd;
	StartRandoms(I, &rnd);
	if (!I->Info) I->FindInfo();
	BuildMagicBuffers();
	memset(&MagicPrefixMag, 0, sizeof MagicPrefixMag);
	memset(&MagicSuffixMag, 0, sizeof MagicSuffixMag);
	// Got Prefix ?
	if ((Random(&rnd) % 2) == 1)
	{
		if (nPrefixBuffer == 0)
		{
			DecodeError = DE_MAGIC_PREFIX_MODULO_ZERO;
			return;
		}
		modMagicPrefix = nPrefixBuffer;
		int Index = modpickMagicPrefix = Random(&rnd) % nPrefixBuffer;
		MagicPrefix = PrefixBuffer[Index];
		int RndGets = MagicPrefix->nMod;
		for (int z = 0; z < 4; z++)
		{
			if (MagicPrefix->Mod[z].Code == 0) continue;
			int diff = MagicPrefix->Mod[z].Max - MagicPrefix->Mod[z].Min;
			if (diff > 0)
			{
				if (RndGets <= 0)
				{
					DecodeError = DE_MAGIC_PREFIX_MODIFIER_VALMISSING;
					return;
				}
				MagicPrefixMag[z] = Random(&rnd) % diff;
				RndGets--;
			}
		}
		for (; RndGets > 0; RndGets--)
			Random(&rnd);
	}
	// Get Suffix ?
	int GetSuffix = 0;
	GetSuffix = Random(&rnd) % 2;
	if (GetSuffix || !MagicPrefix)
	{
		if (nSuffixBuffer == 0)
		{
			DecodeError = DE_MAGIC_SUFFIX_MODULO_ZERO;
			return;
		}
		modMagicSuffix = nSuffixBuffer;
		int Index = modpickMagicSuffix = Random(&rnd) % nSuffixBuffer;
		MagicSuffix = SuffixBuffer[Index];
		int RndGets = MagicSuffix->nMod;
		for (int z = 0; z < 4; z++)
		{
			if (MagicSuffix->Mod[z].Code == 0) continue;
			int diff = MagicSuffix->Mod[z].Max - MagicSuffix->Mod[z].Min;
			if (diff > 0)
			{
				if (RndGets <= 0)
				{
					DecodeError = DE_MAGIC_SUFFIX_MODIFIER_VALMISSING;
					return;
				}
				MagicSuffixMag[z] = Random(&rnd) % diff;
				RndGets--;
			}
		}
		for (; RndGets > 0; RndGets--)
			Random(&rnd);
	}
}
void MagicDecoder::DecodeRare()
{
	RAND rnd;
	StartRandoms(I, &rnd);
	if (!I->Info) I->FindInfo();
	BuildMagicBuffers();
	BuildRareBuffers();
	{ // Name Prefix
		if (nRarePrefixBuffer == 0)
		{
			DecodeError = DE_RARE_NAMEPREFIX_MODULO_ZERO;
			return;
		}
		int iRarePrefix = Random(&rnd) % nRarePrefixBuffer;
		RarePrefix = RarePrefixBuffer[iRarePrefix];
	} // Name Prefix
	{ // Name Suffix
		if (nRareSuffixBuffer == 0)
		{
			DecodeError = DE_RARE_NAMESUFFIX_MODULO_ZERO;
			return;
		}
		int iRareSuffix = Random(&rnd) % nRareSuffixBuffer;
		RareSuffix = RareSuffixBuffer[iRareSuffix];
	} // Name Suffix
	{ // Number of PreSuffixes
		nRareFix = (Random(&rnd) % 3) + 4;
	} // Number of PreSuffixes
	int nPrefixes = 0,
		nSuffixes = 0;
	if (nPrefixBuffer == 0 || nSuffixBuffer == 0)
	{
		DecodeError = DE_RARE_PRESUFFIX_MODULO_ZERO;
		return;
	}
	int Timeout = 0;
	{ // PreSuffixes
		for (int n = 0; n < nRareFix; n++)
		{
			// Select Pre or Suffix
			int PreSuf = Random(&rnd) % 2;
			// Kick random Value
			Random(&rnd);
			if ((PreSuf == 0 && nPrefixes < 3) || nSuffixes > 2)
			{
			reselectprefix:
				int Index = Random(&rnd) % nPrefixBuffer;

				if (Timeout++ > DECODETIMEOUT)
				{
					//DecodeError = DE_RARE_PRESUFFIX_NOPOSSIBLE;
					return;
				}

				// Check Groups
				for (int m = n - 1; m >= 0; m--)
				{
					if (!RareFix[m]) continue;
					if (RareFix[m]->Text == 0) continue;
					if (RareFix[m]->Group != PrefixBuffer[Index]->Group) continue;
					// Kick random Value
					Random(&rnd);
					goto reselectprefix;
				}
				nPrefixes++;
				RareFix[n] = PrefixBuffer[Index];
				tRareFix[n] = PREFIX;
			}
			else
			{
			reselectsuffix:
				int Index = Random(&rnd) % nSuffixBuffer;

				if (Timeout++ > DECODETIMEOUT)
				{
					//DecodeError = DE_RARE_PRESUFFIX_NOPOSSIBLE;
					return;
				}

				// Check Groups
				for (int m = n - 1; m >= 0; m--)
				{
					if (!RareFix[m]) continue;
					if (RareFix[m]->Text == 0) continue;
					if (RareFix[m]->Group != SuffixBuffer[Index]->Group) continue;

					// Kick random Value
					Random(&rnd);

					goto reselectsuffix;
				}

				nSuffixes++;
				RareFix[n] = SuffixBuffer[Index];
				tRareFix[n] = SUFFIX;
			}
		}
		// Get Magnitudes of Modifiers
		if (!Quick)
			for (int n = 0; n < nRareFix; n++)
			{
				if (!RareFix[n]) continue;
				int RndGets = RareFix[n]->nMod;
				for (int z = 0; z < 4; z++)
				{
					if (RareFix[n]->Mod[z].Code == 0) continue;

					int diff = RareFix[n]->Mod[z].Max - RareFix[n]->Mod[z].Min;

					if (diff > 0)
					{
						if (RndGets <= 0)
						{
							DecodeError = DE_RARE_PRESUFFIX_MODIFIER_VALMISSING;
							return;
						}

						RareFixMag[n][z] = Random(&rnd) % diff;
						RndGets--;
					}
				}
				for (; RndGets > 0; RndGets--)
					Random(&rnd);
			}

	} // PreSuffixes
}
void MagicDecoder::DecodeUnique()
{
	if (I->UniqueCode() >= 0 && I->UniqueCode() < nUniqueItems * 2)
	{
		UniqueItem = &UniqueItems[I->UniqueCode() / 2];
	}
	else
		DecodeError = DE_UNIQUE_ITEMCODE;
}
void MagicDecoder::DecodeSet()
{
	if (I->UniqueCode() >= 0 && I->UniqueCode() < nSetItems * 2)
	{
		SetItem = &SetItems[I->UniqueCode() / 2];
		SetItemNum = -1;
		for (int z = 0; z < SetItem->nItems; z++)
		{
			if (I->ItemCode() != SetItem->Item[z].Code && I->ItemCode() != SetItem->Item[z].IC) continue;
			SetItemNum = z;
		}
		if (SetItemNum < 0)
			DecodeError = DE_SET_ITEMCODE;
	}
	else
		DecodeError = DE_SET_ITEMCODE;
}
void MagicDecoder::CollectModifiers()
{
	nMods = 0;
	memset(Mod, 0, sizeof Mod);
	if (DecodeError) return;
	switch (I->Quality())
	{
	default:
		break;
	case MAGICITEM:
	{
		if (MagicPrefix)
		{
			for (int z = 0; z < 3; z++)
			{
				if (!MagicPrefix->Mod[z].Code) continue;
				Mod[nMods].Code = MagicPrefix->Mod[z].Code;
				Mod[nMods++].Mag = MagicPrefix->Mod[z].Min + MagicPrefixMag[z];
			}
		}
		if (MagicSuffix)
		{
			for (int z = 0; z < 3; z++)
			{
				if (!MagicSuffix->Mod[z].Code) continue;
				Mod[nMods].Code = MagicSuffix->Mod[z].Code;
				Mod[nMods++].Mag = MagicSuffix->Mod[z].Min + MagicSuffixMag[z];
			}
		}
	}
	break;
	case SETITEM:
	{
		for (int z = SetItemNum * 2; z < SetItemNum * 2 + 2; z++)
		{
			if (SetItem->Mod[z].Code == 0) continue;
			Mod[nMods].Code = SetItem->Mod[z].Code;
			Mod[nMods++].Mag = SetItem->Mod[z].Min;
		}
	}
	break;
	case RAREITEM:
	{
		for (int z = 0; z < 6; z++)
		{
			if (!RareFix[z]) continue;
			for (int y = 0; y < 3; y++)
			{
				if (!RareFix[z]->Mod[y].Code) continue;

				Mod[nMods].Code = RareFix[z]->Mod[y].Code;
				Mod[nMods++].Mag = RareFix[z]->Mod[y].Min + RareFixMag[z][y];
			}
		}
	}
	break;
	case UNIQUEITEM:
	{
		for (int z = 0; z < 7; z++)
		{
			if (UniqueItem->Mod[z].Code == 0) continue;
			Mod[nMods].Code = UniqueItem->Mod[z].Code;
			Mod[nMods++].Mag = UniqueItem->Mod[z].Min;
		}
	}
	break;
	}
}
bool MagicDecoder::Decode()
{
	Quick = false;
	I->Decoded = false;
	DecodeError = 0;
	ZeroMemory(&ZeroMemoryStart, (&ZeroMemoryEnd - &ZeroMemoryStart));
	switch (I->Quality())
	{
	case CRUDEITEM:
		DecodeCrude();
		break;
	case MAGICITEM:
		DecodeMagical();
		break;
	case SETITEM:
		DecodeSet();
		break;
	case RAREITEM:
		DecodeRare();
		break;
	case UNIQUEITEM:
		DecodeUnique();
		break;
	}
	CollectModifiers();
	I->Decoded = true;
	return I->Decoded;
}
bool MagicDecoder::QuickDecode()
{
	Quick = true;
	bool ODecoded = I->Decoded;
	I->Decoded = false;
	DecodeError = 0;
	ZeroMemory(&ZeroMemoryStart, (&ZeroMemoryEnd - &ZeroMemoryStart));
	switch (I->Quality())
	{
	case CRUDEITEM:
		DecodeCrude();
		break;
	case MAGICITEM:
		DecodeMagical();
		break;
	case SETITEM:
		DecodeSet();
		break;
	case RAREITEM:
		DecodeRare();
		break;
	case UNIQUEITEM:
		DecodeUnique();
		break;
	}
	I->Decoded = ODecoded;
	return true;
}
char MagicDecoder::NameTmp[256];
const char* MagicDecoder::Name()
{
	char* s = NameTmp;
	*s = 0;
	if (!I->Info) I->FindInfo();
	if (!I->Decoded) Decode();
	switch (I->Quality())
	{
	default:
		strcat(s, I->Info->ItemName);
		break;
	case CRUDEITEM:
		strmcat(s, CrudePrefix, " ", I->Info->ItemName, 0);
		break;
	case SUPERIORITEM:
		strmcat(s, "Superior ", I->Info->ItemName, 0);
		break;
	case MAGICITEM:
		if (MagicPrefix)
		{
			strcat(s, MagicPrefix->Text);
			strcat(s, " ");
		}

		strcat(s, I->Info->ItemName);

		if (MagicSuffix)
		{
			strcat(s, " ");
			strcat(s, MagicSuffix->Text);
		}
		break;
	case RAREITEM:
		if (RarePrefix)
			strcat(s, RarePrefix->Text);
		strcat(s, " ");
		if (RareSuffix)
			strcat(s, RareSuffix->Text);
		break;
	case SETITEM:
		strcat(s, I->Info->ItemName);
		break;
	case UNIQUEITEM:
		strcat(s, I->Info->ItemName);
		break;
	}
	return s;
}
char MagicDecoder::AttrTmp[2048];
const char* MagicDecoder::RichAttributes()
{
	*AttrTmp = 0;
	if (DecodeError)
	{
		strcat(AttrTmp, "\\par\\cf6 ");
		strcat(AttrTmp, DecodeErrorString(DecodeError));
		return AttrTmp;
	}
	switch (I->Quality())
	{
	default:
	{
		return "\\par\\cf6 No Attributes!";
	}
	case USUALITEM:
	{
		if (I->Socketed()) {
			for (Item* G = I->Gems; G != 0; G = G->Next())
			{
				if (!G->FindGemInfo()) break;
				switch (I->Info->GemClass)
				{
				case 'W':
				{
					for (int z = 0; z < 3; z++)
					{
						if (!G->GInfo->WeaponMod[z].Code) continue;

						strcat(AttrTmp, "\\par ");
						sprintf(buffer, GetEffect(G->GInfo->WeaponMod[z].Code), G->GInfo->WeaponMod[z].Min);
						strcat(AttrTmp, buffer);
					}
				}
				break;
				case 'H':
				{
					for (int z = 0; z < 3; z++)
					{
						if (!G->GInfo->HelmMod[z].Code) continue;

						strcat(AttrTmp, "\\par ");
						sprintf(buffer, GetEffect(G->GInfo->HelmMod[z].Code), G->GInfo->HelmMod[z].Min);
						strcat(AttrTmp, buffer);
					}
				}
				break;
				case 'S':
				{
					for (int z = 0; z < 3; z++)
					{
						if (!G->GInfo->ShieldMod[z].Code) continue;

						strcat(AttrTmp, "\\par ");
						sprintf(buffer, GetEffect(G->GInfo->ShieldMod[z].Code), G->GInfo->ShieldMod[z].Min);
						strcat(AttrTmp, buffer);
					}
				}
				break;
				}
			}
			break;
		}
		else {
			return "\\par\\cf6 Non-Socketed Item has no attributes!";
		}
	}
	case MAGICITEM:
	{
		if (MagicPrefix)
		{
			for (int z = 0; z < 4; z++)
			{
				if (MagicPrefix->Mod[z].Code == 0) continue;
				strcat(AttrTmp, "\\par ");
				sprintf(buffer, GetEffect(MagicPrefix->Mod[z].Code), MagicPrefix->Mod[z].Min + MagicPrefixMag[z]);
				strcat(AttrTmp, buffer);
#if SHOWMAGICCODE == 1
				strmcat(AttrTmp, "[", CodeStringRev(MagicPrefix->Mod[z].Code), "]", 0);
#endif
			}
		}
		if (MagicSuffix)
		{
			for (int z = 0; z < 4; z++)
			{
				if (MagicSuffix->Mod[z].Code == 0) continue;
				strcat(AttrTmp, "\\par ");
				sprintf(buffer, GetEffect(MagicSuffix->Mod[z].Code), MagicSuffix->Mod[z].Min + MagicSuffixMag[z]);
				strcat(AttrTmp, buffer);
#if SHOWMAGICCODE == 1
				strmcat(AttrTmp, "[", CodeStringRev(MagicSuffix->Mod[z].Code), "]", 0);
#endif
			}
		}
	}
	break;
	case RAREITEM:
	{
		for (int n = 0; n < nRareFix; n++)
		{
			for (int z = 0; z < 4; z++)
			{
				if (!RareFix[n]) continue;
				if (RareFix[n]->Mod[z].Code == 0) continue;
				strcat(AttrTmp, "\\par ");
				sprintf(buffer, GetEffect(RareFix[n]->Mod[z].Code), RareFix[n]->Mod[z].Min + RareFixMag[n][z]);
				strcat(AttrTmp, buffer);
#if SHOWMAGICCODE == 1
				strmcat(AttrTmp, "[", RareFix[n]->Text, "]", 0);
#endif
			}
		}
	}
	break;
	case SETITEM:
	{
		if (!SetItem || SetItemNum < 0)
		{
			return "Set Item decoding error!";
		}
		for (int z = SetItemNum * 2; z < SetItemNum * 2 + 2; z++)
		{
			if (SetItem->Mod[z].Code == 0) continue;
			int y = z - SetItemNum * 2;
			strcat(AttrTmp, "\\par ");
			sprintf(buffer, GetEffect(SetItem->Mod[z].Code), SetItem->Mod[z].Min + UniqueSetMag[y]);
			strcat(AttrTmp, buffer);
#if SHOWMAGICCODE == 1
			strmcat(AttrTmp, "[", CodeStringRev(SetItem->Mod[z].Code), "]", 0);
#endif
		}
	}
	break;
	case UNIQUEITEM:
	{
		if (!UniqueItem)
		{
			return "Unique Item decoding error!";
		}
		for (int z = 0; z < 7; z++)
		{
			if (UniqueItem->Mod[z].Code == 0) continue;
			strcat(AttrTmp, "\\par ");
			sprintf(buffer, GetEffect(UniqueItem->Mod[z].Code), UniqueItem->Mod[z].Min + UniqueSetMag[z]);
			strcat(AttrTmp, buffer);
#if SHOWMAGICCODE == 1
			strmcat(AttrTmp, "[", CodeStringRev(UniqueItem->Mod[z].Code), "]", 0);
#endif
		}
	}
	break;
	}
	return AttrTmp;
}
const char* DecodeErrorString(int Error)
{
	switch (Error)
	{
	default:
		return "No Error";
	case DE_MAGIC_PREFIX_MODULO_ZERO:
		return "Error decoding Magical Prefix! (Modulo is zero)";
	case DE_MAGIC_PREFIX_MODIFIER_VALMISSING:
		return "Error decoding Magical Prefix! (Modifier Specifier missing)";
	case DE_MAGIC_SUFFIX_MODULO_ZERO:
		return "Error decoding Magical Suffix! (Modulo is zero)";
	case DE_MAGIC_SUFFIX_MODIFIER_VALMISSING:
		return "Error decoding Magical Suffix! (Modifier Specifier missing)";
	case DE_RARE_NAMEPREFIX_MODULO_ZERO:
		return "Error decoding Rare Name Prefix! (Modulo is zero)";
	case DE_RARE_NAMESUFFIX_MODULO_ZERO:
		return "Error decoding Rare Name Suffix! (Modulo is zero)";
	case DE_RARE_PRESUFFIX_MODULO_ZERO:
		return "Error decoding Rare Item Attributes! (Buffer Modulo is zero)";
	case DE_RARE_PRESUFFIX_NOPOSSIBLE:
		return "Error decoding Rare Item Attributes! (No more Modifiers available)";
	case DE_RARE_PRESUFFIX_MODIFIER_VALMISSING:
		return "Error decoding Rare Item Attributes! (Modifier Specifier missing)";
	case DE_UNIQUE_ITEMCODE:
		return "Error decoding Unique Item Code! (Invalid Code)";
	case DE_SET_ITEMCODE:
		return "Error decoding Set Item Code! (Invalid Code)";
	}
}
// Double Word History Class
DWHistory::DWHistory()
{
	Top = Bottom = Ptr = 0;
}
DWHistory::~DWHistory()
{
}
bool DWHistory::isNext()
{
	return !(Top == Ptr);
}
bool DWHistory::isBack()
{
	return !(Ptr == Bottom);
}
void DWHistory::StepAdd(Item* I)
{
	ML[Ptr] = I->MagicLevel();
	DWA[Ptr] = I->DWA();
	DWB[Ptr] = I->DWB();
	if (++Ptr >= HISTORYSTEPS)
		Ptr = 0;
	Top = Ptr;
	if (Top == Bottom)
		if (++Bottom >= HISTORYSTEPS)
			Bottom = 0;
}
void DWHistory::StepBack(Item* I)
{
	if (Ptr == Bottom) return;
	ML[Ptr] = I->MagicLevel();
	DWA[Ptr] = I->DWA();
	DWB[Ptr] = I->DWB();
	Ptr--;
	if (Ptr < 0)
		Ptr = HISTORYSTEPS - 1;
	I->SetMagicLevel(ML[Ptr]);
	I->SetDWA(DWA[Ptr]);
	I->SetDWB(DWB[Ptr]);
	I->Info = 0;
	I->Decoded = 0;
}
void DWHistory::StepNext(Item* I)
{
	if (Ptr == Top) return;
	if (++Ptr > HISTORYSTEPS)
		Ptr = 0;
	I->SetMagicLevel(ML[Ptr]);
	I->SetDWA(DWA[Ptr]);
	I->SetDWB(DWB[Ptr]);
	I->Info = 0;
	I->Decoded = 0;
}