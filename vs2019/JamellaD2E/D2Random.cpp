#include "JamellaD2E.h"
int StartRandoms(Item* I, RAND* r)
{
	if (!I->Info)
		if (!I->FindInfo())
			return 0;
	r->Seed = I->DWB();
	r->Carry = 666;
	int UsualAdd[6] = { 1000, 200, 125, 30, 12, 4 };
	int UsualDiv[6] = { 1, 1, 1, 16, 16, 8 };
	int ExcepAdd[6] = { 600, 120, 100, 3, 4, 1 };
	int ExcepDiv[6] = { 1, 1, 1, 100, 16, 8 };
	int* Add = (I->Info->Quality == 'E') ? ExcepAdd : UsualAdd;
	int* Div = (I->Info->Quality == 'E') ? ExcepDiv : UsualDiv;
	int rands = 0;
	for (int z = 0; z < 6; z++)
	{
		int modulo = Add[z] - (I->MagicLevel() / Div[z]);
		if (modulo <= 0) modulo = 1;
		rands++;
		if ((Random(r) % modulo) == 0) break;
	}
	return rands;
}