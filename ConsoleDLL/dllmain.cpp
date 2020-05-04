// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <process.h>
#include <wininet.h>
#include <thread>
#include <atomic>
#include <MyTools/CLProcess.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLResManager.h>
#include <MyTools/Log.h>
#include <MyTools/CmdLog.h>
#include <MyTools/CLAsync.h>
#include "AccountMgr.h"
#include "ConsoleCard.h"
#include "SocketAPI.h"
#include "ConsoleClient.h"
#include "OptimizeGame.h"
#include "CmdLogService.h"
#include "BnsProcExtend.h"
#include "SortWindowService.h"
#include "AccountVirtualSeriaNumber.h"
#include "ConsoleLog.h"
#include <VMProtectSDK.h>
#include "ConsoleDump.h"
#include "../BnsProjects/BnsConfig.h"
#include "ConsoleFun.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Wininet.lib") 
#define _SELF L"dllmain.cpp"
 
typedef struct _OutputDlgInfo
{
	wstring wsAccountName;
	wstring wsContent;
}OutputDlgInfo;
UINT		g_uMaxRunCount = 0;
std::atomic<BOOL>		g_bCheckLoseConnect = FALSE;
UINT		g_uRunCount = 0;
vector<OutputDlgInfo> vOutputLst;
MyTools::CLLock				  Lock_vOutputLst(L"Lock_vOutputLst");

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;

}

extern "C" __declspec(dllexport) void WINAPIV SetGameRun(int);
DWORD WINAPI _WorkThread(_In_ LPVOID lpParm)
{
	VMProtectBegin("ConsoleDll__WorkThread");
	CAccountMgr AccountMgr;
	while (g_bRunConsoleDLL) // Stop
	{
		try
		{
			::Sleep(1000);
			

			// 设置标志符=FALSE
			for (auto i = 0; i < MAX_GAME_COUNT; ++i)
			{
				auto& AccountGame = g_pSharedInfo->arrGameInfo[i];
				if (AccountGame.AccountStatus.bExist && !AccountGame.MyAcount_Info.bLogin && AccountGame.AccountStatus.bLogined)
					AccountGame.MyAcount_Info.bAlive = FALSE;
			}

			// 10秒 顺便收集输入到控制台的消息
			::Sleep(10 * 1000);

			if (!g_bCheckLoseConnect) // 登录线程在工作的时候, 不检查掉线!
				continue;
			
			for (auto i = 0; i < MAX_GAME_COUNT; ++i)
			{
				auto& AccountGame = g_pSharedInfo->arrGameInfo[i];
				// bExist = 这个结构是否有帐号  bLogin = FALSE表示这个帐号登录完毕 bLogined = TRUE表示这个帐号已经登录过
				if (!AccountGame.AccountStatus.bExist || AccountGame.MyAcount_Info.bLogin || !AccountGame.AccountStatus.bLogined || AccountGame.AccountStatus.bFrozen)
					continue;

				auto bClearAccount = AccountGame.AccountStatus.bDone; // bDone = TRUE 刷完了, 需要换号!
				if (AccountGame.GetLoginFaildCount() >= 5) // 5次登录失败 和 5次创建角色失败的处理
				{
					if (AccountGame.AccountStatus.emCloseResult == em_Close_Result_UnExistTime)
					{
						CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"没点卡了,删除帐号.");
						AccountGame.AccountStatus.bDelete = TRUE; // 这种错误就应该人道毁灭
						bClearAccount = TRUE;
					}
					else if (AccountGame.AccountStatus.emCloseResult == em_Close_Result_Trading)
					{
						CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"交易模式，密码错误");
						AccountGame.AccountStatus.bDelete = TRUE; // 这种错误就应该人道毁灭
						bClearAccount = TRUE;
					}
					else
					{
						//AccountGame.AccountStatus.bFrozen = TRUE;
						//OutputDlg(AccountGame.MyAcount_Info.szUserName, L"3次登录失败 或 3次创建角色失败, 冻结该窗口~!");
						//CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"5次登录失败,删除此帐号。");
						//AccountGame.GetLoginFaildCount() = 0;
						//AccountGame.AccountStatus.bDelete = TRUE; // 这种错误就应该人道毁灭
						//bClearAccount = TRUE;
						CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"创建角色5次失败");
					}
				}
				if (AccountGame.AccountStatus.uCreatePlayerFaildCount >= 3)
				{
					CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"创建角色3次失败,怀疑没有50级劵");
					bClearAccount = TRUE;
				}

				if (bClearAccount) // 是否需要删帐号!
				{
					//OutputDlg(AccountGame.MyAcount_Info.szUserName, L"主线已完成。!");
					//CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"bDone=TRUE 刷完帐号了, 换下一个帐号!")
					LOG_CF_D(L"bDone=TRUE 刷完帐号了");
					AccountMgr.SetClearAccount(AccountGame); // 里面会清空整个帐号的内容
					g_uRunCount -= 1; // 当前运行的Client数量-1. 方便下面的登录线程 开始登录下一个号!
					::Sleep(1000);
					break;
				}

				// 检查CPU使用率
				CConsoleFun::GetInstance().IsEmptyCPUUsage(&AccountGame);
				CConsoleFun::GetInstance().ISWindowStopRespond(&AccountGame);

				// bAlive = FALSE 表示10秒后。 游戏还没把标识符=TRUE , 所以判定游戏要嘛是崩溃了， 要嘛是掉线了。 要嘛是停止工作了
				// bClose = TRUE 表示特殊情况需要关闭游戏, 比如登录失败， 进入游戏失败之类的需要重开游戏!
				if (!AccountGame.MyAcount_Info.bAlive || AccountGame.AccountStatus.bClose || AccountGame.AccountStatus.CpuUsage.uCountForCPUUsage >= 3 || AccountGame.AccountStatus.bStopRespond)
				{
					if (AccountGame.AccountStatus.bClose)
					{
						LOG_CF_D( L"bClose=TRUE, 关闭游戏!");
					}
					else if (!AccountGame.MyAcount_Info.bAlive)
					{
						LOG_CF_D( L"检测不到游戏客户端在线! 掉线处理~!");
						CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"检测不到游戏客户端在线! 掉线处理~");
						/*if (AccountGame.AccountStatus.emLoginStep == em_Login_Step::em_Login_Step_InputAccount)
							CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"游戏登录失败,可能是VPN的问题!");
						else
							CBnsConfig::GetInstance().WriteAccountLog(&AccountGame, L"检测不到游戏客户端在线! 掉线处理~");*/
					}
					else if (AccountGame.AccountStatus.bStopRespond)
					{
						LOG_CF_E( L"游戏窗口:%s 未响应! X掉!", AccountGame.GetAccName());
					}

					if (AccountGame.AccountStatus.emCloseResult == em_Close_Result_CloseConsole)
					{
						SetGameRun(0x2);
					}
					//LOG_MSG_CF(L"%s掉线啦!", AccountGame.MyAcount_Info.szUserName);
					AccountMgr.SetResetAccount(AccountGame);
				}
			}
		}
		catch (...)
		{
			LOG_MSG_CF(L"_WorkThread出现异常");
		}
	}
	VMProtectEnd();
	return 0;
}

