
#include "stdafx.h"
#include "resource.h"


#define NSQUARE (2)


#define CHANGE_BACKGROUND (0)
#define CHANGE_WHITE      (1)
#define CHANGE_BLACK      (2)


// Global Variables
extern COLORREF _crBoardWhiteTemp;
extern COLORREF _crBoardBlackTemp;
extern COLORREF _crBackgroundTemp;

WNDPROC _fnOldPreviewProc=NULL;


// Forward declarations of functions included in this code module
void Preview_OnPaint(HWND);
void Preview_Paint(HWND, HDC, LPCRECT);
void Preview_OnLButtonUp(HWND, WPARAM, LPARAM);
void Preview_ChangeColor(HWND, int);


LRESULT CALLBACK Preview_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT:
		Preview_OnPaint(hWnd);
		break;

	case WM_LBUTTONUP:
		Preview_OnLButtonUp(hWnd, wParam, lParam);
		break;

	default:
		return CallWindowProc(_fnOldPreviewProc, hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void Preview_OnPaint(HWND hWnd)
{
	RECT rc;
	PAINTSTRUCT ps;
	HDC hDC=BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rc);
	Draw_SmoothPaint(hWnd, hDC, Preview_Paint, &rc);
	EndPaint(hWnd, &ps);
}

void Preview_Paint(HWND hWnd, HDC hDC, LPCRECT pRC)
{
	RECT rc=*pRC;
	HBRUSH hbr, hbrw, hbrb;
	int i, j, d, a, l, t, w, h;
	HRGN hrg, hrgOut, hrgIn;

	l=rc.left;
	t=rc.top;
	w=rc.right-l;
	h=rc.bottom-t;
	d=(w<h?w:h)/NSQUARE;
	a=d*NSQUARE;
	l+=(w-a)/2;
	t+=(h-a)/2;

	if(_crBackgroundTemp)
	{
		hrgOut=CreateRectRgnIndirect(&rc);
		hrgIn=CreateRectRgn(l, t, l+a, t+a);
	}

	// Draw board
	hbrw=CreateSolidBrush(_crBoardWhiteTemp);
	hbrb=CreateSolidBrush(_crBoardBlackTemp);

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


	if(_crBackgroundTemp)
	{
		// Draw empty area
		hrg=CreateRectRgn(0, 0, 1, 1);
		CombineRgn(hrg, hrgOut, hrgIn, RGN_XOR);

		if(_crBackgroundTemp>0xffffff)
			hbr=GetSysColorBrush(COLOR_WINDOW);
		else
			hbr=CreateSolidBrush(_crBackgroundTemp);

		FillRgn(hDC, hrg, hbr);

		DeleteObject(hrg);
		DeleteObject(hrgOut);
		DeleteObject(hrgIn);

		if(_crBackgroundTemp<=0xffffff)
			DeleteObject(hbr);
	}
}

void Preview_OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	int nChange;
	int i, j, d, a, l, t, w, h;
	POINT pt={(short)LOWORD(lParam), (short)HIWORD(lParam)};

	GetClientRect(hWnd, &rc);

	l=rc.left;
	t=rc.top;
	w=rc.right-l;
	h=rc.bottom-t;
	d=(w<h?w:h)/NSQUARE;
	a=d*NSQUARE;
	l+=(w-a)/2;
	t+=(h-a)/2;


	nChange=CHANGE_WHITE;
	rc.top=rc.bottom=t;

	for(i=0; i<NSQUARE; i++)
	{
		rc.bottom+=d;
		rc.left=rc.right=l;

		for(j=0; j<NSQUARE; j++)
		{
			rc.right+=d;

			if(PtInRect(&rc, pt))
			{
				Preview_ChangeColor(hWnd, nChange);
				return;
			}

			rc.left+=d;
			nChange=nChange==CHANGE_WHITE?CHANGE_BLACK:CHANGE_WHITE;
		}

		rc.top+=d;
		nChange=nChange==CHANGE_WHITE?CHANGE_BLACK:CHANGE_WHITE;
	}

	Preview_ChangeColor(hWnd, CHANGE_BACKGROUND);
}

void Preview_ChangeColor(HWND hWnd, int nChange)
{
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];

	// Initialize CHOOSECOLOR
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=hWnd;
	cc.lpCustColors=(LPDWORD)acrCustClr;
	cc.Flags=CC_FULLOPEN|CC_RGBINIT;

	switch(nChange)
	{
	case CHANGE_BACKGROUND:
		if(_crBackgroundTemp>0xffffff)
			return;

		cc.rgbResult=_crBackgroundTemp;
		break;

	case CHANGE_WHITE:
		cc.rgbResult=_crBoardWhiteTemp;
		break;

	case CHANGE_BLACK:
		cc.rgbResult=_crBoardBlackTemp;
		break;

	default:
		return;
	}

	if(!ChooseColor(&cc))
		return;

	switch(nChange)
	{
	case CHANGE_BACKGROUND:
		_crBackgroundTemp=cc.rgbResult;
		break;

	case CHANGE_WHITE:
		_crBoardWhiteTemp=cc.rgbResult;
		break;

	case CHANGE_BLACK:
		_crBoardBlackTemp=cc.rgbResult;
		break;
	}

	InvalidateRect(hWnd, NULL, FALSE);
}
