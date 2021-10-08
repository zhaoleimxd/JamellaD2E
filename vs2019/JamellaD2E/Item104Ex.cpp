// Item104Ex.cpp from D2E
// Contains methods for the new extended item structure past 1.03
#include "JamellaD2E.h"
Item104Ex::Item104Ex()
{
	ZeroMemory(&Data, sizeof Data);
	MD = new class MagicDecoder(this);
}
Item104Ex::~Item104Ex()
{
	delete MD;
	if (ListNext) {
		delete ListNext;
		ListNext = 0;
	}
}
bool Item104Ex::LoadItemRecord(BYTE* record)
{
	if (*(record + 0) != 'J' && *(record + 1) != 'M')
		return false;
	memcpy(&Data, record, sizeof Data);
	Info = 0;
	return true;
}
BYTE* Item104Ex::GetItemRecord()
{
	return (BYTE*)&Data;
}
DWORD Item104Ex::ItemRecordLength()
{
	return sizeof Data;
}
void Item104Ex::BlankItem()
{
	ZeroMemory(&Data, sizeof Data);
	Data.JM[0] = 0x4A;
	Data.JM[1] = 0x4D;
	Data.bitfield1 = 0x0010;
	Data.bitfield2 = 0x0018;
}
int Item104Ex::Container() const
{
	if ((Data.container & 0x07F8) == 0x0000) return CNT_INVENTORY;
	if ((Data.container & 0x07F8) == 0x0020) return CNT_STASH;
	if ((Data.container & 0x07F8) == 0x0018) return CNT_CUBE;
	if ((Data.container & 0x07F8) == 0x07F8) return CNT_BODY;
	return CNT_NONE;
}
int Item104Ex::xPos() const
{
	return (Data.coordinates & 0x3E) >> 1;
}
int Item104Ex::yPos() const
{
	return (Data.coordinates & 0xC0) >> 6;
}
int Item104Ex::xSize()
{
	if (!this && SelInfo) return SelInfo->SizeX;
	if (!this) return 1;
	if (!Info)
		if (!FindInfo())
			return 1;
	return Info->SizeX;
}
int Item104Ex::ySize()
{
	if (!this && SelInfo) return SelInfo->SizeY;
	if (!this) return 1;
	if (!Info)
		if (!FindInfo())
			return 1;
	return Info->SizeY;
}
bool Item104Ex::SetCoordinates(int Container, int xPos, int yPos)
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

		Data.itemcode &= 0xFFFFFFFC;
		Data.magicrestr &= 0xBFFF;
		return true;
	}
	case CNT_STASH:
	{
		if (xPos < 0 || xPos >= InvGrids.xStash) return false;
		if (yPos < 0 || yPos >= InvGrids.yStash) return false;

		Data.container = (Data.container & 0xF807) | 0x0020;
		Data.coordinates = (Data.coordinates & 0xC1) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0x3F) | (yPos << 6);

		Data.itemcode &= 0xFFFFFFFC;
		Data.magicrestr &= 0xBFFF;
		return true;
	}
	case CNT_CUBE:
	{
		if (xPos < 0 || xPos >= InvGrids.xCube) return false;
		if (yPos < 0 || yPos >= InvGrids.yCube) return false;

		Data.container = (Data.container & 0xF807) | 0x0018;
		Data.coordinates = (Data.coordinates & 0xC1) | (xPos << 1);
		Data.coordinates = (Data.coordinates & 0x3F) | (yPos << 6);

		Data.itemcode &= 0xFFFFFFFC;
		Data.magicrestr &= 0xBFFF;
		return true;
	}
	case CNT_BODY:
	{
		if (xPos < 1 || xPos > 10) return false;

		Data.container = (Data.container & 0xF807) | 0x07F8;
		Data.coordinates &= 0x01;

		Data.itemcode &= 0xFFFFFFFC;

		Data.bodycode = (Data.bodycode & 0xC3) | ((xPos & 0x0F) << 2);
		Data.magicrestr |= 0x4000;
		return true;
	}
	}
}
bool Item104Ex::FindInfo()
{
	Info = &itemunknown;
	RAND RDWB = { DWB(), 666 };
	StartRandoms(this, &RDWB);
	DWORD DWB0 = Random(&RDWB);
	for (int z = 0; z < nItemInfos; z++)
	{
		// Check Item Code
		if (ItemInfos[z].IC == 0xFFFFFFFF) continue;
		if (ItemCode() != ItemInfos[z].IC) continue;
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
				if ((ItemInfos[z].DWBCode & (1 << offset)) == 0)
					continue;
			}
			break;
		}
		Info = &ItemInfos[z];
	}
	if (!Info || (!Info->BitmapID && Info->IC != ' uma' && Info->IC != ' nir'))
	{
		Info = &itemunknown;
		return false;
	}
	return true;
}
HBITMAP Item104Ex::GetBitmap()
{
	if (!Info) FindInfo();
	// Amulets
	if (ItemCode() == ' uma')
	{
		int x = DWARandomOffset(1) % 3;
		return AmuletImages[x].hBmp;
	}
	// Rings
	if (ItemCode() == ' nir')
	{
		return RingImages[DWARandomOffset(1) % 5].hBmp;
	}
	return ItemInfoGetBitmap(Info);
}
DWORD Item104Ex::ItemCode()
{
	return ((Data.itemcode & 0xFFFFFFFC) >> 2) |
		((Data.bodycode & 0x0003) << 30);
}
void Item104Ex::SetItemCode(DWORD x)
{
	Data.itemcode = (Data.itemcode & 0x00000003) | ((x << 2) & 0xFFFFFFFC);
	Data.bodycode = (Data.bodycode & 0xF7) | (BYTE(x >> 30) & 0x03);
	Info = 0;
}
int Item104Ex::UniqueCode()
{
	return Data.uniquecode;
}
void Item104Ex::SetUniqueCode(int x)
{
	Data.uniquecode = x;
	Info = 0;
}
int Item104Ex::Quality()
{
	return Data.quantity & 0x000F;
}
void Item104Ex::SetQuality(int x)
{
	Data.quantity = (Data.quantity & 0xFFF0) | (x & 0x000F);
	Info = 0;
}
int Item104Ex::GemNum()
{
	return ((Data.bodycode & 0x80) >> 7) |
		((Data.magicrestr & 0x03) << 1);
}
void Item104Ex::SetGemNum(int i)
{
	Data.bodycode = (Data.bodycode & 0x7F) | ((i << 7) & 0x80);
	Data.magicrestr = (Data.magicrestr & 0xFFFC) | ((i >> 1) & 0x03);
}
bool Item104Ex::Socketed()
{
	return (Data.bitfield1 & 0x0800) != 0;
}
void Item104Ex::SetSocketed(bool i)
{
	Data.bitfield1 = (Data.bitfield1 & 0xF7FF) | (i << 11);
}
bool Item104Ex::Socketable()
{
	if (!Info) FindInfo();
	return (Info->Sockets != 0);
}
bool Item104Ex::Identified()
{
	return (Data.bitfield1 & 0x0010) != 0;
}
void Item104Ex::SetIdentified(bool i)
{
	Data.bitfield1 = Data.bitfield1 & 0xFFEF | (i << 4);
}
bool Item104Ex::Starter()
{
	return (Data.bitfield2 & 0x0002) != 0;
}
void Item104Ex::SetStarter(bool i)
{
	Data.bitfield2 = Data.bitfield2 & 0xFFFD | (i << 1);
}
int Item104Ex::Quantity()
{
	return ((Data.quantity & 0xFFE0) >> 5) +
		((Data.quantityex & 0xFF) << 11);
	// return (Data.quantity & 0x1FE0) >> 5;
}
void Item104Ex::SetQuantity(int i)
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
int Item104Ex::Durability()
{
	return (Data.durability & 0x01FE) >> 1;
}
void Item104Ex::SetDurability(int i)
{
	if (i > DurabilityMax()) i = DurabilityMax();
	if (i > 255) i = 255;

	Data.durability = (Data.durability & 0xFE01) | ((i & 0xFF) << 1);
}
int Item104Ex::DurabilityMax()
{
	return ((Data.durability & 0xFE00) >> 9) + ((Data.coordinates & 0x01) * 128);
}
void Item104Ex::SetDurabilityMax(int i)
{
	if (i > 255) i = 255;

	Data.durability = (Data.durability & 0x01FF) | ((i & 0x7F) << 9);
	Data.coordinates = (Data.coordinates & 0xFE) | ((i & 0x80) ? 0x01 : 0x00);
}
int Item104Ex::BodyCode()
{
	return (Data.bodycode >> 2) & 0x0F;
}
DWORD Item104Ex::DWA()
{
	return ((Data.DWB & 0x07) << 29) |
		((Data.DWA & 0xFFFFFFF8) >> 3);
}
void Item104Ex::SetDWA(DWORD x)
{
	Data.DWA = (Data.DWA & 0x00000007) | ((x & 0x1FFFFFFF) << 3);
	Data.DWB = (Data.DWB & 0xFFFFFFF8) | ((x & 0xE0000000) >> 29);
}
DWORD Item104Ex::DWB()
{
	return ((Data.container & 0x07) << 29) |
		((Data.DWB & 0xFFFFFFF8) >> 3);
}
void Item104Ex::SetDWB(DWORD x)
{
	Data.container = (Data.container & 0xFFF8) | BYTE((x & 0xE0000000) >> 29);
	Data.DWB = (Data.DWB & 0x00000007) | ((x & 0x1FFFFFFF) << 3);
}
int Item104Ex::MagicLevel()
{
	return (Data.magicrestr & 0x03FC) >> 2;
}
void Item104Ex::SetMagicLevel(int x)
{
	Data.magicrestr = (Data.magicrestr & 0xFC03) | ((x & 0xFF) << 2);
}