DWORD WINAPI _RunGameThread(_In_ LPVOID lpParm)
{
	LOG_CF_D(L"_RunGameThread");
	

	std::vector<std::shared_ptr<std::thread>> ThreadVec;
	VMProtectBegin("ConsoleDll-_RunGameThread");
	auto fnWaitForLoginThread = [&ThreadVec]
	{
		for (auto& itm : ThreadVec)
		{
			if (itm->joinable())
			{
				// wait for thrad finish
				itm->join();
			}
		}
		MyTools::InvokeClassPtr<CSortWindowService>()->Run();
	};

	g_pSharedInfo->ConsoleContent.GlobalAccount.IsNoModule = FALSE;
	for (INT i = 0; i < MAX_GAME_COUNT && g_bRunConsoleDLL; ++i)
	{
		auto pAccountGame = &g_pSharedInfo->arrGameInfo[i];
		if (!pAccountGame->AccountStatus.bExist)
			continue;

		// Max Run Count
		while (g_uRunCount >= (int)g_uMaxRunCount)
		{
			g_bCheckLoseConnect = TRUE;
			Sleep(500);
		}

		// Ingore Deleted Account 
		if (!pAccountGame->AccountStatus.bExist)
			continue;

		// 分配卡号
		CardInfo CardInfo_;
		if (!CConsoleCard::GetInstance().GetNotUseCard(CardInfo_))
		{
			LOG_MSG_CF(L"卡号分配不足!");
			i -= 1;
			continue;
		}
		LOG_CF_D( L"当前帐号:%s, 分配到的卡号是:%s", pAccountGame->GetAccName(), CardInfo_.wsCardNo.c_str());
		MyTools::CCharacter::wstrcpy_my(pAccountGame->MyAcount_Info.szCardNo, CardInfo_.wsCardNo.c_str());

		g_bCheckLoseConnect = FALSE;
		CConsoleLog::GetInstance().Add(em_ConsoleLog::em_ConsoleLog_Content, pAccountGame->GetAccName(), L"登录中……");
		pAccountGame->AccountStatus.bLogined = TRUE;
		pAccountGame->AccountStatus.bRunNcLanucher = FALSE;

		auto pRunGameThread = std::make_shared<std::thread>([&pAccountGame]{CConsoleFun::GetInstance().RunGame(pAccountGame); });
		ThreadVec.push_back(pRunGameThread);

		LOG_C_D(L"Waiting ... Login Next Account");
		while (!pAccountGame->AccountStatus.bRunNcLanucher)
		{
			::Sleep(1000);
		}
		
		LOG_C_D(L"Login Next Account..wait 2 second");
		::Sleep(2000);

		g_uRunCount = CConsoleFun::GetInstance().GetRunAccountCount();
		if (g_uRunCount == g_uMaxRunCount)
		{
			// 
			fnWaitForLoginThread();
			MyTools::CLProcess::TerminateProc_For_ProcName(L"NCLauncherS.exe");
		}

		LOG_CF_D( L"在线数量:%d 最大数量:%d", g_uRunCount, g_uMaxRunCount);
	}
	
	// All of Account Login Finish!
	fnWaitForLoginThread();
	LOG_CF_D(L"所有帐号登录完毕!");
	g_bCheckLoseConnect = TRUE;
	VMProtectEnd();
	return 0;
}

