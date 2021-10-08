// D2E.h Diablo 2 Editor Header File
/*
* Central Configurations for all Projects
*/
#define PUB 0
#define DEV 1
#define DEVPUB 2
#define RELEASE DEV
#define RELEASECODE "PIPER"
#define JEPRELOADITEMIMAGES 0
#if RELEASE == PUB
#define JEVERSION "Version 3.0"
#define BUGGYMESSAGES 1
#elif RELEASE == DEVPUB
#define JEVERSION "Public Development Version"
#define BUGGYMESSAGES 1
#else
#define VERSION "Development Version"
#define BUGGYMESSAGES 0
#endif
#if _DEBUG
#define SHOWMAGICCODE 1
#else
#define SHOWMAGICCODE 0
#endif
#if defined(JAMELLAEDITOR)
#define PROGRAMNAME "Jamella's Diablo 2 Hero Editor"
#define _WIN32_IE 0x0500
#define IDT_TIMER 12345
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <shlobj.h>
#include <richedit.h>
// Resource Identifiers
#include "ResourceIDs.h"
#else if defined(JAMELLACOMSERVER)
#define PROGRAMNAME "Jamella's COM Object"
#define COMPROGID "Jamella.Server"
#define COMVERID "Jamella.Server.3"
#define COMDESCRIPTION "Jamella Diablo 2 COM Server"
// Includes
#include <windows.h>
#include <ole2.h>
#include <commctrl.h>
#include <stdio.h>
#include <shlobj.h>
// Resource Identifiers
#include "ResourceIDs.h"
#endif
#define REGISTRYKEY "Software\\Jamella's Diablo 2 Editor"
#define D2SPROGID "JamellaD2Editor.Character"
#define D2IPROGID "JamellaD2Editor.Item"
#define D2SDESCRIPTION "Diablo II Character"
#define D2IDESCRIPTION "Diablo II Item"
#define PERMANENTURL "http://jamella.cjb.net"
#define EMAIL "jamella@gmx.net"
#define HTMLHELPFILE "JamellaHelp.chm"
#define CLIPBOARDFORMAT "JamellaD2EItem30"
#define WM_VALIDATE WM_USER
#define SEARCHPOLLER 250
#define EXPERTBOXPOLLER 500
#define DECODETIMEOUT 1000000
#define PROGRESSCOLOR RGB(200,0,0)
#define HISTORYSTEPS 32
#define MAXMODLEVEL 99
#define BFASPEEDPAST 10
#define JEINVGRIDS 1
// Windowing Data
extern HINSTANCE hInstance;
extern HWND hMainDialog, hTabDialog;
// Structures
struct Damage
{
	int Hands;
	int OneHandMin, OneHandMax;
	int TwoHandMin, TwoHandMax;
	bool Magic;
};
struct ArmorClass
{
	int BaseAC;
	int AC;
	bool Magic;
};
// MainDlg.cpp
struct maindlgtab
{
	int iddialog;
	int idicon;
	char* text;
	void* proc;
	DLGTEMPLATE* dialogtemplate;
	int imagelistindex;
};
// ToolTips
extern HWND hToolTip;
extern HWND CreateToolTipCtrl(HWND hParent, int ToolSet, bool Relay);
// TextSummary
extern void WriteTextSummary(HWND);
// General Resources
extern HBITMAP hBmpPlus;
extern HICON hIconHelp;
extern HMENU hBatchMenu;
extern void D2ELoadResources();
extern void D2EUnLoadResources();
// Tab0.cpp
extern HBITMAP hBmpJamella;
// Tab1.cpp
extern const char* CharClasses[5];
extern const char* Difficulties[4];
// Item Editor Commons
extern class Item* SelItem;
extern struct ItemInfo* SelInfo;
// Tab2.cpp
extern HBITMAP hBmpBodyWhole, hBmpNotPlaceable;
extern HCURSOR hCurMove, hCurMoveCopy, hCurCross, hCurNo;
extern HIMAGELIST hTVImagelist;
extern HBRUSH hBrushBlack, hBrushNull;
extern HPEN hPenWhite, hPenGreen, hPenBusy, hPenUnwearable;
extern void UpdateTab2();
// Tab2E.cpp
extern HWND hExpertBox;
extern void UpdateTab2E();
// Tab2Random.cpp
extern HWND hRandomBox;
extern int RandomBoxDW;
extern void UpdateTab2Rnd();
// Tab2ItemList.cpp
extern HWND hItemListBox;
extern void UpdateTab2ItemList();
// Tab2Rare.cpp
extern HCURSOR hCurAdd;
// Tab2ItemFileLoadSave
extern bool LoadItemFile(HWND hWnd);
extern bool SaveItemFile(HWND hWnd);
extern Item* MakeItemFromData(BYTE* data, int size);
extern Item* MakeItemFromFile(HANDLE hFile);
// Tab3.cpp
struct skill
{
	int ifoffset;
	char* name;
	int idstring;;
	int idbitmap;
	HBITMAP hbitmap;
};
extern struct skill skills[5 * 30];
// Tab4.cpp
struct queststatus
{
	char* text;
	WORD value;
};
struct quest
{
	int idstring;
	int idbitmap;
	queststatus* stati;
	int offset;
	HANDLE hbitmap;
	char string[64];
};
extern struct quest quests[21];
// Tab5.cpp
extern HBITMAP hBmpWaypointOn;
extern HBITMAP hBmpWaypointOff;
// Infobox.cpp
extern HBITMAP hBmpWebLink;
extern void RTFStreamSend(HWND hWnd, int Ctrl, const char* Stream);
// Item Grid Managment Sizes
struct InvGrids
{
	int xInventory, yInventory;
	int xStash, yStash;
	int xCube, yCube;
	int xBelt, yBelt;
};
extern struct InvGrids InvGrids;
extern void SetInvGridPreset(int i);
// Items Structure
struct ItemInfo
{
	char* ItemName;
	char Quality;
	char* BaseItemName;
	char* ItemType;
	DWORD ItemCode;
	int UniqueCode;
	int DWBCode;
	char ItemSearch;
	DWORD IC;
	bool Sm;
	int BitmapID;
	int TreeID;
	int MagicMask;
	int RareMask;
	int QualityMask;
	int SizeX, SizeY;
	int BodyPlace;
	char Magic;
	int Durability;
	DWORD MinStr;
	DWORD MinDex;
	DWORD MinLvl;
	int Hands;
	int OneHandDmgMin;
	int OneHandDmgMax;
	int TwoHandDmgMin;
	int TwoHandDmgMax;
	int MissileDmgMin;
	int MissileDmgMax;
	int UndeadBonus;
	int ACMin;
	int ACMax;
	int Sockets;
	char GemClass;
	int Quantity;
	char* Description;
	// Zero initialized
	HBITMAP hBmp;
	HTREEITEM hTree;
};
extern struct ItemInfo ItemInfos[];
extern int nItemInfos;
extern struct ItemInfo itemunknown;
extern struct ItemInfo* SelInfo;
// Effect Structure
struct Effect
{
	DWORD Code;
	int Min;
	int Max;
};
// Ring and Amulet Structures
struct ImageMap
{
	int BmpID;
	HBITMAP hBmp;
};
extern struct ImageMap RingImages[];
extern struct ImageMap AmuletImages[];
// Item Tree Structures
struct _ItemTree
{
	int Depth;
	char* Text;
	int TreeID;
	HTREEITEM hTree;
};
extern struct _ItemTree ItemTree[];
extern int nItemTree;
// Magic Modifiers Tables
struct _MagicPreSuffix
{
	int N;
	char* Text;
	int ModLevel;
	int ELevel;
	int Group;
	int nMod;
	Effect Mod[4];
	int MagicMask;
	int Transform;
	int Transformcolor;
	char* Description;
};
extern struct _MagicPreSuffix MagicPrefixTable[], MagicSuffixTable[];
extern int nMagicPrefixTable, nMagicSuffixTable;
struct Modifier
{
	DWORD Code;
	char* Text;
};
extern const struct Modifier Modifiers[];
extern int nModifiers;
// Superior Items Table
struct _SuperiorItem
{
	int X;
	int nMods;
	Effect Mod[2];
	int SuperiorMask;
	int Level;
	int Multiply;
	int Add;
};
extern const struct _SuperiorItem SuperiorItemTable[];
extern int nSuperiorItemTable;
// Magical Items PreSuffix Tree
struct _MagicPreSuffixTree
{
	int Depth;
	char* Text;
	int ModID;
	HTREEITEM hTree;
};
extern struct _MagicPreSuffixTree MagicPreSuffixTree[];
extern int nMagicPreSuffixTree;
// Rare Item PreSuffix Table
struct _RarePreSuffix
{
	char* Text;
	int RareMask;
};
extern const struct _RarePreSuffix RarePrefixTable[], RareSuffixTable[];
extern int nRarePrefixTable, nRareSuffixTable;
// Unique Item Attributes Table
struct _UniqueItem
{
	char* Name;
	int nMod;
	Effect Mod[7];
};
extern const struct _UniqueItem UniqueItems[];
extern int nUniqueItems;
// Set Item Attributes Table
struct _SetItem
{
	char* SetName;
	int SetID;
	char* Prefix;
	int Level;
	int nItems;
	int nProperties;
	int Transform;
	int Transformcolor;
	struct
	{
		DWORD Code;
		DWORD IC;
		char* Suffix;
	}
	Item[6];

