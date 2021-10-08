// Item104Sm.cpp from D2E
// Contains methods for the new short item structure past 1.03
#include "JamellaD2E.h"
Item104Sm::Item104Sm()
{
	ZeroMemory(&Data, sizeof Data);
}
Item104Sm::~Item104Sm()
{
	if (ListNext) {
		delete ListNext;
		ListNext = 0;
	}
}
bool Item104Sm::LoadItemRecord(BYTE* record)
{
	if (*(record + 0) != 'J' && *(record + 1) != 'M')
		return false;
	memcpy(&Data, record, sizeof Data);
	Info = 0;
	return true;
}
BYTE* Item104Sm::GetItemRecord()
{
	return (BYTE*)&Data;
}
DWORD Item104Sm::ItemRecordLength()
{
	return sizeof Data;
}
void Item104Sm::BlankItem()
{
	ZeroMemory(&Data, sizeof Data);
	Data.JM[0] = 0x4A;
	Data.JM[1] = 0x4D;
	Data.bitfield1 = 0x0010;
	Data.bitfield2 = 0x0038;
}
int Item104Sm::Container() const
{
	if ((Data.coordinates & 0xFC01) == 0x0000 && (Data.itemcode & 0x00000003) == 0x00) return CNT_INVENTORY;
	if ((Data.coordinates & 0xFC01) == 0x1000 && (Data.itemcode & 0x00000003) == 0x00) return CNT_STASH;
	if ((Data.coordinates & 0xFC01) == 0x0C00 && (Data.itemcode & 0x00000003) == 0x00) return CNT_CUBE;
	if ((Data.coordinates & 0xFC01) == 0xFC01 && (Data.itemcode & 0x00000003) == 0x03) return CNT_BELT;
	return CNT_NONE;
}
int Item104Sm::xPos() const
{
	switch (Container())
	{
	default:
		return 0;
	case CNT_INVENTORY:
	case CNT_STASH:
	case CNT_CUBE:
		return (Data.coordinates & 0x007C) >> 2;
	case CNT_BELT:
		return (Data.coordinates & 0x000C) >> 2;
	}
}
int Item104Sm::yPos() const
{
	switch (Container())
	{
	default:
		return 0;
	case CNT_INVENTORY:
	case CNT_STASH:
	case CNT_CUBE:
		return (Data.coordinates & 0x0180) >> 7;
	case CNT_BELT:
		return 3 - ((Data.coordinates & 0x0030) >> 4);
	}
}
int Item104Sm::xSize()
{
	if (!this && SelInfo) return SelInfo->SizeX;
	if (!this) return 1;
	if (!Info)
		if (!FindInfo())
			return 1;
	return Info->SizeX;
}
int Item104Sm::ySize()
{
	if (!this && SelInfo) return SelInfo->SizeY;
	if (!this) return 1;
	if (!Info)
		if (!FindInfo())
			return 1;
	return Info->SizeY;
}
bool Item104Sm::SetCoordinates(int Container, int xPos, int yPos)
{
	switch (Container)
	{
	default:
		return false;
	case CNT_INVENTORY:
	{
		if (xPos < 0 || xPos >= InvGrids.xInventory) return false;
		if (yPos < 0 || yPos >= InvGrids.yInventory) return false;
		Data.coordinates = (Data.coordinates & 0x03FE) | 0x0000;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);

		Data.itemcode &= 0xFFFFFFFC;
		return true;
	}
	case CNT_STASH:
	{
		if (xPos < 0 || xPos >= InvGrids.xStash) return false;
		if (yPos < 0 || yPos >= InvGrids.yStash) return false;

		Data.coordinates = (Data.coordinates & 0x03FE) | 0x1000;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);

		Data.itemcode &= 0xFFFFFFFC;
		return true;
	}
	case CNT_CUBE:
	{
		if (xPos < 0 || xPos >= InvGrids.xCube) return false;
		if (yPos < 0 || yPos >= InvGrids.yCube) return false;

		Data.coordinates = (Data.coordinates & 0x03FE) | 0x0C00;
		Data.coordinates = (Data.coordinates & 0xFF83) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFE7F) | (yPos << 7);

		Data.itemcode &= 0xFFFFFFFC;
		return true;
	}
	case CNT_BELT:
	{
		if (xPos < 0 || xPos >= InvGrids.xBelt) return false;
		if (yPos < 0 || yPos >= InvGrids.yBelt) return false;

		Data.coordinates = (Data.coordinates & 0x03FE) | 0xFC01;
		Data.coordinates = (Data.coordinates & 0xFFF3) | (xPos << 2);
		Data.coordinates = (Data.coordinates & 0xFFCF) | ((3 - yPos) << 4);

		Data.itemcode |= 0x00000003;
		return true;
	}
	case CNT_SOCKET:
	{
		if (xPos < 0 || xPos >= 7) return false;
		Data.coordinates = (Data.coordinates & 0x03E0) | 0xFC1B;
		Data.coordinates = (Data.coordinates & 0xFC7F) | (xPos << 7);

		Data.itemcode |= 0x00000003;
		return true;
	}
	}
}
bool Item104Sm::FindInfo()
{
	Info = &itemunknown;
	for (int z = 0; z < nItemInfos; z++)
	{
		// Check Item Code
		if (ItemInfos[z].IC == 0xFFFFFFFF) continue;
		if (ItemCode() != ItemInfos[z].IC) continue;
		Info = &ItemInfos[z];
	}
	if (!Info || !Info->BitmapID)
	{
		Info = &itemunknown;
		return false;
	}
	return true;
}
HBITMAP Item104Sm::GetBitmap()
{
	if (!Info) FindInfo();
	return ItemInfoGetBitmap(Info);
}
bool Item104Sm::FindGemInfo()
{
	GInfo = 0;
	for (int z = 0; z < nGemInfos; z++)
	{
		// Check Item Code
		if (ItemCode() != GemInfos[z].IC) continue;
		GInfo = &GemInfos[z];
		return true;
	}
	return false;
}
DWORD Item104Sm::ItemCode()
{
	return ((Data.itemcode & 0xFFFFFFFC) >> 2) |
		((Data.zero & 0x03) << 30);
}
void Item104Sm::SetItemCode(DWORD x)
{
	Data.itemcode = (Data.itemcode & 0x00000003) | ((x << 2) & 0xFFFFFFFC);
	Data.zero = (Data.zero & 0xF7) | (BYTE(x >> 30) & 0x03);
	Info = 0;
}
const char* Item104Sm::RichText()
{
	char* s = RichTextTmp;
	StartRTF(s);
	strmcat(s, "\\cf0 ", Name(), 0);
	if (Info->Description)
	{
		strmcat(s, "\\par\\cf0 ", Info->Description, 0);
	}
	strcat(s, "\\par }");
	return s;
}