typedef struct _EnumBnsWindowSrc
{
	DWORD dwPid;
	HWND* pWnd;
}EnumBnsWindowSrc;

// 枚举窗口
BOOL CALLBACK fnEnumApplicationError(HWND h, LPARAM l)
{
	if (IsWindow(h))
	{
		static wchar_t wszWindowName[MAX_PATH] = { 0 };
		static wchar_t wszClassName[MAX_PATH];
		if (GetClassNameW(h, wszClassName, MAX_PATH - 1) > 0 && ::GetWindowTextW(h, wszWindowName, MAX_PATH - 1))
		{
			if (MyTools::CCharacter::wstrcmp_my(wszClassName, L"#32770") && MyTools::CCharacter::wstrstr_my(wszWindowName, L"应用程序错误"))//如果能遍历到这个,并且是
			{
				*(HWND*)l = h;
				return FALSE;
			}
			else if (MyTools::CCharacter::wstrcmp_my(wszWindowName, L"System Message") || MyTools::CCharacter::wstrcmp_my(wszWindowName, L"Message"))
			{
				*(HWND*)l = h;
				return FALSE;
			}
		}
	}
	return TRUE;
}

DWORD WINAPI _CloseErrWinThread(LPVOID lpParm)
{
	while (g_bRunConsoleDLL)
	{
		Sleep(1000);
		MyTools::CLProcess::TerminateProc_For_ProcName(L"BnSCrashReport.exe");
		MyTools::CLProcess::TerminateProc_For_ProcName(L"WerFault.exe");

		HWND hApplicationError = NULL;
		EnumWindows((WNDENUMPROC)fnEnumApplicationError, (LPARAM)&hApplicationError);
		if (hApplicationError != NULL)
		{
			LOG_CF_E(L"关闭应用程序错误框!");
			::SendMessageW(hApplicationError, WM_CLOSE, NULL, NULL);
		}
	}
	
	return 0;
}

