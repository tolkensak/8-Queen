
#pragma once


class CPiece
{
public:
	CPiece();
	CPiece(HICON hIcon, CString strText);
	virtual ~CPiece();

	HICON GetIcon();
	CString GetText();

protected:
	HICON m_hIcon;
	CString m_strText;
};

typedef CArray<CPiece> CPieceArray;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent=NULL);
	virtual ~CAboutDlg();

	enum { IDD=IDD_ABOUT };

protected:
	CPieceArray m_pieces;

	virtual BOOL OnInitDialog();
	void ChangePiece(BOOL bInitTime=FALSE);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};
