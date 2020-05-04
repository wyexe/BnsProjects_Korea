#pragma once


// CGameCmdDlg dialog

class CGameCmdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameCmdDlg)

public:
	CGameCmdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGameCmdDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	HWND hCombox;
	afx_msg void OnClose();
	BOOL bCloseWindow;
};