BOOL SetConsoleKeepALive(_In_ BOOL bRun)
{
	static std::thread* pThread = nullptr;
	static BOOL ms_Stop = FALSE;
	if (pThread == nullptr)
	{
		pThread = new std::thread([]
		{
			// 不做任何操作
			CSocketAPI SocketAPI;
			while (!ms_Stop)
			{
				SocketAPI.ConsoleKeepALive();
				MyTools::CLPublic::TimeOut_By_Condition(5 * 1000, []{ return ms_Stop; });
			}
		});
	}

	if (!bRun)
	{
		ms_Stop = TRUE;
		pThread->join();
		delete pThread;
		pThread = nullptr;
	}

	return TRUE;
}
extern "C" __declspec(dllexport) void WINAPIV Init(_In_ LPCWSTR pwszGamePath, _In_ int bOptimize)
{
	VMProtectBegin("ConsoleDll-Init");
	if (!CConsoleFun::GetInstance().Init() || !CConsoleFun::GetInstance().SetIpAddress())
	{
		LOG_MSG_CF(L"初始化DLL失败!");
		ExitProcess(0);
	}
	CreateMutexW(NULL, FALSE, L"CL_BNS_KOR");
	if (!CConsoleFun::GetInstance().LoadNpDLL())
	{
		LOG_MSG_CF(L"缺少NP_Dll");
		SetGameRun(0x2);
		return;
	}
	else if (!CConsoleFun::GetInstance().LoadCommonDLL())
	{
		SetGameRun(0x2);
		return;
	}

	MyTools::CCharacter::wstrcpy_my(g_wszDumpAccountName, L"Console");
	::GetCurrentDirectoryW(MAX_PATH, g_wszDumpPath);
	RegDumpFunction();
	
	MyTools::CLAsync::GetInstance().Run();
	CConsoleClient::GetInstance().SetEchoErrorPtr([](CONST std::wstring& wsText) 
	{ 
		LOG_MSG_CF(wsText.c_str());
		SetGameRun(0x2); 
	});
	
	if (!CConsoleClient::GetInstance().Run(MyTools::CCharacter::ASCIIToUnicode(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp), SERVER_PORT, 10 * 1000))
	{
		LOG_MSG_CF(L"初始化网络失败!");
		ExitProcess(0);
	}

	SetConsoleKeepALive(TRUE);
	CConsoleFun::GetInstance().SetConsoleLogPtr(CConsoleLog::GetInstance().GetAddPtr());
	MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->wszGamePath, pwszGamePath);
	if (bOptimize == 0x1 && !COptimizeGame().Open(std::wstring(pwszGamePath)))
		LOG_MSG_CF(L"开启游戏优化失败!");

	MyTools::CCmdLog::GetInstance().Run(L"ConsoleDLL", CCmdLogService::GetInstance().GetVec());
	
	VMProtectEnd();
}

// 开始停止挂机
extern "C" __declspec(dllexport) void WINAPIV SetGameRun(_In_ int nFlag)
{
	if (nFlag == 0x0) // Stop
	{
		
	}
	else if (nFlag == 0x1) // Start
	{
		// 设置开数, 把木有时间的卡号去掉!
		g_uMaxRunCount = CConsoleCard::GetInstance().GetCardCount();
		SetConsoleKeepALive(FALSE);
		std::thread InvokeVirtualSeraiNumberThread([]
		{
			static CAccountVirtualSeriaNumber AccVirSerNumber;
			AccVirSerNumber.Create();
			AccVirSerNumber.Initialize();
			CConsoleClient::GetInstance().Stop();

			auto hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
			SetResDeleter(hWorkThread, [](HANDLE& hThread) { ::CloseHandle(hThread); });

			auto hRunGameThread = cbBEGINTHREADEX(NULL, NULL, _RunGameThread, NULL, NULL, NULL);
			SetResDeleter(hRunGameThread, [](HANDLE& hThread) { ::CloseHandle(hThread); });

			auto hCloseErrWinThread = cbBEGINTHREADEX(NULL, NULL, _CloseErrWinThread, NULL, NULL, NULL);
			SetResDeleter(hCloseErrWinThread, [](HANDLE& hThread) { ::CloseHandle(hThread); });
		});
		InvokeVirtualSeraiNumberThread.detach();
	}
	else if (nFlag == 0x2) // Termiate Console
	{
		g_bRunConsoleDLL = FALSE;
	
		LOG_CF_D( L"正在结束所有的Client.exe~");
		CBnsProcExtend().DestoryBnsClient();
		if (!COptimizeGame().Close(std::wstring(g_pSharedInfo->wszGamePath)))
		{
			LOG_MSG_CF(L"关闭游戏优化失败!");
		}

		::TerminateProcess(::GetCurrentProcess(), 0);
		MyTools::CLPublic::ForceExit();
	}
	else if (nFlag == 0x3)
	{
		LOG_MSG_CF(COptimizeGame().Close(g_pSharedInfo->wszGamePath) ? L"关闭游戏优化成功" : L"关闭游戏优化失败!");
	}
}

