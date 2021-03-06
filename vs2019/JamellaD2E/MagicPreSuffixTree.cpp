#include "JamellaD2E.h"
struct _MagicPreSuffixTree MagicPreSuffixTree[] =
{
{ 1,"+ Attack Rating",-1 },
{ 2,"Bronze (10-20)",26 },
{ 2,"Iron (21-40)",27 },
{ 2,"Steel (41-60)",28 },
{ 2,"Silver (61-80)",29 },
{ 2,"Silver (61-80)",30 },
{ 2,"Gold (81-100)",31 },
{ 2,"Platinum (101-120)",32 },
{ 2,"Meteoric (121-150)",33 },
{ 1,"+ Damage",-1 },
{ 2,"Jagged (110-120%)",12 },
{ 2,"Deadly (121-130%)",13 },
{ 2,"Vicious (131-140%)",14 },
{ 2,"Brutal (141-150%)",15 },
{ 2,"Massive (151-165%)",16 },
{ 2,"Savage (166-180%)",17 },
{ 2,"Merciless (181-200%)",18 },
{ 1,"+ Attack Rating & + Damage",-1 },
{ 2,"Sharp (10-20 & 110-120%)",34 },
{ 2,"Fine (21-40 & 121-130%)",35 },
{ 2,"Warrior's (41-60 & 131-140%)",36 },
{ 2,"Soldier's (61-80 & 141-150%)",37 },
{ 2,"Knight's (81-100 & 151-165%)",38 },
{ 2,"Lord's (101-120 & 166-180%)",39 },
{ 2,"King's (121-150 & 181-200%)",40 },
{ 1,"+ Defense",-1 },
{ 2,"Sturdy (120-130%)",1 },
{ 2,"Strong (131-140%)",2 },
{ 2,"Glorious (141-150%)",3 },
{ 2,"Blessed (151-160%)",4 },
{ 2,"Saintly (161-180%)",5 },
{ 2,"Holy (181-200%)",6 },
{ 1,"- Magic Damage",-1 },
{ 2,"Devious (Reduced by 1)",7 },
{ 2,"Fortified (Reduced by 2)",8 },
{ 1,"+ Mana",-1 },
{ 2,"Lizard's (+1-5)",52 },
{ 2,"Snake's (+5-10)",54 },
{ 2,"Serpent's (+11-20)",55 },
{ 2,"Serpent's (+11-20)",56 },
{ 2,"Drake's (+21-30)",57 },
{ 2,"Dragon's (+31-40)",58 },
{ 2,"Dragon's (+31-40)",59 },
{ 2,"Wyrm's (+41-60)",60 },
{ 1,"+ Light Radius",-1 },
{ 2,"Glimmering (+1)",48 },
{ 2,"Glowing (+2)",49 },
{ 1,"+ Fire Resist",-1 },
{ 2,"Crimson (5-10%)",74 },
{ 2,"Burgundy (11-20%)",75 },
{ 2,"Burgundy (11-20%)",76 },
{ 2,"Garnet (21-30%)",77 },
{ 2,"Garnet (21-30%)",78 },
{ 2,"Ruby (31-50%)",80 },
{ 1,"+ Cold Resist",-1 },
{ 2,"Azure (5-10%)",65 },
{ 2,"Lapis (11-20%)",66 },
{ 2,"Lapis (11-20%)",67 },
{ 2,"Cobalt (21-30%)",68 },
{ 2,"Cobalt (21-30%)",69 },
{ 2,"Sapphire (31-50%)",71 },
{ 1,"+ Lightning Resist",-1 },
{ 2,"Ocher (5-10%)",83 },
{ 2,"Tangerine (11-20%)",84 },
{ 2,"Tangerine (11-20%)",85 },
{ 2,"Coral (21-30%)",86 },
{ 2,"Coral (21-30%)",87 },
{ 2,"Amber (31-50%)",89 },
{ 1,"+ Poison Resist",-1 },
{ 2,"Beryl (5-10%)",92 },
{ 2,"Jade (11-20%)",93 },
{ 2,"Jade (11-20%)",94 },
{ 2,"Viridian (21-30%)",95 },
{ 2,"Viridian (21-30%)",96 },
{ 2,"Emerald (31-50%)",98 },
{ 1,"+ Resist All",-1 },
{ 2,"Prismatic (15-25%)",63 },
{ 2,"Prismatic (15%)",64 },
{ 1,"+ Skill Levels",-1 },
{ 2,"Fletcher's (+1 to Amazon Skill Levels)",100 },
{ 2,"Archer's (+2 to Amazon Skill Levels)",101 },
{ 2,"Archer's (+2 to Amazon Skill Levels)",102 },
{ 2,"Slayer's (+1 to Barbarian Skill Levels)",112 },
{ 2,"Berserker's (+2 to Barbarian Skill Levels)",113 },
{ 2,"Berserker's (+2 to Barbarian Skill Levels)",114 },
{ 2,"Monk's (+1 to Paladin Skill Levels)",103 },
{ 2,"Priest's (+2 to Paladin Skill Levels)",104 },
{ 2,"Priest's (+2 to Paladin Skill Levels)",105 },
{ 2,"Summoner's (+1 to Necromancer Skill Levels)",106 },
{ 2,"Necromancer's (+2 to Necromancer Skill Levels)",107 },
{ 2,"Necromancer's (+2 to Necromancer Skill Levels)",108 },
{ 2,"Angel's (+1 to Sorceress Skill Levels)",109 },
{ 2,"Arch-Angel's (+2 to Sorceress Skill Levels)",110 },
{ 2,"Arch-Angel's (+2 to Sorceress Skill Levels)",111 },
{ 1,"Damage Generates Mana",-1 },
{ 2,"Vulpine (10%)",19 },
{ 1,"Each Kill Regenerates Mana",-1 },
{ 2,"Triumphant (+1 per Kill)",117 },
{ 1,"Better Chance of Getting Magic Items",-1 },
{ 2,"Fortuitous (10-15%)",42 },
{ 1,"+ Max Stamina",-1 },
{ 2,"Rugged (+5-10)",25 },
{ 1,"+ Stamina Regeneration",-1 },
{ 2,"Tireless",24 },
{ 1,"Make Monsters Flee",-1 },
{ 2,"Howling",41 },
{ 1,"+ Strength",-1 },
{ 2,"of Strength (+1-3)",360 },
{ 2,"of Might (+4-6)",361 },
{ 2,"of the Ox (+7-10)",362 },
{ 2,"of the Ox (+7-10)",363 },
{ 2,"of the Giant (+11-15)",364 },
{ 2,"of the Giant (+11-15)",365 },
{ 2,"of the Titan (+16-20)",366 },
{ 1,"+ Dexterity",-1 },
{ 2,"of Dexterity (+1-3)",308 },
{ 2,"of Dexterity (+1-3)",309 },
{ 2,"of Skill (+4-6)",310 },
{ 2,"of Skill (+4-6)",311 },
{ 2,"of Accuracy (+7-10)",312 },
{ 2,"of Precision (+11-15)",313 },
{ 2,"of Precision (+11-15)",314 },
{ 2,"of Perfection (+16-20)",315 },
{ 1,"+ Energy",-1 },
{ 2,"of Energy (+1-5)",330 },
{ 2,"of Energy (+1-5)",331 },
{ 2,"of Mind (+6-10)",332 },
{ 2,"of Brilliance (+11-15)",333 },
{ 2,"of Sorcery (+16-20)",334 },
{ 2,"of Wizardy (+21-30)",335 },
{ 1,"+ Life",-1 },
{ 2,"of Life (+5)",340 },
{ 2,"of the Jackal (+1-5)",341 },
{ 2,"of the Fox (+5-10)",342 },
{ 2,"of the Wolf (+11-20)",343 },
{ 2,"of the Wolf (+11-20)",344 },
{ 2,"of the Tiger (+21-30)",345 },
{ 2,"of the Mammoth (+31-40)",346 },
{ 2,"of the Mammoth (+31-40)",347 },
{ 2,"of the Colosuss (+41-60)",348 },
{ 1,"+ Light Radius",-1 },
{ 2,"of Light (+1)",337 },
{ 2,"of Radiance (+3)",338 },
{ 2,"of the Sun (+5)",339 },
{ 1,"+ Minimum Damage",-1 },
{ 2,"of Worth (+1)",299 },
{ 2,"of Measure (+2)",300 },
{ 2,"of Measure (+2)",303 },
{ 2,"of Excellence (+3)",301 },
{ 2,"of Performance (+4-5)",302 },
{ 1,"+ Maximum Damage",-1 },
{ 2,"of Craftsmanship (+1-2)",291 },
{ 2,"of Quality (+2)",292 },
{ 2,"of Maiming (+3-4)",293 },
{ 2,"of Maiming (+3-4)",298 },
{ 2,"of Slaying (+5-7)",294 },
{ 2,"of Gore (+8-10)",295 },
{ 2,"of Carnage (+11-15)",296 },
{ 2,"of Slaughter (+15-20)",297 },
{ 1,"+ Fire Damage",-1 },
{ 2,"of Flame (2-6)",283 },
{ 2,"of Flame (2-6)",286 },
{ 2,"of Fire (6-11)",284 },
{ 2,"of Burning (10-20)",285 },
{ 1,"+ Cold Damage",-1 },
{ 2,"of Frost (1-4)",279 },
{ 2,"of Frost (1-4)",281 },
{ 2,"of the Glacier (4-12)",280 },
{ 1,"+ Lightning Damage",-1 },
{ 2,"of Shock (1-8)",287 },
{ 2,"of Shock (1-8)",290 },
{ 2,"of Lightning (1-16)",288 },
{ 2,"of Thunder (1-32)",289 },
{ 1,"+ Poison Damage",-1 },
{ 2,"of Blight (2-7)",304 },
{ 2,"of Blight (2-7)",307 },
{ 2,"of Venom (4-14)",305 },
{ 2,"of Pestilence (12-28)",306 },
{ 1,"Poison Length Reduction",-1 },
{ 2,"of Remedy (25%)",355 },
{ 2,"of Amelioration (50%)",354 },
{ 2,"of Defiance (75%)",353 },
{ 1,"+ Life Stolen",-1 },
{ 2,"of the Leech (4-7%)",349 },
{ 2,"of the Locust (8-10%)",350 },
{ 1,"+ Mana Stolen",-1 },
{ 2,"of the Bat (4-8%)",351 },
{ 2,"of the Vampire (8-10%)",352 },
{ 1,"+ Life Regeneration",-1 },
{ 2,"of Regeneration (+3)",319 },
{ 2,"of Regeneration (+3)",320 },
{ 2,"of Regeneration (+3)",321 },
{ 2,"of Regrowth (+5)",322 },
{ 2,"of Regrowth (+5)",323 },
{ 1,"+ Weapon Speeds",-1 },
{ 2,"of Readiness (Slight Increase)",271 },
{ 2,"of Alacrity (Increase)",272 },
{ 2,"of Swiftness (Increase)",273 },
{ 2,"of Quickness (Greater Increase)",274 },
{ 1,"+ Chance to Block",-1 },
{ 2,"of Blocking (+10%)",275 },
{ 2,"of Deflecting (+20%)",276 },
{ 1,"Faster Hit Recovery",-1 },
{ 2,"of Balance (Fast)",316 },
{ 2,"of Stability (Fastest)",317 },
{ 1,"Damage Reduction",-1 },
{ 2,"of Health (by 1)",256 },
{ 2,"of Protection (by 2)",257 },
{ 2,"of Absorption (by 3)",258 },
{ 2,"of Life (by 4)",259 },
{ 2,"of Life (by 4)",260 },
{ 1,"Magic Damage Reduction",-1 },
{ 2,"of Warding (by 1)",261 },
{ 2,"of the Sentinel (by 2)",263 },
{ 2,"of Guarding (by 3)",262 },
{ 2,"of Negation (by 4)",264 },
{ 1,"More Gold",-1 },
{ 2,"of Greed (40-60%)",326 },
{ 2,"of Wealth (80-120%)",327 },
{ 1,"Better Chance of Getting Magic Item",-1 },
{ 2,"of Chance (10-19%)",328 },
{ 2,"of Fortune (20-35%)",329 },
{ 1,"Monster Defense",-1 },
{ 2,"of Puncturing (-10-20)",268 },
{ 2,"of Bashing (-25-40)",267 },
{ 1,"Cast Time",-1 },
{ 2,"of the Apprentice (Fast)",277 },
{ 2,"of the Magus (Fastest)",278 },
{ 1,"Attacker Takes Damage",-1 },
{ 2,"of Thorns (1-6)",269 },
{ 2,"of Spikes (2-6)",270 },
{ 1,"- Item Requirements",-1 },
{ 2,"of Ease (-20%)",358 },
{ 2,"of Simplicity (-40%)",357 },
{ 1,"Faster Walking/Running",-1 },
{ 2,"of Pacing (Fast)",367 },
{ 2,"of Haste (Faster)",368 },
{ 2,"of Speed (Fastest)",369 },
{ 1,"Monster Knockback",-1 },
{ 2,"of the Bear",336 },
{ 1,"Attack Ignores Target's Defense",-1 },
{ 2,"of Piercing",266 },
{ 1,"Prevent Moster Healing",-1 },
{ 2,"of Vileness",324 },
{ 1,"Freeze Length Reduction",-1 },
{ 2,"of Warmth/Thawing",282 },
};
//int nMagicPrefixTree = sizeof MagicPrefixTree / sizeof MagicPrefixTree[0];
//int nMagicSuffixTree = sizeof MagicSuffixTree / sizeof MagicSuffixTree[0];
int nMagicPreSuffixTree = sizeof MagicPreSuffixTree / sizeof MagicPreSuffixTree[0];