	Effect Mod[16];
};
extern const struct _SetItem SetItems[];
extern int nSetItems;
struct GemInfo
{
	char* Name;
	DWORD ItemCode;
	DWORD IC;
	int Transform;
	int nMods;

	Effect WeaponMod[3];
	Effect HelmMod[3];
	Effect ShieldMod[3];
};
extern const struct GemInfo GemInfos[];
extern int nGemInfos;
// Popup Help Structure
struct PopupHelp
{
	int CtrlID;
	int HelpID;
	const char* HelpText;
};
extern const struct PopupHelp PopupHelps[];
extern int nPopupHelps;
// Main.cpp
int MainDialog(const char* CmdLine);
int ErrorMessage();
// HelpBox.cpp
bool ToggleHelpBox(HWND hWnd, int HELPID);
void CloseHelpBox();
// CommandLine.cpp
extern void ParseCommandLine();
extern const char* CmdLineFile();
extern const char* ProgramFilePath();
// Registry.cpp
struct _RegOptions
{
	int CreateItemRecordFormat;
	int AllItemsSocketable;
	int A7Gems;
	int ExceedQuantity;
	char ItemPath[260];
	int Associations;
	int ToolTips;
	int NoAnnoyingMsgs;
};
extern void CheckShellRegistry();
extern void SaveEditorRegistryValues();
extern void LoadEditorRegistryValues();
extern _RegOptions RegOptions;
LRESULT CALLBACK Tab0DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab1DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2InventoryProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2EDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2RndDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2MagicDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2RareDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2SearchDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2GemsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab3DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab4DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab5DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CowLevelDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InfoDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SaveDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RenameDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UOptionsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EOptionsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Tab2ItemListDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#if INVGRIDS > 0
LRESULT CALLBACK Tab2ExGridDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#pragma pack(1)
class fileclass
{
private:
	char filename[260];
	// Private Structures
	struct
	{
		BYTE W4[4]; // '01 77 34 00'
		BYTE unknown[48];
	} w4;
	struct
	{
		BYTE JM[2]; // 'JM'
		WORD num; // number of items
	} JMinv;

public:
	fileclass();
	~fileclass();
	bool loaded;
	// File operations
	void clear();
	bool loadfile(HWND hWnd, const char* filename);
	bool savefile(HWND hWnd);
	bool reloadfile(HWND hWnd);
	bool discardfile(HWND hWnd);
	inline bool isloaded() { return loaded; }
	int transferdata(BYTE* data, int size);
	void setfilename(const char* file);
	// Public Structures
	struct
	{
		BYTE signatur[8]; // '55 AA 55 AA 47 00 00 00'
		char playername[16];
		BYTE hardcore;
		BYTE diff;
		WORD unknown1[4]; // '00 00 DD 00 10 00 82 00'
		WORD playerclass;
		WORD level;
		BYTE unknown2[50];
		BYTE startingtown;
		BYTE unknown3[41];
	} Header;
	struct
	{
		BYTE Woo[8]; // '57 6F 6F 21 06 00 00 00'
		WORD unknown1; // '2A 01'
		WORD quests1[48];
		WORD quests2[48];
		WORD quests3[48];
	} Woo;
	struct
	{
		BYTE WS[8]; // '57 53 01 00 00 00 50 00'
		WORD unknown1; // '02 01'
		DWORD waypoints1;
		BYTE unused1[18];
		WORD unknown2; // '02 01'
		DWORD waypoints2;
		BYTE unused2[18];
		WORD unknown3; // '02 01'
		DWORD waypoints3;
		BYTE unused3[18];
	} WS;
	struct
	{
		BYTE gf[2]; // 'gf'
		WORD fields; // Bit Field: '7 6 5 4:3 2 1 0|7 6 5 4:3 2 1 0'
		BYTE null1; // Fields: 5 4 F E
		DWORD strength; // 0
		DWORD energy; // 1
		DWORD dexterity; // 2
		DWORD vitality; // 3
		DWORD statsbonus; // 4
		DWORD skillbonus; // 5
		DWORD health; // 6
		DWORD healthmax; // 7
		DWORD mana; // 8
		DWORD manamax; // 9
		DWORD stamina; // A
		DWORD staminamax; // B
		DWORD level; // C
		DWORD experience; // D
		DWORD goldperson; // E
		DWORD goldstash; // F
	} gf;
	struct
	{
		BYTE _if[2]; // 'if'
		BYTE skills[30];
	} IF;
	struct JMitem* JMinvitems;
	int JMinvnum;
};
enum { CNT_NONE, CNT_INVENTORY, CNT_BODY, CNT_STASH, CNT_CUBE, CNT_BELT, CNT_SOCKET, CNT_COPYBUFFER };
enum {
	CRUDEITEM = 0x02,
	USUALITEM = 0x04,
	SUPERIORITEM = 0x06,
	MAGICITEM = 0x08,
	SETITEM = 0x0A,
	RAREITEM = 0x0C,
	UNIQUEITEM = 0x0E
};
enum {
	DE_NONE = 0,