// 检查卡号是否正确
extern "C" __declspec(dllexport) int WINAPIV CheckCard(_In_ LPCWSTR pwszCardNo)
{
	VMProtectBegin("ConsoleDll-CheckCard");
	auto fnGetHour = [](LPCSTR pwszCard)
	{
		CHAR wszHour[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszHour, pwszCard, 4);
		return (DWORD)atoi(wszHour);
	};

	auto fnGetDate = [](LPCSTR pwszCard)
	{
		CHAR wszDate[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszDate, pwszCard + 4, 8);
		return (DWORD)atoi(wszDate);
	};

	auto fnGetRand = [](LPCSTR pwszCard)
	{
		CHAR wszRand[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszRand, pwszCard + 4 + 8, 8);
		return (DWORD)strtol(wszRand, NULL, 16);
	};

	auto fnGetCrc = [](LPCSTR pwszCard)
	{
		CHAR wszCrc[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszCrc, pwszCard + 4 + 8 + 8, 8);
		return (DWORD)strtoll(wszCrc, NULL, 16);
	};

	auto fnConvertNumber = [](LPCSTR pszFormat, DWORD n)
	{
		static CHAR wszText[128];
		wsprintfA(wszText, pszFormat, n);
		return wszText;
	};

	if (wcslen(pwszCardNo) != 0x1C)
		return 0;

	CHAR szCard[128] = { 0 };
	MyTools::CCharacter::UnicodeToASCII(pwszCardNo, szCard);

	auto nHour = fnGetHour(szCard);
	auto nDate = fnGetDate(szCard);
	auto nRand = fnGetRand(szCard);
	//auto nCrc = fnGetCrc(szCard);

	CHAR wszText[128] = { 0 };
	lstrcatA(wszText, fnConvertNumber("%04d", nHour));
	lstrcatA(wszText, fnConvertNumber("%08d", nDate));
	lstrcatA(wszText, fnConvertNumber("%08X", nRand));
	auto dwCrc = MyTools::CLPublic::GetCRC32_DWORD(wszText, strlen(wszText));
	lstrcatA(wszText, fnConvertNumber("%08X", dwCrc));
	VMProtectEnd();
	return MyTools::CCharacter::strcmp_my(wszText, szCard) ? 1 : 0;
}

// 查询卡号的详细信息
extern "C" __declspec(dllexport) int WINAPIV QueryCardInfo(_In_ LPCWSTR pwszCardNo, _In_ int nIsStart)
{
	VMProtectBeginUltra("ConsoleDll-QueryCardInfo");

	SetConsoleKeepALive(TRUE);
	if (nIsStart == 0)
	{
		if (CConsoleCard::GetInstance().ExistCard(pwszCardNo, [](CardInfo&){}))
			return 0x1;

		CardInfo Card;
		Card.wsCardNo = pwszCardNo;
		Card.dwCardCrc = MyTools::CLPublic::GetCRC32_DWORD(pwszCardNo);
		CConsoleCard::GetInstance().AddCard(Card);
		return 0x1;
	}
	else if (nIsStart == 0x1)
	{
		CSocketAPI SocketAPI;
		SocketAPI.QueryCard(CConsoleCard::GetInstance().GetData());

		// 把无效卡号(没时间)给剔除!
		CConsoleCard::GetInstance().RemoveCard([](CONST CardInfo& itm){return itm.nSurplusHour <= 0 || !itm.bExit; });

		// 初始化设置全部都没有帐号占用!
		CConsoleCard::GetInstance().ForEach([](CardInfo& itm){ itm.bExit = FALSE; });

		return 0x1;
	}
	else if (nIsStart == 0x2)
	{
		CConsoleCard::GetInstance().RemoveCard([](CONST CardInfo& itm){return TRUE; });
	}
	VMProtectEnd();
	return 0;
}

// 返回卡号的详细信息
extern "C" __declspec(dllexport) int WINAPIV GetCardInfo(_In_ LPCWSTR pwszCardNo, _Out_ LPWSTR pwszRetMsg)
{
	VMProtectBeginUltra("ConsoleDll-GetCardInfo");
	if (!CConsoleCard::GetInstance().ExistCard(pwszCardNo, [&pwszRetMsg](CardInfo& CardInfo_){
		swprintf_s(pwszRetMsg, 1024, L"%s--%d--%d--%s", CardInfo_.wsCardNo.c_str(), CardInfo_.nMaxHour, CardInfo_.nSurplusHour, CardInfo_.wsActiveTime.c_str());
	}))
	{
		swprintf_s(pwszRetMsg,1024, L"%s--0--0--此卡不存在", pwszCardNo);
	}
	VMProtectEnd();
	return 0x1;
}

