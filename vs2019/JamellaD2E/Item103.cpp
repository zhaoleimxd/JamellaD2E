// Item103.cpp from D2E
// Contains methods for the Class Item103 representing old item data structures from patch version 1.03
#include "JamellaD2E.h"
Item103::Item103()
{
	ZeroMemory(&Data, sizeof Data);
	MD = new class MagicDecoder(this);
}
Item103::~Item103()
{
	delete MD;
	if (ListNext) {
		delete ListNext;
		ListNext = 0;
	}
}
bool Item103::LoadItemRecord(BYTE* record)
{
	if (*(record + 0) != 'J' && *(record + 1) != 'M')
		return false;
	memcpy(&Data, record, sizeof Data);
	Info = 0;
	return true;
}
BYTE* Item103::GetItemRecord()
{
	return (BYTE*)&Data;
}
DWORD Item103::ItemRecordLength()
{
	return sizeof Data;
}
void Item103::BlankItem()
{
	ZeroMemory(&Data, sizeof Data);
	Data.JM[0] = 0x4A;
	Data.JM[1] = 0x4D;
	Data.bitfield1 = 0x0010;
	Data.bitfield2 = 0x0000;
}
int Item103::Container() const
{
	if ((Data.container & 0x07F8) == 0x0000) return CNT_INVENTORY;
	if ((Data.container & 0x07F8) == 0x0020) return CNT_STASH;
	if ((Data.container & 0x07F8) == 0x0018) return CNT_CUBE;
	if ((Data.container & 0x07F8) == 0x07F8)
	{ // On Body or On Belt
		if (Data.itemcode & 0x8000)
			return CNT_BELT;
		if (Data.itemcode & 0x4000)
			return CNT_BODY;
	}
	return CNT_NONE;
}
int Item103::xPos() const
{
	switch (Container())
	{
	default:
		return 0;
	case CNT_INVENTORY:
	case CNT_STASH:
	case CNT_CUBE:
		return (Data.coordinates & 0x3E) >> 1;
	case CNT_BELT:
		return (Data.coordinates & 0x06) >> 1;
	}
}
int Item103::yPos() const
{
	switch (Container())
	{
	default:
		return 0;
	case CNT_INVENTORY:
	case CNT_STASH:
	case CNT_CUBE:
		return (Data.coordinates & 0xC0) >> 6;
	case CNT_BELT:
		return 3 - ((Data.coordinates & 0x18) >> 3);
	}
}
int Item103::xSize()
{
	if (!this && SelInfo) return SelInfo->SizeX;
	if (!this) return 1;
	if (!Info) FindInfo();
	return Info->SizeX;
}
int Item103::ySize()
{
	if (!this && SelInfo) return SelInfo->SizeY;
	if (!this) return 1;
	if (!Info) FindInfo();
	return Info->SizeY;
}
bool Item103::SetCoordinates(int Container, int xPos, int yPos)
{
	switch (Container)
	{
	default:
		return false;
	case CNT_INVENTORY:
	{
		if (xPos < 0 || xPos >= InvGrids.xInventory) return false;
		if (yPos < 0 || yPos >= InvGrids.yInventory) return false;

		Data.container = (Data.container & 0xF807) | 0x0000;
		Data.coordinates = (Data.coordinates & 0xC1) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0x3F) | (yPos << 6);

		Data.bitfield3 &= 0xF0;
		Data.itemcode &= 0x3FFF;
		return true;
	}
	case CNT_STASH:
	{
		if (xPos < 0 || xPos >= InvGrids.xStash) return false;
		if (yPos < 0 || yPos >= InvGrids.yStash) return false;

		Data.container = (Data.container & 0xF807) | 0x0020;
		Data.coordinates = (Data.coordinates & 0xC1) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0x3F) | (yPos << 6);

		Data.bitfield3 &= 0xF0;
		Data.itemcode &= 0x3FFF;
		return true;
	}
	case CNT_CUBE:
	{
		if (xPos < 0 || xPos >= InvGrids.xCube) return false;
		if (yPos < 0 || yPos >= InvGrids.yCube) return false;

		Data.container = (Data.container & 0xF807) | 0x0018;
		Data.coordinates = (Data.coordinates & 0xC1) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0x3F) | (yPos << 6);

		Data.bitfield3 &= 0xF0;
		Data.itemcode &= 0x3FFF;
		return true;
	}
	case CNT_BELT:
	{
		if (xPos < 0 || xPos >= InvGrids.xBelt) return false;
		if (yPos < 0 || yPos >= InvGrids.yBelt) return false;

		Data.container = (Data.container & 0xF807) | 0x07F8;
		Data.coordinates = (Data.coordinates & 0xF9) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0xE7) | ((3 - yPos) << 3);

		Data.bitfield3 = (Data.bitfield3 & 0xF0) | 0x00;
		Data.itemcode &= 0x3FFF;
		Data.itemcode |= 0x8000;
		return true;
	}
	case CNT_SOCKET:
	{
		Data.container = (Data.container & 0xF807) | 0x07F8;
		Data.coordinates = Data.coordinates & 0xF9;
		Data.coordinates = Data.coordinates & 0xE7;

		Data.bitfield3 = (Data.bitfield3 & 0xF0) | 0x00;
		Data.itemcode &= 0x3FFF;
		Data.itemcode |= 0x8000;
		return true;
	}
	case CNT_BODY:
	{
		if (xPos < 1 || xPos > 10) return false;

		Data.container = (Data.container & 0xF807) | 0x07F8;
		Data.coordinates &= 0x01;

		Data.bitfield3 = (Data.bitfield3 & 0xF0) | (xPos & 0x0F);
		Data.itemcode &= 0x3FFF;
		Data.itemcode |= 0x4000;
		return true;
	}
	}
}
bool Item103::FindInfo()
{
	Info = &itemunknown;
	RAND RDWB = { DWB(), 666 };
	StartRandoms(this, &RDWB);
	int DWB0 = Random(&RDWB);
	for (int z = 0; z < nItemInfos; z++)
	{
		// Check Item Code
		if (ItemCode() != ItemInfos[z].ItemCode) continue;
		// Check if Set or Unique Item
		switch (ItemInfos[z].ItemSearch)
		{
		case 'U':
			if (Quality() != UNIQUEITEM) continue;

			// Check Set/Unique Item Class Code
			if (UniqueCode() != ItemInfos[z].UniqueCode) continue;
			break;
		case 'S':
			if (Quality() != SETITEM) continue;
			// Check Set/Unique Item Class Code
			if (UniqueCode() != ItemInfos[z].UniqueCode) continue;
			// Get correct set item when there are multiple possiblities
			if (ItemInfos[z].DWBCode != 0)
			{
				int offset = DWB0 % 0x10;
				if ((ItemInfos[z].DWBCode & (1 << offset)) == 0) continue;
			}
			break;
		}
		Info = &ItemInfos[z];
	}
	if (!Info || (!Info->BitmapID && Info->ItemCode != 0x1170 && Info->ItemCode != 0x1190))
	{
		Info = &itemunknown;
		return false;
	}
	return true;
}
HBITMAP Item103::GetBitmap()
{
	if (!Info) FindInfo();
	// Amulets
	if (ItemCode() == 0x1170)
	{
		int x = DWARandomOffset(1) % 3;
		return AmuletImages[x].hBmp;
	}
	// Rings
	if (ItemCode() == 0x1190)
	{
		return RingImages[DWARandomOffset(1) % 5].hBmp;
	}
	return ItemInfoGetBitmap(Info);
}
DWORD Item103::ItemCode()
{
	return Data.itemcode & 0x3FF0;
}
void Item103::SetItemCode(DWORD x)
{
	Data.itemcode = (Data.itemcode & 0xC00F) | (WORD(x) & 0x3FF0);
	FindInfo();
}
int Item103::UniqueCode()
{
	return Data.specialitemcode;
}
void Item103::SetUniqueCode(int x)
{
	Data.specialitemcode = x;
	FindInfo();
}
int Item103::Quality()
{
	return Data.quantity & 0x000F;
}
void Item103::SetQuality(int x)
{
	Data.quantity = (Data.quantity & 0xFFF0) | (x & 0x000F);
	FindInfo();
}
bool Item103::Socketed()
{
	return (Data.bitfield1 & 0x0800) != 0;
}
void Item103::SetSocketed(bool i)
{
	Data.bitfield1 = (Data.bitfield1 & 0xF7FF) | (i << 11);
}
bool Item103::Socketable()
{
	if (!Info) FindInfo();
	return (Info->Sockets != 0);
}
bool Item103::Identified()
{
	return (Data.bitfield1 & 0x0010) != 0;
}
void Item103::SetIdentified(bool i)
{
	Data.bitfield1 = Data.bitfield1 & 0xFFEF | (i << 4);
}
bool Item103::Starter()
{
	return (Data.bitfield2 & 0x0002) != 0;
}
void Item103::SetStarter(bool i)
{
	Data.bitfield2 = Data.bitfield2 & 0xFFFD | (i << 1);
}
int Item103::Quantity()
{
	return ((Data.quantity & 0xFFE0) >> 5) +
		((Data.quantityex & 0xFF) << 11);
	// return (Data.quantity & 0x1FE0) >> 5;
}
void Item103::SetQuantity(int i)
{
#if defined(JAMELLAEDITOR)
	if (!RegOptions.ExceedQuantity) {
		Data.quantity = (Data.quantity & 0xE01F) | ((i & 0x00FF) << 5);
	}
	else {
		Data.quantity = (Data.quantity & 0x001F) | ((i & 0x1FFF) << 5);
		Data.quantityex = (i >> 11);
	}
#else
	Data.quantity = (Data.quantity & 0xE01F) | ((i & 0x00FF) << 5);
#endif
}
int Item103::Durability()
{
	return (Data.durability & 0x01FE) >> 1;
}
void Item103::SetDurability(int i)
{
	if (i > DurabilityMax()) i = DurabilityMax();
	if (i > 255) i = 255;

	Data.durability = (Data.durability & 0xFE01) | ((i & 0xFF) << 1);
}
int Item103::DurabilityMax()
{
	return ((Data.durability & 0xFE00) >> 9) + ((Data.coordinates & 0x01) * 128);
}
void Item103::SetDurabilityMax(int i)
{
	if (i > 255) i = 255;

	Data.durability = (Data.durability & 0x01FF) | ((i & 0x7F) << 9);
	Data.coordinates = (Data.coordinates & 0xFE) | ((i & 0x80) ? 0x01 : 0x00);
}
int Item103::GemNum()
{
	return (Data.bitfield3 & 0x60) >> 5;
}
void Item103::SetGemNum(int i)
{
	Data.bitfield3 = (Data.bitfield3 & 0x1F) | ((i % 8) << 5);
}
int Item103::BodyCode()
{
	return Data.bitfield3 & 0x0F;
}
DWORD Item103::DWA()
{
	return ((Data.properties2 & 0x07) << 29) |
		((Data.properties1 & 0xFFFFFFF8) >> 3);
}
void Item103::SetDWA(DWORD x)
{
	Data.properties1 = (Data.properties1 & 0x00000007) | ((x & 0x1FFFFFFF) << 3);
	Data.properties2 = (Data.properties2 & 0xFFFFFFF8) | BYTE((x & 0xE0000000) >> 29);
}
DWORD Item103::DWB()
{
	return ((Data.container & 0x07) << 29) |
		((Data.properties2 & 0xFFFFFFF8) >> 3);
}
void Item103::SetDWB(DWORD x)
{
	Data.container = (Data.container & 0xFFF8) | BYTE((x & 0xE0000000) >> 29);
	Data.properties2 = (Data.properties2 & 0x00000007) | ((x & 0x1FFFFFFF) << 3);
}
int Item103::MagicLevel()
{
	return Data.modlevel;
}
void Item103::SetMagicLevel(int x)
{
	Data.modlevel = x;
}
bool Item103::FindGemInfo()
{
	GInfo = 0;
	for (int z = 0; z < nGemInfos; z++)
	{
		// Check Item Code
		if (ItemCode() != GemInfos[z].ItemCode) continue;
		GInfo = &GemInfos[z];
		return true;
	}
	return false;
}