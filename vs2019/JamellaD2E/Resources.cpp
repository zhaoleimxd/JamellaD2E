// File.cpp from D2E
#include "JamellaD2E.h"
// Resources
HICON hIconHelp;
// Progress Bar Macros
inline void ProgressText(const char* text)
{
	SetDlgItemText(hTabDialog, IDC_PROGRESS_Text, text);
}
inline void ProgressBarRange(int min, int max)
{
	SendDlgItemMessage(hTabDialog, IDC_PROGRESS_Bar, PBM_SETRANGE, 0, MAKELPARAM(min, max));
}
inline void ProgressBarSet(int i)
{
	SendDlgItemMessage(hTabDialog, IDC_PROGRESS_Bar, PBM_SETPOS, i, 0);
}
inline void ProgressBarSetStep(int i)
{
	SendDlgItemMessage(hTabDialog, IDC_PROGRESS_Bar, PBM_SETSTEP, i, 0);
}
inline void ProgressBarStep()
{
	SendDlgItemMessage(hTabDialog, IDC_PROGRESS_Bar, PBM_STEPIT, 0, 0);
	PollMessages();
}
HMENU hBatchMenu;
// Bitmaps
struct
{
	int Type;
	int ID;
	HANDLE* Handle;
}
rc[] =
{
 { IMAGE_BITMAP, IDB_JAMELLA, (HANDLE*)&hBmpJamella },
 { IMAGE_BITMAP, IDB_PLUS, (HANDLE*)&hBmpPlus },
 { IMAGE_BITMAP, IDB_WAYPOINT_ON, (HANDLE*)&hBmpWaypointOn },
 { IMAGE_BITMAP, IDB_WAYPOINT_OFF, (HANDLE*)&hBmpWaypointOff },
 { IMAGE_BITMAP, IDB_ITEM_UNKNOWN, (HANDLE*)&itemunknown.hBmp },
 { IMAGE_BITMAP, IDB_NOTPLACEABLE, (HANDLE*)&hBmpNotPlaceable },
 { IMAGE_BITMAP, IDB_INV_WHOLE, (HANDLE*)&hBmpBodyWhole },
 { IMAGE_BITMAP, IDB_WEBLINK, (HANDLE*)&hBmpWebLink },
 { IMAGE_ICON, IDI_CHELP, (HANDLE*)&hIconHelp },
 { IMAGE_CURSOR, IDC_CUR_MOVE, (HANDLE*)&hCurMove },
 { IMAGE_CURSOR, IDC_CUR_MOVECOPY, (HANDLE*)&hCurMoveCopy },
 { IMAGE_CURSOR, IDC_CUR_CROSS, (HANDLE*)&hCurCross },
 { IMAGE_CURSOR, IDC_CUR_ADD, (HANDLE*)&hCurAdd },
 { IMAGE_CURSOR, IDC_CUR_NO, (HANDLE*)&hCurNo },
};
void D2ELoadResources()
{
	ProgressBarSetStep(1);
	int ImgNum = 0;
	ImgNum += 30 * 5;
	ImgNum += 21;
#if PRELOADITEMIMAGES == 1
	ImgNum += nItemInfos;
#endif
	ImgNum += 5 + 3;
	ImgNum += sizeof rc / sizeof rc[0];
	ProgressBarRange(0, ImgNum);
	ProgressText("Loading Skill Bitmaps...");
	for (int z = 0; z < 30 * 5; z++)
	{
		skills[z].hbitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(skills[z].idbitmap));
		ProgressBarStep();
	}
	ProgressText("Loading Quest Bitmaps...");
	for (int z = 0; z < 21; z++)
	{
		quests[z].hbitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(quests[z].idbitmap));
		if (!quests[z].hbitmap)
		{
			ErrorMessage();
			exit(0);
		}
		LoadString(hInstance, quests[z].idstring, quests[z].string, 60);
		ProgressBarStep();
	}
	ProgressText("Loading Item Bitmaps...");