extern "C" __declspec(dllexport) int WINAPIV GetServerTitle(_Out_ LPWSTR pwszMsg)
{
	VMProtectBeginUltra("ConsoleDll-GetServerTitle");
	CSocketAPI SocketAPI;

	std::wstring wsText;
	SocketAPI.GetServerAnnouncement(wsText);
	MyTools::CCharacter::wstrcpy_my(pwszMsg, wsText.c_str(), wsText.length());
	VMProtectEnd();
	return 0x1;
}

// 添加帐号到vector
extern "C" __declspec(dllexport) int WINAPIV AccountHandle(_In_ int nFlag, _In_  LPCWSTR pwszText)
{
	VMProtectBeginUltra("ConsoleDll-AccountHandle");
	if (nFlag == 0x1) // Add Account
	{
		WCHAR wszAccountName[64] = { 0 };
		WCHAR wszAccountPass[64] = { 0 };
		MyTools::CCharacter::GetRemoveLeft(pwszText, L",", wszAccountName);
		MyTools::CCharacter::GetRemoveRight(pwszText, L",", wszAccountPass);
		LOG_CF_D( L"添加帐号:%s", wszAccountName);
		return CAccountMgr::GetInstance().AddAccount(wszAccountName, wszAccountPass) ? 1 : 0;
	}
	else if (nFlag == 0x2) // Delete Account
	{
		if (CAccountMgr::GetInstance().DeleteAccount(pwszText))
		{
			g_uRunCount = CConsoleFun::GetInstance().GetRunAccountCount();
			return 1;
		}
		return 0;
	}
	else if (nFlag == 0x3) // Restar Account
	{
		return CAccountMgr::GetInstance().RestartAccount(pwszText) ? 1 : 0;
	}
	else if (nFlag == 0x4) // StopGame
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			AccountGame.MyAcount_Info.GameCmd = em_GameCmd_Stop;
			return TRUE;
		});
	}
	else if (nFlag == 0x5)// StartGame
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			AccountGame.MyAcount_Info.GameCmd = em_GameCmd_Start;
			AccountGame.AccountStatus.bFrozen = FALSE;
			return TRUE;
		});
	}
	else if (nFlag == 0x6)
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			::SwitchToThisWindow(AccountGame.hGameWnd, TRUE);
			Sleep(500);
			::SetWindowPos(AccountGame.hGameWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			::MoveWindow(AccountGame.hGameWnd, 0, 0, 1024, 768, FALSE);
			Sleep(500);
			return TRUE;
		});
	}
	else if (nFlag == 0x7)
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			AccountGame.AccountStatus.bFrozen = TRUE;
			return TRUE;
		});
	}
	else if (nFlag == 0x8)
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			AccountGame.AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_DeleteAccount;
			return TRUE;
		});
	}
	else if (nFlag == 0x9)
	{
		return CAccountMgr::GetInstance().HandleAccount(pwszText, [](ACCOUNT_INFO_GAME& AccountGame)
		{
			AccountGame.AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_Stop;
			return TRUE;
		});
	}
	VMProtectEnd();
	return 0;
}

