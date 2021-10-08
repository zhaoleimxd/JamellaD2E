// Inventory Grid Options
#include "JamellaD2E.h"
#if INVGRIDS == 0
// D2 Standard Sizes
struct InvGrids InvGrids =
{
	10,4, // X/Y Inventory
	6,4, // X/Y Stash
	3,4, // X/Y Cube
	4,4 // X/Y Belt
};
#else
// Extended Grids for Fusman
struct InvGrids InvGrids =
{
	10,6, // X/Y Inventory
	10,7, // X/Y Stash
	8,9, // X/Y Cube
	4,4 // X/Y Belt
};
#endif
void SetInvGridPreset(int i)
{
}