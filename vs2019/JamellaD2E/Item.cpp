// Item.cpp from D2E
// Contains methods of Item base class
#include "JamellaD2E.h"
class Item* Items = 0;
Item::Item()
{
	ListNext = ListPrev = 0;
	Busy = Decoded = false;
	Info = 0;
	MD = 0;
	Gems = 0;
	GInfo = 0;
}
Item::~Item()
{
	if (ListNext) {
		delete ListNext;
		ListNext = 0;
	}
}
// Chained List Functions
int Item::Count()
{
	if (this == 0) return 0;
	return ListNext->Count() + 1;
}
Item* Item::GetOrdinal(int i)
{
	if (i == 0) return this;
	else if (!ListNext) return 0;
	else return ListNext->GetOrdinal(i - 1);
}
void Item::Attach(Item* i)
{
	if (this == 0)
	{
		Items = i;
		i->ListPrev = i->ListNext = 0;
	}
	else if (!ListNext)
	{
		ListNext = i;
		ListNext->ListPrev = this;
	}
	else ListNext->Attach(i);
}
void Item::Delete()
{
	if (this == 0) return;
	if (ListPrev == 0)
	{
		Items = ListNext;
		if (ListNext)
			ListNext->ListPrev = 0;
		ListNext = 0;
		delete this;
	}
	else
	{
		ListPrev->ListNext = ListNext;
		if (ListNext)
			ListNext->ListPrev = ListPrev;
		ListNext = 0;
		delete this;
	}
}
#undef AssertBox
#define AssertBox
// Virtual Functions
bool Item::LoadItemRecord(BYTE* data)
{
	AssertBox("Base class cannot be loaded with an item record!");
	return false;
}
BYTE* Item::GetItemRecord()
{
	AssertBox("Base class cannot give an item record!");
	return 0;
}
DWORD Item::ItemRecordLength()
{
	return 0;
}
void Item::BlankItem()
{
}
int Item::Container() const
{
	// AssertBox("Base class cannot return a container!");
	return CNT_NONE;
}
int Item::xPos() const
{
	// AssertBox("Base class cannot return a x-coordinate!");
	return 0;
}
int Item::yPos() const
{
	// AssertBox("Base class cannot return a y-coordinate!");
	return 0;
}
int Item::xPixelPos() const
{
	return xPos() * 29 - 1;
}
int Item::yPixelPos() const
{
	return yPos() * 29 - 1;
}
int Item::xSize()
{
	// AssertBox("Base class cannot return a x-size!");
	return 0;
}
int Item::ySize()
{
	// AssertBox("Base class cannot return a y-size!");
	return 0;
}
int Item::xPixelSize()
{
	return xSize() * 29 - 1;
}
int Item::yPixelSize()
{
	return ySize() * 29 - 1;
}
bool Item::isInRegion(int x, int y)
{
	if (x < xPixelPos() || x > xPixelPos() + xPixelSize()) return false;
	if (y < yPixelPos() || y > yPixelPos() + yPixelSize()) return false;
	return true;
}
bool Item::SetCoordinates(int Container, int xPos, int yPos)
{
	AssertBox("Base class cannot be set coordinates!");
	return false;
}
int Item::BodyCode()
{
	// AssertBox("Base class does not have valid BodyCode() method!");
	return 0;
}
int Item::BodyPlace()
{
	if (!this || !Info) return -1;
	return Info->BodyPlace;
}
bool Item::FindInfo()
{
	AssertBox("Base class cannot find item info!");
	return false;
}
HBITMAP Item::GetBitmap()
{
	return itemunknown.hBmp;
}
// Simple Item Properties
DWORD Item::ItemCode()
{
	AssertBox("Base class does not have valid ItemCode() method!");
	return 0;
}
void Item::SetItemCode(DWORD x)
{
	AssertBox("Base class does not have valid SetItemCode(DWORD) method!");
}
int Item::UniqueCode()
{
	AssertBox("Base class does not have valid UniqueCode() method!");
	return 0;
}
void Item::SetUniqueCode(int x)
{
	AssertBox("Base class does not have valid SetUniqueCode(int) method!");
}
int Item::Quality()
{
	return USUALITEM;
}
void Item::SetQuality(int x)
{
	AssertBox("Base class does not have valid SetQuality(int) method!");
}
int Item::Quantity()
{
	AssertBox("Base class does not have valid Quantity() method!");
	return 0;
}
void Item::SetQuantity(int i)
{
	AssertBox("Base class does not have valid SetQuantity(int) method!");
}
int Item::Durability()
{
	AssertBox("Base class does not have valid Durability() method!");
	return 0;
}
void Item::SetDurability(int x)
{
	AssertBox("Base class does not have valid SetDurability(int) method!");
}
int Item::DurabilityMax()
{
	AssertBox("Base class does not have valid DurabilityMax() method!");
	return 0;
}
void Item::SetDurabilityMax(int x)
{
	AssertBox("Base class does not have valid SetDurabilityMax(int) method!");
}
DWORD Item::DWA()
{
	AssertBox("Base class does not have valid DWA() method!");
	return 0;
}
void Item::SetDWA(DWORD x)
{
	AssertBox("Base class does not have valid SetDWA(DWORD) method!");
}
DWORD Item::DWB()
{
	AssertBox("Base class does not have valid DWB() method!");
	return 0;
}
void Item::SetDWB(DWORD x)
{
	AssertBox("Base class does not have valid SetDWB(DWORD) method!");
}
DWORD Item::DWARandomOffset(int x)
{
	RAND r = { DWA(), 666 };
	for (int z = 0; z < x; z++)
		Random(&r);
	return r.Seed;
}
DWORD Item::DWBRandomOffset(int x)
{
	RAND r = { DWB(), 666 };
	for (int z = 0; z < x; z++)
		Random(&r);
	return r.Seed;
}
int Item::MagicLevel()
{
	AssertBox("Base class does not have valid MagicLevel() method!");
	return 0;
}
void Item::SetMagicLevel(int x)
{
	AssertBox("Base class does not have valid SetMagicLevel(int) method!");
}
int Item::GemNum()
{
	AssertBox("Base class does not have valid GemNum() method!");
	return 0;
}
void Item::SetGemNum(int i)
{
	AssertBox("Base class does not have valid SetGemNum(int) method!");
}
bool Item::Socketed()
{
	AssertBox("Base class does not have valid Socketed() method!");
	return 0;
}
void Item::SetSocketed(bool i)
{
	AssertBox("Base class does not have valid SetSocketed(bool) method!");
}
bool Item::Socketable()
{
	if (!Info) FindInfo();
	if (Info) return (Info->Sockets > 0);
	else return 0;
}
bool Item::Identified()
{
	AssertBox("Base class does not have valid Identified() method!");
	return 0;
}
void Item::SetIdentified(bool i)
{
	AssertBox("Base class does not have valid SetIdentified(DWORD) method!");
}
bool Item::Starter()
{
	AssertBox("Base class does not have valid Starter() method!");
	return 0;
}
void Item::SetStarter(bool i)
{
	AssertBox("Base class does not have valid SetStarter(DWORD) method!");
}
// Ear-specific Properties
int Item::OpponentClass()
{
	AssertBox("Base class does not have valid OpponentClass() method!");
	return 0;
}
void Item::SetOpponentClass(int x)
{
	AssertBox("Base class does not have valid SetStarter(int) method!");
}
const char* Item::OpponentClassString()
{
	AssertBox("Base class does not have valid OpponentClassString() method!");
	return 0;
}
int Item::OpponentLevel()
{
	AssertBox("Base class does not have valid OpponentLevel() method!");
	return 0;
}
void Item::SetOpponentLevel(int x)
{
	AssertBox("Base class does not have valid SetOpponentLevel(int) method!");
}
const char* Item::OpponentName()
{
	AssertBox("Base class does not have valid OpponentName() method!");
	return 0;
}
void Item::SetOpponentName(const char* s)
{
	AssertBox("Base class does not have valid SetOpponentName(const char *) method!");
}
int Item::BaseDefense()
{
	AssertBox("Base class does not have valid BaseDefense() method!");
	return 0;
}
ArmorClass Item::Defense()
{
	AssertBox("Base class does not have valid BaseDefense() method!");
	ArmorClass AC = { 0, 0, false };
	return AC;
}
Damage Item::WeaponDamage()
{
	AssertBox("Base class does not have valid WeaponDamage() method!");
	Damage WD = { 0, 0,0, 0,0, false };
	return WD;
}
unsigned int Item::RequiredStrength()
{
	if (!Info)
		if (!FindInfo()) return 0;
	return Info->MinStr;
}
unsigned int Item::RequiredDexterity()
{
	if (!Info)
		if (!FindInfo()) return 0;
	return Info->MinDex;
}
unsigned int Item::RequiredELevel()
{
	AssertBox("Base class does not have valid RequiredELevel() method");
	return 0;
}
bool Item::isWearable()
{
#if defined(JAMELLAEDITOR)
	bool is = true;
	if (Info) {
		if (RequiredStrength() > fc.gf.strength) is = false;
		if (RequiredDexterity() > fc.gf.dexterity) is = false;
	}
	if (RequiredELevel() > fc.gf.level) is = false;
	return is;
#else
	return true;
#endif
}
bool Item::Decode()
{
	// AssertBox("Base class does not have valid Decode() method!");
	return false;
}
const char* Item::Name()
{
	if (!Info)
		if (!FindInfo())
			return 0;
	return Info->ItemName;
}
const char* Item::RichText()
{
	// AssertBox("Base class does not have valid RichText() method!");
	return 0;
}
bool Item::FindGemInfo()
{
	GInfo = 0;
	return false;
}
char Item::NameTmp[256];
char Item::RichTextTmp[2048];
void Item::StartRTF(char* s)
{
	*s = 0;
	// RTF Header
	strcat(s, "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031");
	// RTF Font Table (just normal font)
	strcat(s, "{\\fonttbl{\\f0\\fswiss\\fcharset0 MS Sans Serif;}}");
	// Color Table:
	// 0 = Regular Items
	strmcat(s, "{\\colortbl ;",
		"\\red72\\green80\\blue184;", // 1 = Magic Items
		"\\red255\\green255\\blue0;", // 2 = Rare Items
		"\\red0\\green196\\blue0;", // 3 = Set Items
		"\\red144\\green136\\blue88;", // 4 = Unique Items
		"\\red124\\green124\\blue124;", // 5 = Socketed Items
		"\\red255\\green0\\blue0;}", // 6 = Red (Error or Unidentified)
		0);
	// Start Paragraph
	strcat(s, "\\uc1\\pard\\qc\\b\\f0\\fs16");
}