extern "C" __declspec(dllexport) int WINAPIV SetParm(_In_ int nKey, _In_ LPCWSTR pwszValue)
{
	VMProtectBeginUltra("ConsoleDll-SetParm");
	switch (nKey)
	{
	case 0x1: // 游戏路径
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->wszGamePath, pwszValue, _countof(g_pSharedInfo->wszGamePath) - 1);
		break;
	case 0x2: // 大区Id
		g_pSharedInfo->ConsoleContent.GlobalAccount.dwAreaId = wcstol(pwszValue, nullptr, 16);
		break;
	case 0x3: // 默认启动游戏脚本
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.wszScriptName, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.wszScriptName) - 1);
		break;
	case 0x4: // 是否启动自动运行脚本
		g_pSharedInfo->ConsoleContent.GlobalAccount.bRun = _wtoi(pwszValue) == 1 ? TRUE : FALSE;
		break;
	case 0x5: // 启动脚本类型
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.wszTypeName, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.wszTypeName) - 1);
		break;
	case 0x6: // 邮寄角色名
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Mail.wszPlayerName[g_pSharedInfo->ConsoleContent.GlobalAccount.Mail.uCount++], pwszValue);
		break;
	case 0x7: // 是否要排列窗口
		MyTools::InvokeClassPtr<CSortWindowService>()->SetSortFlag(_wtoi(pwszValue));
		break;
	case 0x8: // 仓库号脚本
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.wszScriptName, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.wszScriptName));
		break;
	case 0x9: // IsClose_By_Banned
		g_pSharedInfo->ConsoleContent.GlobalAccount.IsClose_By_Banned = _wtoi(pwszValue) == 0 ? FALSE : TRUE;
		break;
	case  0xA: // IsPK
		g_pSharedInfo->ConsoleContent.GlobalAccount.IsPK = _wtoi(pwszValue) == 0 ? FALSE : TRUE;
		break;
	case 0xB: // IsDailyTask
		g_pSharedInfo->ConsoleContent.GlobalAccount.IsDailyTask = _wtoi(pwszValue) == 0 ? FALSE : TRUE;
		break;
	case 0xC:
		if (std::wstring(pwszValue) == L"0")
		{
			g_pSharedInfo->ConsoleContent.GlobalAccount.dwLoginCountOnce = 0;
			g_pSharedInfo->ConsoleContent.GlobalAccount.dwLoginModeSleep = 0;
		}
		else
		{
			vector<wstring> ParameterVec;

			vector<wstring> TextVec;
			std::wstring wsText = pwszValue;
			MyTools::CCharacter::GetSplit_By_List(wsText, { L"[", L"]" }, TextVec, Split_Option_None);
			if (TextVec.size() != 1)
			{
				LOG_MSG_CF(L"config.ini的参数FastLoginMode错误! :%s", pwszValue);
				SetGameRun(0x2);
			}
			if (MyTools::CCharacter::Split(TextVec.at(0), L",", ParameterVec, Split_Option_RemoveEmptyEntries) != 0x2)
			{
				LOG_MSG_CF(L"config.ini的参数FastLoginMode错误! :%s", pwszValue);
				SetGameRun(0x2);
			}
			
			g_pSharedInfo->ConsoleContent.GlobalAccount.dwLoginCountOnce = _wtoi(ParameterVec.at(0).c_str());
			g_pSharedInfo->ConsoleContent.GlobalAccount.dwLoginModeSleep = _wtoi(ParameterVec.at(1).c_str());
		}
		break;
	case 0xD:
		// 只设置当前登录的帐号
		CAccountMgr::GetInstance().HandleEachAccount([pwszValue](ACCOUNT_INFO_GAME& AccountGame)
		{
			// bExist = 这个结构是否有帐号  bLogin = FALSE表示这个帐号登录完毕 bLogined = TRUE表示这个帐号已经登录过
			if (!AccountGame.AccountStatus.bExist || AccountGame.MyAcount_Info.bLogin || !AccountGame.AccountStatus.bLogined || AccountGame.AccountStatus.bFrozen)
				return;

			if (MyTools::CCharacter::wstrcmp_my(pwszValue, L"1"))
				AccountGame.AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_DeleteAccount;
			else if (MyTools::CCharacter::wstrcmp_my(pwszValue, L"2"))
				AccountGame.AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_Stop;
			else
				AccountGame.AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_None;
		});

		
		break;
	case 0xE: // 邮寄脚本名
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Mail.wszScriptName, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.Mail.wszScriptName));
		break;
	case 0xF: // 仓库角色名
	{
		vector<wstring> ParameterVec;

		vector<wstring> TextVec;
		std::wstring wsText = pwszValue;
		MyTools::CCharacter::GetSplit_By_List(wsText, { L"[", L"]" }, TextVec, Split_Option_None);
		if (TextVec.size() != 1)
		{
			LOG_MSG_CF(L"config.ini的参数Warehouse错误! :%s TextVec.size=%d", pwszValue, TextVec.size());
			SetGameRun(0x2);
		}
		if (MyTools::CCharacter::Split(TextVec.at(0), L",", ParameterVec, Split_Option_RemoveEmptyEntries) != 0x4)
		{
			LOG_MSG_CF(L"config.ini的参数Warehouse错误! :%s, ParameterVec.size=%d", pwszValue, ParameterVec.size());
			SetGameRun(0x2);
		}

		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.Warehouse[g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.uCount].wszPlayerName, ParameterVec.at(0).c_str());
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.Warehouse[g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.uCount].wszNpcName, ParameterVec.at(2).c_str());
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.Warehouse[g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.uCount].wszCityName, ParameterVec.at(3).c_str());
		g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.Warehouse[g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.uCount].dwChannel = _wtoi(ParameterVec.at(1).c_str());

		g_pSharedInfo->ConsoleContent.GlobalAccount.Warehouse.uCount += 1;
	}
		
		break;
	case 0x10:
		if (MyTools::CCharacter::wstrcmp_my(pwszValue,L"1"))
		{
			::CreateMutexW(NULL, FALSE, L"CL_MUTEX_DLL_INJECTOR");
		}
		break;
	case 0x11:
		g_pSharedInfo->ConsoleContent.GlobalAccount.IsDeletePlayer = MyTools::CCharacter::wstrcmp_my(pwszValue, L"1") ? TRUE : FALSE;
		break;
	case 0x12:
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.wszGroupTitle, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.wszGroupTitle));
		break;
	case 0x13:
		g_pSharedInfo->ConsoleContent.hConsoleWnd = reinterpret_cast<HWND>(_wtoi(pwszValue));
		break;
	case 0x14:
		g_pSharedInfo->ConsoleContent.GlobalAccount.bAccelerate = _wtoi(pwszValue) == 1 ? TRUE : FALSE;
		break;
	case 0x15:
		MyTools::CCharacter::wstrcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher.wszNcLauncherPath, pwszValue, _countof(g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher.wszNcLauncherPath) - 1);
		break;
	case 0x16:
		g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher.dwLoginControlHwndTimeout = static_cast<DWORD>(_wtoi(pwszValue));
		break;
	case 0x17:
		g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher.dwLoginTimeout = static_cast<DWORD>(_wtoi(pwszValue));
		break;
	case 0x18:
		g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher.dwDisableLoginButtonTimeout = static_cast<DWORD>(_wtoi(pwszValue));
		break;
	default:
		LOG_MSG_CF(L"Error nKey:%X", nKey);
		break;
	}
	VMProtectEnd();
	return 0x1;
}

