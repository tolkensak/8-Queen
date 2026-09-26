
#include "stdafx.h"
#include "App.h"
#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPiece::CPiece()
	: m_hIcon(NULL)
	, m_strText(_T(""))
{
}

CPiece::CPiece(HICON hIcon, CString strText)
	: m_hIcon(hIcon)
	, m_strText(strText)
{
}

CPiece::~CPiece()
{
}

HICON CPiece::GetIcon()
{
	return m_hIcon;
}

CString CPiece::GetText()
{
	return m_strText;
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg(CWnd* pParent)
	: CDialog(CAboutDlg::IDD, pParent)
{
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_KING), _T("King")));
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_QUEEN), _T("Queen")));
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_ROOK), _T("Rook")));
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_BISHOP), _T("Bishop")));
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_KNIGHT), _T("Knight")));
	m_pieces.Add(CPiece(theApp.LoadIcon(IDI_PAWN), _T("Pawn")));
}

CAboutDlg::~CAboutDlg()
{
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ChangePiece(TRUE);
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);
	ChangePiece();
	SetTimer(1, 1000, NULL);

	CDialog::OnTimer(nIDEvent);
}

void CAboutDlg::ChangePiece(BOOL bInitTime) 
{
	static int nPiece=0;

	if(bInitTime)
		nPiece=0;

	((CStatic*)GetDlgItem(IDC_STA_PIECE_ICON))->SetIcon(m_pieces[nPiece].GetIcon());
	SetDlgItemText(IDC_STA_PIECE_TEXT, m_pieces[nPiece].GetText());

	if(++nPiece>m_pieces.GetUpperBound())
		nPiece=0;
}
