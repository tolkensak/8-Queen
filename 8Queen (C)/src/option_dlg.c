
#include "stdafx.h"
#include "resource.h"
#include "preview.h"


// Global Variables
extern COLORREF _crBoardWhite;
extern COLORREF _crBoardBlack;
extern COLORREF _crBackground;


COLORREF _crBoardWhiteTemp=0;
COLORREF _crBoardBlackTemp=0;
COLORREF _crBackgroundTemp=0;


// Forward declarations of functions included in this code module
BOOL OptionDlg_OnInitDialog(HWND, WPARAM, LPARAM);
BOOL OptionDlg_OnCommand(HWND, WPARAM, LPARAM);
void OptionDlg_OnEndDialog(HWND, UINT);
void OptionDlg_OnClickChkBackground(HWND hDlg);


LRESULT CALLBACK OptionDlg_WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		return OptionDlg_OnInitDialog(hDlg, wParam, lParam);

	case WM_COMMAND:
		return OptionDlg_OnCommand(hDlg, wParam, lParam);
	}

	return FALSE;
}

BOOL OptionDlg_OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	HWND hWndPreview;

	_crBoardWhiteTemp=_crBoardWhite;
	_crBoardBlackTemp=_crBoardBlack;
	_crBackgroundTemp=_crBackground;

	hWndPreview=GetDlgItem(hDlg, IDC_STA_PREVIEW);
	_fnOldPreviewProc=(WNDPROC)(LONG_PTR)SetWindowLong(hWndPreview, GWLP_WNDPROC, (LONG)(LONG_PTR)Preview_WndProc);

	SendDlgItemMessage(hDlg, IDC_CHK_BACKGROUND, BM_SETCHECK, _crBackgroundTemp>0xffffff?BST_CHECKED:BST_UNCHECKED, 0);

	Wnd_MoveToCenter0(hDlg);

	return TRUE;
}

BOOL OptionDlg_OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case IDOK:
	case IDCANCEL:
		OptionDlg_OnEndDialog(hDlg, LOWORD(wParam));
		return TRUE;

	case IDC_CHK_BACKGROUND:
		OptionDlg_OnClickChkBackground(hDlg);
		return TRUE;
	}

	return FALSE;
}

void OptionDlg_OnEndDialog(HWND hDlg, UINT nID)
{
	HWND hWndPreview=GetDlgItem(hDlg, IDC_STA_PREVIEW);
	SetWindowLong(hWndPreview, GWL_WNDPROC, (LONG)(LONG_PTR)_fnOldPreviewProc);

	if(nID==IDOK)
	{
		_crBoardWhite=_crBoardWhiteTemp;
		_crBoardBlack=_crBoardBlackTemp;
		_crBackground=_crBackgroundTemp;
	}

	EndDialog(hDlg, nID);
}

void OptionDlg_OnClickChkBackground(HWND hDlg)
{
	if(SendDlgItemMessage(hDlg, IDC_CHK_BACKGROUND, BM_GETCHECK, 0, 0)==BST_CHECKED)
		_crBackgroundTemp|=0x1000000;
	else
		_crBackgroundTemp&=0xffffff;

	InvalidateRect(GetDlgItem(hDlg, IDC_STA_PREVIEW), NULL, FALSE);
}
