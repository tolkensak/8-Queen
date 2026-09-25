
#include "stdafx.h"
#include "App.h"
#include "MainDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CApp, CWinApp)
END_MESSAGE_MAP()


CApp::CApp()
{
}

CApp::~CApp()
{
}


CApp theApp;


BOOL CApp::InitInstance()
{
//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	CMainDlg dlg;
	m_pMainWnd=&dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the //  application, rather than start the application's message pump.
	return FALSE;
}
