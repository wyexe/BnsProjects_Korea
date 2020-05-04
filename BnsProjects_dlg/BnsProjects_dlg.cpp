// BnsProjects_dlg.cpp : Defines the initialization routines for the DLL.
//


#include "stdafx.h"
#include "BnsProjects_dlg.h"
#include "GameDlg.h"
#include <GameStruct.h>
#include <MyTools/CLHook.h>
#include <MyTools/Log.h>
#include "Person.h"
#include <SomeFun.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _SELF L"BnsProjects_dlg.cpp"


BEGIN_MESSAGE_MAP(CBnsProjects_dlgApp, CWinApp)
END_MESSAGE_MAP()


// CBnsProjects_dlgApp construction

CBnsProjects_dlgApp::CBnsProjects_dlgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBnsProjects_dlgApp object

CBnsProjects_dlgApp theApp;


// CBnsProjects_dlgApp initialization
unsigned WINAPI _ShowDlgThread(LPVOID lpParm)
{
	CONST std::wstring wsPlayerName = CPerson::GetInstance().GetName();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CGameDlg CG;
	CG.DoModal();
	return 0;
}
HANDLE hShowThread = NULL;
BOOL CBnsProjects_dlgApp::InitInstance()
{
	CWinApp::InitInstance();
	hShowThread = cbBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_ShowDlgThread, NULL, NULL, NULL);
	return TRUE;
}

__declspec(dllexport) int WINAPI BBB(DWORD dwID)
{

	return 0xF;
}

__declspec(dllexport) BOOL WINAPI ReleaseDLL()
{
	std::for_each(vDlg.begin(), vDlg.end(), [](CWnd* dlg){
		dlg->SendMessage(WM_CLOSE, NULL, NULL);
		dlg->CloseWindow();
	});

	LOG_CF_D( L"Wait For Close Windows!");
	::WaitForSingleObject(hShowThread, INFINITE);
	LOG_CF_D( L"Release Hook!");
	MyTools::CLHook::Release();
	LOG_CF_D( L"Release DLL!");
	return TRUE;
}

BOOL CBnsProjects_dlgApp::ExitInstance()
{
	g_bUninstall = TRUE;
	/*HANDLE hKeepALiveThread = CSomeFun::GetInstance().RunKeepALiveThread();
	if (hKeepALiveThread != NULL)
	{
		::WaitForSingleObject(hKeepALiveThread, INFINITE);
	}*/

	if (g_dwTimerId != NULL)
	{
		LOG_CF_D( L"Kill Timer!");
		::KillTimer(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, g_dwTimerId);
	}

	if (CGameVariable::GetInstance().GetAccShareInfo() != NULL)
	{
		LOG_CF_D( L"Delete AccShareInfo!");
		delete CGameVariable::GetInstance().GetAccShareInfo();
	}

	if (g_dwHookD3dAddr != NULL && g_dwOldD3dCall != NULL)
	{
		LOG_CF_D( L"Restore D3D Hook");
		MyTools::CCharacter::WriteDWORD(g_dwHookD3dAddr, g_dwOldD3dCall);
	}

	if (g_dwHookSpeedAddr != NULL && g_dwSpeedCALL != NULL)
	{
		LOG_CF_D( L"Restore Speed Hook");
		MyTools::CCharacter::WriteDWORD(g_dwHookSpeedAddr, g_dwSpeedCALL);
	}

	CGameVariable::GetInstance().DoList_HookList([](std::vector<MyTools::MYHOOK_CONTENT>& vlst)
	{
		for (auto& itm : vlst)
		{
			LOG_CF_D( L"Restore Hook:%X", itm.dwHookAddr);
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&itm);
		}
	});

	CWinApp::ExitInstance();
	return 1;
}