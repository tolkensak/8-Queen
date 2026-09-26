
#include "stdafx.h"
#include "App.h"
#include "MainDlg.h"
#include "AboutDlg.h"
#include ".\maindlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMethod::CMethod()
{
}

CMethod::~CMethod()
{
}

CMethod::CMethod(int nDisp[CELL_COUNT])
{
	for(int i=0; i<CELL_COUNT; i++)
		m_nDisp[i]=nDisp[i];
}

int CMethod::operator[](int nCell)
{
	if(nCell>=0 && nCell<CELL_COUNT)
		return m_nDisp[nCell];

	return -1;
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_STN_CLICKED(IDC_STA_EYE, OnStnClickedStaEye)
	ON_EN_CHANGE(IDC_EDT_ORDER, OnEnChangeEdtOrder)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClickedBtnClose)
END_MESSAGE_MAP()


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
	, m_nCount(0)
	, m_nOrder(0)
	, m_nOrderOld(0)
	, m_x(-1)
	, m_y(-1)
{
	m_hIcon=theApp.LoadIcon(IDI_QUEEN);

	m_bmpEyes[0].LoadBitmap(IDB_EYE_OPEN);
	m_bmpEyes[1].LoadBitmap(IDB_EYE_CLOSE);

	Solve();
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_ORDER, m_nOrder);
	DDV_MinMaxInt(pDX, m_nOrder, 1, m_nCount);
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu. IDM_ABOUT must be in the system command range.
	ASSERT((IDM_ABOUT & 0xFFF0)==IDM_ABOUT);
	ASSERT(IDM_ABOUT < 0xF000);

	CString str;

	CMenu* pSysMenu=GetSystemMenu(FALSE);
	if(pSysMenu)
	{
		str.LoadString(IDS_ABOUT);
		if(!str.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUT, str);
		}

		pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
		pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
		pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_GRAYED);
	}

	// Set the icon for this dialog.  The framework does this automatically when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);  // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon
	
	SetDlgItemInt(IDC_STA_COUNT, m_nCount);

	CRect rc;
	CWnd* pWndBoard=GetDlgItem(IDC_STA_BOARD);
	pWndBoard->GetClientRect(&rc);
	m_nCellLength=rc.Width()/CELL_COUNT;
	m_nCellPadding=(m_nCellLength-32)/2;

	pWndBoard->GetWindowRect(&rc);
	m_nDelta=rc.left;

	CSpinButtonCtrl* pSpinOrder=(CSpinButtonCtrl*)GetDlgItem(IDC_SPN_ORDER);
	pSpinOrder->SetBuddy(GetDlgItem(IDC_EDT_ORDER));
	pSpinOrder->SetRange(1, m_nCount);
	pSpinOrder->SetPos(1);

	SetTimer(1, 2000, NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0)==IDM_ABOUT)
		About();
	else if(nID==SC_CLOSE)
		CDialog::OnCancel();
	else
		CDialog::OnSysCommand(nID, lParam);
}

HCURSOR CMainDlg::OnQueryDragIcon() // The system calls this to obtain the cursor to display while the user drags the minimized window.
{
	return (HCURSOR) m_hIcon;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if(IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		CRect rc;
		GetClientRect(&rc);
		dc.DrawIcon((rc.Width()-GetSystemMetrics(SM_CXICON)+1)/2, (rc.Height()-GetSystemMetrics(SM_CYICON)+1)/2, m_hIcon);
	}
	else
	{
		Draw();
		CDialog::OnPaint();
	}
}

void CMainDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);

	CStatic* pStaEye=(CStatic*)GetDlgItem(IDC_STA_EYE);

	if(pStaEye->GetBitmap()!=m_bmpEyes[1])
	{
		pStaEye->SetBitmap(m_bmpEyes[1]);
		SetTimer(1, 200, NULL);
	}
	else
	{
		pStaEye->SetBitmap(m_bmpEyes[0]);
		SetTimer(1, 2000, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::About()
{
	KillTimer(1);

	CAboutDlg dlg(this);
	dlg.DoModal();

	SetTimer(1, 100, NULL);
}

void CMainDlg::OnStnClickedStaEye()
{
	About();
}

void CMainDlg::OnEnChangeEdtOrder()
{
	m_nOrderOld=0;

	int nOrderPrev=m_nOrder;

	if(UpdateData())
		Draw();
	else
	{
		m_nOrder=nOrderPrev;
		UpdateData(FALSE);
	}
}

void CMainDlg::Draw()
{
	CWnd* pWnd=GetDlgItem(IDC_STA_BOARD);
	pWnd->RedrawWindow();

	CClientDC dc(pWnd);
	for(int i=0; i<CELL_COUNT; i++)
		dc.DrawIcon(i*m_nCellLength+m_nCellPadding, m_methods[m_nOrder-1][i]*m_nCellLength+m_nCellPadding, m_hIcon);
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rc;
	GetDlgItem(IDC_STA_BOARD)->GetClientRect(&rc);

	if(rc.PtInRect(point))
	{
		int x=(point.x-m_nDelta)/m_nCellLength;
		int y=(point.y-m_nDelta)/m_nCellLength;

		if(m_x!=x || m_y!=y)
		{
			m_x=x;
			m_y=y;
			m_nOrderOld=0;
		}
		
		for(int i=m_nOrderOld; i<m_nCount; i++)
			if(m_methods[i][x]==y)
			{
				m_nOrderOld=m_nOrder=i+1;
				UpdateData(FALSE);
				Draw();
				break;
			}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CMainDlg::OnOK()
{
}

void CMainDlg::OnCancel()
{
}

void CMainDlg::OnBnClickedBtnClose()
{
	CDialog::OnCancel();
}

void CMainDlg::Solve()
{
	int i, j;
	int nRow[CELL_COUNT], nCol[CELL_COUNT], nSlash1[15], nSlash2[15];

	for(i=0; i<CELL_COUNT; i++)
	{
		nRow[i]=0;
		nCol[i]=-1;
	}

	for(i=0; i<15; i++)
		nSlash1[i]=nSlash2[i]=0;

	i=0;
	j=0;

	while(i>=0 && i<CELL_COUNT)
	{	
		while(j<CELL_COUNT)
		{	
			if(nRow[j] || nSlash1[i+j] || nSlash2[7+i-j])
				j++;
			else
			{	
				nCol[i]=j;

				nRow[j]=!nRow[j];
				nSlash1[i+j]=!nSlash1[i+j];
				nSlash2[7+i-j]=!nSlash2[7+i-j];

				if(i==7)
				{	
					m_methods.Add(nCol);
					m_nCount++;

					nRow[j]=!nRow[j];
					nSlash1[i+j]=!nSlash1[i+j];
					nSlash2[7+i-j]=!nSlash2[7+i-j];

					j++;
				}
				else
				{	
					i++;
					j=0;
				}
			}
		}

		if(j>7)
		{	
			if(i>0)
			{
				i--;
				j=nCol[i];
				nCol[i]=-1;

				nRow[j]=!nRow[j];
				nSlash1[i+j]=!nSlash1[i+j];
				nSlash2[7+i-j]=!nSlash2[7+i-j];
				
				j++;
			}
			else
				break;
		}
	}
}
