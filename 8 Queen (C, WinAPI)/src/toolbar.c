
#include "stdafx.h"
#include "resource.h"
#include "toolbar.h"


HWND MainToolbar_Create(HINSTANCE hInst, HWND hWndParent, HIMAGELIST hIml)
{
	int i, j;
	HWND hWnd;
	TBBUTTON tbb[MAINTBBTNCOUNT];
	INITCOMMONCONTROLSEX icex;

	//int n;
	//LPTSTR lpsz;


	// Ensure that the common control DLL is loaded.
	icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC=ICC_BAR_CLASSES;

	if(!InitCommonControlsEx(&icex))
		return NULL;

	//TCHAR sz[STR_LEN];
	//LoadString(hInst, IDR_MAIN, sz, STR_LEN);

	// Create a toolbar.
	hWnd=CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_CUSTOMERASE|CCS_TOP,
		0, 0, 0, 0, hWndParent, (HMENU)IDR_MAIN, hInst, NULL);

	if(!hWnd)
		return NULL;


	// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
	SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
	//SendMessage(hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(TBBM_WIDTH, TBBM_HEIGHT));
	//SendMessage(hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(TBBM_WIDTH, TBBM_HEIGHT));
	//SendMessage(hWnd, TB_AUTOSIZE, 0, 0);


	//n=STR_LEN;
	//lpsz=sz;

	//LoadString(hInst, ID_FILE_NEW, lpsz, n);

	//while(*lpsz && *lpsz!=VK_TAB)
	//{
	//	lpsz++;
	//	n--;
	//}

	//*lpsz++=0;

	//LoadString(hInst, ID_FILE_OPEN, lpsz, n);

	//while(*lpsz && *lpsz!=VK_TAB)
	//{
	//	lpsz++;
	//	n--;
	//}

	//*lpsz++=0;

	//LoadString(hInst, ID_FILE_CLOSE, lpsz, n);

	//while(*lpsz && *lpsz!=VK_TAB)
	//{
	//	lpsz++;
	//	n--;
	//}

	//*lpsz++=0;

	//LoadString(hInst, ID_FILE_PROP, lpsz, n);

	//while(*lpsz && *lpsz!=VK_TAB)
	//	lpsz++;
	//*lpsz++=0;
	//*lpsz=0;

	//SendMessage(hWnd, TB_ADDSTRING, 0, (LPARAM)sz);


	SendMessage(hWnd, TB_SETIMAGELIST, 0, (LPARAM)hIml);


	// Fill the TBBUTTON array with button information, and add the
	// buttons to the toolbar. The buttons on this toolbar have text
	// but do not have bitmap images.

	i=0;
	j=0;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_METHOD_FIRST;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	j++;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_METHOD_PREV;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	j++;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_METHOD_NEXT;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	j++;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_METHOD_LAST;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	tbb[i].iBitmap=-1;
	tbb[i].idCommand=0;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_SEP;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	j++;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_TOOLS_OPTION;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	tbb[i].iBitmap=-1;
	tbb[i].idCommand=0;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_SEP;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	i++;
	j++;
	tbb[i].iBitmap=j;
	tbb[i].idCommand=ID_HELP_ABOUT;
	tbb[i].fsState=TBSTATE_ENABLED;
	tbb[i].fsStyle=BTNS_BUTTON;
	tbb[i].dwData=0;
	tbb[i].iString=-1;

	SendMessage(hWnd, TB_ADDBUTTONS, MAINTBBTNCOUNT, (LPARAM)&tbb);

	ShowWindow(hWnd, SW_SHOW);
	return hWnd;
}

LRESULT MainToolbar_Paint(LPNMTBCUSTOMDRAW lpnmtbcd)
{
	FillRect(lpnmtbcd->nmcd.hdc, &lpnmtbcd->nmcd.rc, GetSysColorBrush(COLOR_BTNFACE));
	return 0;
}
