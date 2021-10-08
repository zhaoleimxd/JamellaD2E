#include "JamellaD2E.h"
const struct _SuperiorItem SuperiorItemTable[] =
{
{ 0,1,'ht00',1,3,0,0,0,250,1,2,100 },
{ 1,1,'dm02',5,15,0,0,0,250,1,3,250 },
{ 2,1,'ar02',5,15,0,0,0,1797,1,2,200 },
{ 3,2,'ht00',1,3,'dm02',5,15,250,3,5,350 },
{ 4,1,'du01',10,15,0,0,0,2047,1,2,100 },
{ 5,2,'ht00',1,3,'du01',10,15,250,3,5,350 },
{ 6,2,'dm02',5,15,'du01',10,15,250,3,5,350 },
{ 7,2,'ar02',5,15,'du01',10,15,1797,3,5,350 },
};
int nSuperiorItemTable = sizeof SuperiorItemTable / sizeof SuperiorItemTable[0];