
#include "stdafx.h"
#include "resource.h"
#include "about_dlg.h"
#include "option_dlg.h"
#include "toolbar.h"


#define NSQUARE (8)
#define MAXSQRN (NSQUARE-1)


/* Global Variables */
const LPCTSTR _pcClassName=_T("8QueenClass"); /* the main window class name */
HANDLE _heap=NULL;
HINSTANCE _hInst=NULL;
HICON _hIcon=NULL;
TARRAY _tArrMethod=NULL;
LPCTSTR _pcRegKey=NULL;
LPCTSTR _pcRegKeySettings=NULL;
DWORD _dwMethod=0;
COLORREF _crBoardWhite=0xffffff;
COLORREF _crBoardBlack=0;
COLORREF _crBackground=0;
HWND _hWndMainToolbar=NULL;
HIMAGELIST _hImlMainToolbar=NULL;


/* Forward declarations of functions included in this code module */
LRESULT CALLBACK	App_WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM				App_RegisterClass(HINSTANCE);
BOOL				App_InitVars(HINSTANCE);
void				App_KillVars(HINSTANCE);
BOOL				App_InitInstance(HINSTANCE, int);
void				App_Paint(HWND, HDC, LPCRECT);

int					App_OnCreate(HWND, LPARAM);
BOOL				App_OnCommand(HWND, WPARAM, LPARAM);
void				App_OnPaint(HWND);
void				App_OnSize(HWND, WPARAM, LPARAM);
void				App_OnDestroy(HWND);
LRESULT				App_OnNotify(HWND, WPARAM, LPARAM);

void				App_Solve(TARRAY);

void				Method_Insert(TARRAY, PINT);
void				Method_Remove(TARRAYITEM);
void TOLCALL		Method_Change(HWND, int);


BOOL App_InitVars(HINSTANCE hInst)
{
	int n;
	DWORD dw;
	LPTSTR pc;
	LPCTSTR pcSubKey;

	_heap=GetProcessHeap();
	_hInst=hInst;
	_hIcon=LoadIcon(hInst, (LPCTSTR)IDR_MAIN);
	_hImlMainToolbar=ImageList_LoadImage(hInst, (LPTSTR)IDR_MAIN, 16, 0, RGB(192, 192, 192), IMAGE_BITMAP, LR_CREATEDIBSECTION);

	n=App_RootRegKey(hInst, NULL, 0)+1;
	pc=HeapAlloc(_heap, 0, n*sizeof(TCHAR));
	App_RootRegKey(hInst, pc, n);
	_pcRegKey=pc;

	pcSubKey=_T("Settings");
	dw=Str_Format(NULL, _T("%s\\%s"), _pcRegKey, pcSubKey);
	pc=HeapAlloc(_heap, 0, (dw+1)*sizeof(TCHAR));
	Str_Format(pc, _T("%s\\%s"), _pcRegKey, pcSubKey);
	_pcRegKeySettings=pc;

	_tArrMethod=Array_Make(0);
	Array_SetKillItem(_tArrMethod, Method_Remove);

	App_Solve(_tArrMethod);

	dw=sizeof(int);
	if(Reg_GetValue(HKEY_CURRENT_USER, _pcRegKeySettings, _T("Method"), NULL, (LPBYTE)&_dwMethod, &dw)!=ERROR_SUCCESS)
		_dwMethod=0;

	dw=sizeof(COLORREF);
	if(Reg_GetValue(HKEY_CURRENT_USER, _pcRegKeySettings, _T("BoardWhite"), NULL, (LPBYTE)&_crBoardWhite, &dw)!=ERROR_SUCCESS)
		_crBoardWhite=0xffffff;

	dw=sizeof(COLORREF);
	if(Reg_GetValue(HKEY_CURRENT_USER, _pcRegKeySettings, _T("BoardBlack"), NULL, (LPBYTE)&_crBoardBlack, &dw)!=ERROR_SUCCESS)
		_crBoardBlack=0;

	dw=sizeof(COLORREF);
	if(Reg_GetValue(HKEY_CURRENT_USER, _pcRegKeySettings, _T("Background"), NULL, (LPBYTE)&_crBackground, &dw)!=ERROR_SUCCESS)
		_crBackground=0;

	return TRUE;
}

