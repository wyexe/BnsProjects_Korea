// RecordPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BnsProjects_dlg.h"
#include "RecordPointDlg.h"
#include "afxdialogex.h"
#include "../BnsProjects/GameFunction.h"
#include "GameDlg.h"
#include <BaseObject.h>
#include <Person.h>
// CRecordPointDlg dialog
#define _SELF L"RecordPointDlg.cpp"

IMPLEMENT_DYNAMIC(CRecordPointDlg, CDialogEx)

CRecordPointDlg::CRecordPointDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordPointDlg::IDD, pParent)
{

}

CRecordPointDlg::~CRecordPointDlg()
{
}

void CRecordPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecordPointDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RECORD_START, &CRecordPointDlg::OnBnClickedBtnRecordStart)
	ON_BN_CLICKED(IDC_BTN_RECORD_STOP, &CRecordPointDlg::OnBnClickedBtnRecordStop)
	ON_BN_CLICKED(IDC_BTN_RECORD_CLEAR, &CRecordPointDlg::OnBnClickedBtnRecordClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_NOW, &CRecordPointDlg::OnBnClickedBtnNow)
END_MESSAGE_MAP()

HANDLE hWorkThread = NULL;
unsigned WINAPI _WorkThread(LPVOID lpParm)
{
	CRecordPointDlg* dlg = (CRecordPointDlg *)lpParm;
	CEdit* pEdit = (CEdit *)dlg->GetDlgItem(IDC_EDIT_POINT);

	Point LastPoint;
	while (!dlg->bExit)
	{
		if (!dlg->bRemPoint)
		{
			Sleep(500);
			continue;
		}

		if (CBaseObject::GetPersonDis(LastPoint) < 3.0f)
			continue;
		

		Point NowPt = CBaseObject::GetPersonPoint();
		if (NowPt == LastPoint)
			continue;

		LastPoint = NowPt;
		
		CString str;
		pEdit->GetWindowTextW(str);

		WCHAR wszText[64];
		swprintf_s(wszText, _countof(wszText) - 1, L"设置坐标(%d,%d,%d);\r\n", NowPt.GetX(), NowPt.GetY(), NowPt.GetZ());
		str.Append(wszText);

		pEdit->SetWindowTextW(str);
		for (int i = 0; i < 5 && !dlg->bExit; ++i)
			Sleep(100);
	}

	return 0;
}

// CRecordPointDlg message handlers
BOOL CRecordPointDlg::OnInitDialog()
{
	bRemPoint = FALSE;
	bExit = FALSE;

	if (NULL == hWorkThread)
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, this, NULL, NULL);

	vDlg.push_back(this);
	return TRUE;
}

void CRecordPointDlg::OnBnClickedBtnRecordStart()
{
	bRemPoint = TRUE;
}


void CRecordPointDlg::OnBnClickedBtnRecordStop()
{
	bRemPoint = FALSE;
}


void CRecordPointDlg::OnBnClickedBtnRecordClear()
{
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_POINT);
	pEdit->SetWindowTextW(L"");
}


void CRecordPointDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	DeleteDlgList(this);
	bRemPoint = FALSE;
	bExit = TRUE;
	if (WaitForSingleObject(hWorkThread, 3000) == WAIT_TIMEOUT)
		::TerminateThread(hWorkThread, 0);

	::CloseHandle(hWorkThread);
	hWorkThread = nullptr;

	CDialogEx::OnClose();
}


void CRecordPointDlg::OnBnClickedBtnNow()
{
	CEdit* pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_POINT);

	CString str;
	pEdit->GetWindowTextW(str);

	WCHAR wszText[64];
	Point NowPt = CPerson::GetPersonPoint();
	swprintf_s(wszText, _countof(wszText) - 1, L"走到(%d,%d,%d);\r\n", NowPt.GetX(), NowPt.GetY(), NowPt.GetZ());
	str.Append(wszText);

	pEdit->SetWindowTextW(str);
}


void CRecordPointDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}
