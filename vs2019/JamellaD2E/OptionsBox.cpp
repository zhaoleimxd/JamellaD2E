// OptionsBox.cpp from D2E
#include "JamellaD2E.h"
static struct
{
	bool Expert;
	int DlgID;
	int* Value;
}
CheckBoxes[] =
{
 { 0, IDC_UOPTIONS_Associations, &RegOptions.Associations },
 { 0, IDC_UOPTIONS_ExceedQuantity, &RegOptions.ExceedQuantity },
 { 0, IDC_UOPTIONS_Tooltips, &RegOptions.ToolTips },
 { 0, IDC_UOPTIONS_AnnoyingMsgs, &RegOptions.NoAnnoyingMsgs },
 { 1, IDC_UOPTIONS_AllSocktable, &RegOptions.AllItemsSocketable },
 { 1, IDC_EOPTIONS_7Gems, &RegOptions.A7Gems },
};
LRESULT CALLBACK UOptionsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		for (int u = 0; u < sizeof CheckBoxes / sizeof CheckBoxes[0]; u++)
		{
			if (CheckBoxes[u].Expert) continue;
			CheckDlgButton(hWnd, CheckBoxes[u].DlgID,
				*CheckBoxes[u].Value ? BST_CHECKED : BST_UNCHECKED);
		}
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			for (int u = 0; u < sizeof CheckBoxes / sizeof CheckBoxes[0]; u++)
			{
				if (CheckBoxes[u].Expert) continue;

				if (IsDlgButtonChecked(hWnd, CheckBoxes[u].DlgID) == BST_CHECKED)
					*CheckBoxes[u].Value = true;
				else
					*CheckBoxes[u].Value = false;
			}
			SaveEditorRegistryValues();
		}
		EndDialog(hWnd, IDOK);
		return true;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, wParam);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}
LRESULT CALLBACK EOptionsDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		CheckRadioButton(hWnd, IDC_OPTIONS_IFormat1, IDC_OPTIONS_IFormat3,
			IDC_OPTIONS_IFormat1 + RegOptions.CreateItemRecordFormat);
		CheckDlgButton(hWnd, IDC_UOPTIONS_AllSocktable,
			RegOptions.AllItemsSocketable ? BST_CHECKED : BST_UNCHECKED);
	}
	return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (IsDlgButtonChecked(hWnd, IDC_UOPTIONS_AllSocktable) == BST_CHECKED)
				RegOptions.AllItemsSocketable = true;
			else
				RegOptions.AllItemsSocketable = false;
			RegOptions.CreateItemRecordFormat = 0;
			if (IsDlgButtonChecked(hWnd, IDC_OPTIONS_IFormat2) == BST_CHECKED)
				RegOptions.CreateItemRecordFormat = 1;
			if (IsDlgButtonChecked(hWnd, IDC_OPTIONS_IFormat3) == BST_CHECKED)
				RegOptions.CreateItemRecordFormat = 2;
			SaveEditorRegistryValues();
		}
		EndDialog(hWnd, IDOK);
		return true;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			return true;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWnd, wParam);
		return true;
	case WM_DESTROY:
		return false;
	}
	return false;
}