void App_KillVars(HINSTANCE hInst)
{
	Reg_SetValue(HKEY_CURRENT_USER, _pcRegKeySettings, NULL, _T("Method"), REG_DWORD, (LPBYTE)&_dwMethod, sizeof(int));
	Reg_SetValue(HKEY_CURRENT_USER, _pcRegKeySettings, NULL, _T("BoardWhite"), REG_DWORD, (LPBYTE)&_crBoardWhite, sizeof(COLORREF));
	Reg_SetValue(HKEY_CURRENT_USER, _pcRegKeySettings, NULL, _T("BoardBlack"), REG_DWORD, (LPBYTE)&_crBoardBlack, sizeof(COLORREF));
	Reg_SetValue(HKEY_CURRENT_USER, _pcRegKeySettings, NULL, _T("Background"), REG_DWORD, (LPBYTE)&_crBackground, sizeof(COLORREF));

	Array_Kill(_tArrMethod);

	if(_hImlMainToolbar)
		ImageList_Destroy(_hImlMainToolbar);

	if(_pcRegKey)
		HeapFree(_heap, 0, (LPVOID)_pcRegKey);

	if(_pcRegKeySettings)
		HeapFree(_heap, 0, (LPVOID)_pcRegKeySettings);
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HACCEL hAcl;

	if(!App_RegisterClass(hInst))
		return 0;

	if(!App_InitVars(hInst))
		return 0;

	if(!App_InitInstance(hInst, nCmdShow))
		return 0;

	hAcl=LoadAccelerators(hInst, (LPCTSTR)IDR_MAIN);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(msg.hwnd, hAcl, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	App_KillVars(hInst);

	return (int)msg.wParam;
}


ATOM App_RegisterClass(HINSTANCE hInst)
{
	WNDCLASSEX wcex;
	wcex.cbSize=sizeof(WNDCLASSEX);

	wcex.style			=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc	=(WNDPROC)App_WndProc;
	wcex.cbClsExtra		=0;
	wcex.cbWndExtra		=0;
	wcex.hInstance		=hInst;
	wcex.hIcon			=LoadIcon(hInst, (LPCTSTR)IDR_MAIN);
	wcex.hCursor		=LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	=NULL;
	wcex.lpszMenuName	=(LPCTSTR)IDR_MAIN;
	wcex.lpszClassName	=_pcClassName;
	wcex.hIconSm		=LoadIcon(hInst, (LPCTSTR)IDR_MAIN);

	return RegisterClassEx(&wcex);
}


BOOL App_InitInstance(HINSTANCE hInst, int nCmdShow)
{
	LONG lRes;
	HWND hWnd;
	DWORD dw;
	TCHAR pcTitle[64];
	WINDOWPLACEMENT wp;

	LoadString(_hInst, IDR_MAIN, pcTitle, 64);

	dw=sizeof(WINDOWPLACEMENT);
	lRes=Reg_GetValue(HKEY_CURRENT_USER, _pcRegKeySettings, _T("Placement"), NULL, (LPBYTE)&wp, &dw);

	if(lRes==ERROR_SUCCESS)
	{
		LPRECT pRc=&wp.rcNormalPosition;
		hWnd=CreateWindowEx(0, _pcClassName, pcTitle, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, pRc->left, pRc->top, pRc->right-pRc->left, pRc->bottom-pRc->top, NULL, NULL, hInst, NULL);
	}
	else
		hWnd=CreateWindowEx(0, _pcClassName, pcTitle, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	if(!hWnd)
		return FALSE;

	Method_Change(hWnd, _dwMethod);

	if(lRes==ERROR_SUCCESS && wp.showCmd!=SW_SHOWMINIMIZED)
		ShowWindow(hWnd, wp.showCmd);
	else
		ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK App_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		if(!App_OnCommand(hWnd, wParam, lParam))
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;

	case WM_NOTIFY:
		return App_OnNotify(hWnd, wParam, lParam);

	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		App_OnPaint(hWnd);
		break;

	case WM_SIZE:
		App_OnSize(hWnd, wParam, lParam);
		break;

	case WM_CREATE:
		return App_OnCreate(hWnd, lParam);

	case WM_DESTROY:
		App_OnDestroy(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int App_OnCreate(HWND hWnd, LPARAM lParam)
{
	_hWndMainToolbar=MainToolbar_Create(_hInst, hWnd, _hImlMainToolbar);

	if(!_hWndMainToolbar)
		return -1;

	return 0;
}

BOOL App_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UINT nID=LOWORD(wParam);
	UINT nEvent=HIWORD(wParam);

	// Parse the menu selections
	switch(nID)
	{
	case ID_METHOD_FIRST:
		Method_Change(hWnd, 0);
		break;

	case ID_METHOD_PREV:
		Method_Change(hWnd, _dwMethod-1);
		break;

	case ID_METHOD_NEXT:
		Method_Change(hWnd, _dwMethod+1);
		break;

	case ID_METHOD_LAST:
		Method_Change(hWnd, Array_Count(_tArrMethod)-1);
		break;

	case ID_FILE_EXIT:
		DestroyWindow(hWnd);
		break;

	case ID_TOOLS_OPTION:
		if(DialogBox(_hInst, (LPCTSTR)IDD_OPTION, hWnd, (DLGPROC)OptionDlg_WndProc)==IDOK)
			InvalidateRect(hWnd, NULL, FALSE);
		break;

	case ID_HELP_ABOUT:
		DialogBox(_hInst, (LPCTSTR)IDD_ABOUT, hWnd, (DLGPROC)AboutDlg_WndProc);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

void App_OnPaint(HWND hWnd)
{
	HDC hDC;
	RECT rc;
	PAINTSTRUCT ps;

	GetClientRect(hWnd, &rc);
	hDC=BeginPaint(hWnd, &ps);
	Draw_SmoothPaint(hWnd, hDC, App_Paint, &rc);
	EndPaint(hWnd, &ps);
}

void App_Paint(HWND hWnd, HDC hDC, LPCRECT pRC)
{
	RECT rc=*pRC;
	HBRUSH hbr, hbrw, hbrb;
	int i, j, d, a, l, t, w, h;
	PINT pn;
	HRGN hrg, hrgOut, hrgIn;

	if(_hWndMainToolbar)
	{
		RECT rcToolbar;
		GetWindowRect(_hWndMainToolbar, &rcToolbar);
		rc.top+=rcToolbar.bottom-rcToolbar.top;
	}

	l=rc.left;
	t=rc.top;
	w=rc.right-rc.left;
	h=rc.bottom-rc.top;
	d=(w<h?w:h)/NSQUARE;
	a=d*NSQUARE;
	l+=(w-a)/2;
	t+=(h-a)/2;

	if(_crBackground)
	{
		hrgOut=CreateRectRgnIndirect(&rc);
		hrgIn=CreateRectRgn(l, t, l+a, t+a);
	}

	// Draw board
	hbrw=CreateSolidBrush(_crBoardWhite);
	hbrb=CreateSolidBrush(_crBoardBlack);

	hbr=hbrw;
	rc.top=rc.bottom=t;

	for(i=0; i<NSQUARE; i++)
	{
		rc.bottom+=d;
		rc.left=rc.right=l;

		for(j=0; j<NSQUARE; j++)
		{
			rc.right+=d;

			FillRect(hDC, &rc, hbr);

			rc.left+=d;
			hbr=hbr==hbrb?hbrw:hbrb;
		}

		rc.top+=d;
		hbr=hbr==hbrb?hbrw:hbrb;
	}

	DeleteObject(hbrw);
	DeleteObject(hbrb);



	// Draw queens
	pn=(PINT)Array_At(_tArrMethod, _dwMethod);

	j=d/9;
	for(i=0; i<NSQUARE; i++, pn++)
		DrawIconEx(hDC, l+d*i+j, t+d*(*pn)+j, _hIcon, d-j*2, d-j*2, 0, NULL, DI_NORMAL);


	if(_crBackground)
	{
		// Draw empty area
		hrg=CreateRectRgn(0, 0, 1, 1);
		CombineRgn(hrg, hrgOut, hrgIn, RGN_XOR);

		if(_crBackground>0xffffff)
			hbr=GetSysColorBrush(COLOR_WINDOW);
		else
			hbr=CreateSolidBrush(_crBackground);

		FillRgn(hDC, hrg, hbr);

		DeleteObject(hrg);
		DeleteObject(hrgOut);
		DeleteObject(hrgIn);

		if(_crBackground<=0xffffff)
			DeleteObject(hbr);
	}
}

void App_OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int cx=LOWORD(lParam);
	int cy=HIWORD(lParam);

	if(wParam!=SIZE_MINIMIZED)
	{
		if(_hWndMainToolbar)
		{
			RECT rc;
			GetWindowRect(_hWndMainToolbar, &rc);
			MoveWindow(_hWndMainToolbar, 0, 0, cx, rc.bottom-rc.top, FALSE);
		}
	}
}

void App_OnDestroy(HWND hWnd)
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(hWnd, &wp);
	Reg_SetValue(HKEY_CURRENT_USER, _pcRegKeySettings, NULL, _T("Placement"), REG_BINARY, (LPBYTE)&wp, sizeof(WINDOWPLACEMENT));
	PostQuitMessage(0);
}

LRESULT App_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR lpnmhdr=(LPNMHDR)lParam;

	switch(lpnmhdr->code)
	{
	case TTN_GETDISPINFO:
		{
			LPTOOLTIPTEXT lpttt=(LPTOOLTIPTEXT)lParam;
			lpttt->hinst=_hInst;
			lpttt->lpszText=(LPTSTR)lpttt->hdr.idFrom;
		}
		break;

	case NM_CUSTOMDRAW:
		if(lpnmhdr->hwndFrom==_hWndMainToolbar)
			return MainToolbar_Paint((LPNMTBCUSTOMDRAW)lParam);
		break;
	}

	return 0;
}

void App_Solve(TARRAY tArr)
{
	int i, j;
	int row[NSQUARE], col[NSQUARE], slash[15], backslash[15];

	for(i=0; i<NSQUARE; i++)
	{
		row[i]=0;
		col[i]=-1;
	}

	for(i=0; i<15; i++)
		slash[i]=backslash[i]=0;

	i=0;
	j=0;
	while(i>=0 && i<NSQUARE)
	{	
		while(j<NSQUARE)
		{	
			if(!(row[j] || slash[i+j] || backslash[MAXSQRN+i-j]))
			{	
				col[i]=j;

				row[j]= !row[j];
				slash[i+j]= !slash[i+j];
				backslash[MAXSQRN+i-j]= !backslash[MAXSQRN+i-j];

				if(i==MAXSQRN)
				{	
					Method_Insert(tArr, col);

					row[j]= !row[j];
					slash[i+j]= !slash[i+j];
					backslash[MAXSQRN+i-j]= !backslash[MAXSQRN+i-j];

					j++;
				}
				else
				{	
					i++;
					j=0;
				}

			}//if(!(row[j] || slash[i+j] || backslash[MAXSQRN+i-j]))
			else j++;

		}//while(j<NSQUARE)

		if(j>MAXSQRN)
		{	
			if(i>0)
			{
				i--;
				j=col[i];
				col[i]=-1;

				row[j]= !row[j];
				slash[i+j]= !slash[i+j];
				backslash[MAXSQRN+i-j]= !backslash[MAXSQRN+i-j];
				
				j++;
			}
			else
				break;
		}
	}//while(i>=0 && i<NSQUARE)
}

void Method_Insert(TARRAY tArr, PINT pnQueens)
{
	int i;
	PINT pn=HeapAlloc(_heap, 0, NSQUARE*sizeof(INT));

	for(i=0; i<NSQUARE; i++)
		*(pn+i)=*pnQueens++;

	Array_Add(tArr, pn);
}

void TOLCALL Method_Remove(TARRAYITEM item)
{
	HeapFree(_heap, 0, (PINT)item);
}

void Method_Change(HWND hWnd, int nMethod)
{
	TCHAR pcTitle[64];
	int nMax=Array_Count(_tArrMethod)-1;

	if(nMethod<0)
		nMethod=nMax;
	else if(nMethod>nMax)
		nMethod=0;

	_dwMethod=nMethod;

	LoadString(_hInst, IDR_MAIN, pcTitle, 64);
	wsprintf(pcTitle+lstrlen(pcTitle), _T(" - Method %d of %d"), _dwMethod+1, nMax+1);
	SetWindowText(hWnd, pcTitle);

	InvalidateRect(hWnd, NULL, FALSE);
}