extern "C" __declspec(dllexport) void WINAPIV SetWindowSortRun(_In_ int nRun)
{
	MyTools::InvokeClassPtr<CSortWindowService>()->SetSortFlag(nRun);
}

extern "C" __declspec(dllexport) int WINAPIV GetOnLineMsgByAccount(_In_ LPCWSTR pwszAccountName, _Out_ LPWSTR pwszRetMsg)
{
	auto FormatTimePtr = [](_In_ ULONGLONG ulTick, _Out_ LPWSTR pwszText)
	{
		DWORD dwMinute = static_cast<DWORD>(ulTick / 1000 / 60);
		DWORD dwHour = dwMinute / 60;
		dwMinute -= dwHour * 60;
		wsprintfW(pwszText, L"%d小时%d分钟", dwHour, dwMinute);
	};

	return CAccountMgr::GetInstance().HandleAccount(pwszAccountName, [&](ACCOUNT_INFO_GAME& AccGame)
	{
		if (AccGame.AccountStatus.ulOnLineTick == NULL)
			wsprintfW(pwszRetMsg, L"......");
		else
			FormatTimePtr(::GetTickCount64() - AccGame.AccountStatus.ulOnLineTick, pwszRetMsg);
		return TRUE;
	});
}

extern "C" __declspec(dllexport) VOID WINAPIV GetCompileTime(int nType, _Out_ LPWSTR pwszRetMsg)
{
	if (nType == 0)
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, MyTools::CLPublic::GetCompileTime().c_str());
	else if (nType == 0x1)
	{
		HMODULE hmZlib = ::LoadLibraryW(MyTools::CLPublic::MakeCurrentPath(L"\\CopyFile\\zlib1.dll").c_str());
		HMODULE hm = ::LoadLibraryW(MyTools::CLPublic::MakeCurrentPath(L"\\CopyFile\\BnsProjects.dll").c_str());
		if (hm == NULL)
		{
			LOG_MSG_CF(L"加载BnsProjects.dll失败, 无法获取编译时间!");
			return;
		}

		typedef VOID(WINAPIV* fnGetComipleTime)(_Out_ LPWSTR pwszText);
		fnGetComipleTime GetComipleTimePtr = reinterpret_cast<fnGetComipleTime>(::GetProcAddress(hm, "GetComipleTime"));
		if (GetComipleTimePtr == nullptr)
		{
			LOG_MSG_CF(L"获取BnsProjects.dll的导出函数失败!");
			return;
		}

		GetComipleTimePtr(pwszRetMsg);
		::FreeLibrary(hm);
		::FreeLibrary(hmZlib);
	}
}