#include "JamellaD2E.h"
const struct GemInfo GemInfos[] = {
{ "Empty",0x0000,0xFFFFFFFF,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ "Chipped Amethyst",0x13C0,0x20766367,18,3,'ht00',15,19,0,0,0,0,0,0,'st00',3,3,0,0,0,0,0,0,'ar00',5,6,0,0,0,0,0,0 },
{ "Chipped Topaz",0x1410,0x20796367,14,3,'dl00',1,1,'dl01',6,6,0,0,0,'ib01',7,9,0,0,0,0,0,0,'rl00',10,12,0,0,0,0,0,0 },
{ "Chipped Sapphire",0x1460,0x20626367,5,3,'dc00',1,1,'dc01',3,3,'dc03',25,25,'mm00',6,8,0,0,0,0,0,0,'rc00',10,12,0,0,0,0,0,0 },
{ "Chipped Emerald",0x14B0,0x20676367,11,3,'dp00',8,8,'dp01',8,8,'dp02',75,75,'dx00',3,3,0,0,0,0,0,0,'rp00',10,12,0,0,0,0,0,0 },
{ "Chipped Ruby",0x1500,0x20726367,8,3,'df00',3,3,'df01',4,4,0,0,0,'mh00',6,8,0,0,0,0,0,0,'rf00',10,12,0,0,0,0,0,0 },
{ "Chipped Diamond",0x1550,0x20776367,1,3,'md01',25,29,0,0,0,0,0,0,'ht00',10,10,0,0,0,0,0,0,'ra00',5,6,0,0,0,0,0,0 },
{ "Chipped Skull",0x1640,0x20636B73,2,3,'mm03',1,1,'mh03',2,2,0,0,0,'lf00',2,2,'hl11',8,8,0,0,0,'at00',2,2,0,0,0,0,0,0 },
{ "Flawed Amethyst",0x13D0,0x20766667,18,3,'ht00',20,29,0,0,0,0,0,0,'st00',4,4,0,0,0,0,0,0,'ar00',7,8,0,0,0,0,0,0 },
{ "Flawed Topaz",0x1420,0x20796667,14,3,'dl00',1,1,'dl01',7,7,0,0,0,'ib01',11,13,0,0,0,0,0,0,'rl00',13,16,0,0,0,0,0,0 },
{ "Flawed Sapphire",0x1470,0x20626667,5,3,'dc00',2,2,'dc01',3,3,'dc03',35,35,'mm00',9,11,0,0,0,0,0,0,'rc00',13,16,0,0,0,0,0,0 },
{ "Flawed Emerald",0x14C0,0x20676667,11,3,'dp00',11,11,'dp01',11,11,'dp02',75,75,'dx00',4,4,0,0,0,0,0,0,'rp00',13,16,0,0,0,0,0,0 },
{ "Flawed Ruby",0x1510,0x20726667,8,3,'df00',3,3,'df01',5,5,0,0,0,'mh00',9,11,0,0,0,0,0,0,'rf00',13,16,0,0,0,0,0,0 },
{ "Flawed Diamond",0x1560,0x20776667,1,3,'md01',30,35,0,0,0,0,0,0,'ht00',15,15,0,0,0,0,0,0,'ra00',7,8,0,0,0,0,0,0 },
{ "Flawed Skull",0x1650,0x20666B73,2,3,'mm03',2,2,'mh03',2,2,0,0,0,'lf00',3,3,'hl11',8,8,0,0,0,'at00',3,3,0,0,0,0,0,0 },
{ "Amethyst",0x13E0,0x20767367,18,3,'ht00',30,39,0,0,0,0,0,0,'st00',5,6,0,0,0,0,0,0,'ar00',9,10,0,0,0,0,0,0 },
{ "Topaz",0x1430,0x20797367,14,3,'dl00',1,1,'dl01',8,8,0,0,0,'ib01',14,17,0,0,0,0,0,0,'rl00',17,20,0,0,0,0,0,0 },
{ "Sapphire",0x1480,0x20627367,5,3,'dc00',2,2,'dc01',4,4,'dc03',50,50,'mm00',12,15,0,0,0,0,0,0,'rc00',17,20,0,0,0,0,0,0},
{ "Emerald",0x14D0,0x20677367,11,3,'dp00',14,14,'dp01',14,14,'dp02',75,75,'dx00',5,6,0,0,0,0,0,0,'rp00',17,20,0,0,0,0,0,0 },
{ "Ruby",0x1520,0x20727367,8,3,'df00',4,4,'df01',5,5,0,0,0,'mh00',12,15,0,0,0,0,0,0,'rf00',17,20,0,0,0,0,0,0 },
{ "Diamond",0x1570,0x20777367,1,3,'md01',36,45,0,0,0,0,0,0,'ht00',20,20,0,0,0,0,0,0,'ra00',9,11,0,0,0,0,0,0 },
{ "Skull",0x1660,0x20756B73,2,3,'mm03',2,2,'mh03',3,3,0,0,0,'lf00',3,3,'hl11',12,12,0,0,0,'at00',4,4,0,0,0,0,0,0 },
{ "Flawless Amethyst",0x13F0,0x20767A67,18,3,'ht00',40,49,0,0,0,0,0,0,'st00',7,8,0,0,0,0,0,0,'ar00',11,13,0,0,0,0,0,0 },
{ "Flawless Topaz",0x1440,0x20796C67,14,3,'dl00',1,1,'dl01',10,10,0,0,0,'ib01',18,20,0,0,0,0,0,0,'rl00',21,25,0,0,0,0,0,0 },
{ "Flawless Sapphire",0x1490,0x20626C67,5,3,'dc00',3,3,'dc01',5,5,'dc03',60,60,'mm00',16,20,0,0,0,0,0,0,'rc00',21,25,0,0,0,0,0,0 },
{ "Flawless Emerald",0x14E0,0x20676C67,11,3,'dp00',17,17,'dp01',17,17,'dp02',75,75,'dx00',7,8,0,0,0,0,0,0,'rp00',21,25,0,0,0,0,0,0 },
{ "Flawless Ruby",0x1530,0x20726C67,8,3,'df00',5,5,'df01',6,6,0,0,0,'mh00',16,20,0,0,0,0,0,0,'rf00',21,25,0,0,0,0,0,0 },
{ "Flawless Diamond",0x1580,0x20776C67,1,3,'md01',46,55,0,0,0,0,0,0,'ht00',25,25,0,0,0,0,0,0,'ra00',12,15,0,0,0,0,0,0 },
{ "Flawless Skull",0x1670,0x206C6B73,2,3,'mm03',3,3,'mh03',3,3,0,0,0,'lf00',4,4,'hl11',12,12,0,0,0,'at00',5,5,0,0,0,0,0,0 },
{ "Perfect Amethyst",0x1400,0x20767067,17,3,'ht00',50,60,0,0,0,0,0,0,'st00',9,10,0,0,0,0,0,0,'ar00',15,17,0,0,0,0,0,0 },
{ "Perfect Topaz",0x1450,0x20797067,13,3,'dl00',1,1,'dl01',12,15,0,0,0,'ib01',21,25,0,0,0,0,0,0,'rl00',26,30,0,0,0,0,0,0 },
{ "Perfect Sapphire",0x14A0,0x20627067,6,3,'dc00',3,3,'dc01',6,7,'dc03',75,75,'mm00',21,25,0,0,0,0,0,0,'rc00',26,30,0,0,0,0,0,0 },
{ "Perfect Emerald",0x14F0,0x20677067,12,3,'dp00',20,20,'dp01',20,20,'dp02',75,75,'dx00',9,10,0,0,0,0,0,0,'rp00',26,30,0,0,0,0,0,0 },
{ "Perfect Ruby",0x1540,0x20727067,9,3,'df00',6,6,'df01',7,10,0,0,0,'mh00',21,25,0,0,0,0,0,0,'rf00',26,30,0,0,0,0,0,0 },
{ "Perfect Diamond",0x1590,0x20777067,0,3,'md01',56,70,0,0,0,0,0,0,'ht00',30,30,0,0,0,0,0,0,'ra00',16,20,0,0,0,0,0,0 },
{ "Perfect Skull",0x1680,0x207A6B73,3,3,'mm03',3,3,'mh03',4,4,0,0,0,'lf00',4,5,'hl11',16,20,0,0,0,'at00',6,7,0,0,0,0,0,0 },};
int nGemInfos = sizeof GemInfos / sizeof GemInfos[0];