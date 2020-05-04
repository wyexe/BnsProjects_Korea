// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MyTools/CLResManager.h>
#include "SwAccPkMgr.h"
#include <Shlwapi.h>
#include <MyTools/Log.h>
#include "MiniDump.h"
#include "SomeFun.h"
#include "TaskUpgradeMgr.h"
#include "BnsConfig.h"
#include "GameFunction.h"
#include "ConsoleLog.h"
#include "Person.h"
#include "GameDetection.h"
#include "GameExpression.h"
#include <MyTools/CmdLog.h>
#include <MyTools/CLEchoException.h>
#include <MyTools/CLAsync.h>
#include "../ConsoleDLL/SocketAPI.h"
#include "../ConsoleDLL/ConsoleClient.h"
#pragma comment(linker,"/STACK:10240000,1024000")
#define _SELF L"dllmain.cpp"
#define _SELFA "dllmain.cpp"



BOOL _InitSocket()
{
	CConsoleClient::GetInstance().SetEchoErrorPtr([] (CONST std::wstring& wsLogText)
	{
		CBnsGameFun::GetInstance().SetCloseGame(wsLogText, em_Close_Result::em_Close_Result_ReStart);
	});

	CConsoleClient::GetInstance().SetPrintToConsolePtr([](CONST std::wstring& wsConsoleLog) 
	{ 
		CConsoleLog(em_ConsoleLog::em_ConsoleLog_Content, wsConsoleLog); 
	});

	CSocketAPI SocketAPI;
	if (!SocketAPI.Run(MyTools::CCharacter::ASCIIToUnicode(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.szServerIp), SERVER_PORT))
		return FALSE;

	LOG_CF_D(L"Run Succ!");
	SocketAPI.AlreadyLogin(
		std::stof(BNSVERSION), 
		CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName, 
		CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord, 
		CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szCardNo);

	LOG_CF_D(L"Login Succ!");
	return TRUE;
}

static DWORD WINAPI _SockWorkThread(LPVOID lpParm)
{
	if (!CSomeFun::GetInstance().InitMapView())
	{
		g_bExitGame = TRUE;
		LOG_MSG_CF( L"初始化共享内存失败!");
		return 0;
	}
	//CGameVariable::GetInstance().GetAccShareInfo() = new ACCOUNT_INFO_GAME;
	//CCharacter::wstrcpy_my(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szCardNo, L"0720201601201FDAF0F8DAA77CFB");
	// SocketThread

	MyTools::CCharacter::wstrcpy_my(g_wszDumpPath, CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath, _countof(g_wszDumpPath) - 1);
	MyTools::CCharacter::wstrcpy_my(g_wszDumpAccountName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName());

	CBnsConfig::GetInstance().SetConfigPath(CGameVariable::GetInstance().GetShareInfo(), CGameVariable::GetInstance().GetAccShareInfo(), CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath);

	std::wstring wsPath = CGameVariable::GetInstance().GetShareInfo()->wszGamePath;
	wsPath += L"\\bin\\Log\\";

	MyTools::CLog::GetInstance().SetClientName(CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), wsPath);
	MyTools::CLog::GetInstance().ClearLogFile();
	MyTools::CLog::GetInstance().SetInfiniteSave();//调用马上就会保存文件


	// 防止游戏检测特征码....
	//if (!CGameDetection::GetInstance().Run())
	//	return FALSE;
	//MyTools::CLog::GetInstance().SetSyncSendLog();

	MyTools::CLEchoException::GetInstance().SetExceptionPtr([](CONST std::wstring& wsExceptionContent) 
	{
		CConsoleLog(em_ConsoleLog::em_ConsoleLog_BugReport, wsExceptionContent);
	});
	//MyTools::CCmdLog::GetInstance().Run(CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), CGameExprFun::GetInstance().GetVec());

	// 开始进入游戏!
	StartGame;
	MyTools::CLAsync::GetInstance().Run();
	if (!_InitSocket())
	{
		CBnsGameFun::GetInstance().SetCloseGame(L"连接服务器失败!", em_Close_Result_ReStart);
		return 0;
	}

	// 
	CSomeFun::GetInstance().CreateKeepServerAliveThread();

	CONST std::wstring wsTypeName = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName;
	LOG_CF_D( L"GlobalAccountType=%s", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER, CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.IsDeletePlayer ? FALSE : TRUE);
	if (MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"SwitchAccountPK"))
		return CSwAccPkMgr(CGameVariable::GetInstance().GetShareInfo(), CGameVariable::GetInstance().GetAccShareInfo()).Run([]{ return FALSE; });
	else if (MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"TaskUpgrade_PK"))
	{
		return CSwAccPkMgr(CGameVariable::GetInstance().GetShareInfo(), CGameVariable::GetInstance().GetAccShareInfo()).Run([](){
			// 重新设置回练级模式!
			CGameVariable::GetInstance().GetAccShareInfo()->PkSetting.emPkSchedule = em_Pk_Schedule_TaskUpgrade;
			CBnsConfig::GetInstance().WriteAccountLog(L"帐号的仅仅练级 刷完了!");
			return TRUE;
		});
	}
	else
	{
		CTaskUpgradeMgr::GetInstance().Run(wsTypeName);
	}

	return TRUE;
}

BOOL IsLoadInGame()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, wszPath, MAX_PATH);
	return MyTools::CCharacter::MakeTextToLower(std::wstring(wszPath)).find(L"client.exe") != -1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	static HANDLE hThread = NULL;
	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_BNS_KOR");
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (hThread == NULL && hMutex != NULL && IsLoadInGame())
		{
			RegDumpFunction();
			hThread = cbBEGINTHREADEX(NULL, NULL, _SockWorkThread, NULL, NULL, NULL);
			SetResDeleter(hThread, [](HANDLE& hThread){::CloseHandle(hThread); });
			SetResDeleter(hMutex, [](HANDLE& hMutex){::CloseHandle(hMutex); });
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
