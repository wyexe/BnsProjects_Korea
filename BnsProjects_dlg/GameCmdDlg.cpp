// GameCmdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BnsProjects_dlg.h"
#include "GameCmdDlg.h"
#include "afxdialogex.h"
#include "../BnsProjects/GrammarFunction.h"
#include "GameDlg.h"
#define _SELF L"GameCmdDlg.cpp"
// CGameCmdDlg dialog

IMPLEMENT_DYNAMIC(CGameCmdDlg, CDialogEx)

CGameCmdDlg::CGameCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameCmdDlg::IDD, pParent)
{
	bCloseWindow = FALSE;
}

CGameCmdDlg::~CGameCmdDlg()
{
}

void CGameCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameCmdDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGameCmdDlg message handlers
BOOL CGameCmdDlg::OnInitDialog()
{
	CComboBox * pCombox = ((CComboBox *)GetDlgItem(IDC_COMBO1));
	hCombox = pCombox->GetSafeHwnd();

	vDlg.push_back(this);
	return TRUE;
}

void CGameCmdDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if (bCloseWindow)
		CDialogEx::OnOK();
}

unsigned WINAPI _GrammarThread(LPVOID lpParm)
{
	CGameCmdDlg* dlg = (CGameCmdDlg *)lpParm;
	CComboBox* pComBox = (CComboBox *)dlg->GetDlgItem(IDC_COMBO1);
	WCHAR wszText[1024];
	pComBox->GetWindowText(wszText, 1024);
	WCHAR wsRetMsg[1024] = { 0 };
	StartGame;
	if (MyTools::CLGrammar::AnalysisGrammar(wszText, wsRetMsg))
	{
		if (pComBox->FindString(-1, wszText) == CB_ERR)
			pComBox->AddString(wszText);
	}
	//StopGame;
	pComBox->SetCurSel(-1);
	CEdit* pEdit = (CEdit*)dlg->GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowText(wsRetMsg);

	return 0;
}

BOOL CGameCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocuWnd = GetFocus();
		if (pFocuWnd != NULL && pFocuWnd->GetParent() != NULL && pFocuWnd->GetParent()->m_hWnd == hCombox)
		{
			cbBEGINTHREADEX(NULL, NULL, _GrammarThread, this, NULL, NULL);
			return TRUE;
		}
	}
	else if (pMsg->message == WM_CLOSE)
	{
		bCloseWindow = TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGameCmdDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	bCloseWindow = TRUE;
	DeleteDlgList(this);
	CDialogEx::OnClose();
}
