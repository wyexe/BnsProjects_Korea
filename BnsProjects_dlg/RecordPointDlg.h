#pragma once


// CRecordPointDlg dialog

class CRecordPointDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordPointDlg)

public:
	CRecordPointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecordPointDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRecordStart();
public:
	BOOL bRemPoint;
	BOOL bExit;
	afx_msg void OnBnClickedBtnRecordStop();
	afx_msg void OnBnClickedBtnRecordClear();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnNow();
	virtual void OnOK();
};
