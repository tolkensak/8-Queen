
#pragma once


#define CELL_COUNT 8


class CMethod
{
public:
	CMethod();
	CMethod(int nDisp[CELL_COUNT]);
	virtual ~CMethod();

	int operator[](int nCell);

protected:
	int m_nDisp[CELL_COUNT];
};

typedef CArray<CMethod> CMethodArray;


class CMainDlg : public CDialog
{
public:
	CMainDlg(CWnd* pParent=NULL);	// standard constructor
	virtual ~CMainDlg();

	enum { IDD=IDD_MAIN };

protected:
	HICON m_hIcon;
	CBitmap m_bmpEyes[2];
	CMethodArray m_methods;

	int m_x;
	int m_y;

	int m_nCellLength;
	int m_nCellPadding;
	int m_nDelta;

	int m_nCount;
	int	m_nOrder;
	int m_nOrderOld;

	void Solve();
	void Draw();
	void About();

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	void DoDataExchange(CDataExchange* pDX);

	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedStaEye();
	afx_msg void OnEnChangeEdtOrder();
	afx_msg void OnBnClickedBtnClose();

	DECLARE_MESSAGE_MAP()
};