	DE_MAGIC_PREFIX_MODULO_ZERO,
	DE_MAGIC_PREFIX_MODIFIER_VALMISSING,
	DE_MAGIC_SUFFIX_MODULO_ZERO,
	DE_MAGIC_SUFFIX_MODIFIER_VALMISSING,

	DE_RARE_NAMEPREFIX_MODULO_ZERO,
	DE_RARE_NAMESUFFIX_MODULO_ZERO,
	DE_RARE_PRESUFFIX_MODULO_ZERO,
	DE_RARE_PRESUFFIX_NOPOSSIBLE,
	DE_RARE_PRESUFFIX_MODIFIER_VALMISSING,

	DE_UNIQUE_ITEMCODE,
	DE_SET_ITEMCODE,
};
const char* DecodeErrorString(int Error);
#define PREFIX 0
#define SUFFIX 1
class MagicDecoder
{
public:
	bool Quick;
	Item* I;
	// Item magic decoding data is buffered for maximum speed
	DWORD ItemBuffered;
	DWORD RareItemCodeBuffered;
	int ItemBufferedModLevel;
	// Magical Attributes Buffering
	int nPrefixBuffer, nSuffixBuffer;
	_MagicPreSuffix
		** PrefixBuffer, ** SuffixBuffer;
	void BuildMagicBuffers();
	// Rare Name Attributes Buffering
	int nRarePrefixBuffer, nRareSuffixBuffer;
	const _RarePreSuffix
		** RarePrefixBuffer, ** RareSuffixBuffer;
	void BuildRareBuffers();
	BYTE ZeroMemoryStart;
	// Pointers to Magical Attributes with Magnitude
	// Crude Item Prefix
	const char* CrudePrefix;
	// Magical Items
	int modMagicPrefix, modMagicSuffix;
	int modpickMagicPrefix, modpickMagicSuffix;
	_MagicPreSuffix* MagicPrefix, * MagicSuffix;
	int MagicPrefixMag[4];
	int MagicSuffixMag[4];
	// Rare Items
	const _RarePreSuffix
		* RarePrefix, * RareSuffix;
	// Set & Unique Items
	int nRareFix;
	bool tRareFix[6];
	_MagicPreSuffix
		* RareFix[6];
	int RareFixMag[6][4];
	const _UniqueItem
		* UniqueItem;
	const _SetItem
		* SetItem;
	int SetItemNum;
	int UniqueSetMag[16];
	BYTE ZeroMemoryEnd;
	// Decoding Procs
	void DecodeCrude();
	void DecodeMagical();
	void DecodeRare();
	void DecodeUnique();
	void DecodeSet();
	void CollectModifiers();
	static char NameTmp[256];
	static char AttrTmp[2048];
public:
	MagicDecoder(Item* I);
	~MagicDecoder();
	int DecodeError;
	bool Decode();
	bool QuickDecode();
	// Collected Modifiers for public
	int nMods;
	struct
	{
		DWORD Code;
		int Mag;
	}
	Mod[32];
	const char* Name();
	const char* RichAttributes();
};
class DWHistory
{
private:
	int ML[HISTORYSTEPS];
	DWORD DWA[HISTORYSTEPS];
	DWORD DWB[HISTORYSTEPS];
	int Top, Bottom, Ptr;
public:
	DWHistory();
	~DWHistory();
	bool isNext();
	bool isBack();
	void StepBack(Item* I);
	void StepNext(Item* I);
	void StepAdd(Item* I);
};
// Item Record IDs
#define IT_BASE 0x0000
#define IT_103 0x1030
#define IT_103EAR 0x1031
#define IT_104EX 0x1040
#define IT_104SM 0x1041
#define IT_104EAR 0x1042
// Base Class
class Item
{
protected:
	// Chained list pointers
	Item* ListNext;
	Item* ListPrev;
public:
	// States
	bool Busy;
	// Constructor & Destructor
	Item();
	virtual ~Item();
	// Chained List Functions
	inline Item* Next()
	{
		return ListNext;
	}
	inline Item* Prev()
	{
		return ListPrev;
	}
	int Count();
	Item* GetOrdinal(int i);
	void Attach(Item* i);
	void Delete();
	// The following virtual functions are filled in derived classes
	// with the apropriate functions. Here only the most basic properties
	// are defined.
	// Item Data Loading and Saving virtual
	virtual bool LoadItemRecord(BYTE* data);
	virtual BYTE* GetItemRecord();
	virtual DWORD ItemRecordLength();
	virtual void BlankItem();
	virtual int ItemRecordID() { return IT_BASE; }
	virtual const char* ItemRecordName() { return "Base Class"; }
	// Coordinates & Size virtuals
	virtual int Container() const;
	virtual int xPos() const; // logical 0-9
	virtual int yPos() const; // logical 0-9
	int xPixelPos() const;
	int yPixelPos() const;
	virtual int xSize(); // logical 1-4
	virtual int ySize(); // logical 1-4
	int xPixelSize();
	int yPixelSize();
	bool isInRegion(int x, int y);
	virtual bool SetCoordinates(int Container, int xPos, int yPos);
	virtual int BodyCode();
	int BodyPlace();
	// ItemInfo handling and creation
	ItemInfo* Info;
	virtual bool FindInfo();
	virtual HBITMAP GetBitmap();
	// Virtual Simple Item Properties
	virtual DWORD ItemCode();
	virtual void SetItemCode(DWORD x);
	virtual int UniqueCode();
	virtual void SetUniqueCode(int x);
	virtual int Quality();
	virtual void SetQuality(int x);
	virtual int Quantity();
	virtual void SetQuantity(int i);
	virtual int Durability();
	virtual void SetDurability(int x);
	virtual int DurabilityMax();
	virtual void SetDurabilityMax(int x);
	virtual DWORD DWA();
	virtual void SetDWA(DWORD x);
	virtual DWORD DWB();
	virtual void SetDWB(DWORD x);
	DWORD DWARandomOffset(int x);
	DWORD DWBRandomOffset(int x);
	virtual int MagicLevel();
	virtual void SetMagicLevel(int x);
	virtual int GemNum();
	virtual void SetGemNum(int i);
	// Virtual Item Property Flags
	virtual bool Socketed();
	virtual void SetSocketed(bool i);
	bool Socketable();
	virtual bool Identified();
	virtual void SetIdentified(bool i);
	virtual bool Starter();
	virtual void SetStarter(bool i);
	// Gemmed Items
	Item* Gems; // Chained List of Socketed Gems
	const GemInfo* GInfo; // Info about Gem if this is one
	virtual bool FindGemInfo();
	// Ear-specific Properties
	virtual int OpponentClass();
	virtual void SetOpponentClass(int x);
	virtual const char* OpponentClassString();
	virtual int OpponentLevel();
	virtual void SetOpponentLevel(int x);
	virtual const char* OpponentName();
	virtual void SetOpponentName(const char* s);
	// Virtual Computed Item Properties
	virtual int BaseDefense();
	virtual ArmorClass Defense();
	virtual Damage WeaponDamage();
	virtual unsigned int RequiredStrength();
	virtual unsigned int RequiredDexterity();
	virtual unsigned int RequiredELevel();
	virtual bool isWearable();
	bool Decoded;
	virtual bool Decode();
	class MagicDecoder* MD;
	virtual const char* Name();
	virtual const char* RichText();
	DWHistory DWBHistory;
protected:
	static char NameTmp[256];
	static char RichTextTmp[2048];
	void StartRTF(char* s);
};
class ItemMg : public Item
{
public:
	// Computed Item Properties
	virtual int BaseDefense();
	virtual ArmorClass Defense();
	virtual Damage WeaponDamage();
	virtual unsigned int RequiredStrength();
	virtual unsigned int RequiredDexterity();
	virtual unsigned int RequiredELevel();
	virtual bool Decode();
	virtual const char* Name();
	virtual const char* RichText();
};
// Item Structure from 1.03
class Item103 : public ItemMg
{
private:
	struct _JMItem
	{
		BYTE JM[2]; // 'JM'
		WORD bitfield1; // contains socket and identification flags
		WORD bitfield2; // contains starting item flag
		BYTE bitfield3; // contains gem info
		BYTE modlevel;
		WORD itemcode;
		WORD quantity;
		BYTE quantityex;
		WORD durability;
		BYTE coordinates;
		BYTE specialitemcode;
		DWORD properties1;
		DWORD properties2;
		WORD container;
	} Data;
public:
	Item103();
	~Item103();
	// Item Data Loading and Saving virtual
	bool LoadItemRecord(BYTE* data);
	BYTE* GetItemRecord();
	DWORD ItemRecordLength();
	void BlankItem();
	int ItemRecordID() { return IT_103; }
	const char* ItemRecordName() { return "v1.03 Record"; }
	// Coordinates & Size
	int Container() const;
	int xPos() const; // logical 0-9
	int yPos() const; // logical 0-9
	int xSize(); // logical 1-4
	int ySize(); // logical 1-4
	int BodyCode();
	bool SetCoordinates(int Container, int xPos, int yPos);
	// Iteminfo functions
	bool FindInfo();
	HBITMAP GetBitmap();
	// Item Properties
	DWORD ItemCode();
	void SetItemCode(DWORD x);
	int UniqueCode();
	void SetUniqueCode(int x);
	int Quality();
	void SetQuality(int x);
	int Quantity();
	void SetQuantity(int i);
	int GemNum();
	void SetGemNum(int i);
	int Durability();
	void SetDurability(int x);
	int DurabilityMax();
	void SetDurabilityMax(int x);
	// Image and Defence Specifier
	DWORD DWA();
	void SetDWA(DWORD x);
	// Magical Double Word & Magic Mod Level
	DWORD DWB();
	void SetDWB(DWORD x);
	int MagicLevel();
	void SetMagicLevel(int x);
	// Flags
	bool Socketed();
	void SetSocketed(bool i);
	bool Socketable();
	bool Identified();
	void SetIdentified(bool i);
	bool Starter();
	void SetStarter(bool i);
	// Gem Functions
	bool FindGemInfo();
};
// Ear Item Structure from 1.03
class Item103Ear : public Item
{
private:
	struct _JMItemEar
	{
		BYTE JM[2];
		WORD bitfield1;
		WORD bitfield2;
		WORD itemcode;
		WORD coordinates;
		BYTE container;
		BYTE string[16];
	} Data;
public:
	Item103Ear();
	~Item103Ear();
	// Item Data Loading and Saving virtual
	bool LoadItemRecord(BYTE* data);
	BYTE* GetItemRecord();
	DWORD ItemRecordLength();
	void BlankItem();
	int ItemRecordID() { return IT_103EAR; }
	const char* ItemRecordName() { return "v1.03 Ear Record"; }
	// Coordinates & Size
	int Container() const;
	int xPos() const; // logical 0-9
	int yPos() const; // logical 0-9
	int xSize(); // logical 1-4
	int ySize(); // logical 1-4
	bool SetCoordinates(int Container, int xPos, int yPos);
	// Iteminfo functions
	bool FindInfo();
	HBITMAP GetBitmap();
	// Item Properties
	DWORD ItemCode();
	void SetItemCode(DWORD x);
	// Ear-specific Properties
	int OpponentClass();
	void SetOpponentClass(int x);
	const char* OpponentClassString();
	int OpponentLevel();
	void SetOpponentLevel(int x);
	char OpponentNameChar(int n);
	void SetOpponentNameChar(int n, char c);
	const char* OpponentName();
	void SetOpponentName(const char* s);
	// Computed Properties
	const char* Name();
	const char* RichText();
};
class Item104Ex : public ItemMg
{
private:
	struct _JMItem
	{
		BYTE JM[2]; // 'JM'
		WORD bitfield1;
		WORD bitfield2; // contains starting item flag
		BYTE bitfield3;
		DWORD itemcode;
		BYTE bodycode;
		WORD magicrestr;
		WORD quantity;
		BYTE quantityex;
		WORD durability;
		BYTE coordinates;
		BYTE uniquecode;
		DWORD DWA;
		DWORD DWB;
		WORD container;
	} Data;
public:
	Item104Ex();
	~Item104Ex();
	// Item Data Loading and Saving virtual
	bool LoadItemRecord(BYTE* data);
	BYTE* GetItemRecord();
	DWORD ItemRecordLength();
	void BlankItem();
	int ItemRecordID() { return IT_104EX; }
	const char* ItemRecordName() { return "v1.04 Extended Record"; }
	// Coordinates & Size
	int Container() const;
	int xPos() const; // logical 0-9
	int yPos() const; // logical 0-9
	int xSize(); // logical 1-4
	int ySize(); // logical 1-4
	int BodyCode();
	bool SetCoordinates(int Container, int xPos, int yPos);
	// Iteminfo functions
	bool FindInfo();
	void CreateItem(ItemInfo* info);
	HBITMAP GetBitmap();
	// Item Properties
	DWORD ItemCode();
	void SetItemCode(DWORD x);
	int UniqueCode();
	void SetUniqueCode(int x);
	int Quality();
	void SetQuality(int x);
	int Quantity();
	void SetQuantity(int i);
	int GemNum();
	void SetGemNum(int i);
	int Durability();
	void SetDurability(int x);
	int DurabilityMax();
	void SetDurabilityMax(int x);
	// Image and Defence Specifier
	DWORD DWA();
	void SetDWA(DWORD x);
	// Magical Double Word & Magic Mod Level
	DWORD DWB();
	void SetDWB(DWORD x);
	int MagicLevel();
	void SetMagicLevel(int x);
	// Flags
	bool Socketed();
	void SetSocketed(bool i);
	bool Socketable();
	bool Identified();
	void SetIdentified(bool i);
	bool Starter();
	void SetStarter(bool i);
};
class Item104Sm : public Item
{
private:
	struct _JMItem
	{
		BYTE JM[2]; // 'JM'
		WORD bitfield1;
		WORD bitfield2; // contains starting item flag
		WORD bitfield3;
		WORD coordinates;
		DWORD itemcode;
		BYTE zero;
	} Data;
public:
	Item104Sm();
	~Item104Sm();
	// Item Data Loading and Saving virtual
	bool LoadItemRecord(BYTE* data);
	BYTE* GetItemRecord();
	DWORD ItemRecordLength();
	void BlankItem();
	int ItemRecordID() { return IT_104SM; }
	const char* ItemRecordName() { return "v1.04 Simple Record"; }
	// Coordinates & Size
	int Container() const;
	int xPos() const; // logical 0-9
	int yPos() const; // logical 0-9
	int xSize(); // logical 1-4
	int ySize(); // logical 1-4
	bool SetCoordinates(int Container, int xPos, int yPos);
	// Iteminfo functions
	bool FindInfo();
	HBITMAP GetBitmap();
	// Gem Functions
	bool FindGemInfo();
	// Item Properties
	DWORD ItemCode();
	void SetItemCode(DWORD x);
	const char* RichText();
};
// Ear Item Structure from 1.04
class Item104Ear : public Item
{
private:
	struct _JMItemEar
	{
		BYTE JM[2];
		WORD bitfield1;
		WORD bitfield2;
		WORD itemcode;
		WORD coordinates;
		BYTE string[16];
	} Data;
public:
	Item104Ear();
	~Item104Ear();
	// Item Data Loading and Saving virtual
	bool LoadItemRecord(BYTE* data);
	BYTE* GetItemRecord();
	DWORD ItemRecordLength();
	void BlankItem();
	int ItemRecordID() { return IT_104EAR; }
	const char* ItemRecordName() { return "v1.04 Ear Record"; }
	// Coordinates & Size
	int Container() const;
	int xPos() const; // logical 0-9
	int yPos() const; // logical 0-9
	int xSize(); // logical 1-4
	int ySize(); // logical 1-4
	bool SetCoordinates(int Container, int xPos, int yPos);
	// Iteminfo functions
	bool FindInfo();
	HBITMAP GetBitmap();
	// Item Properties
	DWORD ItemCode();
	void SetItemCode(DWORD x);
	// Ear-specific Properties
	int OpponentClass();
	void SetOpponentClass(int x);
	const char* OpponentClassString();
	int OpponentLevel();
	void SetOpponentLevel(int x);
	char OpponentNameChar(int n);
	void SetOpponentNameChar(int n, char c);
	const char* OpponentName();
	void SetOpponentName(const char* s);
	// Computed Properties
	const char* Name();
	const char* RichText();
};
#pragma pack(4)
extern class fileclass fc;
extern class Item* Items;
extern class Item* CopyBuffer;
extern HBITMAP ItemInfoGetBitmap(ItemInfo* Info);
extern Item* CreateItem(Item** IChain, ItemInfo* Info);
extern char buffer[256];
// D2Decode.cpp
struct RAND
{
	DWORD Seed;
	DWORD Carry;
};
inline DWORD Random(RAND* rnd)
{
	DWORDLONG x = rnd->Seed;
	x *= 0x6AC690C5;
	x += rnd->Carry;
	rnd->Seed = DWORD(x);
	rnd->Carry = DWORD(x >> 32);
	return rnd->Seed;
}
extern int StartRandoms(Item* I, RAND* r);
#pragma pack(1)
#define TAB2COMMON \
 bool Running; \
 DWORD Counter; \
 HANDLE Thread; \
 HGLOBAL ThreadData; \
 HWND Dialog; \
 Item* Item; \
 bool Advanced; \
 bool TraverseMagicLevels; \
 bool HitsSelection; \
 SearchHit* Hits; \
 DWORD Past[BFASPEEDPAST];

