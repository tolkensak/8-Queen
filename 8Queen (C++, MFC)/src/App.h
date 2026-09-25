
#pragma once


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"		// main symbols


class CApp : public CWinApp
{
public:
	CApp();
	virtual ~CApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CApp theApp;