#if PRELOADITEMIMAGES == 1
	for (z = 0; z < nItemInfos; z++)
	{
		if (ItemInfos[z].BitmapID)
		{
			ItemInfos[z].hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(ItemInfos[z].BitmapID));

			if (ItemInfos[z].hBmp)
			{
				BITMAP bmpinfo;
				if (GetObject(ItemInfos[z].hBmp, sizeof bmpinfo, &bmpinfo) == 0)
				{
					ErrorMessage();
					exit(0);
				}

				if (bmpinfo.bmWidth != ItemInfos[z].SizeX * 29 - 1 ||
					bmpinfo.bmHeight != ItemInfos[z].SizeY * 29 - 1)
				{
					sprintf(buffer, "Resource Error [%i|%i]\nID = %i", bmpinfo.bmWidth, bmpinfo.bmHeight, ItemInfos[z].Bitma
						pID);
					MessageBox(hTabDialog, buffer, PROGRAMNAME,
						MB_OK | MB_ICONSTOP | MB_APPLMODAL);
					exit(0);
				}
			}
			else
			{
				ErrorMessage();
				exit(0);
			}
		}
		ProgressBarStep();
	}
#endif
	for (int z = 0; z < 5; z++)
	{
		RingImages[z].hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(RingImages[z].BmpID));
		if (!RingImages[z].hBmp)
		{
			ErrorMessage();
			exit(0);
		}
		ProgressBarStep();
	}
	for (int z = 0; z < 3; z++)
	{
		AmuletImages[z].hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(AmuletImages[z].BmpID));
		if (!AmuletImages[z].hBmp)
		{
			ErrorMessage();
			exit(0);
		}
		ProgressBarStep();
	}
	ProgressText("Loading Miscellaneous Resources...");
	for (int z = 0; z < sizeof rc / sizeof rc[0]; z++)
	{
		*rc[z].Handle = LoadImage(hInstance, MAKEINTRESOURCE(rc[z].ID), rc[z].Type, 0, 0, LR_DEFAULTCOLOR | LR_SHARED);
		if (!*rc[z].Handle)
		{
			ErrorMessage();
			exit(0);
		}
		ProgressBarStep();
	}
	hBatchMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_BATCH));
	hBrushBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);
	hBrushNull = (HBRUSH)GetStockObject(NULL_BRUSH);
	hPenWhite = CreatePen(PS_SOLID, 1, RGB(240, 240, 240));
	hPenGreen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	hPenBusy = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	hPenUnwearable = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
}
/*
 // Imagelist for ItemTree
 if ((hTVImagelist = ImageList_Create(16,16,ILC_COLORDDB,3,0)) == NULL) return false;

 HDC hdcbitmap = CreateCompatibleDC(NULL);
 HDC hdcbitmaps = CreateCompatibleDC(NULL);

 HBITMAP hbmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ITEM_1170_S_00));
 SelectObject(hdcbitmap, hbmp);
 HBITMAP hsbmp = CreateCompatibleBitmap(hdcbitmap,16,16);
 SelectObject(hdcbitmaps, hsbmp);

 if(!StretchBlt(hdcbitmaps,0,0,16,16,hdcbitmap,0,0,28,28,SRCCOPY)) OutputDebugString("FUCK\n");
 DeleteDC(hdcbitmap);
 DeleteDC(hdcbitmaps);
 BITMAP info;
 GetObject(hsbmp,sizeof(info),&info);
 ImageList_Add(hTVImagelist,hsbmp,NULL);
 DeleteObject(hbmp);
 DeleteObject(hsbmp);
*/
void D2EUnLoadResources()
{
	for (int z = 0; z < 30 * 5; z++)
	{
		if (skills[z].hbitmap)
			DeleteObject(skills[z].hbitmap);
	}
	for (int z = 0; z < 21; z++)
	{
		if (quests[z].hbitmap)
			DeleteObject(quests[z].hbitmap);
	}
	for (int z = 0; z < nItemInfos; z++)
	{
		if (ItemInfos[z].hBmp)
			DeleteObject(ItemInfos[z].hBmp);
	}
	for (int z = 0; z < 5; z++)
	{
		if (RingImages[z].hBmp)
			DeleteObject(RingImages[z].hBmp);
	}
	for (int z = 0; z < 3; z++)
	{
		if (AmuletImages[z].hBmp)
			DeleteObject(AmuletImages[z].hBmp);
	}
	for (int z = 0; z < sizeof rc / sizeof rc[0]; z++)
	{
		if (rc[z].Handle)
			DeleteObject(*rc[z].Handle);
	}

	DestroyMenu(hBatchMenu);
	DeleteObject(hBrushBlack);
	DeleteObject(hPenWhite);
	DeleteObject(hPenGreen);
	DeleteObject(hPenBusy);
	DeleteObject(hPenUnwearable);
}