// Tab2 Search Threads
struct SearchHit
{
	int MagicLevel;
	int DWA;
	int DWB;
	SearchHit* List;
};
struct SearchThread
{
	TAB2COMMON
};
struct MagicSearchThread
{
	TAB2COMMON
		bool PrefixMatch;
	_MagicPreSuffix* Prefix;
	bool SuffixMatch;
	_MagicPreSuffix* Suffix;
	bool ForcePrefixMatch[4];
	int ForcePrefixValue[4];
	bool ForceSuffixMatch[1];
	int ForceSuffixValue[1];
};
struct RareSearchThread
{
	TAB2COMMON
		int NamePrefix, NameSuffix;
	_MagicPreSuffix* Attribute[6];
	int iAttribute[6];
	int tAttribute[6];
};
struct DefenseSearchThread
{
	TAB2COMMON
		int SelectAC;
};
struct RingAmuletSearchThread
{
	TAB2COMMON
		bool RingAmulet; // Ring == false; Amulet == true;
	DWORD Image;
};
#pragma pack(4)
// Macro Functions
inline void PollMessages()
{
	MSG msg;
	while (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
inline bool CheckPollMessages()
{
	MSG msg;
	while (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return false;
}
inline void ErrorBox(const char* error, HWND hWnd = hMainDialog)
{
	MessageBox(hWnd, error, PROGRAMNAME, MB_OK | MB_ICONHAND | MB_APPLMODAL);
}
#if DEVELOPMENT == 1
#define AssertBox(str) ErrorBox(str)
#else
#define AssertBox(str)
#endif
inline const PopupHelp* FindPopupHelp(int ID)
{
	for (int z = 0; z < nPopupHelps; z++)
	{
		if (PopupHelps[z].CtrlID == ID)
			return &PopupHelps[z];
	}
	return 0;
}
// Info Macros
inline int FindItemInfoByItemCode(DWORD ItemCode)
{
	for (int z = 0; z < nItemInfos; z++)
	{
		if (ItemInfos[z].ItemCode == ItemCode)
			return z;
	}
	return -1;
}
inline int FindMagicPreSuffixByPointer(bool PreSuff, const _MagicPreSuffix* p)
{
	if (PreSuff == PREFIX) {
		for (int z = 0; z < nMagicPrefixTable; z++)
		{
			if (&MagicPrefixTable[z] == p)
				return z;
		}
		return -1;
	}
	else {
		for (int z = 0; z < nMagicSuffixTable; z++)
		{
			if (&MagicSuffixTable[z] == p)
				return z | 256;
		}
		return -1;
	}
}
inline int FindRareNamePrefixByPointer(const _RarePreSuffix* p)
{
	for (int z = 0; z < nRarePrefixTable; z++)
	{
		if (&RarePrefixTable[z] == p)
			return z;
	}
	return -1;
}
inline int FindRareNameSuffixByPointer(const _RarePreSuffix* p)
{
	for (int z = 0; z < nRareSuffixTable; z++)
	{
		if (&RareSuffixTable[z] == p)
			return z;
	}
	return -1;
}
inline const char* GetEffect(DWORD code)
{
	if (code == 0) return "Zero String error";
	for (int z = 0; z < nModifiers; z++)
	{
		if (Modifiers[z].Code == code && Modifiers[z].Text)
			return Modifiers[z].Text;
	}
	return "Unknown Effect";
}
extern char CodeStringTemp[5];
inline const char* CodeString(DWORD code)
{
	CodeStringTemp[0] = char((code & 0x000000FF) >> 0);
	CodeStringTemp[1] = char((code & 0x0000FF00) >> 8);
	CodeStringTemp[2] = char((code & 0x00FF0000) >> 16);
	CodeStringTemp[3] = char((code & 0xFF000000) >> 24);
	CodeStringTemp[4] = 0;
	return CodeStringTemp;
}
inline const char* CodeStringRev(DWORD code)
{
	CodeStringTemp[0] = char((code & 0xFF000000) >> 24);
	CodeStringTemp[1] = char((code & 0x00FF0000) >> 16);
	CodeStringTemp[2] = char((code & 0x0000FF00) >> 8);
	CodeStringTemp[3] = char((code & 0x000000FF) >> 0);
	CodeStringTemp[4] = 0;
	return CodeStringTemp;
}
// Diablo 2 Directory Key
inline bool GetDiabloSaveDirectory(char* dest)
{
	HKEY regkey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Blizzard Entertainment\\Diablo II", 0, KEY_READ, &regkey) == ERROR_SUCCESS)
	{
		DWORD type = REG_SZ;
		DWORD fdirsize = 260;
		DWORD x = RegQueryValueEx(regkey, "Save Path", 0, &type, (unsigned char*)dest, &fdirsize);
		if (x != ERROR_SUCCESS)
			return false;
		RegCloseKey(regkey);
		return true;
	}
	return false;
}
// strcat Multiple
inline void strmcat(char* d, ...)
{
	va_list args;
	va_start(args, d);
	char* s;
	while (s = va_arg(args, char*))
		strcat(d, s);
	va_end(args);
}
// ASCIItoRTF
inline void ASCIItoRTF(char* d, const char* s)
{
	while (*s)
	{
		if (*s == '\n') {
			*d++ = '\\';
			*d++ = 'p';
			*d++ = 'a';
			*d++ = 'r';
			*d++ = ' ';
			s++;
			if (*s == '\r') s++;
		}
		else
			*d++ = *s++;
	}
	*d = 0;
}
// Item Class Duplicator
inline Item* ItemDuplicate(Item* I)
{
	switch (I->ItemRecordID())
	{
	default: return new Item;
	case IT_103: return new Item103;
	case IT_103EAR: return new Item103Ear;
	case IT_104EX: return new Item104Ex;
	case IT_104SM: return new Item104Sm;
	case IT_104EAR: return new Item104Ear;
	}
}