// CreateItem.cpp from D2E
// Contains the function that creates item from item info
#include "JamellaD2E.h"
int MaximumItemStructure()
{
	int m = -1;
	for (Item* I = Items; I != 0; I = I->Next())
		if (I->ItemRecordID() > m)
			m = I->ItemRecordID();
	return m;
}
Item* CreateItem(Item** IChain, ItemInfo* Info)
{
	Item* I = 0;
	int toCreate = IT_BASE;
	if (RegOptions.CreateItemRecordFormat == 1) // 1.03 Records
	{
		if (Info->IC != ' rae')
			toCreate = IT_103;
		else if (Info->IC == ' rae')
			toCreate = IT_103EAR;
	}
	else if (RegOptions.CreateItemRecordFormat == 2) // 1.04 Records
	{
		if (Info->IC != ' rae') {
			if (!Info->Sm)
				toCreate = IT_104EX;
			else
				toCreate = IT_104SM;
		}
		else if (Info->IC == ' rae')
			toCreate = IT_104EAR;
	}
	else // Auto-detect Record
	{
		int m = MaximumItemStructure();
		if (m < IT_104EX && Info->IC != ' rae')
			toCreate = IT_103;
		else if (m < IT_104EX && Info->IC == ' rae')
			toCreate = IT_103EAR;
		else if (m >= IT_104EX && Info->IC != ' rae') {
			if (!Info->Sm)
				toCreate = IT_104EX;
			else
				toCreate = IT_104SM;
		}
		else if (m >= IT_104EX && Info->IC == ' rae')
			toCreate = IT_104EAR;
	}
	switch (toCreate)
	{
	case IT_103:
	{ // Create Item103

		I = new Item103;
		if (*IChain)
			(*IChain)->Attach(I);
		else
			*IChain = I;

		I->BlankItem();

		I->SetItemCode(Info->ItemCode);
		I->SetUniqueCode(Info->UniqueCode);
		I->SetIdentified(true);

		switch (Info->ItemSearch)
		{
		case 'N': I->SetQuality(USUALITEM); break;
		case 'M': I->SetQuality(MAGICITEM); break;
		case 'S': I->SetQuality(SETITEM); break;
		case 'U': I->SetQuality(UNIQUEITEM); break;
		}

		I->SetDurabilityMax(Info->Durability);
		I->SetDurability(Info->Durability);
		I->SetQuantity(Info->Quantity);

		I->SetMagicLevel((rand() % 90) + 1);

		I->SetDWA(rand() + (rand() << 16));
		I->SetDWB(rand() + (rand() << 16));

		I->Info = Info;
		if (Info->DWBCode != 0 && I->Quality() == SETITEM)
		{
			for (int z = 0; z < 20000; z++)
			{
				RAND RDWB = { I->DWB(), 666 };
				StartRandoms(I, &RDWB);

				int offset = Random(&RDWB) % 0x10;
				if ((Info->DWBCode & (1 << offset)) != 0) break;

				I->SetDWB(rand() + (rand() << 16));
			}
		}

		I->FindInfo();

	} // Create Item103
	break;
	case IT_103EAR:
	{ // Create Item103Ear

		I = new Item103Ear;

		if (*IChain)
			(*IChain)->Attach(I);
		else
			*IChain = I;

		I->BlankItem();

		I->SetItemCode(Info->ItemCode);
		I->SetOpponentName("SetNewName");

	} // Create Item103Ear
	break;
	case IT_104EX:
	{ // Create Item104Ex

		I = new Item104Ex;
		if (*IChain)
			(*IChain)->Attach(I);
		else
			*IChain = I;

		I->BlankItem();

		I->SetItemCode(Info->IC);
		I->SetUniqueCode(Info->UniqueCode);
		I->SetIdentified(true);

		switch (Info->ItemSearch)
		{
		case 'N': I->SetQuality(USUALITEM); break;
		case 'M': I->SetQuality(MAGICITEM); break;
		case 'S': I->SetQuality(SETITEM); break;
		case 'U': I->SetQuality(UNIQUEITEM); break;
		}

		I->SetDurabilityMax(Info->Durability);
		I->SetDurability(Info->Durability);
		I->SetQuantity(Info->Quantity);

		I->SetMagicLevel((rand() % 90) + 1);

		I->SetDWA(rand() + (rand() << 16));
		I->SetDWB(rand() + (rand() << 16));
		I->Info = Info;

		if (Info->DWBCode != 0 && I->Quality() == SETITEM)
		{
			for (int z = 0; z < 20000; z++)
			{
				RAND RDWB = { I->DWB(), 666 };
				StartRandoms(I, &RDWB);

				int offset = Random(&RDWB) % 0x10;
				if ((Info->DWBCode & (1 << offset)) != 0)
					break;

				I->SetDWB(rand() + (rand() << 16));
			}
		}
		I->FindInfo();
	} // Create Item104Ex
	break;
	case IT_104SM:
	{ // Create Item104Sm
		I = new Item104Sm;
		if (*IChain)
			(*IChain)->Attach(I);
		else
			*IChain = I;

		I->BlankItem();

		I->SetItemCode(Info->IC);
		I->FindInfo();
	} // Create Item104Sm
	break;
	case IT_104EAR:
	{ // Create Item104Ear

		I = new Item104Ear;

		if (*IChain)
			(*IChain)->Attach(I);
		else
			*IChain = I;

		I->BlankItem();
		I->SetOpponentName("SetNewName");

	} // Create Item104Ear
	break;
	}
	return I;
}