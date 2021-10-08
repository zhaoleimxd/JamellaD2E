// Item103EarEar.cpp from D2E
// Contains methods for the Class Item104Ear representing old item data structures from patch version 1.03 for ears
#include "JamellaD2E.h"
Item104Ear::Item104Ear()
{
	ZeroMemory(&Data, sizeof Data);
}
Item104Ear::~Item104Ear()
{
	if (ListNext) {
		delete ListNext;
		ListNext = 0;
	}
}
bool Item104Ear::LoadItemRecord(BYTE* record)
{
	if (*(record + 0) != 'J' && *(record + 1) != 'M')
		return false;
	memcpy(&Data, record, sizeof Data);
	Info = 0;
	return true;
}
BYTE* Item104Ear::GetItemRecord()
{
	return (BYTE*)&Data;
}
DWORD Item104Ear::ItemRecordLength()
{
	return sizeof Data;
}
void Item104Ear::BlankItem()
{
	ZeroMemory(&Data, sizeof Data);
	Data.JM[0] = 0x4A;
	Data.JM[1] = 0x4D;
	Data.bitfield1 = 0x0010;
	Data.bitfield2 = 0x0039;
	SetOpponentName("SetNewName");
}
int Item104Ear::Container() const
{
	if ((Data.coordinates & 0x1C00) == 0x0000) return CNT_INVENTORY;
	if ((Data.coordinates & 0x1C00) == 0x1000) return CNT_STASH;
	if ((Data.coordinates & 0x1C00) == 0x0C00) return CNT_CUBE;
	return CNT_NONE;
}
int Item104Ear::xPos() const
{
	return (Data.coordinates & 0x007C) >> 2;
}
int Item104Ear::yPos() const
{
	return (Data.coordinates & 0x0180) >> 7;
}
int Item104Ear::xSize()
{
	if (!this && SelInfo) return SelInfo->SizeX;
	if (!this) return 1;
	if (!Info) FindInfo();
	return Info->SizeX;
}
int Item104Ear::ySize()
{
	if (!this && SelInfo) return SelInfo->SizeY;
	if (!this) return 1;
	if (!Info) FindInfo();
	return Info->SizeY;
}
bool Item104Ear::SetCoordinates(int Container, int xPos, int yPos)
{
	switch (Container)
	{
	default:
		return false;
	case CNT_INVENTORY:
	{
		if (xPos < 0 || xPos >= InvGrids.xInventory) return false;
		if (yPos < 0 || yPos >= InvGrids.yInventory) return false;

		Data.coordinates = (Data.coordinates & 0xE3FF) | 0x0000;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);
		return true;
	}
	case CNT_STASH:
	{
		if (xPos < 0 || xPos >= InvGrids.xStash) return false;
		if (yPos < 0 || yPos >= InvGrids.yStash) return false;

		Data.coordinates = (Data.coordinates & 0xE3FF) | 0x1000;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);
		return true;
	}
	case CNT_CUBE:
	{
		if (xPos < 0 || xPos >= InvGrids.xCube) return false;
		if (yPos < 0 || yPos >= InvGrids.yCube) return false;

		Data.coordinates = (Data.coordinates & 0xE3FF) | 0x0C00;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);
		return true;
	}
	}
	return true;
}
bool Item104Ear::FindInfo()
{
	Info = &itemunknown;
	for (int z = 0; z < nItemInfos; z++)
	{
		// Check Item Code
		if (ItemCode() != ItemInfos[z].IC) continue;
		// Check if Set or Unique Item
		if (ItemInfos[z].ItemSearch != 'N') continue;
		Info = &ItemInfos[z];
	}
	if (!Info || !Info->BitmapID)
	{
		Info = &itemunknown;
		return false;
	}
	return true;
}
HBITMAP Item104Ear::GetBitmap()
{
	if (!Info) FindInfo();
	return ItemInfoGetBitmap(Info);
}
DWORD Item104Ear::ItemCode()
{
	return ' rae';
}
void Item104Ear::SetItemCode(DWORD x)
{
	// No Item Code
}
int Item104Ear::OpponentClass()
{
	return (Data.string[0] & 0x1C) >> 2;
}
void Item104Ear::SetOpponentClass(int x)
{
	Data.string[0] = (Data.string[0] & 0xE3) | ((x % 5) << 2);
}
const char* Item104Ear::OpponentClassString()
{
	int x = OpponentClass();
	if (x >= 0 && x < 5)
		return CharClasses[x];
	else
		return "Error!!!";
}
int Item104Ear::OpponentLevel()
{
	return ((Data.string[0] & 0xE0) >> 5) +
		((Data.string[1] & 0x1F) << 3);
}
void Item104Ear::SetOpponentLevel(int x)
{
	x %= 100;
	Data.string[0] = (Data.string[0] & 0x1F) | ((x & 0x07) << 5);
	Data.string[1] = (Data.string[1] & 0xE0) | ((x & 0xF8) >> 3);
}
char Item104Ear::OpponentNameChar(int n)
{
	int bitoffset = 13 + n * 7;

	// Get First Part
	int B = bitoffset / 8;
	int V = bitoffset % 8;
	char c = (Data.string[B] >> V) & 0x7F;

	// Get Secound Part
	int rest = 8 - V;
	int mask = (0xFF << (V - 1)) ^ 0xFF;
	c |= (Data.string[B + 1] & mask) << rest;

	return c;
}
void Item104Ear::SetOpponentNameChar(int n, char c)
{
	int bitoffset = 13 + n * 7;

	// Get First Part
	int B = bitoffset / 8;
	int V = bitoffset % 8;
	int mask1 = (0xFF << V) ^ 0xFF;
	Data.string[B] = (Data.string[B] & mask1) | ((c << V) & (mask1 ^ 0xFF));

	int rest = 8 - V;
	int mask2 = 0xFF << (V - 1);
	Data.string[B + 1] = (Data.string[B + 1] & mask2) | ((c >> rest) & (mask2 ^ 0xFF));
}
static char ItemOpponentNameTemp[20];
const char* Item104Ear::OpponentName()
{
	memset(ItemOpponentNameTemp, 0, sizeof ItemOpponentNameTemp);

	for (int z = 0; z < 15; z++)
		ItemOpponentNameTemp[z] = OpponentNameChar(z);

	return ItemOpponentNameTemp;
}
void Item104Ear::SetOpponentName(const char* s)
{
	for (int z = 0; z < 15; z++)
		SetOpponentNameChar(z, *s++);

	SetOpponentNameChar(15, 0);
}
const char* Item104Ear::Name()
{
	sprintf(NameTmp, "%s's Ear", OpponentName());
	return NameTmp;
}
const char* Item104Ear::RichText()
{
	char* s = RichTextTmp;
	StartRTF(s);
	strmcat(s, "\\cf0 ", Name(), 0);
	char buffers[2][64];
	// Ear Descriptions
	strmcat(s, "\\par ", OpponentClassString(), 0);
	sprintf(buffers[0], "\\par Level %i", OpponentLevel());
	strcat(s, buffers[0]);
	strcat(s, "\\par }");
	return s;
}