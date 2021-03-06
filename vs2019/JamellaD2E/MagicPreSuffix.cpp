#include "JamellaD2E.h"
struct _MagicPreSuffix MagicPrefixTable[] =
{
{ 0,"Resiliant",1,0,101,0,'ar00',1,1,0,0,0,0,0,0,0,0,0,8192,0,16,"Not Available" },
{ 1,"Sturdy",4,3,102,1,'ar02',20,30,0,0,0,0,0,0,0,0,0,1797,0,16,"+20-30% to Defense" },
{ 2,"Strong",9,6,102,1,'ar02',31,40,0,0,0,0,0,0,0,0,0,1797,0,16,"+31-40% to Defense" },
{ 3,"Glorious",19,14,102,1,'ar02',41,50,0,0,0,0,0,0,0,0,0,1797,0,16,"+41-50% to Defense" },
{ 4,"Blessed",25,18,102,1,'ar02',51,65,0,0,0,0,0,0,0,0,0,1797,0,16,"+51-60% to Defense" },
{ 5,"Saintly",31,23,102,1,'ar02',66,80,0,0,0,0,0,0,0,0,0,1797,1,16,"+61-80% to Defense" },
{ 6,"Holy",36,27,102,1,'ar02',81,100,0,0,0,0,0,0,0,0,0,1797,1,16,"+81-100% to Defense" },
{ 7,"Devious",7,5,103,0,'ar03',1,1,0,0,0,0,0,0,0,0,0,4,1,5,"Magic Damage Reduced by 1" },
{ 8,"Fortified",14,10,103,0,'ar03',2,2,0,0,0,0,0,0,0,0,0,4,1,5,"Magic Damage Reduced by 2" },
{ 9,"Urgent",9,6,104,0,'at02',10,10,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 10,"Fleet",6,4,104,0,'at03',20,20,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 11,"Muscular",12,9,105,0,'dm01',1,1,0,0,0,0,0,0,0,0,0,8192,0,3,"Not Available" },
{ 12,"Jagged",1,0,106,2,'dm02',10,20,0,0,0,0,0,0,0,0,0,242,0,16,"+20-30% to Damage" },
{ 13,"Deadly",5,3,106,2,'dm02',21,30,0,0,0,0,0,0,0,0,0,8434,0,16,"+21-30% to Damage" },
{ 14,"Vicious",8,6,106,2,'dm02',31,40,0,0,0,0,0,0,0,0,0,242,0,16,"+31-40% to Damage" },
{ 15,"Brutal",14,10,106,2,'dm02',41,50,0,0,0,0,0,0,0,0,0,242,0,16,"+41-50% to Damage" },
{ 16,"Massive",20,15,106,2,'dm02',51,65,0,0,0,0,0,0,0,0,0,242,1,16,"+51-65% to Damage" },
{ 17,"Savage",26,19,106,2,'dm02',66,80,0,0,0,0,0,0,0,0,0,146,1,16,"+66-80% to Damage" },
{ 18,"Merciless",32,24,106,2,'dm02',81,100,0,0,0,0,0,0,0,0,0,146,1,16,"+81-100% to Damage" },
{ 19,"Vulpine",9,6,107,0,'dm03',10,10,0,0,0,0,0,0,0,0,0,4100,1,6,"Damage Generates 10% Mana" },
{ 20,"Swift",4,3,108,0,'dx00',1,1,0,0,0,0,0,0,0,0,0,0,0,16,"Not Available" },
{ 21,"Artful",3,2,108,0,'dx00',20,20,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 22,"Skillful",23,17,108,0,'dx00',50,50,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 23,"Adroit",26,19,108,0,'dx00',100,100,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 24,"Tireless",14,10,109,0,'hl08',50,50,0,0,0,0,0,0,0,0,0,256,0,2,"+ Stamina Regeneration" },
{ 25,"Rugged",14,10,110,1,'hl12',5,10,0,0,0,0,0,0,0,0,0,16128,0,2,"+ 5-10 to Max Stamina" },
{ 26,"Bronze",1,0,111,1,'ht00',10,20,0,0,0,0,0,0,0,0,0,6898,0,15,"+10-20 to Attack Rating" },
{ 27,"Iron",4,3,111,1,'ht00',21,40,0,0,0,0,0,0,0,0,0,2290,0,15,"+21-40 to Attack Rating" },
{ 28,"Steel",8,6,111,1,'ht00',41,60,0,0,0,0,0,0,0,0,0,2290,0,15,"+41-60 to Attack Rating" },
{ 29,"Silver",12,9,111,1,'ht00',61,80,0,0,0,0,0,0,0,0,0,2290,0,15,"+61-80 to Attack Rating" },
{ 30,"Silver",17,12,111,1,'ht00',40,80,0,0,0,0,0,0,0,0,0,0,0,15,"Not Available" },
{ 31,"Gold",17,12,111,1,'ht00',81,100,0,0,0,0,0,0,0,0,0,242,1,15,"+81-100 to Attack Rating" },
{ 32,"Platinum",22,16,111,1,'ht00',101,120,0,0,0,0,0,0,0,0,0,2290,1,15,"+101-120 to Attack Rating" },
{ 33,"Meteoric",27,20,111,1,'ht00',121,150,0,0,0,0,0,0,0,0,0,2194,1,15,"+121-150 to Attack Rating" },
{ 34,"Sharp",5,3,112,3,'ht00',10,20,'dm02',10,20,0,0,0,0,0,0,242,0,16,"+10-20 to AR & +10-20 to Damage" },
{ 35,"Fine",9,6,112,3,'ht00',21,40,'dm02',21,30,0,0,0,0,0,0,242,0,16,"+21-40 to AR & +21-30 to Damage" },
{ 36,"Warrior's",15,11,112,3,'ht00',41,60,'dm02',31,40,0,0,0,0,0,0,242,0,16,"+41-60 to AR & +31-40 to Damage" },
{ 37,"Soldier's",21,15,112,3,'ht00',61,80,'dm02',41,50,0,0,0,0,0,0,242,0,16,"+61-80 to AR & +41-50 to Damage" },
{ 38,"Knight's",25,18,112,3,'ht00',81,100,'dm02',51,65,0,0,0,0,0,0,242,1,16,"+81-100 to AR & +51-65 to Damage" },
{ 39,"Lord's",30,22,112,3,'ht00',101,120,'dm02',66,80,0,0,0,0,0,0,146,1,16,"+101-120 to AR & +66-80 to Damage" },
{ 40,"King's",35,26,112,3,'ht00',121,150,'dm02',81,100,0,0,0,0,0,0,146,1,16,"+121-150 to AR & +81-100 to Damage" },
{ 41,"Howling",16,12,113,0,'hw00',1,1,0,0,0,0,0,0,0,0,0,114,1,19,"Make Monsters Flee" },
{ 42,"Fortuitous",5,3,114,1,'ib01',10,15,0,0,0,0,0,0,0,0,0,14336,0,15,"10-15% Better Chance of Getting Magic Items" },
{ 43,"Brilliant",4,3,115,0,'mm00',10,10,0,0,0,0,0,0,0,0,0,0,0,16,"Not Available" },
{ 44,"Omniscient",1,0,115,0,'in00',10,10,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 45,"Sage",8,6,115,0,'in00',20,20,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 46,"Shrewd",23,17,115,0,'in00',50,50,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 47,"Vivid",27,20,115,0,'in00',100,100,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 48,"Glimmering",1,0,116,0,'lt00',1,1,0,0,0,0,0,0,0,0,0,32613,0,13,"+1 to Light Radius" },
{ 49,"Glowing",6,4,116,0,'lt00',2,2,0,0,0,0,0,0,0,0,0,32613,1,13,"+2 to Light Radius" },
{ 50,"Bright",9,6,117,0,'mh01',50,50,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 51,"Solar",20,15,117,0,'mh01',100,100,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 52,"Lizard's",3,2,118,1,'mm00',1,5,0,0,0,0,0,0,0,0,0,31861,0,6,"+1-5 to Mana" },
{ 53,"Forceful",13,-1,118,0,'mm00',5,5,0,0,0,0,0,0,0,0,0,24576,0,6,"Not Available" },
{ 54,"Snake's",6,4,118,1,'mm00',5,10,0,0,0,0,0,0,0,0,0,23668,0,6,"+5-10 to Mana" },
{ 55,"Serpent's",14,10,118,1,'mm00',11,20,0,0,0,0,0,0,0,0,0,23668,0,6,"+11-20 to Mana" },
{ 56,"Serpent's",37,27,118,1,'mm00',11,20,0,0,0,0,0,0,0,0,0,771,0,6,"+11-20 to Mana" },
{ 57,"Drake's",20,15,118,1,'mm00',21,30,0,0,0,0,0,0,0,0,0,23664,1,6,"+21-30 to Mana" },
{ 58,"Dragon's",24,18,118,1,'mm00',31,40,0,0,0,0,0,0,0,0,0,22640,1,6,"+31-40 to Mana" },
{ 59,"Dragon's",52,39,118,1,'mm00',31,40,0,0,0,0,0,0,0,0,0,769,1,6,"+31-40 to Mana" },
{ 60,"Wyrm's",30,22,118,1,'mm00',41,60,0,0,0,0,0,0,0,0,0,22640,1,6,"+41-50 to Mana" },
{ 61,"Dazzling",9,6,119,0,'mm01',50,50,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 62,"Facinating",20,15,119,0,'mm01',100,100,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 63,"Prismatic",27,20,120,4,'rf00',15,25,'rl00',15,25,'rc00',15,25,'rp00',15,25,4096,1,17,"+15-25% to all Resistances"},
{ 64,"Prismatic",62,46,120,0,'ra00',15,15,0,0,0,0,0,0,0,0,0,2048,1,17,"+15% to all Resistances" },
{ 65,"Azure",5,3,121,1,'rc00',5,10,0,0,0,0,0,0,0,0,0,32757,0,4,"+5-10% to Cold Resist" },
{ 66,"Lapis",12,9,121,1,'rc00',11,20,0,0,0,0,0,0,0,0,0,24437,0,4,"+11-20% to Cold Resist" },
{ 67,"Lapis",35,26,121,1,'rc00',11,20,0,0,0,0,0,0,0,0,0,642,0,4,"+11-20% to Cold Resist" },
{ 68,"Cobalt",18,13,121,1,'rc00',21,30,0,0,0,0,0,0,0,0,0,23925,0,4,"+21-30% to Cold Resist" },
{ 69,"Cobalt",55,41,121,1,'rc00',21,30,0,0,0,0,0,0,0,0,0,642,0,4,"+21-30% to Cold Resist" },
{ 70,"Indigo",3,2,121,0,'rc00',25,25,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 71,"Sapphire",25,18,121,1,'rc00',31,50,0,0,0,0,0,0,0,0,0,31200,1,4,"+31-50% to Cold Resist" },
{ 72,"Cerulean",7,5,121,0,'rc00',50,50,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 73,"Red",7,5,122,0,'rf00',2,2,0,0,0,0,0,0,0,0,0,24576,0,7,"Not Available" },
{ 74,"Crimson",5,3,122,1,'rf00',5,10,0,0,0,0,0,0,0,0,0,24565,0,7,"+5-10% to Fire Resist" },
{ 75,"Burgundy",12,9,122,1,'rf00',11,20,0,0,0,0,0,0,0,0,0,24437,0,7,"+11-20% to Fire Resist" },
{ 76,"Burgundy",35,26,122,1,'rf00',11,20,0,0,0,0,0,0,0,0,0,130,0,7,"+11-20% to Fire Resist" },
{ 77,"Garnet",18,13,122,1,'rf00',21,30,0,0,0,0,0,0,0,0,0,23925,0,7,"+21-30% to Fire Resist" },
{ 78,"Garnet",55,41,122,3,'rf00',21,30,0,0,0,0,0,0,0,0,0,642,0,7,"+21-30% to Fire Resist" },
{ 79,"Russet",3,2,122,0,'rf00',25,25,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 80,"Ruby",25,18,122,1,'rf00',31,50,0,0,0,0,0,0,0,0,0,22880,1,7,"+31-50% to Fire Resist" },
{ 81,"Vermilion",7,5,122,0,'rf00',50,50,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 82,"Orange",7,5,123,0,'rl00',2,2,0,0,0,0,0,0,0,0,0,24576,0,13,"Not Available" },
{ 83,"Ocher",5,3,123,1,'rl00',5,10,0,0,0,0,0,0,0,0,0,24565,0,13,"+5-10% to Lightning Resist" },
{ 84,"Tangerine",12,9,123,1,'rl00',11,20,0,0,0,0,0,0,0,0,0,24437,0,13,"+11-20% to Lightning Resist" },
{ 85,"Tangerine",35,26,123,1,'rl00',11,20,0,0,0,0,0,0,0,0,0,130,0,13,"+11-20% to Lightning Resist" },
{ 86,"Coral",18,13,123,1,'rl00',21,30,0,0,0,0,0,0,0,0,0,23925,0,13,"+21-30% to Lightning Resist" },
{ 87,"Coral",55,41,123,1,'rl00',21,30,0,0,0,0,0,0,0,0,0,642,0,13,"+21-30% to Lightning Resist" },
{ 88,"Crackling",3,2,123,0,'rl00',25,25,'tm00',150,150,0,0,0,0,0,0,16384,0,3,"Not Available" },
{ 89,"Amber",25,18,123,1,'rl00',31,50,0,0,0,0,0,0,0,0,0,23905,1,13,"+31-50% to Lightning Resist" },
{ 90,"Forked",21,15,123,0,'rl00',50,50,'tm00',150,150,0,0,0,0,0,0,16384,1,3,"Not Available" },
{ 91,"Green",7,5,124,0,'rp00',2,2,0,0,0,0,0,0,0,0,0,8192,0,10,"Not Available" },
{ 92,"Beryl",5,3,124,1,'rp00',5,10,0,0,0,0,0,0,0,0,0,8149,0,10,"+5-10% to Poison Resist" },
{ 93,"Jade",12,9,124,1,'rp00',11,20,0,0,0,0,0,0,0,0,0,8021,0,10,"+11-20% to Poison Resist" },
{ 94,"Jade",35,26,124,1,'rp00',11,20,0,0,0,0,0,0,0,0,0,2210,0,10,"+11-20% to Poison Resist" },
{ 95,"Viridian",18,13,124,1,'rp00',21,30,0,0,0,0,0,0,0,0,0,6229,0,10,"+21-30% to Poison Resist" },
{ 96,"Viridian",55,41,124,1,'rp00',21,30,0,0,0,0,0,0,0,0,0,1954,0,10,"+21-30% to Poison Resist" },
{ 97,"Vital",11,8,124,0,'rp00',25,25,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 98,"Emerald",25,18,124,1,'rp00',31,50,0,0,0,0,0,0,0,0,0,6160,1,10,"+31-50% to Poison Resist" },
{ 99,"Enduring",14,10,124,0,'rp00',50,50,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 100,"Fletcher's",30,22,125,0,'sk00',1,1,0,0,0,0,0,0,0,0,0,4224,0,12,"+1 to Amazon Skill Levels" },
{ 101,"Archer's",40,30,125,0,'sk00',2,2,0,0,0,0,0,0,0,0,0,128,1,12,"+2 to Amazon Skill Levels" },
{ 102,"Archer's",90,67,125,0,'sk00',2,2,0,0,0,0,0,0,0,0,0,4096,1,12,"+2 to Amazon Skill Levels" },
{ 103,"Monk's",30,22,126,0,'sk01',1,1,0,0,0,0,0,0,0,0,0,4112,0,12,"+1 to Paladin Skill Levels" },
{ 104,"Priest's",40,30,126,0,'sk01',2,2,0,0,0,0,0,0,0,0,0,16,1,12,"+2 to Paladin Skill Levels" },
{ 105,"Priest's",90,67,126,0,'sk01',2,2,0,0,0,0,0,0,0,0,0,4096,1,12,"+2 to Paladin Skill Levels" },
{ 106,"Summoner's",30,22,127,0,'sk02',1,1,0,0,0,0,0,0,0,0,0,4128,0,12,"+1 to Necromancer Skill Levels" },
{ 107,"Necromancer's",40,30,127,0,'sk02',2,2,0,0,0,0,0,0,0,0,0,32,1,12,"+2 to Necromancer Skill Levels" },
{ 108,"Necromancer's",90,67,127,0,'sk02',2,2,0,0,0,0,0,0,0,0,0,4096,1,12,"+2 to Necromancer Skill Levels" },
{ 109,"Angel's",30,22,128,0,'sk03',1,1,0,0,0,0,0,0,0,0,0,20544,0,12,"+1 to Sorceress Skill Levels" },
{ 110,"Arch-Angel's",40,30,128,0,'sk03',2,2,0,0,0,0,0,0,0,0,0,64,1,12,"+2 to Sorceress Skill Levels" },
{ 111,"Arch-Angel's",90,67,128,0,'sk03',2,2,0,0,0,0,0,0,0,0,0,20480,1,12,"+2 to Sorceress Skill Levels" },
{ 112,"Slayer's",30,22,129,0,'sk04',1,1,0,0,0,0,0,0,0,0,0,4098,0,12,"+1 to Barbarian Skill Levels" },
{ 113,"Berserker's",40,30,129,0,'sk04',2,2,0,0,0,0,0,0,0,0,0,2,1,12,"+2 to Barbarian Skill Levels" },
{ 114,"Berserker's",90,67,129,0,'sk04',2,2,0,0,0,0,0,0,0,0,0,4096,1,12,"+2 to Barbarian Skill Levels" },
{ 115,"unused",9,6,130,0,'sk05',1,1,0,0,0,0,0,0,0,0,0,0,0,0,"Not Available" },
{ 116,"Kicking",3,2,131,1,'sp04',1,3,0,0,0,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 117,"Triumphant",3,2,132,0,'sp05',1,1,0,0,0,0,0,0,0,0,0,2162,0,6,"+1 to Life per Kill" },
{ 118,"Mighty",4,3,133,0,'st00',1,1,0,0,0,0,0,0,0,0,0,0,0,16,"Not Available" },
{ 119,"Energizing",3,2,133,0,'st00',20,20,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 120,"Strengthening",23,17,133,0,'st00',50,50,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 121,"Empowering",26,19,133,0,'st00',100,100,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 122,"Brisk",5,3,134,0,'ve01',10,10,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 123,"Swift",19,14,134,0,'ve02',20,20,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 124,"Tough",4,3,135,0,'mh00',10,10,0,0,0,0,0,0,0,0,0,0,0,16,"Not Available" },
{ 125,"Sturdy",11,8,135,0,'mh00',20,20,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 126,"Hardy",23,17,135,0,'mh00',50,50,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 127,"Robust",27,20,135,0,'mh00',100,100,'tm00',150,150,0,0,0,0,0,0,0,0,3,"Not Available" },
};
struct _MagicPreSuffix MagicSuffixTable[] =
{
{ 0,"of Health",7,5,1,0,'ar01',1,1,0,0,0,0,0,0,0,0,0,6149,0,5,"Damage Reduction by 1" },
{ 1,"of Protection",18,13,1,0,'ar01',2,2,0,0,0,0,0,0,0,0,0,6144,0,5,"Damage Reduction by 2" },
{ 2,"of Absorption",26,19,1,0,'ar01',3,3,0,0,0,0,0,0,0,0,0,4096,0,5,"Damage Reduction by 3" },
{ 3,"of Life",35,26,1,0,'ar01',4,4,0,0,0,0,0,0,0,0,0,12288,1,5,"Damage Reduction by 4" },
{ 4,"of Life",35,26,1,0,'ar01',4,4,0,0,0,0,0,0,0,0,0,0,1,5,"Damage Reduction by 4" },
{ 5,"of Warding",7,5,2,0,'ar03',1,1,0,0,0,0,0,0,0,0,0,14341,0,5,"Magic Damage Reduction by 1" },
{ 6,"of the Sentinel",18,13,2,0,'ar03',2,2,0,0,0,0,0,0,0,0,0,6144,0,5,"Magic Damage Reduction by 2" },
{ 7,"of Guarding",26,19,2,0,'ar03',3,3,0,0,0,0,0,0,0,0,0,4096,0,5,"Magic Damage Reduction by 3" },
{ 8,"of Negation",35,26,2,0,'ar03',4,4,0,0,0,0,0,0,0,0,0,4096,0,5,"Magic Damage Reduction by 4" },
{ 9,"unused",35,26,2,1,'ar03',5,6,0,0,0,0,0,0,0,0,0,0,1,5,"Not Available" },
{ 10,"of Piercing",25,18,3,0,'ar04',1,1,0,0,0,0,0,0,0,0,0,112,0,19,"Attack Ignores Target's Defense" },
{ 11,"of Bashing",16,12,4,1,'ar05',-25,-40,0,0,0,0,0,0,0,0,0,18,0,19,"-25-40 of Monster's Defense" },
{ 12,"of Puncturing",6,4,4,1,'ar05',-10,-20,0,0,0,0,0,0,0,0,0,18,0,19,"-10-20 of Monster's Defense" },
{ 13,"of Thorns",14,10,5,1,'at00',1,3,0,0,0,0,0,0,0,0,0,1029,0,19,"Attack Takes 1-4 Damage" },
{ 14,"of Spikes",21,15,5,1,'at00',2,6,0,0,0,0,0,0,0,0,0,1029,1,19,"Attack Takes 2-6 Damage" },
{ 15,"of Readiness",1,0,6,0,'at01',10,10,0,0,0,0,0,0,0,0,0,754,0,14,"Slightly Increased Weapon Speed" },
{ 16,"of Alacrity",8,6,6,0,'at02',20,20,0,0,0,0,0,0,0,0,0,242,0,14,"Increased Weapon Speed" },
{ 17,"of Swiftness",19,14,6,0,'at02',30,30,0,0,0,0,0,0,0,0,0,18,0,14,"Increased Weapon Speed" },
{ 18,"of Quickness",25,18,6,0,'at03',40,40,0,0,0,0,0,0,0,0,0,18,1,14,"Greatly Increased Weapon Speed" },
{ 19,"of Blocking",1,0,7,0,'bl00',10,10,0,0,0,0,0,0,0,0,0,4,0,14,"+10% Chance to Block" },
{ 20,"of Deflecting",11,8,7,0,'bl00',20,20,0,0,0,0,0,0,0,0,0,4,0,14,"+20% Chance to Block" },
{ 21,"of the Apprentice",5,3,8,0,'ca00',10,10,0,0,0,0,0,0,0,0,0,22640,0,14,"Fast Cast Time" },
{ 22,"of the Magus",17,12,8,0,'ca02',20,20,0,0,0,0,0,0,0,0,0,16496,0,14,"Fastest Cast Time" },
{ 23,"of Frost",4,3,9,1,'dc00',1,1,'dc01',1,4,'dc03',125,125,0,0,0,242,0,5,"+1-4 Cold Damage" },
{ 24,"of the Glacier",18,13,9,1,'dc00',4,4,'dc01',4,12,'dc03',150,150,0,0,0,114,0,5,"+4-12 Cold Damage" },
{ 25,"of Frost",55,41,9,1,'dc00',1,1,'dc01',1,4,'dc03',125,125,0,0,0,5120,0,5,"+1-4 Cold Damage" },
{ 26,"of Warmth/Thawing",10,7,10,0,'dc02',1,1,0,0,0,0,0,0,0,0,0,23300,0,4,"Freeze Length Reduction" },
{ 27,"of Flame",4,3,11,1,'df00',1,1,'df01',2,6,0,0,0,0,0,0,242,0,8,"+2-6 Fire Damage" },
{ 28,"of Fire",15,11,11,1,'df00',2,2,'df01',6,11,0,0,0,0,0,0,242,0,8,"+6-11 Fire Damage" },
{ 29,"of Burning",25,18,11,1,'df00',10,10,'df01',10,20,0,0,0,0,0,0,242,1,8,"+10-20 Fire Damage" },
{ 30,"of Flame",40,30,11,1,'df00',1,1,'df01',2,6,0,0,0,0,0,0,6656,0,8,"+2-6 Fire Damage" },
{ 31,"of Shock",4,3,12,0,'dl00',1,1,'dl01',8,8,0,0,0,0,0,0,242,0,14,"+1-8 Lightning Damage" },
{ 32,"of Lightning",15,11,12,0,'dl00',1,1,'dl01',16,16,0,0,0,0,0,0,242,0,14,"+1-16 Lightning Damage" },
{ 33,"of Thunder",25,18,12,0,'dl00',1,1,'dl01',32,32,0,0,0,0,0,0,242,1,14,"+1-32 Lightning Damage" },
{ 34,"of Shock",50,37,12,0,'dl00',1,1,'dl01',8,8,0,0,0,0,0,0,6402,0,14,"+1-8 Lightning Damage" },
{ 35,"of Craftsmanship",1,0,13,1,'dm00',1,2,0,0,0,0,0,0,0,0,0,6386,0,3,"+1-2 to Maximum Damage" },
{ 36,"of Quality",4,3,13,1,'dm00',2,2,0,0,0,0,0,0,0,0,0,242,0,3,"+2 to Maximum Damage" },
{ 37,"of Maiming",7,5,13,1,'dm00',3,4,0,0,0,0,0,0,0,0,0,242,0,3,"+3-4 to Maximum Damage" },
{ 38,"of Slaying",11,8,13,1,'dm00',5,7,0,0,0,0,0,0,0,0,0,242,0,3,"+5-7 to Maximum Damage" },
{ 39,"of Gore",14,10,13,1,'dm00',8,10,0,0,0,0,0,0,0,0,0,242,0,3,"+8-10 to Maximum Damage" },
{ 40,"of Carnage",19,14,13,1,'dm00',11,14,0,0,0,0,0,0,0,0,0,146,1,3,"+11-15 to Maximum Damage" },
{ 41,"of Slaughter",25,18,13,1,'dm00',15,20,0,0,0,0,0,0,0,0,0,146,1,3,"+15-20 to Maximum Damage" },
{ 42,"of Maiming",42,31,13,1,'dm00',3,4,0,0,0,0,0,0,0,0,0,6148,0,3,"+3-4 to Maximum Damage" },
{ 43,"of Worth",2,0,14,0,'dm01',1,1,0,0,0,0,0,0,0,0,0,8322,0,3,"+1 to Minimum Damage" },
{ 44,"of Measure",6,4,14,0,'dm01',2,2,0,0,0,0,0,0,0,0,0,242,0,3,"+2 to Minimum Damage" },
{ 45,"of Excellence",12,9,14,0,'dm01',3,3,0,0,0,0,0,0,0,0,0,6386,0,3,"+3 to Minimum Damage" },
{ 46,"of Performance",18,13,14,1,'dm01',4,5,0,0,0,0,0,0,0,0,0,242,1,3,"+4-5 to Minimum Damage" },
{ 47,"of Measure",37,27,14,0,'dm01',2,2,0,0,0,0,0,0,0,0,0,6145,0,3,"+2 to Minimum Damage" },
{ 48,"of Blight",5,3,15,0,'dp00',8,8,'dp01',24,24,'dp02',75,75,0,0,0,226,0,11,"+2-7 to Poison Damage" },
{ 49,"of Venom",15,11,15,0,'dp00',16,16,'dp01',48,48,'dp02',75,75,0,0,0,226,0,11,"+4-14 to Poison Damage" },
{ 50,"of Pestilence",25,18,15,0,'dp00',32,32,'dp01',72,72,'dp02',100,100,0,0,0,226,1,11,"+12-28 to Poison Damage" },
{ 51,"of Blight",45,33,15,0,'dp00',8,8,'dp01',24,24,'dp02',75,75,0,0,0,6144,0,11,"+2-7 to Poison Damage" },
{ 52,"of Dexterity",4,3,16,0,'dx00',1,1,0,0,0,0,0,0,0,0,0,14336,0,16,"+1-3 to Dexterity" },
{ 53,"of Dexterity",5,3,16,1,'dx00',1,3,0,0,0,0,0,0,0,0,0,7056,0,16,"+1-3 to Dexterity" },
{ 54,"of Skill",10,7,16,1,'dx00',4,6,0,0,0,0,0,0,0,0,0,7056,0,16,"+4-6 to Dexterity" },
{ 55,"of Skill",45,33,16,1,'dx00',4,6,0,0,0,0,0,0,0,0,0,1029,0,16,"+4-6 to Dexterity" },
{ 56,"of Accuracy",18,13,16,1,'dx00',7,10,0,0,0,0,0,0,0,0,0,7056,0,16,"+7-10 to Dexterity" },
{ 57,"of Precision",22,16,16,1,'dx00',11,15,0,0,0,0,0,0,0,0,0,4240,0,16,"+11-15 to Dexterity" },
{ 58,"of Precision",60,45,16,1,'dx00',11,15,0,0,0,0,0,0,0,0,0,3845,0,16,"+11-15 to Dexterity" },
{ 59,"of Perfection",30,22,16,1,'dx00',16,20,0,0,0,0,0,0,0,0,0,4736,1,16,"+16-20 to Dexterity" },
{ 60,"of Balance",5,3,17,0,'gh00',10,10,0,0,0,0,0,0,0,0,0,9477,0,14,"Fast Hit Recovery" },
{ 61,"of Stability",9,6,17,0,'gh02',20,20,0,0,0,0,0,0,0,0,0,1285,1,14,"Fastest Hit Recovery" },
{ 62,"of the Horse",1,0,18,0,'hl08',50,50,0,0,0,0,0,0,0,0,0,16384,0,15,"Not Available" },
{ 63,"of Regeneration",10,7,19,0,'lf00',3,3,0,0,0,0,0,0,0,0,0,7184,0,9,"+3 to Life Regeneration" },
{ 64,"of Regeneration",40,30,19,0,'lf00',3,3,0,0,0,0,0,0,0,0,0,516,0,9,"+3 to Life Regeneration" },
{ 65,"of Regeneration",70,52,19,0,'lf00',3,3,0,0,0,0,0,0,0,0,0,483,0,9,"+3 to Life Regeneration" },
{ 66,"of Regrowth",17,12,19,0,'lf00',5,5,0,0,0,0,0,0,0,0,0,4112,1,9,"+5 to Life Regeneration" },
{ 67,"of Regrowth",55,41,19,0,'lf00',5,5,0,0,0,0,0,0,0,0,0,3072,1,9,"+5 to Life Regeneration" },
{ 68,"of Vileness",9,6,20,0,'hl10',1,1,0,0,0,0,0,0,0,0,0,98,0,3,"Prevent Monster Healing" },
{ 69,"of the Ox",1,0,21,1,'hl12',3,5,0,0,0,0,0,0,0,0,0,0,0,2,"Not Available" },
{ 70,"of Greed",2,0,22,1,'ib00',40,60,0,0,0,0,0,0,0,0,0,6144,0,15,"40-60% More Gold" },
{ 71,"of Wealth",17,12,22,1,'ib00',80,120,0,0,0,0,0,0,0,0,0,5888,1,15,"80-120% More Gold" },
{ 72,"of Chance",12,9,23,1,'ib01',10,19,0,0,0,0,0,0,0,0,0,30720,0,15,"10-19% Better Chance of Getting Magic Item" },
{ 73,"of Fortune",16,12,23,1,'ib01',20,35,0,0,0,0,0,0,0,0,0,13056,1,15,"20-35% Better Chance of Getting Magic Item" },
{ 74,"of Energy",4,3,24,0,'mm00',1,1,0,0,0,0,0,0,0,0,0,30720,0,16,"+1-5 to Energy" },
{ 75,"of Energy",5,3,24,1,'mm00',1,5,0,0,0,0,0,0,0,0,0,22640,0,16,"+1-5 to Energy" },
{ 76,"of the Mind",10,7,24,1,'mm00',6,10,0,0,0,0,0,0,0,0,0,22640,0,16,"+6-10 to Energy" },
{ 77,"of Brilliance",18,13,24,1,'mm00',11,15,0,0,0,0,0,0,0,0,0,22640,0,16,"+11-15 to Energy" },
{ 78,"of Sorcery",22,16,24,1,'mm00',16,20,0,0,0,0,0,0,0,0,0,20592,1,16,"+16-20 to Energy" },
{ 79,"of Wizardry",30,22,24,1,'mm00',21,30,0,0,0,0,0,0,0,0,0,22640,1,16,"+21-30 to Energy" },
{ 80,"of the Bear",8,6,25,0,'ko00',1,1,0,0,0,0,0,0,0,0,0,114,0,19,"Monster Knockback" },
{ 81,"of Light",6,4,26,0,'lt00',1,1,0,0,0,0,0,0,0,0,0,32757,0,13,"+1 to Light Radius" },
{ 82,"of Radiance",15,11,26,0,'lt00',3,3,0,0,0,0,0,0,0,0,0,22768,0,13,"+2 to Light Radius" },
{ 83,"of the Sun",17,12,26,0,'lt00',5,5,0,0,0,0,0,0,0,0,0,31472,0,13,"+5 to Light Radius" },
{ 84,"of Life",23,17,27,0,'mh00',5,5,0,0,0,0,0,0,0,0,0,13317,0,9,"+5 to Life" },
{ 85,"of the Jackal",3,2,27,1,'mh00',1,5,0,0,0,0,0,0,0,0,0,16245,0,9,"+1-5 to Life" },
{ 86,"of the Fox",6,4,27,1,'mh00',5,10,0,0,0,0,0,0,0,0,0,8053,0,9,"+5-10 to Life" },
{ 87,"of the Wolf",14,10,27,1,'mh00',11,20,0,0,0,0,0,0,0,0,0,8053,0,9,"+11-20 to Life" },
{ 88,"of the Wolf",45,33,27,1,'mh00',11,20,0,0,0,0,0,0,0,0,0,130,0,9,"+11-20 to Life" },
{ 89,"of the Tiger",20,15,27,1,'mh00',21,30,0,0,0,0,0,0,0,0,0,7941,0,9,"+21-30 to Life" },
{ 90,"of the Mammoth",24,18,27,1,'mh00',31,40,0,0,0,0,0,0,0,0,0,6149,1,9,"+31-40 to Life" },
{ 91,"of the Mammoth",60,45,27,1,'mh00',31,40,0,0,0,0,0,0,0,0,0,1538,1,9,"+31-40 to Life" },
{ 92,"of the Colosuss",30,22,27,1,'mh00',41,60,0,0,0,0,0,0,0,0,0,13317,1,9,"+41-60 to Life" },
{ 93,"of the Leech",6,4,28,1,'mh03',4,7,0,0,0,0,0,0,0,0,0,6386,0,9,"+4-7% Life Stolen" },
{ 94,"of the Locust",20,15,28,1,'mh03',8,10,0,0,0,0,0,0,0,0,0,114,1,9,"+8-10% Life Stolen" },
{ 95,"of the Bat",6,4,29,1,'mm03',4,8,0,0,0,0,0,0,0,0,0,6386,0,6,"+4-8% Mana Stolen" },
{ 96,"of the Vampire",20,15,29,1,'mm03',9,12,0,0,0,0,0,0,0,0,0,114,1,6,"+8-10% Mana Stolen" },
{ 97,"of Defiance",25,18,30,0,'pd00',75,75,0,0,0,0,0,0,0,0,0,4101,1,8,"75% Poison Length Reduction" },
{ 98,"of Amelioration",18,13,30,0,'pd00',50,50,0,0,0,0,0,0,0,0,0,4101,0,8,"50% Poison Length Reduction" },
{ 99,"of Remedy",7,5,30,0,'pd00',25,25,0,0,0,0,0,0,0,0,0,7941,0,8,"25% Poison Length Reduction" },
{ 100,"of Ice",9,6,31,0,'rc00',2,2,0,0,0,0,0,0,0,0,0,0,0,5,"Not Available" },
{ 101,"of Simplicity",25,18,32,0,'rq00',-40,-40,0,0,0,0,0,0,0,0,0,135,0,0,"-40% Item Requirements" },
{ 102,"of Ease",15,11,32,0,'rq00',-20,-20,0,0,0,0,0,0,0,0,0,135,0,0,"-20% Item Requirements" },
{ 103,"of the Mule",3,2,33,1,'sp04',1,3,0,0,0,0,0,0,0,0,0,0,0,3,"Not Available" },
{ 104,"of Strength",5,3,34,1,'st00',1,3,0,0,0,0,0,0,0,0,0,7168,0,16,"+1-3 to Strength" },
{ 105,"of Might",10,7,34,1,'st00',4,6,0,0,0,0,0,0,0,0,0,14358,0,16,"+4-6 to Strength" },
{ 106,"of the Ox",18,13,34,1,'st00',7,10,0,0,0,0,0,0,0,0,0,7186,0,16,"+7-10 to Strength" },
{ 107,"of the Ox",47,35,34,1,'st00',7,10,0,0,0,0,0,0,0,0,0,772,0,16,"+7-10 to Strength" },
{ 108,"of the Giant",22,16,34,1,'st00',11,15,0,0,0,0,0,0,0,0,0,5138,1,16,"+11-15 to Strength" },
{ 109,"of the Giant",62,46,34,1,'st00',11,15,0,0,0,0,0,0,0,0,0,2820,1,16,"+11-15 to Strength" },
{ 110,"of the Titan",30,22,34,1,'st00',16,20,0,0,0,0,0,0,0,0,0,5376,1,16,"+16-20 to Strength" },
{ 111,"of Pacing",2,0,35,0,'ve00',10,10,0,0,0,0,0,0,0,0,0,256,0,14,"Fast Walking/Running" },
{ 112,"of Haste",12,9,35,0,'ve01',20,20,0,0,0,0,0,0,0,0,0,256,0,14,"Faster Walking/Running" },
{ 113,"of Speed",22,16,35,0,'ve02',30,30,0,0,0,0,0,0,0,0,0,256,0,14,"Fastest Walking/Running" },
};
int nMagicPrefixTable = sizeof MagicPrefixTable / sizeof MagicPrefixTable[0];
int nMagicSuffixTable = sizeof MagicSuffixTable / sizeof MagicSuffixTable[0];